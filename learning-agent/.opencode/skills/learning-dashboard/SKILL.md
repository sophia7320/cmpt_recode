---
name: learning-dashboard
description: 学习数据可视化仪表盘——从学习历史生成学习时长趋势图、科目分布饼图、掌握度分布图、自评校准对比图和薄弱点排行图，帮助用户直观了解学习状况和数据趋势。
---

# 学习数据可视化仪表盘

## 何时使用

- 用户说"仪表盘"、"学习报告"、"可视化"、"图表"、"我的学习数据"
- 用户说"查看进度"并希望看到图形化展示
- 定期（每周/每月）回顾学习状况
- 发现薄弱点后需要直观对比

## 生成的图表

脚本 `generate_dashboard.py` 解析 `knowledge-base/` 下的 `learning-history.md` 和 `learning-profile.md`，生成 6 张 PNG 图表：

| 图表 | 文件名 | 说明 |
|------|--------|------|
| 学习时长趋势 | trend_learning_time.png | 每日学习时长变化折线图 |
| 科目分布 | pie_subjects.png | 各科目学习时长占比饼图 |
| 掌握度分布 | bar_mastery.png | 1-5 分掌握度柱状图 |
| 自评校准 | bar_calibration.png | 用户自评 vs 评估实际得分对比 |
| 薄弱点追踪 | bar_weaknesses.png | 顽固薄弱点出现次数排行 |
| 综合摘要 | summary.png | 关键指标卡片 + 近期学习时间线 |

## 调用方式

```bash
# 生成所有独立图表
python .opencode/skills/learning-dashboard/generate_dashboard.py \
  --data-dir knowledge-base/ \
  --output knowledge-base/dashboard/

# 生成单张综合摘要大图
python .opencode/skills/learning-dashboard/generate_dashboard.py \
  --data-dir knowledge-base/ \
  --output knowledge-base/dashboard/summary.png \
  --combine
```

## 输出

- PNG 图表保存至 `knowledge-base/dashboard/` 目录
- 无数据时生成友好的占位提示图，不会崩溃
- 生成后告知用户文件路径，用户可以打开查看

## 环境要求

- Python 3.8+
- matplotlib（已包含在 knowledge-graph 的 requirements.txt 中）
- 中文字体（自动检测系统可用字体）

## 数据来源

脚本自动解析以下文件：
- `knowledge-base/learning-history.md`：学习记录表格
- `knowledge-base/learning-profile.md`：自评校准和薄弱点追踪表

如果这些文件不存在或数据不足，对应图表将生成占位提示。

## 反模式

- 每次对话都生成仪表盘（属于低频操作，仅在用户明确请求时生成）
- 数据不足时强行生成无意义图表（脚本已自动处理）
- 用英文展示中文学习数据
