# AI Talking - 智能陪伴小工具（ESP32版本）

## 📋 项目概述

这是AI Talking项目的ESP32版本，专为ESP32-S3-DevKitC-1开发板设计，使用**Arduino IDE + C++**开发。这是一个为6岁儿童设计的AI智能陪伴工具，通过语音交互提供通识教育内容。

## ⚠️ 重要说明

- **硬件平台**：ESP32-S3-DevKitC-1 ✅
- **编程语言**：**C++**（Arduino框架）✅
- **开发环境**：**Arduino IDE** ✅

---

## 🎯 项目需求

### 硬件需求
- ✅ 不需要外壳
- ✅ 尽可能少的接线
- ✅ 尽可能使用拔插方式接线

### 软件需求
- ✅ 大模型可自定义配置
- ✅ 陪伴内容主要针对小学生通识教育
- ✅ 内容过滤：不能出现成人内容
- ✅ 音色可选择

---

## 📦 购买清单

### 必需硬件
- [x] **主控板**：ESP32-S3-DevKitC-1-N8R8（8MB Flash + 8MB PSRAM，¥30-50）✅ 已购买
- [x] **麦克风模块**：INMP441 I2S数字麦克风模块（¥5-10）✅ 已购买
- [x] **扬声器模块**：MAX98357A I2S音频功放模块（¥10-15）✅ 已购买
- [x] **扬声器单元**：8Ω/1W小型扬声器（¥3-5，MAX98357A模块通常自带）✅ 已购买
- [x] **杜邦线**：用于连接I2S模块（¥5）✅ 已购买
- [x] **USB线**：USB-C接口，用于编程和供电 ✅ 已购买

**详细选型说明：** 见下方"硬件选型"章节

### 软件服务
- [x] **大模型API**：硅基流动平台（SiliconFlow）- DeepSeek-V3.2 ✅
- [ ] **语音识别**：科大讯飞"语音听写"服务（WebSocket实时识别）
- [ ] **语音合成**：科大讯飞在线语音合成服务

---

## 🎤 硬件选型

### 一、麦克风模块选型

#### ✅ 推荐方案：INMP441 I2S数字麦克风模块

**兼容性确认：** ✅ **完全兼容ESP32-S3-DevKitC-1**
- ESP32-S3集成2个标准I2S接口，支持I2S_NUM_0和I2S_NUM_1
- INMP441使用标准I2S接口，与ESP32-S3完全兼容
- 已在多个ESP32-S3项目中验证可用

**优势：**
- ✅ I2S接口，与ESP32-S3兼容性好
- ✅ 高信噪比（SNR ≥ 64dB），适合语音识别
- ✅ 价格便宜（¥5-10），性价比高
- ✅ 模块化设计，即插即用
- ✅ 3.3V供电，与ESP32-S3匹配
- ✅ 全向型MEMS麦克风，拾音效果好

**技术参数：**
- 接口：I2S数字输出（标准I2S协议）
- 供电电压：3.3V（ESP32-S3提供）
- 信噪比：≥64dB
- 灵敏度：-26dBFS ±2dB
- 采样率：支持16kHz（本项目使用）
- I2S接口：使用I2S_NUM_0（ESP32-S3第一个I2S接口）

**其他可选方案：**
- **IM69D130**（麦克风阵列）：性能更强，但价格高（¥50+），适合远场识别
- **SPH0645LM4H**：类似INMP441，价格略高

**结论：** INMP441是最佳选择，性价比高，完全满足项目需求，与ESP32-S3-DevKitC-1完全兼容。

---

### 二、扬声器/功放模块选型

#### ✅ 推荐方案：MAX98357A I2S数字功放模块

**兼容性确认：** ✅ **完全兼容ESP32-S3-DevKitC-1**
- ESP32-S3集成2个标准I2S接口，支持I2S_NUM_0和I2S_NUM_1
- MAX98357A使用标准I2S接口，与ESP32-S3完全兼容
- 已在多个ESP32-S3项目中验证可用
- ESP32-S3的I2S接口支持10kHz-40MHz的BCK时钟，完全满足16kHz采样率需求

