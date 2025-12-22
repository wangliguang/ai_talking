/**
 * I2S音频输出模块（扬声器）
 */

#ifndef AUDIO_OUTPUT_H
#define AUDIO_OUTPUT_H

#include <driver/i2s.h>
#include "config.h"

class AudioOutput {
public:
  AudioOutput();
  bool begin();
  bool play(uint8_t* audioData, size_t audioSize);
  void end();
  
private:
  i2s_config_t i2s_config;
  i2s_pin_config_t pin_config;
  bool initialized;
};

AudioOutput::AudioOutput() {
  initialized = false;
}

bool AudioOutput::begin() {
  // 配置I2S
  i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
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
    .bck_io_num = I2S_SPEAKER_SERIAL_CLOCK,
    .ws_io_num = I2S_SPEAKER_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_SPEAKER_SERIAL_DATA,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  
  // 安装I2S驱动
  esp_err_t err = i2s_driver_install(I2S_NUM_1, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("I2S驱动安装失败: %d\n", err);
    return false;
  }
  
  // 设置I2S引脚
  err = i2s_set_pin(I2S_NUM_1, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("I2S引脚设置失败: %d\n", err);
    return false;
  }
  
  initialized = true;
  return true;
}

bool AudioOutput::play(uint8_t* audioData, size_t audioSize) {
  if (!initialized || audioData == nullptr || audioSize == 0) {
    return false;
  }
  
  size_t bytesWritten;
  esp_err_t err = i2s_write(I2S_NUM_1, audioData, audioSize, &bytesWritten, portMAX_DELAY);
  
  return (err == ESP_OK && bytesWritten == audioSize);
}

void AudioOutput::end() {
  if (initialized) {
    i2s_driver_uninstall(I2S_NUM_1);
    initialized = false;
  }
}

#endif

