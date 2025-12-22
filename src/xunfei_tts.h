/**
 * 科大讯飞语音合成（TTS）模块
 */

#ifndef XUNFEI_TTS_H
#define XUNFEI_TTS_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <base64.h>
#include "config.h"

class XunfeiTTS {
public:
  XunfeiTTS();
  void begin(const char* appId, const char* apiKey, const char* apiSecret);
  bool synthesize(const String& text, uint8_t** audioData, size_t* audioSize);
  void end();
  
private:
  String generateAuthUrl();
  String appId;
  String apiKey;
  String apiSecret;
};

XunfeiTTS::XunfeiTTS() {
}

void XunfeiTTS::begin(const char* appId, const char* apiKey, const char* apiSecret) {
  this->appId = String(appId);
  this->apiKey = String(apiKey);
  this->apiSecret = String(apiSecret);
}

String XunfeiTTS::generateAuthUrl() {
  // 生成鉴权URL（简化版，实际需要HMAC-SHA256签名）
  // TODO: 实现完整的鉴权URL生成
  return "https://tts-api.xfyun.cn/v2/tts";
}

bool XunfeiTTS::synthesize(const String& text, uint8_t** audioData, size_t* audioSize) {
  HTTPClient http;
  
  // 生成请求URL
  String url = generateAuthUrl();
  http.begin(url);
  
  // 设置请求头
  http.addHeader("Content-Type", "application/json");
  // TODO: 添加鉴权头
  
  // 构建请求体
  DynamicJsonDocument requestDoc(512);
  requestDoc["common"]["app_id"] = appId;
  requestDoc["business"]["aue"] = "raw";
  requestDoc["business"]["auf"] = "audio/L16;rate=16000";
  requestDoc["business"]["vcn"] = "xiaoyan"; // 音色
  requestDoc["business"]["speed"] = 50;
  requestDoc["business"]["volume"] = 50;
  requestDoc["business"]["pitch"] = 50;
  requestDoc["business"]["bgs"] = 0;
  requestDoc["data"]["status"] = 2;
  requestDoc["data"]["text"] = base64::encode(text);
  
  String requestBody;
  serializeJson(requestDoc, requestBody);
  
  // 发送请求
  int httpCode = http.POST(requestBody);
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    
    // 解析响应
    DynamicJsonDocument responseDoc(4096);
    deserializeJson(responseDoc, response);
    
    if (responseDoc["code"] == 0 && responseDoc.containsKey("data")) {
      String audioBase64 = responseDoc["data"]["audio"].as<String>();
      
      // 解码base64音频数据
      *audioSize = base64::decodeLength(audioBase64);
      *audioData = (uint8_t*)malloc(*audioSize);
      
      if (*audioData != nullptr) {
        base64::decode(audioBase64, *audioData, *audioSize);
        http.end();
        return true;
      }
    }
  }
  
  http.end();
  return false;
}

void XunfeiTTS::end() {
  // 清理资源
}

#endif

