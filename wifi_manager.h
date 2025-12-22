/**
 * WiFi管理模块
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
public:
  WiFiManager();
  bool connect(const char* ssid, const char* password);
  bool isConnected();
  void disconnect();
  
private:
  bool connected;
};

WiFiManager::WiFiManager() {
  connected = false;
}

bool WiFiManager::connect(const char* ssid, const char* password) {
  Serial.print("连接WiFi: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    connected = true;
    return true;
  } else {
    connected = false;
    return false;
  }
}

bool WiFiManager::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::disconnect() {
  WiFi.disconnect();
  connected = false;
}

#endif

