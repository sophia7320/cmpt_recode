# 学习助理 Agent 模板

基于 [OpenCode](https://opencode.ai) 的智能学习助理系统。1 个主 agent + 7 个隐藏子 agent + 14 个专业技能，安装后只需自然语言对话即可完成学习、搜索、整理、评估、规划、导入和可视化。

## 架构总览

```
用户
 │
 ▼
┌──────────────────────────────────────────────────────────┐
│  study-assistant（主入口，用户唯一对话对象）                │
│  · 意图识别  · 科目路由  · 子 agent 调度                  │
│  · 内置 10 项认知心理学学习原则                            │
└────┬──────┬──────┬──────┬──────┬──────┬──────┬───────────┘
     │      │      │      │      │      │      │
     ▼      ▼      ▼      ▼      ▼      ▼      ▼
┌────────┐┌──────┐┌─────┐┌──────┐┌──────┐┌──────┐┌──────────┐
│ setup  ││note  ││eval ││profile││plan ││research││material  │
│assistant││org   ││     ││      ││     ││helper ││importer  │
│        ││      ││     ││      ││     ││      ││          │
│首次设置││笔记  ││水平 ││学习  ││学习  ││搜索  ││外部素材  │
│科目管理││整理  ││评估 ││档案  ││计划  ││真伪  ││导入      │
└────────┘└──────┘└─────┘└──────┘└──────┘└──────┘└──────────┘
     │      │      │      │      │      │      │
     └──────┴──────┴──────┴──────┴──────┴──────┘
                       │
         ┌─────────────┼─────────────┐
         ▼             ▼             ▼
     knowledge-base/  14 个技能     .debug/
     (笔记/档案/计划)  (skill)      (错误日志)
```

## 安装

将本文件夹内容复制到你的 OpenCode 项目：

```bash
# 假设你的项目路径为 ~/my-learning-project

# 1. 复制 AGENTS.md 到项目根目录
cp learning-agent/AGENTS.md ~/my-learning-project/

# 2. 复制 agent 文件
mkdir -p ~/my-learning-project/.opencode/agents
cp learning-agent/agents/*.md ~/my-learning-project/.opencode/agents/

# 3. 复制 skill 目录
cp -r learning-agent/skills/* ~/my-learning-project/.opencode/skills/
```

完成后项目结构：

```
my-learning-project/
├── AGENTS.md
├── .opencode/
│   ├── agents/
│   │   ├── study-assistant.md        ← 主 agent
│   │   ├── setup-assistant.md
│   │   ├── note-organizer.md
│   │   ├── knowledge-evaluator.md
│   │   ├── learning-profile.md
│   │   ├── study-planner.md
│   │   └── research-helper.md
│   └── skills/
│       ├── terminal-math/
│       ├── knowledge-graph/
│       ├── cornell-note/
│       ├── ...（共 12 个）
```

## 首次使用

在项目目录下打开 OpenCode，系统会自动检测 `knowledge-base/` 是否存在：

- **首次使用**：setup-assistant 自动启动，通过对话完成：
  1. 确定要学习的科目
  2. 了解每门课的当前水平、目标
  3. 了解你的学习风格偏好
  4. 确认可用学习时间
  5. 创建知识库目录和初始学习计划

- **已有知识库**：直接进入学习模式

## 使用示例

| 你说的话 | 系统实际做的事 |
|---|---|
| "什么是熵增定律？" | 识别科目→读取档案→匹配风格讲解→整理笔记→记录历史 |
| "帮我搜一下 Transformer 的最新论文" | 调用 research-helper 搜索→综合评估→返回结构化摘要 |
| "整理一下刚才学的微积分笔记" | note-organizer 按康奈尔笔记法整理→写入 knowledge-base |
| "制定两周的线性代数复习计划" | study-planner 反向规划→间隔重复安排→番茄钟分块 |
| "测试一下我对概率论的掌握程度" | knowledge-evaluator 三轮对话式评估→生成诊断报告 |
| "我这周学了什么？" | learning-profile 查询→按科目/时间汇总展示 |
| "我想学数据结构" | setup-assistant 新增科目→创建目录→更新 index |
| "画一张线性代数的知识图谱" | 整理知识关联→调用 Python 脚本生成 PNG/SVG |
| "导入这本电子书到数学" | material-importer 提取 EPUB 文本→SQ3R 处理→存入知识库 |
| "生成我的学习报告" | learning-dashboard 解析数据→生成 6 张可视化图表 |

## Agent 清单

### 主 Agent

| Agent | 职责 |
|---|---|
| **study-assistant** | 统一入口，意图识别、科目路由、教学讲解。内置 10 项认知心理学原则（主动回忆、间隔重复、精加工编码、双重编码、认知负荷管理、刻意练习、成长心态、生成效应、元认知监控、交替练习） |

### 子 Agent（hidden，自动调用）

| Agent | 职责 | 触发场景 |
|---|---|---|
| **setup-assistant** | 首次访谈、新增科目 | 首次启动 / "加一门课" / "新科目" |
| **note-organizer** | 整理笔记为结构化 markdown | 讲解完成 / "整理笔记" / "总结" |
| **knowledge-evaluator** | 三轮对话式诊断评估 | "评估" / "测试" / "我的水平" |
| **learning-profile** | 档案管理、历史记录、习惯分析 | 每次学习结束自动调用 / "进度" / "历史" |
| **study-planner** | 制定学习计划（间隔重复+番茄钟） | "制定计划" / "怎么复习" / "安排" |
| **research-helper** | 网络搜索、信息真伪评估 | "搜索" / "帮我查" / "有哪些" |
| **material-importer** | 导入 PDF/EPUB/TXT/网页，提取并结构化存入知识库 | "导入" / "添加素材" / "把这个加进XX科" |

## Skill 索引

| Skill | 功能 | 适用场景 |
|---|---|---|
| **terminal-math** | LaTeX → 终端可读格式（Unicode + ASCII art） | 所有涉及数学公式的输出（自动触发） |
| **knowledge-graph** | 生成 PNG/SVG 知识关系图 | "画图" / "知识图谱" / 笔记可视化 |
| **cornell-note** | 康奈尔笔记法模板（笔记区+线索栏+总结栏） | 笔记整理（自动调用） |
| **mind-mapping** | 文本缩进层级思维导图 | 笔记末尾知识结构可视化 |
| **feynman-technique** | 费曼学习法（大白话讲解→识别卡顿→简化类比） | 概念讲解（自动调用） |
| **learning-psychology** | 10 项认知心理学原则的教学指令 | 所有教学场景（自动调用） |
| **memory-techniques** | 记忆宫殿/联想/口诀/故事串联/对比图表 | 讲解概念时提供记忆辅助 |
| **error-analysis** | 错误分类（概念/程序/迁移/遗漏）+ 纠正策略 | 评估诊断后结构化输出 |
| **source-evaluation** | CRAAP + SIFT 信息真伪评估 | 搜索结果可信度判断 |
| **sq3r-reading** | SQ3R 阅读法（浏览→提问→精读→复述→回顾） | 深度处理搜索资料 |
| **pomodoro** | 番茄工作法（25+5 分钟分块） | 学习计划中的时间分块 |
| **motivation-strategies** | SDT 动机策略 + SMART 目标 + 里程碑激励 | 学习计划中的激励节点 |
| **document-import** | EPUB 纯文本提取 + 多格式文档导入指南 | 导入外部学习素材 |
| **learning-dashboard** | 学习数据可视化——趋势图、科目饼图、掌握度柱状图、自评校准图、薄弱点排行 | "仪表盘" / "学习报告" / "可视化" |

## 知识库结构

运行时自动生成的 `knowledge-base/` 目录：

```
knowledge-base/
├── index.md                  # 科目列表索引
├── learning-profile.md       # 学习风格档案 + 自评校准分析
├── learning-history.md       # 学习历史时间线
├── 数学/
│   ├── overview.md           # 科目概况
│   ├── study-plan.md         # 学习计划
│   ├── 微积分/
│   │   ├── 极限.md
│   │   └── 导数.md
│   ├── 线性代数/
│   ├── imported/             # 导入的外部素材笔记
│   │   └── 微积分教材笔记.md
│   └── graphs/               # 知识图谱 PNG/SVG
├── 编程/
├── plans/                    # 学习计划文件
│   └── 数学-计划.md
├── evaluations/              # 评估诊断报告
│   └── 数学-2026-04-27.md
└── dashboard/                # 学习数据可视化图表
    ├── summary.png
    ├── trend_learning_time.png
    └── pie_subjects.png
```

## 环境依赖

| 依赖 | 用途 | 安装方式 |
|---|---|---|
| [OpenCode CLI](https://opencode.ai) | Agent 运行平台 | 按官网指引安装 |
| Python 3.8+ | 公式渲染、知识图谱生成 | `python --version` 确认 |
| networkx, matplotlib, pydot | 知识图谱图形渲染 | `pip install -r .opencode/skills/knowledge-graph/requirements.txt` |
| Graphviz（系统包） | 知识图谱树形布局 | `brew install graphviz` / `apt install graphviz` |

> Python 环境缺失时，公式渲染和知识图谱会自动降级为纯文本输出，不影响核心学习功能。

## 自定义

### 教学风格

在首次设置时选择，之后可随时通过对话调整：
- **类比型**：多用生活比喻
- **公式推导型**：强调逻辑链和推导过程
- **图示型**：多用文字描述图表结构
- **综合型**：混合多种方式

### 添加自定义 Agent

在 `.opencode/agents/` 下新建 `.md` 文件，参考现有 agent 的 YAML frontmatter：

```yaml
---
description: 你的 agent 职责描述
mode: subagent
hidden: true
permission:
  write: allow
  edit: allow
---
```

### 添加自定义 Skill

在 `.opencode/skills/` 下新建目录，放入 `SKILL.md`，参考现有 skill 的 frontmatter：

```yaml
---
name: my-skill
description: 技能功能描述
---
```

## 常见问题

**Q: 首次启动报错？**
A: 检查 AGENTS.md 是否在项目根目录，agents/ 和 skills/ 是否在 `.opencode/` 下。

**Q: 知识图谱生成失败？**
A: 检查 `pip install -r .opencode/skills/knowledge-graph/requirements.txt` 和 Graphviz 是否安装。无 Python 环境时自动降级为文本大纲。

**Q: 看到 $...$ 乱码？**
A: terminal-math skill 会自动处理公式渲染。如果出现 raw LaTeX，说明 skill 未正确加载，检查 `.opencode/skills/terminal-math/` 是否存在。

**Q: 如何备份学习数据？**
A: 整个 `knowledge-base/` 目录即全部数据，直接复制或 git 提交即可。

**Q: 如何重置？**
A: 删除 `knowledge-base/` 目录，下次启动时 setup-assistant 会重新初始化。
