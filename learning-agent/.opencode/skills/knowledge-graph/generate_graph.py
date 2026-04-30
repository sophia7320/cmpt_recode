#!/usr/bin/env python3
"""知识图谱生成脚本

支持三种模式：
  graph  - 概念关系图（networkx graphviz 布局）
  tree   - 知识树图（networkx 谱系布局）
  path   - 学习路径图（拓扑排序布局）

用法:
  python generate_graph.py --mode graph --data input.json --output output.png
  python generate_graph.py --mode tree --data input.json --output output.svg --format svg
  python generate_graph.py --mode path --data input.json --output output.png --lang zh
"""

import argparse
import json
import os
import sys
import textwrap


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


def load_graph_data(data_path):
    """加载 JSON 数据文件"""
    with open(data_path, "r", encoding="utf-8") as f:
        data = json.load(f)

    assert "nodes" in data, "数据文件缺少 'nodes' 字段"
    nodes = data["nodes"]
    edges = data.get("edges", [])
    title = data.get("title", "知识图谱")
    mode = data.get("mode", "graph")

    return title, mode, nodes, edges


def build_nx_graph(nodes, edges):
    """构建 networkx 有向图"""
    import networkx as nx

    G = nx.DiGraph()
    for node in nodes:
        G.add_node(
            node["id"],
            label=node.get("label", node["id"]),
            group=node.get("group", ""),
            mastery=node.get("mastery", 3),
        )
    for edge in edges:
        G.add_edge(
            edge["from"],
            edge["to"],
            label=edge.get("label", ""),
        )
    return G


def validate_graph_data(data):
    """验证 JSON 数据合法性，返回 (errors, warnings) 元组。

    errors: 致命错误，阻止继续执行
    warnings: 可恢复的问题，跳过对应条目后继续
    """
    errors = []
    warnings = []

    # 1. 检查必需字段
    if "nodes" not in data:
        errors.append("缺少 'nodes' 字段")
        return errors, warnings
    if not isinstance(data["nodes"], list):
        errors.append("'nodes' 必须是数组")
        return errors, warnings
    if len(data["nodes"]) == 0:
        errors.append("'nodes' 数组不能为空")
        return errors, warnings

    # 2. 节点校验
    node_ids = set()
    for i, node in enumerate(data["nodes"]):
        if not isinstance(node, dict):
            errors.append(f"节点[{i}] 必须是对象")
            continue

        if "id" not in node:
            errors.append(f"节点[{i}] 缺少 'id' 字段")
        else:
            nid = node["id"]
            if not isinstance(nid, str) or not nid.strip():
                errors.append(f"节点[{i}] id 不能为空")
            elif nid in node_ids:
                errors.append(f"节点 id '{nid}' 重复")
            else:
                node_ids.add(nid)

        if "mastery" in node:
            m = node["mastery"]
            if not isinstance(m, (int, float)) or m < 1 or m > 5:
                warnings.append(
                    f"节点 '{node.get('id', i)}' mastery={m} 不在 1-5 范围内，已使用默认值 3"
                )

    if errors:
        return errors, warnings

    # 3. 边校验
    edges = data.get("edges", [])
    if not isinstance(edges, list):
        errors.append("'edges' 必须是数组")
        return errors, warnings

    for i, edge in enumerate(edges):
        if not isinstance(edge, dict):
            warnings.append(f"边[{i}] 不是对象，已跳过")
            continue

        if "from" not in edge:
            warnings.append(f"边[{i}] 缺少 'from' 字段，已跳过")
        elif edge["from"] not in node_ids:
            warnings.append(
                f"边[{i}] from='{edge['from']}' 引用了不存在的节点，已跳过"
            )

        if "to" not in edge:
            warnings.append(f"边[{i}] 缺少 'to' 字段，已跳过")
        elif edge["to"] not in node_ids:
            warnings.append(
                f"边[{i}] to='{edge['to']}' 引用了不存在的节点，已跳过"
            )

    # 4. 孤立节点检测（未被任何边引用的节点）
    if edges:
        connected_ids = set()
        for edge in edges:
            if "from" in edge and "to" in edge:
                if edge["from"] in node_ids and edge["to"] in node_ids:
                    connected_ids.add(edge["from"])
                    connected_ids.add(edge["to"])

        orphans = node_ids - connected_ids
        for oid in orphans:
            node_label = next(
                (n.get("label", oid) for n in data["nodes"] if n.get("id") == oid),
                oid,
            )
            warnings.append(f"节点 '{node_label}' (id={oid}) 没有任何边连接，是孤立节点")

    return errors, warnings


def filter_valid_edges(edges, node_ids):
    """过滤掉引用不存在节点的边"""
    valid = []
    for edge in edges:
        if not isinstance(edge, dict):
            continue
        frm = edge.get("from", "")
        to = edge.get("to", "")
        if frm in node_ids and to in node_ids:
            valid.append(edge)
    return valid


