import json
from dataclasses import dataclass
from pathlib import Path
from typing import Any

import graph_ext

INPUT_DIRECTORY = Path("data")
INPUT_DIRECTORY.mkdir(parents=True, exist_ok=True)

@dataclass(frozen=True)
class GraphData:
    num_vertices: int
    edges: list[tuple[int, int]]
    labels: dict[int, str]


def parse_graph_json(file_path: str | Path) -> GraphData:
    path = Path(INPUT_DIRECTORY / file_path)
    if not path.exists():
        raise FileNotFoundError(f"File {file_path} does not exist")

    with open(path, "r", encoding="utf-8") as f:
        data: dict[str, Any] = json.load(f)

    nodes = data.get("nodes", [])
    edges = data.get("edges", [])

    labels = {node["id"]: str(node.get("label", node["id"])) for node in nodes}
    edges = [(edge["source"], edge["target"]) for edge in edges]

    return GraphData(
        num_vertices=len(nodes),
        edges=edges,
        labels=labels
    )


def to_cpp_graph(data: GraphData) -> tuple[graph_ext.Graph, dict[int, str]]:
    graph = graph_ext.Graph(data.num_vertices)

    for u, v in data.edges:
        graph.add_edge(u, v)

    return graph, data.labels
