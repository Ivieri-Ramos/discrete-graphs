import matplotlib
matplotlib.use("Agg")

import networkx as nx
import matplotlib as mpl
import matplotlib.pyplot as plt
from pathlib import Path

from discrete_app.io.core import GraphData

OUTPUT_DIRECTORY = Path("output")

def _get_consistent_layout(nx_graph: nx.Graph) -> dict:
    return nx.spring_layout(nx_graph, seed=42)

def _resolve_dynamic_colormap(num_colors: int) -> mpl.colors.Colormap:
    if num_colors <= 10:
        return mpl.colormaps["tab10"]
    elif num_colors <= 20:
        return mpl.colormaps["tab20"]
    else:
        return mpl.colormaps["turbo"]


def to_network_graph(data: GraphData) -> nx.Graph:
    graph = nx.Graph()

    for node_id, label in data.labels.items():
        graph.add_node(node_id, label=label)

    graph.add_edges_from(data.edges)

    return graph


def plot_chromatic_result(
        nx_graph: nx.Graph,
        colors: list[int],
        labels: dict[int, str],
        output_path: str | Path = "color_graph.svg"
) -> None:
    pos = _get_consistent_layout(nx_graph)
    num_colors = max(colors) + 1 if colors else 0
    node_order = list(nx_graph.nodes())

    fig, axes = plt.subplots(1, 2, figsize=(14, 6))

    axes[0].set_title("Grafo Original (Topologia de Recursos)", fontsize=13, fontweight="bold")
    nx.draw_networkx_edges(nx_graph, pos, ax=axes[0], edge_color="#888888", width=1.5)
    nx.draw_networkx_nodes(
        nx_graph,
        pos,
        nodelist=node_order,
        ax=axes[0],
        node_color="#ADD8E6",
        node_size=700,
        edgecolors="#333333"
    )
    nx.draw_networkx_labels(nx_graph, pos, labels=labels, ax=axes[0], font_size=10, font_weight="bold")
    axes[0].axis("off")

    axes[1].set_title(f"Coloração Mínima: χ(G) = {num_colors} Slots", fontsize=13, fontweight="bold")

    cmap = _resolve_dynamic_colormap(num_colors)

    node_colors = [colors[node] for node in node_order]

    nx.draw_networkx_edges(nx_graph, pos, ax=axes[1], edge_color="#888888", width=1.5)
    nx.draw_networkx_nodes(
        nx_graph,
        pos,
        nodelist=node_order,
        ax=axes[1],
        node_color=node_colors,
        cmap=cmap,
        vmin=0,
        vmax=max(num_colors - 1, 1),
        node_size=650,
        edgecolors="#222222"
    )
    nx.draw_networkx_labels(nx_graph, pos, labels=labels, ax=axes[1], font_size=10, font_weight="bold")
    axes[1].axis("off")

    plt.tight_layout()

    out = Path(OUTPUT_DIRECTORY / output_path)
    out.parent.mkdir(parents=True, exist_ok=True)

    fig.savefig(out, format="svg", bbox_inches="tight")
    plt.close(fig)


def plot_tarjan_result(
        nx_graph: nx.Graph,
        cut_vertices: list[int],
        components: list[list[int]],
        labels: dict[int, str],
        output_path: str | Path = "tarjan_graph.svg"
) -> None:
    pos = _get_consistent_layout(nx_graph)
    cut_set = set(cut_vertices)

    fig, axes = plt.subplots(1, 2, figsize=(15, 6))

    axes[0].set_title("Identificação de Vértices de Corte (Tarjan)", fontsize=13, fontweight="bold")
    node_colors_tarjan = ["#FF5733" if n in cut_set else "#A8DADC" for n in nx_graph.nodes()]

    nx.draw_networkx_edges(nx_graph, pos, ax=axes[0], edge_color="#777777", width=1.5)
    nx.draw_networkx_nodes(
        nx_graph,
        pos,
        ax=axes[0],
        node_color=node_colors_tarjan,
        node_size=700,
        edgecolors="#222222"
    )
    axes[1].set_title(f"Rede Fracionada ({len(components)} Componentes Conexas)", fontsize=13, fontweight="bold")

    operating_nodes = [n for n in nx_graph.nodes() if n not in cut_set]
    subgraph = nx_graph.subgraph(operating_nodes)

    component_color_map = {}
    for comp_idx, comp_nodes in enumerate(components):
        for node in comp_nodes:
            component_color_map[node] = comp_idx

    colors_subgraph = [component_color_map[n] for n in operating_nodes]
    cmap_comp = mpl.colormaps["Set2"].resampled(max(len(components), 1))

    if cut_set:
        nx.draw_networkx_nodes(
            nx_graph,
            pos,
            nodelist=list(cut_set),
            ax=axes[1],
            node_color="#CCCCCC",
            node_shape="X",
            node_size=600,
            edgecolors="#888888"
        )
        cut_labels = {n: labels[n] for n in cut_set}
        nx.draw_networkx_labels(nx_graph, pos, labels=cut_labels, ax=axes[1], font_size=9, alpha=0.6)

    if operating_nodes:
        nx.draw_networkx_edges(subgraph, pos, ax=axes[1], edge_color="#555555", width=1.5)
        nx.draw_networkx_nodes(
            nx_graph,
            pos,
            nodelist=operating_nodes,
            ax=axes[1],
            node_color=colors_subgraph,
            cmap=cmap_comp,
            node_size=700,
            edgecolors="#222222"
        )
        op_labels = {n: labels[n] for n in operating_nodes}
        nx.draw_networkx_labels(nx_graph, pos, labels=op_labels, ax=axes[1], font_size=10, font_weight="bold")

    axes[1].axis("off")

    plt.tight_layout()

    out = Path(OUTPUT_DIRECTORY / output_path)
    out.parent.mkdir(parents=True, exist_ok=True)

    plt.savefig(out, dpi=300, bbox_inches="tight")
    plt.close(fig)