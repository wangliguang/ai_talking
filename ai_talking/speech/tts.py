"""
语音合成模块
支持多种语音合成提供商，支持音色选择和音量控制
"""

from abc import ABC, abstractmethod
from typing import Optional, List
from ..config import get_config


class TTSProvider(ABC):
    """语音合成提供商基类"""
    
    def __init__(self, config):
        self.config = config
        self.volume = self.config.volume
    
    @abstractmethod
    def synthesize(self, text: str, voice: Optional[str] = None, output_file: Optional[str] = None) -> bytes:
        """
        合成语音
        
        Args:
            text: 要合成的文本
            voice: 音色名称，如果为None则使用默认音色
            output_file: 输出文件路径，如果为None则返回字节数据
            
        Returns:
            音频数据（字节）
        """
        pass
    
    @abstractmethod
    def list_voices(self) -> List[dict]:
        """
        列出可用的音色
        
        Returns:
            音色列表，每个元素包含name和description
        """
        pass
    
    def set_volume(self, volume: float):
        """
        设置音量
        
        Args:
            volume: 音量值（0.0-1.0）
        """
        self.volume = max(0.0, min(1.0, volume))
    
    def play_audio(self, audio_data: bytes):
        """
        播放音频数据
        
        Args:
            audio_data: 音频数据（字节）
        """
        try:
            import pygame
            import io
            
            pygame.mixer.init()
            audio_stream = io.BytesIO(audio_data)
            pygame.mixer.music.load(audio_stream)
            pygame.mixer.music.set_volume(self.volume)
            pygame.mixer.music.play()
            
            # 等待播放完成
            while pygame.mixer.music.get_busy():
                import time
                time.sleep(0.1)
                
        except ImportError:
            print("pygame not installed. Cannot play audio.")
        except Exception as e:
            print(f"播放音频错误: {str(e)}")


class EdgeTTSProvider(TTSProvider):
    """Edge TTS提供商（使用edge-tts库）"""
    
    def __init__(self, config=None):
        super().__init__(config or get_config())
        self.default_voice = self.config.tts_voice
        self._voices_cache = None
    
    def synthesize(self, text: str, voice: Optional[str] = None, output_file: Optional[str] = None) -> bytes:
        """合成语音"""
        try:
            import edge_tts
            import asyncio
            
            voice_name = voice or self.default_voice
            
            async def _synthesize():
                communicate = edge_tts.Communicate(text, voice_name)
                if output_file:
                    await communicate.save(output_file)
                    # 读取文件并返回
                    with open(output_file, 'rb') as f:
                        return f.read()
                else:
                    # 直接返回音频数据
                    audio_data = b""
                    async for chunk in communicate.stream():
                        if chunk["type"] == "audio":
                            audio_data += chunk["data"]
                    return audio_data
            
            return asyncio.run(_synthesize())
            
        except ImportError:
            raise ImportError("edge-tts not installed. Install with: pip install edge-tts")
        except Exception as e:
            raise Exception(f"TTS合成错误: {str(e)}")
    
    def list_voices(self) -> List[dict]:
        """列出可用的音色"""
        try:
            import edge_tts
            import asyncio
            
            if self._voices_cache is not None:
                return self._voices_cache
            
            async def _list_voices():
                voices = await edge_tts.list_voices()
                # 过滤中文音色
                chinese_voices = [
                    {
                        'name': v['ShortName'],
                        'description': f"{v['Gender']} - {v['Locale']}",
                        'gender': v['Gender'],
                        'locale': v['Locale']
                    }
                    for v in voices
                    if 'zh' in v['Locale'].lower()
                ]
                return chinese_voices
            
            self._voices_cache = asyncio.run(_list_voices())
            return self._voices_cache
            
        except ImportError:
            return []
        except Exception as e:
            print(f"获取音色列表错误: {str(e)}")
            return []


def create_tts_provider(provider_name: Optional[str] = None, config=None) -> TTSProvider:
    """
    创建语音合成提供商实例
    
    Args:
        provider_name: 提供商名称
        config: 配置对象
        
    Returns:
        TTSProvider实例
    """
    if config is None:
        config = get_config()
    
    if provider_name is None:
        provider_name = config.tts_provider
    
    provider_name = provider_name.lower()
    
    if provider_name == 'edge-tts':
        return EdgeTTSProvider(config)
    # 可以在这里添加其他提供商
    # elif provider_name == 'baidu':
    #     return BaiduTTSProvider(config)
    else:
        raise ValueError(f"Unsupported TTS provider: {provider_name}")

