/**
 * 硅基流动大模型API模块
 */

#ifndef SILICONFLOW_LLM_H
#define SILICONFLOW_LLM_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

class SiliconFlowLLM {
public:
  SiliconFlowLLM();
  void begin(const char* apiKey, const char* baseUrl, const char* model);
  String chat(const String& userInput, const String& conversationHistory);
  void end();
  
private:
  String apiKey;
  String baseUrl;
  String model;
  String systemPrompt;
};

SiliconFlowLLM::SiliconFlowLLM() {
  // 系统提示词（针对6岁儿童的通识教育）
  systemPrompt = "你是一位友善、耐心的AI陪伴老师，专门为6岁的小朋友提供通识教育。\n"
                 "请遵循以下原则：\n"
                 "1. 用简单易懂的语言，适合6岁儿童的理解水平\n"
                 "2. 回答要生动有趣，可以适当使用比喻和故事\n"
                 "3. 内容要健康积极，绝对不能包含任何成人内容、暴力、恐怖等不适合儿童的内容\n"
                 "4. 如果小朋友问的问题不适合回答，要温和地引导到其他有趣的话题\n"
                 "5. 可以主动介绍一些有趣的知识，比如科学小知识、历史故事、地理常识、自然现象等\n"
                 "6. 保持对话的趣味性和互动性\n"
                 "请用中文回答。";
}

void SiliconFlowLLM::begin(const char* apiKey, const char* baseUrl, const char* model) {
  this->apiKey = String(apiKey);
  this->baseUrl = String(baseUrl);
  this->model = String(model);
}

String SiliconFlowLLM::chat(const String& userInput, const String& conversationHistory) {
  HTTPClient http;
  
  // 构建请求URL
  String url = baseUrl + "/chat/completions";
  http.begin(url);
  
  // 设置请求头
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + apiKey);
  
  // 构建请求体
  DynamicJsonDocument requestDoc(4096);
  requestDoc["model"] = model;
  requestDoc["temperature"] = 0.7;
  
  JsonArray messages = requestDoc.createNestedArray("messages");
  
  // 添加系统提示词
  JsonObject systemMsg = messages.createNestedObject();
  systemMsg["role"] = "system";
  systemMsg["content"] = systemPrompt;
  
  // 添加对话历史（简化处理）
  if (conversationHistory.length() > 0) {
    // TODO: 解析对话历史并添加到messages
  }
  
  // 添加用户输入
  JsonObject userMsg = messages.createNestedObject();
  userMsg["role"] = "user";
  userMsg["content"] = userInput;
  
  String requestBody;
  serializeJson(requestDoc, requestBody);
  
  // 发送请求
  int httpCode = http.POST(requestBody);
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    
    // 解析响应
    DynamicJsonDocument responseDoc(4096);
    deserializeJson(responseDoc, response);
    
    if (responseDoc.containsKey("choices")) {
      JsonArray choices = responseDoc["choices"];
      if (choices.size() > 0) {
        JsonObject choice = choices[0];
        if (choice.containsKey("message")) {
          JsonObject message = choice["message"];
          if (message.containsKey("content")) {
            String content = message["content"].as<String>();
            http.end();
            return content;
          }
        }
      }
    }
  }
  
  http.end();
  return "";
}

void SiliconFlowLLM::end() {
  // 清理资源
}

#endif

