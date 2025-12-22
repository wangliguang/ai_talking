#!/bin/bash

# GitHub 推送脚本
# 使用方法: ./push_to_github.sh YOUR_GITHUB_USERNAME

if [ -z "$1" ]; then
    echo "❌ 错误: 请提供您的 GitHub 用户名"
    echo "使用方法: ./push_to_github.sh YOUR_GITHUB_USERNAME"
    exit 1
fi

GITHUB_USERNAME=$1
REPO_NAME="ai_talking"

echo "🚀 准备推送到 GitHub..."
echo "📦 仓库: $GITHUB_USERNAME/$REPO_NAME"
echo ""

# 检查是否已有远程仓库
if git remote get-url origin &>/dev/null; then
    echo "⚠️  远程仓库已存在，更新中..."
    git remote set-url origin git@github.com:$GITHUB_USERNAME/$REPO_NAME.git
else
    echo "➕ 添加远程仓库..."
    git remote add origin git@github.com:$GITHUB_USERNAME/$REPO_NAME.git
fi

# 确保在 main 分支
git branch -M main

# 推送代码
echo "📤 推送代码到 GitHub..."
git push -u origin main

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ 成功推送到 GitHub!"
    echo "🔗 访问: https://github.com/$GITHUB_USERNAME/$REPO_NAME"
else
    echo ""
    echo "❌ 推送失败，请检查："
    echo "   1. GitHub 仓库是否已创建"
    echo "   2. SSH 密钥是否正确配置"
    echo "   3. 仓库名称是否正确"
fi

