"""
OpenAI大模型提供商实现
"""

from typing import List, Dict, Optional
from .base import LLMProvider, LLMResponse
from ..config import get_config


class OpenAIProvider(LLMProvider):
    """OpenAI大模型提供商"""
    
    def __init__(self, config=None):
        super().__init__(config or get_config())
        self.api_key = self.config.llm_api_key
        self.model = self.config.llm_model
        self.base_url = self.config.llm_base_url
        self.temperature = self.config.llm_temperature
        
        if not self.api_key:
            raise ValueError("OpenAI API key is required")
    
    def chat(self, messages: List[Dict[str, str]], system_prompt: Optional[str] = None) -> LLMResponse:
        """发送聊天请求"""
        try:
            import openai
            
            # 如果提供了base_url，使用自定义端点（兼容国内API）
            client_kwargs = {
                'api_key': self.api_key,
            }
            if self.base_url:
                client_kwargs['base_url'] = self.base_url
            
            client = openai.OpenAI(**client_kwargs)
            
            # 构建消息列表
            chat_messages = []
            if system_prompt:
                chat_messages.append({"role": "system", "content": system_prompt})
            chat_messages.extend(messages)
            
            # 调用API
            response = client.chat.completions.create(
                model=self.model,
                messages=chat_messages,
                temperature=self.temperature,
            )
            
            content = response.choices[0].message.content
            usage = {
                'prompt_tokens': response.usage.prompt_tokens,
                'completion_tokens': response.usage.completion_tokens,
                'total_tokens': response.usage.total_tokens,
            } if response.usage else None
            
            return LLMResponse(content=content, usage=usage)
            
        except ImportError:
            return LLMResponse(
                content="",
                error="OpenAI library not installed. Please install it with: pip install openai"
            )
        except Exception as e:
            return LLMResponse(content="", error=str(e))
    
    def generate_educational_response(self, user_input: str, conversation_history: List[Dict[str, str]]) -> LLMResponse:
        """生成教育性回复"""
        # 构建系统提示词（针对6岁儿童的通识教育）
        system_prompt = """你是一位友善、耐心的AI陪伴老师，专门为6岁的小朋友提供通识教育。

请遵循以下原则：
1. 用简单易懂的语言，适合6岁儿童的理解水平
2. 回答要生动有趣，可以适当使用比喻和故事
3. 内容要健康积极，绝对不能包含任何成人内容、暴力、恐怖等不适合儿童的内容
4. 如果小朋友问的问题不适合回答，要温和地引导到其他有趣的话题
5. 可以主动介绍一些有趣的知识，比如科学小知识、历史故事、地理常识、自然现象等
6. 保持对话的趣味性和互动性

请用中文回答。"""
        
        # 构建消息列表
        messages = conversation_history + [{"role": "user", "content": user_input}]
        
        return self.chat(messages, system_prompt=system_prompt)

