/**
 * 配置文件
 * 请根据实际情况修改以下配置
 */

#ifndef CONFIG_H
#define CONFIG_H

// WiFi配置
#define WIFI_SSID "mamameibabashuai"
#define WIFI_PASSWORD "wojiaxingxingzuikeai"

// 硅基流动API配置
#define SILICONFLOW_API_KEY "sk-ztwctasjprgtzqalhskhybmskeidxblxjdegiengzygecvra"
#define SILICONFLOW_BASE_URL "https://api.siliconflow.cn/v1"
#define SILICONFLOW_MODEL "deepseek-ai/DeepSeek-V3.2"

// 讯飞API配置
#define XUNFEI_APP_ID "4ebec607"
#define XUNFEI_API_KEY "6dd1cf9a8b8923c06fad38041bc89702"
#define XUNFEI_API_SECRET "MTMyYTc4NjkzNTYzZTY0OTAyNDM1NTgx"

// I2S音频配置（ESP32-S3-DevKitC-1）
// 麦克风（INMP441）- 使用I2S_NUM_0
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_4   // I2S_SCK (BCLK)
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_5   // I2S_WS (LRCLK)
#define I2S_MIC_SERIAL_DATA GPIO_NUM_6    // I2S_SD (DATA)

// 扬声器（MAX98357A）- 使用I2S_NUM_1
#define I2S_SPEAKER_SERIAL_CLOCK GPIO_NUM_7   // I2S_BCLK
#define I2S_SPEAKER_LEFT_RIGHT_CLOCK GPIO_NUM_8   // I2S_LRCLK
#define I2S_SPEAKER_SERIAL_DATA GPIO_NUM_9    // I2S_DOUT

// 音频参数
#define SAMPLE_RATE 16000
#define BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_16BIT
#define CHANNELS 1

// 缓冲区大小
#define AUDIO_BUFFER_SIZE 1024

#endif

