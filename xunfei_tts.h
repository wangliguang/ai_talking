/**
 * 科大讯飞语音合成（TTS）模块
 */

#ifndef XUNFEI_TTS_H
#define XUNFEI_TTS_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <mbedtls/sha256.h>
#include <mbedtls/md.h>
#include <string.h>
#include <ctype.h>
#include "config.h"

// Base64编码函数（ESP32内置实现）
String base64Encode(const String& text) {
  return base64Encode((const uint8_t*)text.c_str(), text.length());
}

String base64Encode(const uint8_t* data, size_t length) {
  const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String encoded = "";
  int i = 0;
  int j = 0;
  uint8_t char_array_3[3];
  uint8_t char_array_4[4];
  
  while (length--) {
    char_array_3[i++] = *(data++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;
      
      for (i = 0; i < 4; i++) {
        encoded += base64_chars[char_array_4[i]];
      }
      i = 0;
    }
  }
  
  if (i) {
    for (j = i; j < 3; j++) {
      char_array_3[j] = '\0';
    }
    
    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;
    
    for (j = 0; j < i + 1; j++) {
      encoded += base64_chars[char_array_4[j]];
    }
    
    while (i++ < 3) {
      encoded += '=';
    }
  }
  
  return encoded;
}

// Base64解码函数
size_t base64DecodeLength(const String& encoded) {
  size_t len = encoded.length();
  size_t padding = 0;
  if (len > 0 && encoded[len-1] == '=') padding++;
  if (len > 1 && encoded[len-2] == '=') padding++;
  return (len * 3) / 4 - padding;
}

bool base64Decode(const String& encoded, uint8_t* output, size_t* outputLength) {
  const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  int in_len = encoded.length();
  int i = 0;
  int j = 0;
  int in = 0;
  uint8_t char_array_4[4], char_array_3[3];
  size_t decoded_len = 0;
  
  while (in_len-- && (encoded[in] != '=') && isBase64(encoded[in])) {
    char_array_4[i++] = encoded[in]; in++;
    if (i == 4) {
      for (i = 0; i < 4; i++) {
        const char* pos = strchr(base64_chars, char_array_4[i]);
        if (pos != nullptr) {
          char_array_4[i] = pos - base64_chars;
        } else {
          char_array_4[i] = 0;
        }
      }
      
      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
      
      for (i = 0; (i < 3); i++) {
        output[decoded_len++] = char_array_3[i];
      }
      i = 0;
    }
  }
  
  if (i) {
    for (j = i; j < 4; j++) {
      char_array_4[j] = 0;
    }
    
    for (j = 0; j < 4; j++) {
      const char* pos = strchr(base64_chars, char_array_4[j]);
      if (pos != nullptr) {
        char_array_4[j] = pos - base64_chars;
      } else {
        char_array_4[j] = 0;
      }
    }
    
    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
    
    for (j = 0; (j < i - 1); j++) {
      output[decoded_len++] = char_array_3[j];
    }
  }
  
  *outputLength = decoded_len;
  return true;
}

bool isBase64(char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

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
  requestDoc["data"]["text"] = base64Encode(text);
  
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
      *audioSize = base64DecodeLength(audioBase64);
      *audioData = (uint8_t*)malloc(*audioSize);
      
      if (*audioData != nullptr) {
        size_t decodedLen = 0;
        base64Decode(audioBase64, *audioData, &decodedLen);
        *audioSize = decodedLen;
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

