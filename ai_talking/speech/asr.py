"""
语音识别模块
支持多种语音识别提供商
"""

from abc import ABC, abstractmethod
from typing import Optional
from ..config import get_config


class ASRProvider(ABC):
    """语音识别提供商基类"""
    
    def __init__(self, config):
        self.config = config
    
    @abstractmethod
    def recognize(self, audio_data: bytes, sample_rate: int = 16000) -> str:
        """
        识别语音
        
        Args:
            audio_data: 音频数据（字节）
            sample_rate: 采样率
            
        Returns:
            识别出的文本
        """
        pass
    
    @abstractmethod
    def recognize_file(self, audio_file: str) -> str:
        """
        识别音频文件
        
        Args:
            audio_file: 音频文件路径
            
        Returns:
            识别出的文本
        """
        pass


class WhisperASRProvider(ASRProvider):
    """Whisper本地语音识别提供商"""
    
    def __init__(self, config=None):
        super().__init__(config or get_config())
        self.language = self.config.asr_language
        self._model = None
    
    def _load_model(self):
        """延迟加载模型"""
        if self._model is None:
            try:
                import whisper
                self._model = whisper.load_model("base")
            except ImportError:
                raise ImportError("Whisper not installed. Install with: pip install openai-whisper")
    
    def recognize(self, audio_data: bytes, sample_rate: int = 16000) -> str:
        """识别语音"""
        try:
            import numpy as np
            import io
            
            self._load_model()
            
            # 将字节数据转换为numpy数组
            audio_array = np.frombuffer(audio_data, dtype=np.int16).astype(np.float32) / 32768.0
            
            # 识别
            result = self._model.transcribe(
                audio_array,
                language=self.language if self.language != 'zh' else 'zh',
            )
            
            return result['text'].strip()
        except Exception as e:
            return f"识别错误: {str(e)}"
    
    def recognize_file(self, audio_file: str) -> str:
        """识别音频文件"""
        try:
            self._load_model()
            result = self._model.transcribe(
                audio_file,
                language=self.language if self.language != 'zh' else 'zh',
            )
            return result['text'].strip()
        except Exception as e:
            return f"识别错误: {str(e)}"


def create_asr_provider(provider_name: Optional[str] = None, config=None) -> ASRProvider:
    """
    创建语音识别提供商实例
    
    Args:
        provider_name: 提供商名称
        config: 配置对象
        
    Returns:
        ASRProvider实例
    """
    if config is None:
        config = get_config()
    
    if provider_name is None:
        provider_name = config.asr_provider
    
    provider_name = provider_name.lower()
    
    if provider_name == 'whisper':
        return WhisperASRProvider(config)
    # 可以在这里添加其他提供商
    # elif provider_name == 'baidu':
    #     return BaiduASRProvider(config)
    else:
        raise ValueError(f"Unsupported ASR provider: {provider_name}")

