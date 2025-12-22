"""
通识教育辅助模块
提供适合6岁儿童的教育内容和话题
"""

from typing import List, Dict
from ..config import get_config


class EducationHelper:
    """通识教育辅助类"""
    
    def __init__(self, config=None):
        self.config = config or get_config()
        self.topics = self.config.education_topics
        
        # 预设的教育话题和引导语
        self.topic_prompts = {
            '科学': [
                "你知道为什么天空是蓝色的吗？",
                "我们来聊聊小动物是怎么生活的吧！",
                "你知道植物是怎么长大的吗？",
            ],
            '历史': [
                "你想听古代的故事吗？",
                "我们来聊聊古代的小朋友是怎么生活的吧！",
            ],
            '地理': [
                "你知道世界上有哪些有趣的地方吗？",
                "我们来聊聊不同地方的小朋友是怎么生活的吧！",
            ],
            '自然': [
                "你知道四季是怎么变化的吗？",
                "我们来聊聊大自然的神奇吧！",
            ],
            '数学': [
                "我们来玩一个有趣的数字游戏吧！",
                "你知道数字可以做什么吗？",
            ],
            '艺术': [
                "我们来聊聊美丽的画和音乐吧！",
                "你知道颜色是怎么混合的吗？",
            ],
        }
    
    def get_topic_prompts(self, topic: str) -> List[str]:
        """
        获取某个话题的引导语
        
        Args:
            topic: 话题名称
            
        Returns:
            引导语列表
        """
        return self.topic_prompts.get(topic, [])
    
    def get_all_topics(self) -> List[str]:
        """
        获取所有话题
        
        Returns:
            话题列表
        """
        return self.topics
    
    def add_topic(self, topic: str, prompts: List[str] = None):
        """
        添加新话题
        
        Args:
            topic: 话题名称
            prompts: 引导语列表
        """
        if topic not in self.topics:
            self.topics.append(topic)
        
        if prompts:
            self.topic_prompts[topic] = prompts
    
    def get_random_prompt(self) -> str:
        """
        获取随机引导语
        
        Returns:
            引导语
        """
        import random
        
        all_prompts = []
        for prompts in self.topic_prompts.values():
            all_prompts.extend(prompts)
        
        if all_prompts:
            return random.choice(all_prompts)
        return "我们来聊点有趣的事情吧！"

