/**
 * 科大讯飞语音识别（ASR）模块
 * 使用WebSocket实时识别
 */

#ifndef XUNFEI_ASR_H
#define XUNFEI_ASR_H

#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <base64.h>
#include <sha256.h>
#include <time.h>
#include "config.h"

class XunfeiASR {
public:
  XunfeiASR();
  void begin(const char* appId, const char* apiKey, const char* apiSecret);
  String recognizeRealtime(int16_t* audioData, int audioLength);
  void end();
  
private:
  String generateAuthUrl();
  void onWebSocketEvent(WStype_t type, uint8_t* payload, size_t length);
  static void webSocketEvent(WStype_t type, uint8_t* payload, size_t length);
  
  WebSocketsClient webSocket;
  String appId;
  String apiKey;
  String apiSecret;
  String recognizedText;
  bool isRecognizing;
  static XunfeiASR* instance;
};

XunfeiASR* XunfeiASR::instance = nullptr;

XunfeiASR::XunfeiASR() {
  isRecognizing = false;
  recognizedText = "";
}

void XunfeiASR::begin(const char* appId, const char* apiKey, const char* apiSecret) {
  this->appId = String(appId);
  this->apiKey = String(apiKey);
  this->apiSecret = String(apiSecret);
  instance = this;
  
  // 生成WebSocket URL
  String wsUrl = generateAuthUrl();
  
  // 初始化WebSocket
  webSocket.begin(wsUrl.c_str());
  webSocket.onEvent(webSocketEvent);
}

String XunfeiASR::generateAuthUrl() {
  // 生成鉴权URL（简化版，实际需要HMAC-SHA256签名）
  // 这里需要实现完整的鉴权逻辑
  String host = "iat-api.xfyun.cn";
  String path = "/v2/iat";
  
  // TODO: 实现完整的鉴权URL生成
  // 需要：HMAC-SHA256签名、base64编码等
  
  return "wss://" + host + path;
}

void XunfeiASR::webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  if (instance == nullptr) return;
  instance->onWebSocketEvent(type, payload, length);
}

void XunfeiASR::onWebSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("WebSocket断开连接");
      break;
      
    case WStype_CONNECTED:
      Serial.println("WebSocket连接成功");
      break;
      
    case WStype_TEXT:
      {
        // 解析识别结果
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        
        if (doc["code"] == 0 && doc.containsKey("data")) {
          String dataStr = doc["data"].as<String>();
          DynamicJsonDocument dataDoc(1024);
          deserializeJson(dataDoc, dataStr);
          
          if (dataDoc.containsKey("result")) {
            JsonObject result = dataDoc["result"];
            if (result.containsKey("ws")) {
              JsonArray wsArray = result["ws"];
              for (JsonObject wsItem : wsArray) {
                if (wsItem.containsKey("cw")) {
                  JsonArray cwArray = wsItem["cw"];
                  for (JsonObject cwItem : cwArray) {
                    if (cwItem.containsKey("w")) {
                      recognizedText += cwItem["w"].as<String>();
                    }
                  }
                }
              }
            }
          }
        }
      }
      break;
      
    default:
      break;
  }
}

String XunfeiASR::recognizeRealtime(int16_t* audioData, int audioLength) {
  if (!isRecognizing) {
    // 开始识别
    isRecognizing = true;
    recognizedText = "";
    
    // 发送开始帧
    DynamicJsonDocument frame(512);
    frame["common"]["app_id"] = appId;
    frame["business"]["language"] = "zh_cn";
    frame["business"]["domain"] = "iat";
    frame["business"]["accent"] = "mandarin";
    frame["data"]["status"] = 0;
    frame["data"]["format"] = "audio/L16;rate=16000";
    frame["data"]["encoding"] = "raw";
    
    String frameStr;
    serializeJson(frame, frameStr);
    webSocket.sendTXT(frameStr);
  }
  
  // 发送音频数据
  String base64Audio = base64::encode((uint8_t*)audioData, audioLength * sizeof(int16_t));
  
  DynamicJsonDocument frame(2048);
  frame["common"]["app_id"] = appId;
  frame["business"]["language"] = "zh_cn";
  frame["business"]["domain"] = "iat";
  frame["business"]["accent"] = "mandarin";
  frame["data"]["status"] = 1; // 中间数据
  frame["data"]["format"] = "audio/L16;rate=16000";
  frame["data"]["audio"] = base64Audio;
  frame["data"]["encoding"] = "raw";
  
  String frameStr;
  serializeJson(frame, frameStr);
  webSocket.sendTXT(frameStr);
  
  webSocket.loop();
  
  // 返回当前识别结果
  return recognizedText;
}

void XunfeiASR::end() {
  // 发送结束帧
  DynamicJsonDocument frame(512);
  frame["common"]["app_id"] = appId;
  frame["business"]["language"] = "zh_cn";
  frame["business"]["domain"] = "iat";
  frame["business"]["accent"] = "mandarin";
  frame["data"]["status"] = 2; // 结束
  frame["data"]["format"] = "audio/L16;rate=16000";
  frame["data"]["audio"] = "";
  frame["data"]["encoding"] = "raw";
  
  String frameStr;
  serializeJson(frame, frameStr);
  webSocket.sendTXT(frameStr);
  
  webSocket.disconnect();
  isRecognizing = false;
}

#endif

