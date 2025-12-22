"""
大模型提供商工厂
"""

from typing import Optional
from .base import LLMProvider
from .openai_provider import OpenAIProvider
from ..config import get_config


def create_llm_provider(provider_name: Optional[str] = None, config=None) -> LLMProvider:
    """
    创建大模型提供商实例
    
    Args:
        provider_name: 提供商名称，如果为None则从配置读取
        config: 配置对象，如果为None则使用全局配置
        
    Returns:
        LLMProvider实例
        
    Raises:
        ValueError: 如果提供商名称不支持
    """
    if config is None:
        config = get_config()
    
    if provider_name is None:
        provider_name = config.llm_provider
    
    provider_name = provider_name.lower()
    
    if provider_name == 'openai':
        return OpenAIProvider(config)
    # 可以在这里添加其他提供商
    # elif provider_name == 'qianwen':
    #     return QianwenProvider(config)
    # elif provider_name == 'zhipu':
    #     return ZhipuProvider(config)
    else:
        raise ValueError(f"Unsupported LLM provider: {provider_name}")

