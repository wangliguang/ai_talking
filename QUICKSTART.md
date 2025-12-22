# 快速开始指南

## 前置要求

1. Python 3.8 或更高版本
2. 大模型API密钥（OpenAI或其他支持的提供商）

## 安装步骤

### 1. 安装Python依赖

```bash
pip install -r requirements.txt
```

**注意**：首次安装Whisper时，会自动下载模型文件（约500MB-1GB），请确保网络畅通。

### 2. 配置API密钥

```bash
# 复制配置文件示例
cp config.example.ini config.ini

# 编辑配置文件，填入你的API密钥
# 可以使用任何文本编辑器打开 config.ini
```

在 `config.ini` 中至少需要配置：

```ini
[llm]
provider = openai
api_key = your_api_key_here  # 替换为你的实际API密钥
```

### 3. 运行程序

```bash
python main.py
```

## 使用说明

### 基本对话

程序启动后，直接输入文本与AI对话：

```
你: 你好
AI: 你好！很高兴认识你！我是你的AI陪伴老师...

你: 为什么天空是蓝色的？
AI: 这是一个很好的问题！天空是蓝色的原因...
```

### 音量控制

```
你: volume 50
音量已设置为: 50%
```

音量范围：0-100

### 音色选择

查看可用音色：
```
你: voice
可用音色:
  1. zh-CN-XiaoxiaoNeural - Female - zh-CN
  2. zh-CN-YunxiNeural - Male - zh-CN
  ...
```

设置音色：
```
你: voice zh-CN-YunxiNeural
音色已设置为: zh-CN-YunxiNeural
```

### 退出程序

```
你: quit
# 或
你: exit
```

## 配置说明

### 大模型配置

支持多种大模型提供商，通过修改 `config.ini` 中的 `[llm]` 部分：

```ini
[llm]
provider = openai          # 提供商名称
api_key = your_key         # API密钥
model = gpt-3.5-turbo     # 模型名称
temperature = 0.7          # 温度参数（0.0-1.0）
```

### 语音识别配置

```ini
[asr]
provider = whisper         # 使用Whisper本地识别
language = zh              # 识别语言：zh（中文）或 en（英文）
```

### 语音合成配置

```ini
[tts]
provider = edge-tts        # 使用edge-tts（免费）
voice = zh-CN-XiaoxiaoNeural  # 默认音色
```

### 音频配置

```ini
[audio]
volume = 0.8               # 默认音量（0.0-1.0）
```

### 内容过滤配置

```ini
[content]
filter_enabled = true      # 启用内容过滤
age_limit = 6              # 年龄限制
```

## 常见问题

### Q: 提示"OpenAI library not installed"
A: 运行 `pip install openai` 安装OpenAI库

### Q: 提示"Whisper not installed"
A: 运行 `pip install openai-whisper` 安装Whisper

### Q: 提示"edge-tts not installed"
A: 运行 `pip install edge-tts` 安装edge-tts

### Q: 语音播放没有声音
A: 
1. 检查系统音量设置
2. 确认已安装pygame：`pip install pygame`
3. 检查音频设备是否正常工作

### Q: API调用失败
A:
1. 检查API密钥是否正确
2. 检查网络连接
3. 检查API余额是否充足
4. 如果使用国内API，可能需要配置 `base_url`

## 下一步

- [ ] 添加语音输入功能（实时录音）
- [ ] 添加更多大模型提供商支持
- [ ] 优化内容过滤机制
- [ ] 添加更多通识教育话题
- [ ] 实现硬件集成（树莓派/ESP32）

