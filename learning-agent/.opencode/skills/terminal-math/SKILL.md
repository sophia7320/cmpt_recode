---
name: terminal-math
description: 将 LaTeX 数学公式转化为终端可直接阅读的格式——单字符上下标用 Unicode 替换，复杂公式（分式/积分/求和/矩阵/根号）用 ASCII art 多行渲染。零外部依赖，解析失败时保留原文降级。
---

# 终端数学公式渲染

## 何时使用

- 向用户展示数学公式时
- 将公式写入知识库笔记前
- 任何涉及 `$...$` / `$$...$$` 的输出

## 调用方式

```bash
# 渲染单个表达式
python .opencode/skills/terminal-math/render_math.py --expr "x^2"

# 渲染多行块级公式
python .opencode/skills/terminal-math/render_math.py --expr "\frac{a}{b}" --mode block

# 处理整个 markdown 文件（就地替换）
python .opencode/skills/terminal-math/render_math.py --file knowledge-base/{科目}/{笔记}.md --inplace
```

## 支持的 LaTeX 语法

### 上标与下标
| LaTeX | 终端输出 |
|-------|---------|
| `x^2` | `x²` |
| `x^{n+1}` | `x^(n+1)` |
| `x^{y^2}` | `x^(y²)` |
| `x_1` | `x₁` |
| `x_{i+1}` | `x_(i+1)` |
| `x_i^2` | `xᵢ²` |

### 希腊字母
| LaTeX | 输出 | LaTeX | 输出 |
|-------|------|-------|------|
| `\alpha` | α | `\beta` | β |
| `\gamma` | γ | `\delta` | δ |
| `\epsilon` | ε | `\theta` | θ |
| `\lambda` | λ | `\mu` | μ |
| `\pi` | π | `\sigma` | σ |
| `\phi` | φ | `\omega` | ω |
| `\Gamma` | Γ | `\Delta` | Δ |
| `\Theta` | Θ | `\Lambda` | Λ |
| `\Sigma` | Σ | `\Omega` | Ω |

### 分式
| LaTeX | 终端输出 |
|-------|---------|
| `\frac{a}{b}` | `a / b`（内联）或多行分式线（块级） |
| `\frac{x^2+1}{x-1}` | 多行分数线 |

### 根号
| LaTeX | 终端输出 |
|-------|---------|
| `\sqrt{x}` | `√x` |
| `\sqrt[3]{x}` | `∛x` |

### 积分、累加、极限
| LaTeX | 终端输出 |
|-------|---------|
| `\int_a^b f(x) dx` | 积分符号 + 上下限多行 |
| `\sum_{i=1}^n` | Σ + 上下限多行 |
| `\prod_{i=1}^n` | Π + 上下限多行 |
| `\lim_{x \to 0}` | `lim [x→0]` |

### 常用符号
| LaTeX | 输出 | LaTeX | 输出 |
|-------|------|-------|------|
| `\infty` | ∞ | `\partial` | ∂ |
| `\nabla` | ∇ | `\pm` | ± |
| `\times` | × | `\cdot` | · |
| `\leq` | ≤ | `\geq` | ≥ |
| `\neq` | ≠ | `\approx` | ≈ |
| `\to` / `\rightarrow` | → | `\Rightarrow` | ⇒ |
| `\sin` / `\cos` / `\tan` | sin / cos / tan | `\log` / `\ln` | log / ln |

### 矩阵（块级）
`\begin{pmatrix} a & b \\ c & d \end{pmatrix}` 渲染为 ASCII 矩阵边框。

### 分段函数（块级）
`\begin{cases}` 渲染为条件对齐。

### 行间公式（块级）
`\[ ... \]` 或 `$$...$$` 内容渲染为居中多行。

## 降级规则

不支持以下语法（会原样保留 `$...$` 包裹并注明原因）：
- 复杂矩阵（> 4列）
- TikZ 图形
- 化学式
- \begin{align} 等多行环境

## 笔记中的双格式保存

**简单公式（渲染后 < 30 字符）**：直接内联到正文：
```markdown
导数的定义：f'(x) = lim[h→0] (f(x+h)-f(x))/h
```

**复杂公式（多行输出）**：终端版放入代码块，原始 LaTeX 保留在 HTML 注释中：
```markdown
<!-- $$ \frac{d}{dx} \int_a^x f(t) dt = f(x) $$ -->
```
 d   x
── ∫  f(t) dt = f(x)
dx  a
```

这样用户可以直接在终端中阅读笔记，同时保留了 LaTeX 源码以备后续在支持 MathJax/KaTeX 的环境中渲染。

## 给 Agent 的行动指令

1. 加载本 skill 后，以下场景自动调用脚本：
   - note-organizer 写入包含公式的笔记段落
   - study-assistant 在终端展示包含公式的讲解内容
2. 将 LaTeX 表达式传给 `render_math.py --expr`
3. 根据输出决定内联 or 代码块包裹
4. 脚本执行失败时，手动转换为 Unicode（如 `x^2 → x²`）并标注"公式渲染受限"