**优势：**
- ✅ I2S接口，与ESP32-S3兼容性好
- ✅ 集成DAC和功放，电路简单
- ✅ 价格便宜（¥10-15），性价比高
- ✅ 模块化设计，即插即用
- ✅ 输出功率：3.2W（8Ω负载），足够使用
- ✅ 自带扬声器接口，连接简单

**技术参数：**
- 接口：I2S数字输入（标准I2S协议）
- 供电电压：5V（ESP32-S3提供5V引脚）
- 输出功率：3.2W（8Ω，10% THD）
- 采样率：支持16kHz（本项目使用）
- 信噪比：≥90dB
- I2S接口：使用I2S_NUM_1（ESP32-S3第二个I2S接口）

**其他可选方案：**
- **AW88298**：性能类似，但价格略高
- **PAM8302**：模拟输入，需要额外DAC，不推荐

**扬声器单元：**
- MAX98357A模块通常自带小型扬声器（8Ω/1W）
- 如需更换，选择8Ω阻抗、1W功率的小型扬声器

**结论：** MAX98357A是最佳选择，集成度高，使用简单，完全满足项目需求，与ESP32-S3-DevKitC-1完全兼容。

---

## 🔧 硬件配置

### ESP32-S3-DevKitC-1 规格
- **CPU**：双核Xtensa LX7，240MHz
- **内存**：512KB SRAM + 8MB PSRAM
- **Flash**：8MB（推荐）或16MB
- **I2S接口**：2个（I2S_NUM_0用于麦克风，I2S_NUM_1用于扬声器）
- **USB**：USB-C接口（编程和供电）

### 硬件连接

**INMP441麦克风模块连接：**
```
INMP441模块引脚    ESP32-S3-DevKitC-1引脚
─────────────────────────────────────────
VDD               → 3.3V
GND               → GND
SCK (BCLK)        → GPIO4
WS (LRCLK)        → GPIO5
SD (DATA)         → GPIO6
```

**MAX98357A扬声器模块连接：**
```
MAX98357A模块引脚  ESP32-S3-DevKitC-1引脚
─────────────────────────────────────────
VIN               → 5V
GND               → GND
BCLK              → GPIO7
LRCLK             → GPIO8
DIN               → GPIO9
```

**连接说明：**
- ✅ **兼容性确认**：INMP441和MAX98357A均使用标准I2S接口，与ESP32-S3-DevKitC-1完全兼容
- INMP441使用I2S_NUM_0接口（输入），供电3.3V
- MAX98357A使用I2S_NUM_1接口（输出），供电5V
- ESP32-S3支持双I2S接口，可同时使用I2S_NUM_0和I2S_NUM_1
- 所有GND共地连接（重要！）
- 确保电源供电充足（建议使用USB供电或外部5V电源）
- 麦克风和扬声器之间保持适当距离，避免回声干扰

**I2S接口兼容性：**
- ESP32-S3的I2S接口支持标准I2S协议
- 支持8/16/24/32位数据模式（本项目使用16位）
- 支持10kHz-40MHz的BCK时钟（本项目使用16kHz采样率）
- 支持主机模式（ESP32-S3作为主机，控制时钟）

---

## 🚀 快速开始

### 1. 安装Arduino IDE

