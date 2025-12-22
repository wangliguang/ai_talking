# 软件服务选型文档

## ✅ 已选定的服务组合

- **大模型API**：硅基流动平台（SiliconFlow）✅
  - 模型：`deepseek-ai/DeepSeek-V3.2` ✅
  - API密钥：已配置 ✅
- **语音识别（ASR）**：科大讯飞语音识别 ✅
- **语音合成（TTS）**：科大讯飞语音合成 ✅

---

## 一、大模型API服务：硅基流动平台（SiliconFlow）

### 稳定性与可靠性

- 国内领先的大模型服务平台
- 提供多种主流大模型（Llama、Qwen、DeepSeek等）
- 服务稳定，响应速度快
- 完善的API文档和技术支持
- 支持OpenAI兼容API，易于集成
- 适合教育场景

### 价格

- 多种模型可选，价格从 ¥0.06 / 1M tokens 起
- 提供免费额度（具体以官网为准）
- 支持按需付费，成本可控
- **预估月成本**：假设每天100次对话，每次500 tokens，约**5-15元/月**（取决于选择的模型）

### 配置方式

```ini
[llm]
provider = openai
api_key = your_siliconflow_api_key
model = deepseek-ai/DeepSeek-V3.2  # DeepSeek V3.2（已选定）
base_url = https://api.siliconflow.cn/v1
temperature = 0.7
```

### 推荐模型

- `deepseek-ai/DeepSeek-V3.2`：DeepSeek V3.2，高性能模型，适合教育场景（✅ 已选定）
- `qwen-plus`：通义千问Plus，中文理解能力强
- `qwen-turbo`：通义千问Turbo，速度快
- `llama-3.1-8b-instruct`：Llama 3.1，多语言支持好

### 注册步骤

1. 访问 [硅基流动平台](https://siliconflow.cn)
2. 注册账号并完成实名认证
3. 进入控制台，创建API密钥
4. 复制API密钥并配置到项目中

---

## 二、语音识别服务（ASR）：科大讯飞

### 稳定性与可靠性

- 国内知名语音服务商，行业领先
- 服务稳定，识别准确率高
- 支持实时识别和离线识别
- 完善的SDK和文档
- 支持多种方言和语言
- 企业级服务保障

### 价格

- **免费额度**：每天500次调用
- **超出后**：¥0.003 / 次（标准版）
- **预估月成本**：假设每天100次识别，**完全免费**（在免费额度内）
- 如果超出免费额度，月成本约：¥9 / 月（每天100次 × 30天 = 3000次，超出500次/天部分）

### 配置方式

```ini
[asr]
provider = xunfei
app_id = your_app_id
api_key = your_api_key
api_secret = your_api_secret
language = zh
```

### 注册步骤

1. 访问 [讯飞开放平台](https://www.xfyun.cn)
2. 注册账号并完成实名认证
3. 创建应用，选择"语音听写"服务
4. 获取 `APPID`、`API Key` 和 `API Secret`
5. 配置到项目中

### 优势

- ✅ 识别准确率高，支持多种方言
- ✅ 免费额度足够日常使用（每天500次）
- ✅ 企业级服务保障
- ✅ 与TTS使用同一账号，减少注册步骤

---

## 三、语音合成服务（TTS）：科大讯飞

### 稳定性与可靠性

- 国内知名TTS服务商
- 服务稳定，音质优秀
- 支持多种音色和情感表达
- 完善的SDK和文档
- 企业级服务保障

### 价格

- **免费额度**：每天500次调用
- **超出后**：¥0.01 / 次（标准版）
- **预估月成本**：假设每天100次合成，**完全免费**（在免费额度内）
- 如果超出免费额度，月成本约：¥25 / 月（每天100次 × 30天 = 3000次，超出500次/天部分）

### 配置方式

```ini
[tts]
provider = xunfei
app_id = your_app_id
api_key = your_api_key
api_secret = your_api_secret
voice = xiaoyan  # 音色名称，可选：xiaoyan, aisjiuxu, aisxping等
```

### 可用音色

讯飞提供多种音色选择，常见的有：
- `xiaoyan`：小燕（女声，温柔）
- `aisjiuxu`：许久（男声，沉稳）
- `aisxping`：小萍（女声，甜美）
- 更多音色可在讯飞开放平台查看

### 注册步骤

1. 使用与ASR相同的讯飞开放平台账号
2. 在已创建的应用中，添加"在线语音合成"服务
3. 使用相同的 `APPID`、`API Key` 和 `API Secret`
4. 配置到项目中

### 优势

- ✅ 音质优秀，音色丰富
- ✅ 免费额度足够日常使用（每天500次）
- ✅ 与ASR使用同一账号，只需注册一次
- ✅ 企业级服务保障

---

## 综合方案总结

### 服务组合

- **大模型**：硅基流动平台（SiliconFlow）
- **语音识别**：科大讯飞语音识别
- **语音合成**：科大讯飞语音合成

### 成本估算

| 服务 | 免费额度 | 超出价格 | 月成本（估算） |
|------|---------|---------|---------------|
| 硅基流动 | 有免费额度 | ¥0.06+ / 1M tokens | 5-15元 |
| 讯飞ASR | 500次/天 | ¥0.003 / 次 | 0-9元 |
| 讯飞TTS | 500次/天 | ¥0.01 / 次 | 0-25元 |
| **总计** | - | - | **5-49元/月** |

**说明**：
- 成本估算基于每天100次对话
- 如果使用量在免费额度内，ASR和TTS成本为0
- 实际成本取决于使用量

### 使用门槛

- **需要注册的账号**：2个
  1. 硅基流动平台账号（大模型）
  2. 讯飞开放平台账号（ASR + TTS，同一账号）

### 优势

1. ✅ **稳定性高**：国内知名服务商，企业级保障
2. ✅ **识别准确率高**：讯飞在语音识别领域领先
3. ✅ **音质优秀**：讯飞TTS音质自然流畅
4. ✅ **成本可控**：免费额度足够日常使用
5. ✅ **易于集成**：完善的SDK和文档
6. ✅ **统一账号**：ASR和TTS使用同一讯飞账号

### 适用场景

- ✅ 需要国内服务商（访问速度快）
- ✅ 追求高识别准确率和音质
- ✅ 需要企业级服务保障
- ✅ 使用量在免费额度内或可接受范围内

---

## 完整配置示例

```ini
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

## 下一步行动

1. [ ] 注册硅基流动平台账号并获取API密钥
2. [ ] 注册讯飞开放平台账号并创建应用
3. [ ] 获取讯飞ASR和TTS的API凭证
4. [ ] 安装依赖：`pip install websocket-client requests`
5. [ ] 配置 `config.ini` 文件
6. [ ] 测试各服务功能
7. [ ] 进行压力测试

---

## 相关链接

- [硅基流动平台](https://siliconflow.cn)
- [讯飞开放平台](https://www.xfyun.cn)
- [服务配置指南](SETUP_GUIDE.md)
