#!/usr/bin/env python3
"""
AI Talking - 主程序入口
智能陪伴小工具的主程序
"""

import sys
import os
from pathlib import Path

# 添加项目根目录到Python路径
project_root = Path(__file__).parent
sys.path.insert(0, str(project_root))

from ai_talking.config import get_config
from ai_talking.llm import create_llm_provider
from ai_talking.speech import create_asr_provider, create_tts_provider
from ai_talking.content import ContentFilter, EducationHelper


class AITalkingApp:
    """AI Talking应用主类"""
    
    def __init__(self):
        """初始化应用"""
        self.config = get_config()
        
        # 初始化各个模块
        print("正在初始化AI Talking...")
        
        try:
            self.llm = create_llm_provider()
            print(f"✓ 大模型提供商: {self.config.llm_provider}")
        except Exception as e:
            print(f"✗ 大模型初始化失败: {e}")
            sys.exit(1)
        
        try:
            self.asr = create_asr_provider()
            print(f"✓ 语音识别提供商: {self.config.asr_provider}")
        except Exception as e:
            print(f"✗ 语音识别初始化失败: {e}")
            sys.exit(1)
        
        try:
            self.tts = create_tts_provider()
            print(f"✓ 语音合成提供商: {self.config.tts_provider}")
        except Exception as e:
            print(f"✗ 语音合成初始化失败: {e}")
            sys.exit(1)
        
        # 初始化内容过滤和教育辅助
        self.content_filter = ContentFilter(self.config)
        self.education_helper = EducationHelper(self.config)
        
        # 对话历史
        self.conversation_history = []
        
        print("✓ AI Talking初始化完成！")
        print(f"  当前音量: {int(self.config.volume * 100)}%")
        print(f"  当前音色: {self.config.tts_voice}")
        print()
    
    def process_voice_input(self, audio_file: str) -> str:
        """
        处理语音输入
        
        Args:
            audio_file: 音频文件路径
            
        Returns:
            识别出的文本
        """
        print(f"正在识别语音: {audio_file}")
        text = self.asr.recognize_file(audio_file)
        print(f"识别结果: {text}")
        return text
    
    def generate_response(self, user_input: str) -> str:
        """
        生成回复
        
        Args:
            user_input: 用户输入文本
            
        Returns:
            AI回复文本
        """
        # 内容过滤
        is_safe, error_msg = self.content_filter.filter(user_input)
        if not is_safe:
            return "这个问题不太适合讨论，我们来聊点其他有趣的事情吧！"
        
        # 生成回复
        print("正在生成回复...")
        response = self.llm.generate_educational_response(
            user_input,
            self.conversation_history
        )
        
        if response.error:
            return f"抱歉，出现了错误: {response.error}"
        
        # 再次过滤AI回复
        is_safe, error_msg = self.content_filter.filter(response.content)
        if not is_safe:
            return "让我换个方式回答你吧！"
        
        # 更新对话历史
        self.conversation_history.append({"role": "user", "content": user_input})
        self.conversation_history.append({"role": "assistant", "content": response.content})
        
        # 限制对话历史长度（保留最近10轮对话）
        if len(self.conversation_history) > 20:
            self.conversation_history = self.conversation_history[-20:]
        
        return response.content
    
    def speak(self, text: str, voice: str = None):
        """
        语音合成并播放
        
        Args:
            text: 要合成的文本
            voice: 音色名称（可选）
        """
        print(f"正在合成语音...")
        try:
            audio_data = self.tts.synthesize(text, voice=voice)
            print("正在播放...")
            self.tts.play_audio(audio_data)
        except Exception as e:
            print(f"语音合成或播放错误: {e}")
    
    def set_volume(self, volume: float):
        """
        设置音量
        
        Args:
            volume: 音量值（0.0-1.0）
        """
        volume = max(0.0, min(1.0, volume))
        self.tts.set_volume(volume)
        self.config.config.set('audio', 'volume', str(volume))
        print(f"音量已设置为: {int(volume * 100)}%")
    
    def set_voice(self, voice: str):
        """
        设置音色
        
        Args:
            voice: 音色名称
        """
        self.config.config.set('tts', 'voice', voice)
        print(f"音色已设置为: {voice}")
    
    def list_voices(self):
        """列出可用的音色"""
        voices = self.tts.list_voices()
        print("\n可用音色:")
        for i, voice in enumerate(voices, 1):
            print(f"  {i}. {voice['name']} - {voice.get('description', '')}")
        return voices
    
    def run_interactive(self):
        """运行交互式模式（文本输入）"""
        print("\n=== AI Talking 交互模式 ===")
        print("输入 'quit' 或 'exit' 退出")
        print("输入 'volume <0-100>' 设置音量")
        print("输入 'voice' 查看可用音色")
        print("输入 'voice <音色名称>' 设置音色")
        print()
        
        while True:
            try:
                user_input = input("你: ").strip()
                
                if not user_input:
                    continue
                
                if user_input.lower() in ['quit', 'exit', '退出']:
                    print("再见！")
                    break
                
                # 处理音量设置
                if user_input.lower().startswith('volume '):
                    try:
                        volume_value = int(user_input.split()[1])
                        self.set_volume(volume_value / 100.0)
                        continue
                    except:
                        print("音量设置格式错误，请使用: volume <0-100>")
                        continue
                
                # 处理音色设置
                if user_input.lower() == 'voice':
                    self.list_voices()
                    continue
                
                if user_input.lower().startswith('voice '):
                    voice_name = user_input[6:].strip()
                    self.set_voice(voice_name)
                    continue
                
                # 生成回复
                response = self.generate_response(user_input)
                print(f"AI: {response}")
                
                # 语音播放（可选）
                # self.speak(response)
                
            except KeyboardInterrupt:
                print("\n\n再见！")
                break
            except Exception as e:
                print(f"错误: {e}")


def main():
    """主函数"""
    app = AITalkingApp()
    
    # 运行交互式模式
    app.run_interactive()


if __name__ == '__main__':
    main()