1. 下载 [Arduino IDE](https://www.arduino.cc/en/software)（推荐2.x版本）
2. 安装并打开Arduino IDE

### 2. 安装ESP32开发板支持

1. **文件 → 首选项**
2. 在"附加开发板管理器网址"中添加：
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. **工具 → 开发板 → 开发板管理器**
4. 搜索"ESP32"并安装最新版本（建议2.0.0+）

### 3. 配置ESP32-S3

1. **工具 → 开发板 → ESP32 Arduino → ESP32S3 Dev Module**
2. **工具 → PSRAM → OPI PSRAM** ✅（重要！）
3. **工具 → Partition Scheme → 8M with spiffs**
4. **工具 → Port → 选择ESP32-S3端口**

### 4. 安装必需的库

**工具 → 管理库 → 搜索并安装：**
- **WebSockets** by Markus Sattler（版本2.4.1+）
- **ArduinoJson** by Benoit Blanchon（版本6.21.3+）

### 5. 配置项目

1. **文件 → 打开 → 选择 `ai_talking.ino`**
2. 编辑 `config.h`，填入以下配置：

```cpp
// WiFi配置
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"

// 硅基流动API配置（已配置）
#define SILICONFLOW_API_KEY "sk-ztwctasjprgtzqalhskhybmskeidxblxjdegiengzygecvra"
#define SILICONFLOW_BASE_URL "https://api.siliconflow.cn/v1"
#define SILICONFLOW_MODEL "deepseek-ai/DeepSeek-V3.2"

// 讯飞API配置（待配置）
#define XUNFEI_APP_ID "your_xunfei_app_id"
#define XUNFEI_API_KEY "your_xunfei_api_key"
#define XUNFEI_API_SECRET "your_xunfei_api_secret"
```

### 6. 编译和上传

1. 点击"✓"（验证/编译）按钮
2. 点击"→"（上传）按钮
3. 打开串口监视器（115200波特率）查看日志

**如果上传失败：**
- 按住ESP32-S3的BOOT按钮，然后点击上传
- 尝试降低上传速度：工具 → Upload Speed → 115200

---

## 🔑 服务配置

### 一、硅基流动平台（大模型）

1. 访问 [硅基流动平台](https://siliconflow.cn)
2. 注册账号并完成实名认证
3. 创建API密钥
4. 已配置到项目中（DeepSeek-V3.2模型）

### 二、科大讯飞（语音识别和合成）

#### 注册和创建应用

1. 访问 [讯飞开放平台](https://www.xfyun.cn)
2. 注册账号并完成实名认证
3. 创建新应用（应用平台选择"Web API"）
4. 获取应用凭证：`APPID`、`API Key`、`API Secret`

#### ⚠️ 重要：开通"语音听写"服务

**必须开通"语音听写"服务（WebSocket实时识别），而不是"语音转写"服务。**

1. 在应用详情页面，找到"服务管理"
2. **开通"语音听写"服务**（实时流式识别）
3. 如需购买，选择"语音听写"套餐（不要选"语音转写"）

#### 开通语音合成服务

1. 在应用详情页面，开通"在线语音合成"服务
2. 选择适合的音色（推荐儿童友好音色）

#### 配置到项目

将获取的凭证填入 `config.h`：
```cpp
#define XUNFEI_APP_ID "your_app_id"
#define XUNFEI_API_KEY "your_api_key"
#define XUNFEI_API_SECRET "your_api_secret"
```

---

## 📁 项目结构

```
ai_talking/
├── ai_talking.ino            # 主程序文件（Arduino IDE）
├── config.h                  # 配置文件
├── wifi_manager.h            # WiFi管理模块
├── audio_input.h             # I2S音频输入（麦克风）
├── audio_output.h            # I2S音频输出（扬声器）
├── xunfei_asr.h             # 讯飞语音识别模块
├── xunfei_tts.h             # 讯飞语音合成模块
└── siliconflow_llm.h        # 硅基流动大模型模块
```

---

## 🔧 技术栈

- **硬件平台**：ESP32-S3-DevKitC-1
- **编程语言**：C++（Arduino框架）
- **开发环境**：Arduino IDE
- **语音识别**：科大讯飞语音听写API（WebSocket实时识别）
- **语音合成**：科大讯飞在线语音合成API
- **大模型**：硅基流动平台API（HTTP，DeepSeek-V3.2）

---

## ⚠️ 常见问题

### Q: 编译错误 "WiFi.h: No such file or directory"
**A:** ESP32开发板支持未正确安装，重新安装ESP32开发板支持包

### Q: 编译错误 "WebSocketsClient.h: No such file or directory"
**A:** WebSockets库未安装，工具 → 管理库 → 搜索"WebSockets" → 安装

### Q: 上传失败 "Failed to connect to ESP32"
**A:** 
1. 检查USB线连接
2. 检查端口是否正确选择
3. 按住BOOT按钮，然后点击上传
4. 降低上传速度：工具 → Upload Speed → 115200

### Q: PSRAM未启用
**A:** 工具 → PSRAM → 选择"OPI PSRAM"，重新编译上传

### Q: I2S接口无法工作
**A:** 
1. 检查引脚连接是否正确（INMP441使用GPIO4/5/6，MAX98357A使用GPIO7/8/9）
2. 确认引脚配置支持I2S功能（ESP32-S3的GPIO4-9都支持I2S）
3. 检查电源供电是否充足（INMP441需要3.3V，MAX98357A需要5V）
4. 确认所有GND共地连接
5. 检查I2S驱动是否正确安装（查看串口输出）
6. 查看串口输出的错误信息

### Q: INMP441/MAX98357A是否兼容ESP32-S3？
**A:** ✅ **完全兼容**
- ESP32-S3集成2个标准I2S接口（I2S_NUM_0和I2S_NUM_1）
- INMP441和MAX98357A都使用标准I2S协议
- 已在多个项目中验证可用
- 本项目代码已针对ESP32-S3-DevKitC-1优化配置

### Q: 讯飞ASR连接失败
**A:** 
1. 确认已开通"语音听写"服务（不是"语音转写"）
2. 检查API凭证是否正确
3. 检查网络连接

---

## 📝 项目进度

### ✅ 已完成
- [x] 项目规划和需求分析
- [x] 硬件选型
  - [x] 主控板：ESP32-S3-DevKitC-1 ✅
  - [x] 麦克风模块：INMP441 I2S数字麦克风 ✅
  - [x] 扬声器模块：MAX98357A I2S音频功放 ✅
- [x] 硬件采购 ✅ 已完成
- [x] 软件服务选型（硅基流动、讯飞）
- [x] 代码框架搭建（Arduino IDE + C++）
- [x] I2S音频输入输出模块
- [x] 讯飞ASR/TTS接口实现
- [x] 硅基流动LLM接口实现

### ⏳ 进行中
- [x] 硬件采购 ✅ 已完成
- [ ] 硬件连接和测试
- [ ] 讯飞API凭证配置
- [ ] 功能测试和调试

### 📋 待完成
- [ ] 内容安全性测试
- [ ] 用户体验优化
- [ ] 性能优化

---

## 📌 注意事项

### 硬件注意事项
- 所有内容必须经过安全过滤，确保适合6岁儿童
- 硬件连接尽量使用标准接口（USB、I2S等），便于拔插
- INMP441和MAX98357A都是即插即用模块，连接简单
- 确保所有GND共地连接，避免信号干扰
- 电源供电要充足，建议使用USB供电或外部5V电源
- 麦克风和扬声器之间保持适当距离，避免回声干扰

### 软件注意事项
- 首次使用需要配置WiFi和API密钥
- 确保PSRAM已启用，避免内存不足
- 讯飞ASR必须使用"语音听写"服务（WebSocket实时识别）
- I2S引脚配置已针对ESP32-S3-DevKitC-1优化，无需修改

---

## 🔗 相关资源

- [Arduino IDE官网](https://www.arduino.cc/en/software)
- [ESP32 Arduino文档](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
- [硅基流动平台](https://siliconflow.cn)
- [讯飞开放平台](https://www.xfyun.cn)
- [ESP32-S3-DevKitC-1用户指南](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32s3/hw-reference/esp32s3/user-guide-devkitc-1.html)