def get_mastery_color(mastery):
    """根据掌握度返回颜色：绿(4-5)、黄(3)、橙(2)、红(1)"""
    if mastery is None:
        return "#4A90D9"  # blue default
    if mastery >= 4:
        return "#27AE60"  # green
    elif mastery >= 3:
        return "#F39C12"  # yellow
    elif mastery >= 2:
        return "#E67E22"  # orange
    else:
        return "#E74C3C"  # red


def wrap_label(text, width=10):
    """中文字符宽度约为英文2倍，折行处理"""
    if len(text) <= width:
        return text
    return "\n".join(textwrap.wrap(text, width=width))


def render_graph(G, title, output_path, fmt, lang):
    """渲染概念关系图"""
    import matplotlib.pyplot as plt
    import networkx as nx

    plt.figure(figsize=(20, 12))
    plt.title(title, fontsize=20, fontweight="bold", pad=20)

    # 使用 graphviz 布局
    try:
        pos = nx.nx_agraph.graphviz_layout(G, prog="neato", args="-Goverlap=false")
    except Exception:
        pos = nx.spring_layout(G, k=3, iterations=50, seed=42)
        print("提示: graphviz 布局不可用，已降级为力导向布局")

    # 节点颜色
    node_colors = [get_mastery_color(G.nodes[n].get("mastery", 3)) for n in G.nodes]

    # 绘制
    nx.draw_networkx_nodes(
        G, pos,
        node_color=node_colors,
        node_size=2000,
        edgecolors="#333333",
        linewidths=1.5,
        alpha=0.9,
    )

    nx.draw_networkx_edges(
        G, pos,
        edge_color="#888888",
        arrows=True,
        arrowsize=15,
        width=1.2,
        connectionstyle="arc3,rad=0.1",
    )

    # 标签
    labels = {n: wrap_label(G.nodes[n].get("label", n)) for n in G.nodes}
    nx.draw_networkx_labels(
        G, pos, labels,
        font_size=9,
        font_family="sans-serif",
    )

    # 边标签
    edge_labels = {(u, v): d.get("label", "") for u, v, d in G.edges(data=True)}
    edge_labels = {k: v for k, v in edge_labels.items() if v}
    if edge_labels:
        nx.draw_networkx_edge_labels(
            G, pos, edge_labels,
            font_size=7,
            font_color="#555555",
        )

    # 图例
    from matplotlib.patches import Patch
    legend_elements = [
        Patch(facecolor="#27AE60", label="精通 (4-5)"),
        Patch(facecolor="#F39C12", label="掌握 (3)"),
        Patch(facecolor="#E67E22", label="了解 (2)"),
        Patch(facecolor="#E74C3C", label="薄弱 (1)"),
    ]
    plt.legend(handles=legend_elements, loc="lower right", fontsize=8)

    plt.axis("off")
    plt.tight_layout()
    plt.savefig(output_path, dpi=150, bbox_inches="tight", format=fmt)
    plt.close()
    print(f"图形已保存至: {output_path}")


def render_tree(G, title, output_path, fmt, lang):
    """渲染知识树图"""
    import matplotlib.pyplot as plt
    import networkx as nx

    plt.figure(figsize=(18, 14))
    plt.title(title, fontsize=20, fontweight="bold", pad=20)

    try:
        pos = nx.nx_agraph.graphviz_layout(G, prog="dot", args="-Grankdir=TB -Gnodesep=0.3")
    except Exception:
        print("提示: graphviz 布局不可用，已降级为力导向布局。安装 Graphviz 可获得更好的树形效果。")
        try:
            pos = nx.bfs_layout(G, start=list(G.nodes())[0])
        except Exception:
            pos = nx.spring_layout(G, k=2, iterations=50, seed=42)

    node_colors = [get_mastery_color(G.nodes[n].get("mastery", 3)) for n in G.nodes]

    nx.draw_networkx_nodes(
        G, pos,
        node_color=node_colors,
        node_size=2500,
        edgecolors="#333333",
        linewidths=1.5,
        alpha=0.9,
    )

    nx.draw_networkx_edges(
        G, pos,
        edge_color="#888888",
        arrows=False,
        width=1.5,
    )

    labels = {n: wrap_label(G.nodes[n].get("label", n)) for n in G.nodes}
    nx.draw_networkx_labels(
        G, pos, labels,
        font_size=10,
        font_family="sans-serif",
    )

    edge_labels = {(u, v): d.get("label", "") for u, v, d in G.edges(data=True)}
    edge_labels = {k: v for k, v in edge_labels.items() if v}
    if edge_labels:
        nx.draw_networkx_edge_labels(
            G, pos, edge_labels,
            font_size=7,
            font_color="#555555",
        )

    plt.axis("off")
    plt.tight_layout()
    plt.savefig(output_path, dpi=150, bbox_inches="tight", format=fmt)
    plt.close()
    print(f"图形已保存至: {output_path}")


