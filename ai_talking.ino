/**
 * AI Talking - ESP32主程序
 * 使用Arduino IDE开发
 * 硬件：ESP32-S3-DevKitC-1
 * 语言：C++
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "wifi_manager.h"
#include "audio_input.h"
#include "audio_output.h"
#include "xunfei_asr.h"
#include "xunfei_tts.h"
#include "siliconflow_llm.h"

// 全局对象
WiFiManager wifiManager;
AudioInput audioInput;
AudioOutput audioOutput;
XunfeiASR xunfeiASR;
XunfeiTTS xunfeiTTS;
SiliconFlowLLM llm;

// 状态变量
bool isListening = false;
String conversationHistory = "";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=== AI Talking ESP32 ===");
  Serial.println("硬件：ESP32-S3-DevKitC-1");
  Serial.println("初始化中...");
  
  // 检查PSRAM
  if (psramFound()) {
    Serial.print("PSRAM: ");
    Serial.print(ESP.getPsramSize() / 1024);
    Serial.println(" KB");
  } else {
    Serial.println("警告：未检测到PSRAM！");
  }
  
  // 初始化WiFi
  Serial.println("连接WiFi...");
  if (!wifiManager.connect(WIFI_SSID, WIFI_PASSWORD)) {
    Serial.println("WiFi连接失败！");
    while(1) delay(1000);
  }
  Serial.println("WiFi连接成功！");
  Serial.print("IP地址: ");
  Serial.println(WiFi.localIP());
  
  // 初始化音频输入（I2S麦克风）
  Serial.println("初始化音频输入...");
  if (!audioInput.begin()) {
    Serial.println("音频输入初始化失败！");
    while(1) delay(1000);
  }
  Serial.println("音频输入初始化成功！");
  
  // 初始化音频输出（I2S扬声器）
  Serial.println("初始化音频输出...");
  if (!audioOutput.begin()) {
    Serial.println("音频输出初始化失败！");
    while(1) delay(1000);
  }
  Serial.println("音频输出初始化成功！");
  
  // 初始化讯飞ASR
  Serial.println("初始化讯飞ASR...");
  xunfeiASR.begin(XUNFEI_APP_ID, XUNFEI_API_KEY, XUNFEI_API_SECRET);
  
  // 初始化讯飞TTS
  Serial.println("初始化讯飞TTS...");
  xunfeiTTS.begin(XUNFEI_APP_ID, XUNFEI_API_KEY, XUNFEI_API_SECRET);
  
  // 初始化大模型
  Serial.println("初始化大模型...");
  llm.begin(SILICONFLOW_API_KEY, SILICONFLOW_BASE_URL, SILICONFLOW_MODEL);
  
  Serial.println("\n=== 初始化完成 ===");
  Serial.println("等待语音输入...");
  Serial.println("（说话时自动开始识别）\n");
}

void loop() {
  // 检查音频输入
  if (audioInput.available()) {
    if (!isListening) {
      Serial.println("检测到语音输入，开始识别...");
      isListening = true;
    }
    
    // 读取音频数据
    int16_t* audioBuffer = audioInput.read();
    int audioLength = audioInput.getBufferSize();
    
    // 发送到讯飞ASR进行实时识别
    String recognizedText = xunfeiASR.recognizeRealtime(audioBuffer, audioLength);
    
    if (recognizedText.length() > 0) {
      Serial.print("识别结果: ");
      Serial.println(recognizedText);
      
      // 发送到大模型获取回复
      Serial.println("正在生成回复...");
      String aiResponse = llm.chat(recognizedText, conversationHistory);
      
      if (aiResponse.length() > 0) {
        Serial.print("AI回复: ");
        Serial.println(aiResponse);
        
        // 更新对话历史
        conversationHistory += "用户: " + recognizedText + "\n";
        conversationHistory += "AI: " + aiResponse + "\n";
        
        // 限制对话历史长度（避免内存溢出）
        if (conversationHistory.length() > 1000) {
          conversationHistory = conversationHistory.substring(conversationHistory.length() - 1000);
        }
        
        // 将AI回复转换为语音
        Serial.println("正在合成语音...");
        uint8_t* audioData = nullptr;
        size_t audioSize = 0;
        
        if (xunfeiTTS.synthesize(aiResponse, &audioData, &audioSize)) {
          // 播放语音
          Serial.println("正在播放...");
          audioOutput.play(audioData, audioSize);
          
          // 释放内存
          free(audioData);
        } else {
          Serial.println("语音合成失败！");
        }
      } else {
        Serial.println("获取AI回复失败！");
      }
      
      isListening = false;
      Serial.println("\n等待下一次语音输入...\n");
    }
  }
  
  delay(10); // 短暂延迟，避免CPU占用过高
}

