# 学习助理 Agent 模板

这是一个基于 OpenCode 的智能学习助理系统，由 1 个主 agent 调度 6 个隐藏子 agent 协同完成学习任务。

## 安装方式

将本文件夹中的内容放入目标项目：

```
your-project/
├── AGENTS.md          ← 复制本文件到项目根目录
└── .opencode/         ← 将 agents/ 和 skills/ 放进去
    ├── agents/        ← learning-agent/agents/*.md 放这里
    └── skills/        ← learning-agent/skills/*/ 放这里
```

## 项目约定

- 知识库路径：`knowledge-base/`，所有笔记、档案、计划均在此目录下
- 文件格式：所有产出为 markdown（`.md`），确保用户可直接阅读
- 数学公式：使用 `$inline$` 和 `$$block$$` 包裹
- 优先使用中文与用户交流

## Agent 体系

- `study-assistant`：主入口，负责意图识别、科目路由和子 agent 调度
- 所有子 agent 均为 `hidden: true`，用户无需感知其存在
- 子 agent 通过 `knowledge-base/` 下的文件共享上下文

## 维护规则

- 每次学习后，主 agent 必须调用 learning-profile 追加历史记录
- 笔记统一写入 `knowledge-base/{科目}/` 对应目录
- 新增科目通过 setup-assistant 完成，会自动创建所需文件
- 知识库中的 `index.md` 需在每次内容变更时更新索引

## 公式渲染规则（重要）

任何 agent 在**向用户展示**或**写入 knowledge-base** 包含数学公式的内容时，必须确保终端可读：
- 简单公式（纯上下标）：使用 Unicode 直接表示（`x²`、`eˣ`、`aᵢ`），不得输出 raw LaTeX `$...$`
- 复杂公式（分式/积分/矩阵）：调用 `python .opencode/skills/terminal-math/render_math.py --expr "公式"` 获取终端版再输出
- 写入笔记/报告前，先调用 `python .opencode/skills/terminal-math/render_math.py --file {路径} --inplace` 就地转换
- 违反此规则导致用户看到 `$...$` raw LaTeX 是不可接受的

## 调试日志

任何工具调用或子 agent 调用失败时，必须使用 bash 将错误信息追加写入 `.opencode/.debug/errors.log`：

格式（JSONL，一行一条）：
```json
{"ts":"{ISO8601时间}","agent":"{当前agent名}","tool":"{工具名}","error":"{错误摘要，1行}"}
```

示例：
```bash
bash -c "echo '{\"ts\":\"2026-01-15T14:30:00\",\"agent\":\"study-assistant\",\"tool\":\"task\",\"error\":\"knowledge-evaluator returned no output\"}' >> .opencode/.debug/errors.log"
```

- 时间必须用 ISO8601 格式（如 2026-01-15T14:30:00）
- error 字段用一句话描述失败原因，不包含换行
- 确保 `.opencode/.debug/` 目录存在后再写入

## 环境依赖

- [OpenCode](https://opencode.ai) CLI 工具
- Python 3.8+（用于知识图谱生成和公式渲染）
- 知识图谱图形生成需要额外安装：
  ```bash
  pip install -r .opencode/skills/knowledge-graph/requirements.txt
  ```
  以及 Graphviz 系统包：`brew install graphviz`（macOS）或 `apt install graphviz`（Linux）
