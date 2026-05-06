#!/usr/bin/env python3
"""EPUB 纯文本提取器

纯标准库实现（zipfile + xml.etree.ElementTree），零外部依赖。
EPUB 本质上是 ZIP 压缩包，内含 XHTML/HTML 文件。

用法:
  python extract_epub.py book.epub
  python extract_epub.py book.epub --output book.txt
  python extract_epub.py book.epub --toc-only
"""

import argparse
import html
import os
import re
import sys
import xml.etree.ElementTree as ET
import zipfile

# XHTML 命名空间
NS = {"xhtml": "http://www.w3.org/1999/xhtml", "html": "http://www.w3.org/1999/xhtml"}

# 需要跳过的标签
SKIP_TAGS = {"script", "style", "nav", "footer", "head", "link", "meta"}

# 需要在这些标签后插入换行
BLOCK_TAGS = {
    "p", "div", "h1", "h2", "h3", "h4", "h5", "h6",
    "li", "br", "hr", "tr", "section", "article", "header",
    "blockquote", "pre", "table",
}


def strip_ns(tag: str) -> str:
    """去除 XML 命名空间前缀，返回本地标签名"""
    if "}" in tag:
        return tag.split("}", 1)[1]
    return tag


def extract_text_from_xhtml(content: str) -> str:
    """从 XHTML 字符串中提取纯文本"""
    try:
        root = ET.fromstring(content)
    except ET.ParseError:
        # 尝试用宽松的 HTML 解析（直接正则清理）
        text = re.sub(r"<script[^>]*>.*?</script>", "", content, flags=re.DOTALL | re.IGNORECASE)
        text = re.sub(r"<style[^>]*>.*?</style>", "", text, flags=re.DOTALL | re.IGNORECASE)
        text = re.sub(r"<[^>]+>", "", text)
        text = html.unescape(text)
        return clean_whitespace(text)

    lines = []

    def walk(elem, depth=0):
        tag = strip_ns(elem.tag)

        if tag in SKIP_TAGS:
            return

        text = elem.text or ""
        tail = elem.tail or ""

        if tag in BLOCK_TAGS:
            if lines and lines[-1] != "":
                lines.append("")

        if tag.startswith("h") and len(tag) == 2 and tag[1].isdigit():
            level = int(tag[1])
            prefix = "#" * level + " "
            processed = html.unescape(text.strip())
            if processed:
                lines.append(prefix + processed)
        else:
            processed = html.unescape(text)
            if processed.strip():
                if tag in ("li",):
                    lines.append("  - " + processed.strip())
                elif tag in BLOCK_TAGS:
                    lines.append(processed.strip())
                else:
                    if lines and not lines[-1].endswith(" ") and not lines[-1] == "":
                        lines[-1] += processed
                    else:
                        lines.append(processed)

        for child in elem:
            walk(child, depth + 1)

        tl = html.unescape(tail)
        if tl.strip():
            if lines and not lines[-1].endswith(" ") and not lines[-1] == "":
                lines[-1] += tl
            else:
                lines.append(tl)

    walk(root)
    return clean_whitespace("\n".join(lines))


def clean_whitespace(text: str) -> str:
    """清理多余空行和空白字符"""
    text = re.sub(r" +", " ", text)
    text = re.sub(r"\n{3,}", "\n\n", text)
    text = re.sub(r"^\s*\n", "\n", text)
    return text.strip()


def get_epub_toc(zf: zipfile.ZipFile) -> list[dict]:
    """从 EPUB 中提取目录信息"""
    # 查找 container.xml 获取 OPF 路径
    try:
        container = zf.read("META-INF/container.xml").decode("utf-8")
        m = re.search(r'full-path="([^"]+)"', container)
        if not m:
            return []
        opf_path = m.group(1)
    except (KeyError, zipfile.BadZipFile):
        return []

    # 解析 OPF 文件
    opf_dir = os.path.dirname(opf_path)
    try:
        opf_content = zf.read(opf_path).decode("utf-8")
    except KeyError:
        return []

    opf_root = ET.fromstring(opf_content)

    # 构建 manifest id → href 映射
    manifest = {}
    for item in opf_root.iter():
        tag = strip_ns(item.tag)
        if tag == "item":
            item_id = item.get("id", "")
            href = item.get("href", "")
            if item_id and href:
                full_href = os.path.normpath(os.path.join(opf_dir, href))
                manifest[item_id] = full_href

    # 解析 spine（阅读顺序）
    toc = []
    seen = set()
    for item in opf_root.iter():
        tag = strip_ns(item.tag)
        if tag == "itemref":
            ref = item.get("idref", "")
            if ref in manifest and ref not in seen:
                seen.add(ref)
                href = manifest[ref]
                label = item.get("linear", "yes")
                toc.append({"id": ref, "href": href, "label": href})

    # 尝试从 NCX 获取章节标题
    ncx_path = None
    for item in opf_root.iter():
        tag = strip_ns(item.tag)
        if tag == "item":
            mtype = item.get("media-type", "")
            if mtype == "application/x-dtbncx+xml":
                href = item.get("href", "")
                ncx_path = os.path.normpath(os.path.join(opf_dir, href))
                break

    if ncx_path:
        try:
            ncx_content = zf.read(ncx_path).decode("utf-8")
            ncx_root = ET.fromstring(ncx_content)
            ncx_ns = "http://www.daisy.org/z3986/2005/ncx/"
            nav_map = None
            for child in ncx_root:
                if strip_ns(child.tag) == "navMap":
                    nav_map = child
                    break

            if nav_map is not None:
                titles_map = {}

                def extract_nav(elem, depth=0):
                    for child in elem:
                        tag = strip_ns(child.tag)
                        if tag == "navPoint":
                            nid = child.get("id", "")
                            title_text = ""
                            src = ""
                            for nav_child in child:
                                nav_tag = strip_ns(nav_child.tag)
                                if nav_tag == "navLabel":
                                    for lbl in nav_child:
                                        if strip_ns(lbl.tag) == "text":
                                            title_text = (lbl.text or "").strip()
                                elif nav_tag == "content":
                                    src = nav_child.get("src", "")
                            if src:
                                full_src = os.path.normpath(os.path.join(opf_dir, src.split("#")[0]))
                                titles_map[full_src] = title_text
                            extract_nav(child, depth + 1)

                extract_nav(nav_map)

                if titles_map:
                    for entry in toc:
                        href = entry["href"]
                        if href in titles_map and titles_map[href]:
                            entry["title"] = titles_map[href]
                        else:
                            entry["title"] = os.path.basename(href)
        except Exception:
            for entry in toc:
                entry["title"] = os.path.basename(entry["href"])

    return toc


