---
name: knowledge-graph
description: 将学习内容构造为知识关系图，支持概念图谱、知识树、学习路径图三种模式的 PNG/SVG 图形输出。通过 Python 脚本 (generate_graph.py) 渲染图形，无 Python 环境时自动降级为文本版大纲。
---

# 知识图形化生成

## 何时使用

- 笔记整理完成后，为章节生成概念关系图
- 用户要求"画一下XX的知识图谱"
- 评估完成后生成着色掌握度知识图谱
- 制定计划时生成学习路径图

## 三种图形模式

### 模式 1：概念关系图 (graph)
以节点和边表示知识点间的关联关系。
- 节点：知识点名称
- 边：前置依赖 / 相似关联 / 对比关系
- 适用：展示某一章节内部概念网络

### 模式 2：知识树图 (tree)
以层级结构展示学科章节框架。
- 根节点：学科名或大主题
- 子节点：章节 → 小节 → 知识点
- 适用：展示完整科目的知识结构

### 模式 3：学习路径图 (path)
以有向图展示推荐的学习顺序。
- 节点按学习顺序排列
- 边标注前置依赖关系
- 适用：制定学习计划时的路线可视化

## 输入数据格式

准备 JSON 数据文件，写入临时路径后调用脚本：

```json
{
  "title": "图标题",
  "mode": "graph|tree|path",
  "nodes": [
    {"id": "unique_id", "label": "显示名称", "group": "章节", "mastery": 3}
  ],
  "edges": [
    {"from": "source_id", "to": "target_id", "label": "关系标签"}
  ]
}
```

字段说明：
- `id`：唯一标识符（英文/拼音，用于节点间引用）
- `label`：显示名称（中文）
- `group`：(可选) 用于按组着色
- `mastery`：(可选) 1-5 掌握度，用于红/黄/绿着色
- `from` / `to`：边的起点和终点节点 id
- `label`：(可选) 边上的文字标注

## 调用方式

```bash
python .opencode/skills/knowledge-graph/generate_graph.py \
  --mode graph \
  --data /tmp/kg_data.json \
  --output knowledge-base/{科目}/graphs/{图名}.png \
  --format png \
  --lang zh
```

## 输出

- 默认生成 PNG（1920x1080, dpi 150），可选 SVG
- 保存至 `knowledge-base/{科目}/graphs/` 目录
- 生成后告知用户文件路径

## 环境要求

- Python 3.8+
- networkx, matplotlib, pydot
- 安装命令：`pip install -r .opencode/skills/knowledge-graph/requirements.txt`
- Graphviz 系统包（用于树形布局）：`brew install graphviz` / `apt install graphviz`

## 降级策略

如果 Python 环境不可用或脚本执行失败，自动降级输出文本版大纲：
```
## {图标题}

概念关系图（文本版）：

{节点1}
├── 前置关联 → {节点2}
├── 相似关联 ~~ {节点3}
└── 对比关联 ≠ {节点4}
```
