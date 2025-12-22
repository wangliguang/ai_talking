"""
内容过滤模块
确保所有内容都适合6岁儿童
"""

import re
from typing import List, Optional
from ..config import get_config


class ContentFilter:
    """内容过滤器"""
    
    def __init__(self, config=None):
        self.config = config or get_config()
        self.enabled = self.config.content_filter_enabled
        self.age_limit = self.config.content_age_limit
        
        # 敏感词列表（可以根据需要扩展）
        self.sensitive_keywords = [
            # 暴力相关
            '暴力', '血腥', '杀戮', '死亡', '尸体',
            # 成人内容
            '性', '色情', '成人', '情色',
            # 恐怖内容
            '鬼', '恐怖', '吓人', '害怕',
            # 其他不适合儿童的内容
            '毒品', '酒精', '抽烟', '赌博',
        ]
        
        # 构建正则表达式
        self.pattern = re.compile('|'.join(self.sensitive_keywords))
    
    def filter(self, text: str) -> tuple[bool, Optional[str]]:
        """
        过滤内容
        
        Args:
            text: 要过滤的文本
            
        Returns:
            (是否通过, 错误信息)
        """
        if not self.enabled:
            return True, None
        
        # 检查敏感词
        if self.pattern.search(text):
            return False, "内容包含不适合儿童的内容"
        
        # 可以添加更多过滤规则
        # 例如：检查长度、检查特殊字符等
        
        return True, None
    
    def is_safe(self, text: str) -> bool:
        """
        检查内容是否安全
        
        Args:
            text: 要检查的文本
            
        Returns:
            是否安全
        """
        is_safe, _ = self.filter(text)
        return is_safe
    
    def add_keyword(self, keyword: str):
        """
        添加敏感词
        
        Args:
            keyword: 敏感词
        """
        if keyword not in self.sensitive_keywords:
            self.sensitive_keywords.append(keyword)
            self.pattern = re.compile('|'.join(self.sensitive_keywords))
    
    def remove_keyword(self, keyword: str):
        """
        移除敏感词
        
        Args:
            keyword: 敏感词
        """
        if keyword in self.sensitive_keywords:
            self.sensitive_keywords.remove(keyword)
            self.pattern = re.compile('|'.join(self.sensitive_keywords))

