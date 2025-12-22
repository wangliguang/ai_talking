"""
语音处理模块
包含语音识别(ASR)和语音合成(TTS)功能
"""

from .asr import ASRProvider, create_asr_provider
from .tts import TTSProvider, create_tts_provider

__all__ = ['ASRProvider', 'create_asr_provider', 'TTSProvider', 'create_tts_provider']