def extract_epub(epub_path: str) -> tuple[str, list[dict]]:
    """提取 EPUB 全文，返回 (纯文本, 目录列表)"""
    if not os.path.exists(epub_path):
        raise FileNotFoundError(f"文件不存在: {epub_path}")

    text_parts = []
    with zipfile.ZipFile(epub_path, "r") as zf:
        toc = get_epub_toc(zf)

        # 按 spine 顺序提取各章节
        extracted = set()
        for entry in toc:
            href = entry["href"]
            if href in extracted:
                continue
            extracted.add(href)

            title = entry.get("title", os.path.basename(href))
            text_parts.append(f"## {title}")

            try:
                content = zf.read(href).decode("utf-8")
                chapter_text = extract_text_from_xhtml(content)
                if chapter_text:
                    text_parts.append(chapter_text)
                else:
                    text_parts.append("(本部分无可提取的文字内容)")
            except (KeyError, UnicodeDecodeError) as e:
                text_parts.append(f"(读取失败: {e})")

        # 提取未在 spine 中的 HTML/XHTML 文件（额外内容）
        for name in zf.namelist():
            if name in extracted:
                continue
            ext = os.path.splitext(name)[1].lower()
            if ext in (".html", ".htm", ".xhtml", ".xml"):
                try:
                    content = zf.read(name).decode("utf-8")
                    chapter_text = extract_text_from_xhtml(content)
                    if chapter_text and len(chapter_text) > 50:
                        text_parts.append(f"## [附] {os.path.basename(name)}")
                        text_parts.append(chapter_text)
                except Exception:
                    pass

    full_text = "\n\n".join(text_parts)
    return full_text, toc


def main():
    parser = argparse.ArgumentParser(
        description="EPUB 纯文本提取器——零依赖标准库实现"
    )
    parser.add_argument("epub", help="EPUB 文件路径")
    parser.add_argument("--output", "-o", default=None, help="输出文本文件路径")
    parser.add_argument("--toc-only", action="store_true", help="仅显示目录，不提取全文")
    args = parser.parse_args()

    try:
        if args.toc_only:
            with zipfile.ZipFile(args.epub, "r") as zf:
                toc = get_epub_toc(zf)
            if not toc:
                print("未找到目录信息")
                sys.exit(1)
            print(f"\n{'=' * 60}")
            print(f"  目录：{os.path.basename(args.epub)}")
            print(f"{'=' * 60}\n")
            for i, entry in enumerate(toc, 1):
                title = entry.get("title", os.path.basename(entry["href"]))
                print(f"  {i:3d}. {title}")
                print(f"       文件: {entry['href']}\n")
            print(f"{'=' * 60}")
            print(f"  共 {len(toc)} 个章节")
            return

        full_text, toc = extract_epub(args.epub)

        header = f"# {os.path.basename(args.epub)}\n"
        header += f"来源: {os.path.abspath(args.epub)}\n"
        header += f"章节数: {len(toc)}\n\n---\n\n"

        output = header + full_text

        if args.output:
            output_dir = os.path.dirname(args.output)
            if output_dir and not os.path.exists(output_dir):
                os.makedirs(output_dir, exist_ok=True)
            with open(args.output, "w", encoding="utf-8") as f:
                f.write(output)
            print(f"已提取至: {args.output}")
            print(f"总字符数: {len(full_text)}")
            print(f"章节数:   {len(toc)}")
        else:
            print(output)

    except FileNotFoundError as e:
        print(f"错误: {e}", file=sys.stderr)
        sys.exit(1)
    except zipfile.BadZipFile:
        print(f"错误: {args.epub} 不是有效的 EPUB/ZIP 文件", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"错误: {type(e).__name__}: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
