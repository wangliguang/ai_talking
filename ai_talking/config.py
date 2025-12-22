"""
配置管理模块
支持从配置文件和环境变量读取配置
"""

import os
import configparser
from typing import Optional, Dict, Any
from pathlib import Path


class Config:
    """配置管理类"""
    
    def __init__(self, config_file: Optional[str] = None):
        """
        初始化配置
        
        Args:
            config_file: 配置文件路径，如果为None则使用默认路径
        """
        self.config = configparser.ConfigParser()
        
        if config_file is None:
            config_file = os.path.join(
                Path(__file__).parent.parent, 
                'config.ini'
            )
        
        self.config_file = config_file
        
        # 如果配置文件存在，则加载
        if os.path.exists(config_file):
            self.config.read(config_file, encoding='utf-8')
    
    def get(self, section: str, key: str, default: Optional[str] = None) -> str:
        """
        获取配置值，优先从环境变量读取
        
        Args:
            section: 配置段名
            key: 配置键名
            default: 默认值
            
        Returns:
            配置值
        """
        # 先尝试从环境变量读取（格式：SECTION_KEY）
        env_key = f"{section.upper()}_{key.upper()}"
        env_value = os.getenv(env_key)
        if env_value:
            return env_value
        
        # 从配置文件读取
        try:
            return self.config.get(section, key)
        except (configparser.NoSectionError, configparser.NoOptionError):
            return default
    
    def getint(self, section: str, key: str, default: Optional[int] = None) -> int:
        """获取整数配置值"""
        value = self.get(section, key)
        if value is None:
            return default
        try:
            return int(value)
        except ValueError:
            return default
    
    def getfloat(self, section: str, key: str, default: Optional[float] = None) -> float:
        """获取浮点数配置值"""
        value = self.get(section, key)
        if value is None:
            return default
        try:
            return float(value)
        except ValueError:
            return default
    
    def getboolean(self, section: str, key: str, default: Optional[bool] = None) -> bool:
        """获取布尔配置值"""
        value = self.get(section, key)
        if value is None:
            return default
        return value.lower() in ('true', '1', 'yes', 'on')
    
    # LLM配置
    @property
    def llm_provider(self) -> str:
        """大模型提供商"""
        return self.get('llm', 'provider', 'openai')
    
    @property
    def llm_api_key(self) -> str:
        """大模型API密钥"""
        return self.get('llm', 'api_key', '')
    
    @property
    def llm_base_url(self) -> Optional[str]:
        """大模型API基础URL（用于自定义端点）"""
        return self.get('llm', 'base_url', None)
    
    @property
    def llm_model(self) -> str:
        """大模型名称"""
        return self.get('llm', 'model', 'gpt-3.5-turbo')
    
    @property
    def llm_temperature(self) -> float:
        """大模型温度参数"""
        return self.getfloat('llm', 'temperature', 0.7)
    
    # 语音识别配置
    @property
    def asr_provider(self) -> str:
        """语音识别提供商"""
        return self.get('asr', 'provider', 'whisper')
    
    @property
    def asr_api_key(self) -> Optional[str]:
        """语音识别API密钥（如果需要）"""
        return self.get('asr', 'api_key', None)
    
    @property
    def asr_language(self) -> str:
        """语音识别语言"""
        return self.get('asr', 'language', 'zh')
    
    # 语音合成配置
    @property
    def tts_provider(self) -> str:
        """语音合成提供商"""
        return self.get('tts', 'provider', 'edge-tts')
    
    @property
    def tts_voice(self) -> str:
        """默认音色"""
        return self.get('tts', 'voice', 'zh-CN-XiaoxiaoNeural')
    
    @property
    def tts_api_key(self) -> Optional[str]:
        """语音合成API密钥（如果需要）"""
        return self.get('tts', 'api_key', None)
    
    @property
    def volume(self) -> float:
        """音量（0.0-1.0）"""
        return self.getfloat('audio', 'volume', 0.8)
    
    # 内容过滤配置
    @property
    def content_filter_enabled(self) -> bool:
        """是否启用内容过滤"""
        return self.getboolean('content', 'filter_enabled', True)
    
    @property
    def content_age_limit(self) -> int:
        """内容年龄限制"""
        return self.getint('content', 'age_limit', 6)
    
    # 通识教育配置
    @property
    def education_topics(self) -> list:
        """通识教育主题列表"""
        topics_str = self.get('education', 'topics', '')
        if topics_str:
            return [t.strip() for t in topics_str.split(',')]
        return ['科学', '历史', '地理', '自然', '数学', '艺术']
    
    def save(self):
        """保存配置到文件"""
        with open(self.config_file, 'w', encoding='utf-8') as f:
            self.config.write(f)


# 全局配置实例
_config_instance: Optional[Config] = None


def get_config() -> Config:
    """获取全局配置实例"""
    global _config_instance
    if _config_instance is None:
        _config_instance = Config()
    return _config_instance

