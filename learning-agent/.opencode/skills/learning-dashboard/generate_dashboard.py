#!/usr/bin/env python3
"""学习数据可视化仪表盘生成器

解析 knowledge-base/ 中的学习数据，生成可视化图表 PNG 文件。

依赖: matplotlib（与 knowledge-graph 共用）
无数据时生成友好的占位提示图，不会崩溃。

用法:
  python generate_dashboard.py --data-dir knowledge-base/ --output knowledge-base/dashboard/
  python generate_dashboard.py --data-dir knowledge-base/ --output dashboard.png --combine
"""

import argparse
import os
import re
import sys
from datetime import datetime, timedelta


def find_chinese_font():
    """查找系统中可用的中文字体"""
    candidates = []
    if sys.platform == "win32":
        candidates = [
            "Microsoft YaHei",
            "SimHei",
            "SimSun",
            "FangSong",
            "KaiTi",
        ]
    elif sys.platform == "darwin":
        candidates = [
            "PingFang SC",
            "Heiti SC",
            "STHeiti",
            "Hiragino Sans GB",
            "Apple LiGothic",
        ]
    else:
        candidates = [
            "Noto Sans CJK SC",
            "Noto Sans CJK",
            "WenQuanYi Micro Hei",
            "WenQuanYi Zen Hei",
            "Droid Sans Fallback",
        ]
    return candidates


def configure_matplotlib():
    """配置 matplotlib 中文字体"""
    import matplotlib
    import matplotlib.pyplot as plt

    matplotlib.use("Agg")
    fonts = find_chinese_font()
    plt.rcParams["font.sans-serif"] = fonts
    plt.rcParams["axes.unicode_minus"] = False


def parse_markdown_table(content: str, table_header_pattern: str) -> list[dict]:
    """解析 markdown 表格，返回 dict 列表。跳过表头和分隔行。"""
    lines = content.split("\n")
    rows = []
    headers = []
    in_table = False

    for line in lines:
        line = line.strip()
        if not line.startswith("|"):
            in_table = False
            continue

        cells = [c.strip() for c in line.strip("|").split("|")]

        if not in_table:
            headers = cells
            in_table = True
            continue

        # 跳过分隔行 (|---|---|)
        if all(re.match(r"^:?-{3,}:?$", c) for c in cells):
            continue

        if len(cells) == len(headers):
            row = {}
            for h, c in zip(headers, cells):
                row[h] = c
            # 跳过占位行
            if not all(v in ("", "-", "...") for v in row.values()):
                rows.append(row)

    return rows


def extract_minutes(time_str: str) -> float:
    """从时间字符串中提取分钟数。支持：'30'、'35分钟'、'🍅x2' → 50min"""
    if not time_str:
        return 0
    s = str(time_str).strip()
    # 番茄钟格式：🍅x2 → 50min
    tomato_match = re.search(r"[🍅]?\s*x\s*(\d+)", s)
    if tomato_match:
        return int(tomato_match.group(1)) * 25
    # 数字提取
    num_match = re.search(r"(\d+(?:\.\d+)?)", s)
    if num_match:
        return float(num_match.group(1))
    return 0


def parse_mastery(val: str) -> float | None:
    """解析掌握度值 1-5"""
    if not val:
        return None
    s = str(val).strip()
    num_match = re.search(r"(\d+(?:\.\d+)?)", s)
    if num_match:
        v = float(num_match.group(1))
        if 1 <= v <= 5:
            return v
    return None


def load_learning_history(data_dir: str) -> list[dict]:
    """加载并解析学习历史数据"""
    path = os.path.join(data_dir, "learning-history.md")
    if not os.path.exists(path):
        return []

    with open(path, "r", encoding="utf-8") as f:
        content = f.read()

    raw_rows = parse_markdown_table(content, "")
    rows = []
    for r in raw_rows:
        date_str = r.get("日期", "")
        subject = r.get("科目", "")
        content_summary = r.get("内容摘要", "")
        time_str = r.get("耗时(分钟)", "")
        mastery_str = r.get("掌握度(1-5)", r.get("掌握度", ""))
        note = r.get("备注", r.get("教学风格", ""))

        # 跳过含"待数据积累"的占位行
        if "待数据积累" in date_str or "待数据积累" in content_summary:
            continue

        minutes = extract_minutes(time_str)
        mastery = parse_mastery(mastery_str)

        rows.append({
            "date": date_str,
            "subject": subject,
            "content": content_summary,
            "minutes": minutes,
            "mastery": mastery,
            "note": note,
        })

    return rows


