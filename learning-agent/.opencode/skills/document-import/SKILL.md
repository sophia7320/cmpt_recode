---
name: document-import
description: 外部文档导入技能——支持 PDF、EPUB、TXT、Markdown 文件和网页 URL 的导入，包含 EPUB 纯文本提取脚本、内容结构化处理指南和输出格式规范。
---

# 外部文档导入技能

## 核心理念

将外部学习素材转化为知识库中结构化、可检索、与已有知识关联的学习笔记。

## 何时使用

- 用户提供 PDF/EPUB/TXT/MD 文件路径要求导入
- 用户提供网页 URL 要求导入
- 用户说"把这篇资料加到XX科目里"
- research-helper 搜索到有价值的资料，需要存为本地笔记

## 素材类型处理指南

### PDF 文件
- 使用 Read 工具读取，无需额外脚本
- 注意：扫描版 PDF（图片型）无法提取文字，需告知用户
- 学术论文关注：摘要、引言、结论、方法、图表标题
- 教材章节关注：标题层级、粗体定义、公式、例题

### EPUB 文件
- 调用 `python .opencode/skills/document-import/extract_epub.py <路径> --output .opencode/.tmp/epub_out.txt`
- 提取后读取 `.opencode/.tmp/epub_out.txt` 获取纯文本
- EPUB 通常按章节分文件，脚本自动合并为一个文本

### TXT / Markdown 文件
- 直接使用 Read 工具读取
- Markdown 文件保留原有的标题层级结构

### 网页 URL
- 使用 WebFetch 工具抓取
- 网页内容可能包含大量导航、广告等噪音，需在笔记中过滤
- 注意标注原始 URL 和抓取时间

## EPUB 提取脚本

位置：`.opencode/skills/document-import/extract_epub.py`

```bash
# 基本用法
python .opencode/skills/document-import/extract_epub.py book.epub

# 指定输出文件
python .opencode/skills/document-import/extract_epub.py book.epub --output book_text.txt

# 仅显示章节列表（不提取全文）
python .opencode/skills/document-import/extract_epub.py book.epub --toc-only
```

## 内容处理规范

### 优先级排序
处理长篇素材时按以下优先级提取：
1. 核心定义和定理
2. 公式和推导过程
3. 示例和案例
4. 背景和延伸阅读

### 知识关联
导入内容时主动与已有知识库关联：
- 检查 `knowledge-base/index.md` 中的已有笔记
- 在"关键概念速查"表中标注与已有知识的关联
- 如果新内容与已有笔记冲突，标注差异而非覆盖

### 可信度评估
每条导入的素材必须包含可信度说明：
```
可靠性评级：⭐⭐⭐ 高 / ⭐⭐ 中 / ⭐ 低
评估方法：CRAAP / SIFT
关键依据：
  - 权威性：{作者/机构背景}
  - 准确性：{是否有同行评审/交叉验证}
  - 时效性：{发布/更新时间}
注意事项：{偏见/商业动机/局限}
```

## 输出格式

导入笔记统一放在 `knowledge-base/{科目}/imported/` 目录下，文件名用素材来源命名（去除特殊字符，限 50 字符）。

## 反模式

- 直接复制原文大段文字而不加提炼
- 跳过可信度评估
- 导入后不更新 index.md
- 将多个不相关的素材合并为一篇笔记
- 用英文处理中文素材
- 忽略素材中的图表描述（至少标注"原文含图表：[描述]"，建议查看原文）
