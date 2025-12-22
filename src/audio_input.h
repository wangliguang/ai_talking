/**
 * I2S音频输入模块（麦克风）
 */

#ifndef AUDIO_INPUT_H
#define AUDIO_INPUT_H

#include <driver/i2s.h>
#include "config.h"

class AudioInput {
public:
  AudioInput();
  bool begin();
  bool available();
  int16_t* read();
  int getBufferSize();
  void end();
  
private:
  i2s_config_t i2s_config;
  i2s_pin_config_t pin_config;
  int16_t* audioBuffer;
  bool initialized;
};

AudioInput::AudioInput() {
  initialized = false;
  audioBuffer = nullptr;
}

bool AudioInput::begin() {
  // 配置I2S
  i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = BITS_PER_SAMPLE,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = AUDIO_BUFFER_SIZE,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };
  
  pin_config = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA
  };
  
  // 安装I2S驱动
  esp_err_t err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("I2S驱动安装失败: %d\n", err);
    return false;
  }
  
  // 设置I2S引脚
  err = i2s_set_pin(I2S_NUM_0, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("I2S引脚设置失败: %d\n", err);
    return false;
  }
  
  // 分配音频缓冲区
  audioBuffer = (int16_t*)malloc(AUDIO_BUFFER_SIZE * sizeof(int16_t));
  if (audioBuffer == nullptr) {
    Serial.println("音频缓冲区分配失败！");
    return false;
  }
  
  initialized = true;
  return true;
}

bool AudioInput::available() {
  if (!initialized) return false;
  
  size_t bytesRead;
  i2s_read(I2S_NUM_0, audioBuffer, AUDIO_BUFFER_SIZE * sizeof(int16_t), &bytesRead, portMAX_DELAY);
  
  return bytesRead > 0;
}

int16_t* AudioInput::read() {
  return audioBuffer;
}

int AudioInput::getBufferSize() {
  return AUDIO_BUFFER_SIZE;
}

void AudioInput::end() {
  if (initialized) {
    i2s_driver_uninstall(I2S_NUM_0);
    if (audioBuffer != nullptr) {
      free(audioBuffer);
      audioBuffer = nullptr;
    }
    initialized = false;
  }
}

#endif