def load_learning_profile(data_dir: str) -> dict:
    """加载学习档案中的自评校准和薄弱点数据"""
    path = os.path.join(data_dir, "learning-profile.md")
    if not os.path.exists(path):
        return {"calibration": [], "weaknesses": []}

    with open(path, "r", encoding="utf-8") as f:
        content = f.read()

    result = {"calibration": [], "weaknesses": [], "subjects": {}}

    # 解析自评准确性表
    cal_match = re.search(
        r"## 自评准确性.*?\n(.*?)(?:\n## |\Z)",
        content, re.DOTALL
    )
    if cal_match:
        cal_rows = parse_markdown_table(cal_match.group(1), "")
        for r in cal_rows:
            date_val = r.get("日期", "")
            if date_val and date_val != "-" and "待数据积累" not in date_val:
                result["calibration"].append({
                    "date": date_val,
                    "scope": r.get("评估范围", ""),
                    "self_avg": parse_mastery(r.get("自评均分", "")),
                    "actual_avg": parse_mastery(r.get("实际均分", "")),
                    "bias": r.get("偏差", ""),
                })

    # 解析薄弱点追踪
    weak_match = re.search(
        r"## 薄弱点追踪.*?\n(.*?)(?:\n## |\Z)",
        content, re.DOTALL
    )
    if weak_match:
        weak_rows = parse_markdown_table(weak_match.group(1), "")
        for r in weak_rows:
            name = r.get("知识点", "")
            if name and name != "-" and "待数据积累" not in name:
                count_str = r.get("次数", r.get("出现次数", "1"))
                try:
                    count = int(count_str)
                except ValueError:
                    count = 1
                result["weaknesses"].append({
                    "name": name,
                    "subject": r.get("科目", ""),
                    "count": count,
                    "trend": r.get("趋势", ""),
                })

    # 解析各科目状态
    subj_match = re.search(
        r"## 各科目状态.*?\n(.*?)(?:\n## |\Z)",
        content, re.DOTALL
    )
    if subj_match:
        subj_rows = parse_markdown_table(subj_match.group(1), "")
        for r in subj_rows:
            name = r.get("科目", "")
            if name and name != "-" and "逐科填写" not in name:
                result["subjects"][name] = {
                    "level": r.get("当前水平", ""),
                    "goal": r.get("学习目标", ""),
                    "deadline": r.get("截止日期", ""),
                    "style": r.get("偏好风格", ""),
                }

    return result


def generate_empty_chart(title: str, message: str, output_path: str):
    """生成空白占位图"""
    import matplotlib.pyplot as plt

    fig, ax = plt.subplots(figsize=(10, 6))
    ax.text(
        0.5, 0.5, message,
        transform=ax.transAxes,
        ha="center", va="center",
        fontsize=14, color="#999999",
    )
    ax.set_title(title, fontsize=16, fontweight="bold", pad=15)
    ax.axis("off")
    plt.tight_layout()
    plt.savefig(output_path, dpi=150, bbox_inches="tight")
    plt.close()


