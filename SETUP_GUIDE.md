# 服务配置指南

本指南将帮助您完成AI Talking项目的服务配置。

## ✅ 已选定的服务组合

- **大模型**：硅基流动平台（SiliconFlow）
- **语音识别**：科大讯飞语音识别
- **语音合成**：科大讯飞语音合成

---

## 一、硅基流动平台（SiliconFlow）配置

### 1. 注册账号

1. 访问 [硅基流动平台](https://siliconflow.cn)
2. 注册账号并完成实名认证
3. 进入控制台

### 2. 获取API密钥

1. 在控制台中，找到"API密钥"或"API Keys"选项
2. 创建新的API密钥
3. 复制API密钥（格式类似：`sk-xxx...`）

### 3. 选择模型

硅基流动平台提供多种模型，推荐用于教育场景的模型：
- `deepseek-ai/DeepSeek-V3.2`：DeepSeek V3.2，高性能模型（✅ 已选定）
- `qwen-plus`：通义千问Plus，中文理解能力强
- `qwen-turbo`：通义千问Turbo，速度快
- `llama-3.1-8b-instruct`：Llama 3.1，多语言支持好

### 4. 配置到项目

编辑 `config.ini` 文件：

```ini
[llm]
provider = openai
api_key = sk-your_siliconflow_api_key_here
base_url = https://api.siliconflow.cn/v1
model = deepseek-ai/DeepSeek-V3.2
temperature = 0.7
```

**注意**：硅基流动平台使用OpenAI兼容API，所以 `provider` 设置为 `openai`，但需要设置 `base_url` 指向硅基流动的API地址。

---

## 二、科大讯飞语音识别（ASR）配置

### 1. 注册账号

1. 访问 [讯飞开放平台](https://www.xfyun.cn)
2. 注册账号并完成实名认证
3. 进入控制台

### 2. 创建应用

1. 在控制台中，点击"创建新应用"
2. 填写应用信息：
   - 应用名称：AI Talking
   - 应用平台：Web API
   - 应用场景：教育
3. 提交创建

### 3. 开通语音听写服务

1. 在应用详情页面，找到"服务管理"
2. 开通"语音听写"服务
3. 获取以下信息：
   - `APPID`（应用ID）
   - `API Key`
   - `API Secret`

### 4. 安装依赖

```bash
pip install websocket-client requests
```

### 5. 配置到项目

编辑 `config.ini` 文件：

```ini
[asr]
provider = xunfei
app_id = your_xunfei_app_id
api_key = your_xunfei_api_key
api_secret = your_xunfei_api_secret
language = zh
```

### 6. 免费额度说明

- 每天500次免费调用
- 超出后：¥0.003 / 次
- 对于日常使用（每天100次左右），完全在免费额度内

---

## 三、科大讯飞语音合成（TTS）配置

### 1. 使用同一讯飞账号

如果已配置讯飞ASR，可以使用同一账号的应用。

### 2. 开通在线语音合成服务

1. 在已创建的应用详情页面，找到"服务管理"
2. 开通"在线语音合成"服务
3. 使用相同的 `APPID`、`API Key` 和 `API Secret`

### 3. 选择音色

讯飞提供多种音色，推荐用于儿童教育的音色：
- `xiaoyan`：小燕（女声，温柔，推荐）
- `aisxping`：小萍（女声，甜美）
- `aisjiuxu`：许久（男声，沉稳）

更多音色可在讯飞开放平台的"在线语音合成"服务页面查看。

### 4. 安装依赖

如果已安装ASR依赖，则无需重复安装：

```bash
pip install websocket-client requests
```

### 5. 配置到项目

编辑 `config.ini` 文件：

```ini
[tts]
provider = xunfei
app_id = your_xunfei_app_id
api_key = your_xunfei_api_key
api_secret = your_xunfei_api_secret
voice = xiaoyan
```

**注意**：ASR和TTS使用相同的 `app_id`、`api_key` 和 `api_secret`。

### 6. 免费额度说明

- 每天500次免费调用
- 超出后：¥0.01 / 次
- 对于日常使用（每天100次左右），完全在免费额度内

---

## 四、完整配置示例

创建 `config.ini` 文件（复制自 `config.example.ini`）：

```ini
# AI Talking 配置文件

[llm]
provider = openai
api_key = sk-your_siliconflow_api_key
base_url = https://api.siliconflow.cn/v1
model = deepseek-ai/DeepSeek-V3.2
temperature = 0.7

[asr]
provider = xunfei
app_id = your_xunfei_app_id
api_key = your_xunfei_api_key
api_secret = your_xunfei_api_secret
language = zh

[tts]
provider = xunfei
app_id = your_xunfei_app_id
api_key = your_xunfei_api_key
api_secret = your_xunfei_api_secret
voice = xiaoyan

[audio]
volume = 0.8

[content]
filter_enabled = true
age_limit = 6

[education]
topics = 科学,历史,地理,自然,数学,艺术
```

---

## 五、测试配置

### 1. 安装所有依赖

```bash
pip install -r requirements.txt
pip install websocket-client requests
```

### 2. 运行测试

配置完成后，运行以下命令测试：

```bash
python main.py
```

如果配置正确，程序会显示：
```
正在初始化AI Talking...
✓ 大模型提供商: openai
✓ 语音识别提供商: xunfei
✓ 语音合成提供商: xunfei
✓ AI Talking初始化完成！
```

### 3. 测试功能

- 输入文本测试大模型回复
- 测试语音识别功能（需要音频文件）
- 测试语音合成功能

---

## 六、常见问题

### Q: 硅基流动API调用失败

**A:** 
1. 检查API密钥是否正确
2. 检查 `base_url` 是否正确设置为 `https://api.siliconflow.cn/v1`
3. 检查模型名称是否正确（可在硅基流动平台查看可用模型）
4. 检查账户余额是否充足

### Q: 讯飞API调用失败

**A:**
1. 检查 `app_id`、`api_key`、`api_secret` 是否正确
2. 检查是否已开通对应的服务（ASR或TTS）
3. 检查免费额度是否已用完
4. 查看讯飞开放平台的错误日志

### Q: 如何查看讯飞API调用次数和余额？

**A:**
1. 登录讯飞开放平台
2. 进入应用详情页面
3. 查看"服务统计"或"用量统计"

### Q: 如何切换讯飞TTS音色？

**A:**
1. 在讯飞开放平台查看可用音色列表
2. 修改 `config.ini` 中的 `voice` 参数
3. 重启程序

### Q: 超出免费额度怎么办？

**A:**
1. 讯飞ASR：超出500次/天后，按 ¥0.003 / 次计费
2. 讯飞TTS：超出500次/天后，按 ¥0.01 / 次计费
3. 可以在讯飞开放平台充值，或考虑升级套餐

### Q: 如何降低使用成本？

**A:**
1. 合理使用免费额度（每天500次ASR + 500次TTS）
2. 优化对话流程，减少不必要的API调用
3. 考虑使用本地Whisper替代ASR（如果硬件允许）

---

## 七、成本优化建议

1. **合理使用免费额度**
   - 每天500次ASR + 500次TTS足够日常使用
   - 避免不必要的重复调用

2. **优化对话流程**
   - 缓存常用回复
   - 减少重复的语音识别和合成

3. **监控使用量**
   - 定期查看讯飞开放平台的使用统计
   - 设置使用量提醒

---

## 八、下一步

配置完成后，您可以：
1. 运行 `python main.py` 开始使用
2. 测试各项功能
3. 根据实际使用情况调整配置
4. 进行硬件集成（树莓派/ESP32等）

---

## 相关链接

- [硅基流动平台](https://siliconflow.cn)
- [讯飞开放平台](https://www.xfyun.cn)
- [服务选型文档](SERVICE_SELECTION.md)