def render_path(G, title, output_path, fmt, lang):
    """渲染学习路径图"""
    import matplotlib.pyplot as plt
    import networkx as nx

    plt.figure(figsize=(20, 8))
    plt.title(title, fontsize=20, fontweight="bold", pad=20)

    try:
        # 拓扑排序确定x坐标
        order = list(nx.topological_sort(G))
        x_positions = {node: i for i, node in enumerate(order)}

        pos = {}
        y_base = 0
        for node in order:
            if G.in_degree(node) == 0:
                pos[node] = (x_positions[node], y_base)
            else:
                pred_y = sum(pos[p][1] for p in G.predecessors(node)) / max(1, G.in_degree(node))
                pos[node] = (x_positions[node], pred_y)
    except nx.NetworkXUnfeasible:
        print(
            "警告: 图中存在循环依赖，无法生成严格拓扑排序的学习路径，"
            "已降级为力导向布局"
        )
        pos = nx.spring_layout(G, k=2, iterations=50, seed=42)
    except nx.NetworkXError:
        print(
            "警告: 拓扑排序失败，已降级为力导向布局"
        )
        pos = nx.spring_layout(G, k=2, iterations=50, seed=42)

    node_colors = [get_mastery_color(G.nodes[n].get("mastery", 3)) for n in G.nodes]

    nx.draw_networkx_nodes(
        G, pos,
        node_color=node_colors,
        node_size=2200,
        edgecolors="#333333",
        linewidths=1.5,
        alpha=0.9,
    )

    nx.draw_networkx_edges(
        G, pos,
        edge_color="#666666",
        arrows=True,
        arrowsize=18,
        width=1.5,
        connectionstyle="arc3,rad=0.05",
    )

    labels = {n: wrap_label(G.nodes[n].get("label", n)) for n in G.nodes}
    nx.draw_networkx_labels(
        G, pos, labels,
        font_size=10,
        font_family="sans-serif",
    )

    edge_labels = {(u, v): d.get("label", "") for u, v, d in G.edges(data=True)}
    edge_labels = {k: v for k, v in edge_labels.items() if v}
    if edge_labels:
        nx.draw_networkx_edge_labels(
            G, pos, edge_labels,
            font_size=7,
            font_color="#555555",
        )

    plt.axis("off")
    plt.tight_layout()
    plt.savefig(output_path, dpi=150, bbox_inches="tight", format=fmt)
    plt.close()
    print(f"图形已保存至: {output_path}")


def configure_matplotlib(lang):
    """配置 matplotlib 中文字体"""
    import matplotlib
    import matplotlib.pyplot as plt

    if lang == "zh":
        fonts = find_chinese_font()
    else:
        fonts = ["Arial", "Helvetica", "sans-serif"]

    plt.rcParams["font.sans-serif"] = fonts
    plt.rcParams["axes.unicode_minus"] = False

    matplotlib.use("Agg")


def main():
    parser = argparse.ArgumentParser(
        description="知识图谱生成工具",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument(
        "--mode", required=True,
        choices=["graph", "tree", "path"],
        help="图形模式: graph(概念关系图), tree(知识树), path(学习路径)",
    )
    parser.add_argument(
        "--data", required=True,
        help="输入 JSON 数据文件路径",
    )
    parser.add_argument(
        "--output", required=True,
        help="输出图像文件路径",
    )
    parser.add_argument(
        "--format", default="png",
        choices=["png", "svg"],
        help="输出格式 (默认: png)",
    )
    parser.add_argument(
        "--lang", default="zh",
        choices=["zh", "en"],
        help="语言 (默认: zh)",
    )

    args = parser.parse_args()

    # 配置 matplotlib
    configure_matplotlib(args.lang)

    # 确保输出目录存在
    output_dir = os.path.dirname(args.output)
    if output_dir and not os.path.exists(output_dir):
        os.makedirs(output_dir, exist_ok=True)

    # 加载数据
    title, mode, nodes, edges = load_graph_data(args.data)

    # 验证数据
    errors, warnings = validate_graph_data(
        {"title": title, "mode": mode, "nodes": nodes, "edges": edges}
    )

    # 打印警告（不阻塞）
    for w in warnings:
        print(f"警告: {w}", file=sys.stderr)

    # 打印错误（阻塞）
    if errors:
        for e in errors:
            print(f"错误: {e}", file=sys.stderr)
        print(
            f"\n共 {len(errors)} 个错误，{len(warnings)} 个警告。"
            f"请修正 JSON 数据后重试。",
            file=sys.stderr,
        )
        sys.exit(1)

    # 过滤无效边
    node_ids = {n["id"] for n in nodes}
    edges = filter_valid_edges(edges, node_ids)

    if warnings:
        print(f"已跳过无效条目，使用 {len(edges)} 条边继续生成。")

    # 构建图
    G = build_nx_graph(nodes, edges)

    # 渲染
    render_fns = {
        "graph": render_graph,
        "tree": render_tree,
        "path": render_path,
    }
    render_fn = render_fns[mode]
    render_fn(G, title, args.output, args.format, args.lang)


if __name__ == "__main__":
    main()