def chart_learning_time(history: list[dict], output_path: str):
    """图表1：每日学习时长趋势图"""
    import matplotlib.pyplot as plt
    import matplotlib.dates as mdates

    if not history:
        generate_empty_chart("学习时长趋势", "暂无学习记录\n开始学习后这里将显示每日学习时长变化", output_path)
        return

    # 按日期聚合
    daily = {}
    for h in history:
        d = h["date"]
        if d not in daily:
            daily[d] = 0
        daily[d] += h["minutes"]

    # 排序日期
    sorted_dates = sorted(daily.keys())
    if len(sorted_dates) < 2:
        generate_empty_chart("学习时长趋势", "需要至少 2 天的数据才能生成趋势图\n继续坚持学习吧！", output_path)
        return

    values = [daily[d] for d in sorted_dates]

    fig, ax = plt.subplots(figsize=(12, 5))
    ax.plot(sorted_dates, values, marker="o", linewidth=2, markersize=6, color="#4A90D9")
    ax.fill_between(range(len(values)), values, alpha=0.15, color="#4A90D9")

    # 趋势线
    if len(values) >= 3:
        import numpy as np
        x = range(len(values))
        z = np.polyfit(x, values, 1)
        p = np.poly1d(z)
        ax.plot(sorted_dates, p(x), "--", color="#E67E22", linewidth=1.2, alpha=0.7, label="趋势")

    ax.set_title("每日学习时长趋势", fontsize=16, fontweight="bold")
    ax.set_ylabel("学习时长（分钟）")
    ax.set_xlabel("日期")
    ax.grid(axis="y", alpha=0.3)
    ax.legend()

    # 格式化 x 轴日期
    if len(sorted_dates) > 10:
        step = max(1, len(sorted_dates) // 8)
        ax.set_xticks(sorted_dates[::step])

    plt.xticks(rotation=30, ha="right", fontsize=8)
    plt.tight_layout()
    plt.savefig(output_path, dpi=150, bbox_inches="tight")
    plt.close()


def chart_subject_distribution(history: list[dict], output_path: str):
    """图表2：科目学习时长分布饼图"""
    import matplotlib.pyplot as plt

    if not history:
        generate_empty_chart("科目分布", "暂无学习记录", output_path)
        return

    # 按科目聚合
    subjects = {}
    for h in history:
        s = h["subject"]
        if s not in subjects:
            subjects[s] = 0
        subjects[s] += h["minutes"]

    if not subjects:
        generate_empty_chart("科目分布", "暂无学习记录", output_path)
        return

    sorted_subj = sorted(subjects.items(), key=lambda x: x[1], reverse=True)
    labels = [s[0] for s in sorted_subj]
    values = [s[1] for s in sorted_subj]

    colors = ["#4A90D9", "#27AE60", "#F39C12", "#E74C3C", "#9B59B6",
              "#1ABC9C", "#E67E22", "#2C3E50"]

    fig, ax = plt.subplots(figsize=(8, 8))
    wedges, texts, autotexts = ax.pie(
        values, labels=None, autopct="%1.1f%%",
        colors=colors[:len(labels)],
        startangle=90, pctdistance=0.75,
    )

    # 图例
    legend_labels = [f"{l} ({v:.0f}分钟)" for l, v in zip(labels, values)]
    ax.legend(wedges, legend_labels, title="科目", loc="center left",
              bbox_to_anchor=(1, 0, 0.5, 1), fontsize=9)

    ax.set_title("各科目学习时长分布", fontsize=16, fontweight="bold")
    plt.tight_layout()
    plt.savefig(output_path, dpi=150, bbox_inches="tight")
    plt.close()


def chart_mastery_distribution(history: list[dict], profile: dict, output_path: str):
    """图表3：掌握度分布图"""
    import matplotlib.pyplot as plt

    mastery_counts = {1: 0, 2: 0, 3: 0, 4: 0, 5: 0}
    for h in history:
        m = h["mastery"]
        if m is not None:
            m_int = int(round(m))
            mastery_counts[m_int] = mastery_counts.get(m_int, 0) + 1

    total = sum(mastery_counts.values())

    if total == 0:
        generate_empty_chart("掌握度分布", "暂无掌握度数据\n每次学习后给自己打个分（1-5），这里将显示掌握度变化", output_path)
        return

    fig, ax = plt.subplots(figsize=(10, 6))

    levels = [1, 2, 3, 4, 5]
    counts = [mastery_counts[l] for l in levels]
    colors = ["#E74C3C", "#E67E22", "#F39C12", "#27AE60", "#27AE60"]
    labels = ["未掌握(1)", "薄弱(2)", "了解(3)", "掌握(4)", "精通(5)"]

    bars = ax.bar(levels, counts, color=colors, edgecolor="#333", linewidth=0.5)
    ax.set_xticks(levels)
    ax.set_xticklabels(labels)

    # 数值标签
    for bar, count in zip(bars, counts):
        if count > 0:
            ax.text(bar.get_x() + bar.get_width() / 2, bar.get_height() + 0.5,
                    str(count), ha="center", fontsize=11, fontweight="bold")

    ax.set_title("知识掌握度分布", fontsize=16, fontweight="bold")
    ax.set_ylabel("知识点数量")
    ax.set_xlabel("掌握等级")
    ax.set_ylim(0, max(counts) * 1.2 + 2 if counts else 5)
    ax.grid(axis="y", alpha=0.3)

    # 弱弱点标注
    weaknesses = profile.get("weaknesses", [])
    if weaknesses:
        text = f"⚠ 顽固薄弱点: {len(weaknesses)} 个\n"
        text += "\n".join(f"  · {w['name']} ({w['count']}次)" for w in weaknesses[:5])
        ax.text(0.98, 0.95, text, transform=ax.transAxes,
                fontsize=9, verticalalignment="top", horizontalalignment="right",
                bbox=dict(boxstyle="round,pad=0.5", facecolor="#FFF3CD", alpha=0.8))

    plt.tight_layout()
    plt.savefig(output_path, dpi=150, bbox_inches="tight")
    plt.close()


def chart_calibration(profile: dict, output_path: str):
    """图表4：自评vs实测校准图"""
    import matplotlib.pyplot as plt

    cal_data = profile.get("calibration", [])
    if not cal_data:
        generate_empty_chart("自评校准分析", "暂无评估数据\n完成知识评估后，这里将对比你的自评与实际得分", output_path)
        return

    dates = [c["date"] for c in cal_data]
    self_vals = [c["self_avg"] or 0 for c in cal_data]
    actual_vals = [c["actual_avg"] or 0 for c in cal_data]

    fig, ax = plt.subplots(figsize=(10, 6))

    x = range(len(dates))
    width = 0.35

    bars1 = ax.bar([i - width / 2 for i in x], self_vals, width, label="自评分数",
                   color="#4A90D9", alpha=0.8)
    bars2 = ax.bar([i + width / 2 for i in x], actual_vals, width, label="评估实际分数",
                   color="#E74C3C", alpha=0.8)

    ax.set_xticks(x)
    ax.set_xticklabels(dates, rotation=30, ha="right", fontsize=8)
    ax.set_ylabel("掌握度 (1-5)")
    ax.set_title("自评 vs 实际评估校准", fontsize=16, fontweight="bold")
    ax.set_ylim(0, 5.5)
    ax.legend()
    ax.grid(axis="y", alpha=0.3)

    # 偏差注释
    for i, (s, a) in enumerate(zip(self_vals, actual_vals)):
        if s and a:
            bias = s - a
            if abs(bias) >= 0.5:
                color = "#E74C3C" if bias > 0 else "#27AE60"
                label = f"+{bias:.1f}" if bias > 0 else f"{bias:.1f}"
                ax.annotate(label, (i, max(s, a) + 0.15), ha="center",
                            fontsize=8, color=color, fontweight="bold")

    plt.tight_layout()
    plt.savefig(output_path, dpi=150, bbox_inches="tight")
    plt.close()


def chart_weaknesses(profile: dict, output_path: str):
    """图表5：薄弱点追踪排行"""
    import matplotlib.pyplot as plt

    weaknesses = profile.get("weaknesses", [])
    if not weaknesses:
        generate_empty_chart("薄弱点追踪", "暂无薄弱点数据\n完成知识评估后，这里将追踪反复出现的薄弱环节", output_path)
        return

    # 按出现次数排序
    sorted_w = sorted(weaknesses, key=lambda x: x["count"], reverse=True)[:10]
    names = [w["name"] for w in sorted_w]
    counts = [w["count"] for w in sorted_w]

    fig, ax = plt.subplots(figsize=(10, 6))

    colors = []
    for w in sorted_w:
        trend = w.get("trend", "")
        if "改善" in trend or "↓" in trend:
            colors.append("#27AE60")
        elif "顽固" in trend or "⚠" in trend:
            colors.append("#E74C3C")
        else:
            colors.append("#F39C12")

    bars = ax.barh(range(len(names)), counts, color=colors, edgecolor="#333", linewidth=0.5)
    ax.set_yticks(range(len(names)))
    ax.set_yticklabels(names, fontsize=10)
    ax.invert_yaxis()
    ax.set_xlabel("出现次数")
    ax.set_title("顽固薄弱点排行 (Top 10)", fontsize=16, fontweight="bold")
    ax.grid(axis="x", alpha=0.3)

    # 数值标签
    for bar, count in zip(bars, counts):
        ax.text(bar.get_width() + 0.1, bar.get_y() + bar.get_height() / 2,
                str(count), va="center", fontsize=10, fontweight="bold")

    # 图例
    from matplotlib.patches import Patch
    legend_elements = [
        Patch(facecolor="#E74C3C", label="顽固"),
        Patch(facecolor="#F39C12", label="待观察"),
        Patch(facecolor="#27AE60", label="改善中"),
    ]
    ax.legend(handles=legend_elements, loc="lower right", fontsize=8)

    plt.tight_layout()
    plt.savefig(output_path, dpi=150, bbox_inches="tight")
    plt.close()


def chart_summary(history: list[dict], profile: dict, output_path: str):
    """生成综合摘要大图"""
    import matplotlib.pyplot as plt

    if not history:
        generate_empty_chart("学习仪表盘", "暂无学习记录，无法生成仪表盘\n\n开始你的第一次学习后，这里将变得丰富多彩！", output_path)
        return

    total_minutes = sum(h["minutes"] for h in history)
    total_entries = len(history)
    subjects = set(h["subject"] for h in history)
    mastery_vals = [h["mastery"] for h in history if h["mastery"] is not None]
    avg_mastery = sum(mastery_vals) / len(mastery_vals) if mastery_vals else 0
    weaknesses = profile.get("weaknesses", [])

    # 计算活跃天数
    dates = set(h["date"] for h in history)
    active_days = len(dates)

    fig = plt.figure(figsize=(16, 10))
    fig.suptitle("学习仪表盘 · 综合概览", fontsize=22, fontweight="bold", y=0.98)

    # 指标卡片区域
    metrics = [
        ("累计学习\n时长", f"{total_minutes:.0f}分钟\n({total_minutes / 60:.1f}小时)"),
        ("活跃\n天数", f"{active_days}天"),
        ("学习\n记录", f"{total_entries}条"),
        ("科目", f"{len(subjects)}门"),
        ("平均自评\n掌握度", f"{avg_mastery:.1f}/5" if mastery_vals else "N/A"),
        ("顽固薄\n弱点", f"{len(weaknesses)}个"),
    ]

    for i, (title, value) in enumerate(metrics):
        ax = fig.add_subplot(2, 6, i + 1)
        ax.axis("off")
        ax.text(0.5, 0.65, value, transform=ax.transAxes,
                ha="center", va="center", fontsize=14, fontweight="bold",
                color="#2C3E50")
        ax.text(0.5, 0.25, title, transform=ax.transAxes,
                ha="center", va="center", fontsize=9, color="#7F8C8D")
        # 卡片边框
        ax.add_patch(plt.Rectangle((0.05, 0.05), 0.9, 0.9,
                                   fill=False, edgecolor="#BDC3C7", linewidth=1,
                                   transform=ax.transAxes))

    # 最近活动时间线
    ax_timeline = fig.add_subplot(2, 1, 2)
    if len(history) >= 2:
        daily_list = {}
        for h in history:
            d = h["date"]
            if d not in daily_list:
                daily_list[d] = 0
            daily_list[d] += h["minutes"]

        sorted_dates = sorted(daily_list.keys())[-30:]
        values = [daily_list[d] for d in sorted_dates]

        ax_timeline.bar(range(len(values)), values, color="#4A90D9", alpha=0.7)
        ax_timeline.plot(range(len(values)), values, color="#2C3E50", linewidth=1, marker="o", markersize=3)

        if len(sorted_dates) > 10:
            step = max(1, len(sorted_dates) // 10)
            ax_timeline.set_xticks(range(0, len(sorted_dates), step))
            ax_timeline.set_xticklabels([sorted_dates[i] for i in range(0, len(sorted_dates), step)],
                                        rotation=30, ha="right", fontsize=8)
        else:
            ax_timeline.set_xticks(range(len(sorted_dates)))
            ax_timeline.set_xticklabels(sorted_dates, rotation=30, ha="right", fontsize=8)

        ax_timeline.set_title("最近学习时长 (分钟/天)", fontsize=14, fontweight="bold")
        ax_timeline.set_ylabel("分钟")
        ax_timeline.grid(axis="y", alpha=0.3)
    else:
        ax_timeline.text(0.5, 0.5, "需要更多数据来生成时间线",
                         transform=ax_timeline.transAxes, ha="center", va="center",
                         fontsize=12, color="#999")
        ax_timeline.axis("off")

    plt.tight_layout(rect=[0, 0, 1, 0.95])
    plt.savefig(output_path, dpi=150, bbox_inches="tight")
    plt.close()


def main():
    parser = argparse.ArgumentParser(
        description="学习数据可视化仪表盘生成器"
    )
    parser.add_argument(
        "--data-dir", required=True,
        help="knowledge-base/ 目录路径"
    )
    parser.add_argument(
        "--output", required=True,
        help="输出目录（生成多个文件）或输出文件路径（配合 --combine）"
    )
    parser.add_argument(
        "--combine", action="store_true",
        help="生成单张综合摘要大图（不生成独立图表）"
    )
    parser.add_argument(
        "--lang", default="zh",
        choices=["zh", "en"],
        help="语言 (默认: zh)"
    )

    args = parser.parse_args()

    if not os.path.exists(args.data_dir):
        print(f"错误: 数据目录不存在: {args.data_dir}", file=sys.stderr)
        sys.exit(1)

    configure_matplotlib()

    history = load_learning_history(args.data_dir)
    profile = load_learning_profile(args.data_dir)

    if args.combine:
        chart_summary(history, profile, args.output)
        print(f"综合仪表盘已保存至: {args.output}")
    else:
        # 确保输出目录存在
        os.makedirs(args.output, exist_ok=True)

        charts = [
            ("trend_learning_time.png", chart_learning_time),
            ("pie_subjects.png", chart_subject_distribution),
            ("bar_mastery.png", chart_mastery_distribution),
            ("bar_calibration.png", chart_calibration),
            ("bar_weaknesses.png", chart_weaknesses),
            ("summary.png", chart_summary),
        ]

        for filename, chart_fn in charts:
            output_path = os.path.join(args.output, filename)
            try:
                if filename == "bar_calibration.png":
                    chart_fn(profile, output_path)
                elif filename == "bar_weaknesses.png":
                    chart_fn(profile, output_path)
                elif filename == "pie_subjects.png":
                    chart_fn(history, output_path)
                elif filename == "bar_mastery.png":
                    chart_fn(history, profile, output_path)
                elif filename == "summary.png":
                    chart_fn(history, profile, output_path)
                else:
                    chart_fn(history, output_path)
                print(f"  ✓ {filename}")
            except Exception as e:
                print(f"  ✗ {filename}: {e}", file=sys.stderr)
                generate_empty_chart(
                    filename.replace(".png", "").replace("_", " ").title(),
                    f"图表生成失败: {e}",
                    output_path
                )

        print(f"\n所有图表已保存至: {args.output}/")


if __name__ == "__main__":
    main()
