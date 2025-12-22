"""
大模型基础接口
"""

from abc import ABC, abstractmethod
from typing import List, Dict, Optional
from dataclasses import dataclass


@dataclass
class LLMResponse:
    """大模型响应"""
    content: str
    usage: Optional[Dict] = None
    error: Optional[str] = None


class LLMProvider(ABC):
    """大模型提供商基类"""
    
    def __init__(self, config):
        """
        初始化大模型提供商
        
        Args:
            config: 配置对象
        """
        self.config = config
    
    @abstractmethod
    def chat(self, messages: List[Dict[str, str]], system_prompt: Optional[str] = None) -> LLMResponse:
        """
        发送聊天请求
        
        Args:
            messages: 消息列表，格式为 [{"role": "user", "content": "..."}]
            system_prompt: 系统提示词
            
        Returns:
            LLMResponse对象
        """
        pass
    
    @abstractmethod
    def generate_educational_response(self, user_input: str, conversation_history: List[Dict[str, str]]) -> LLMResponse:
        """
        生成教育性回复（针对6岁儿童的通识教育）
        
        Args:
            user_input: 用户输入
            conversation_history: 对话历史
            
        Returns:
            LLMResponse对象
        """
        pass

