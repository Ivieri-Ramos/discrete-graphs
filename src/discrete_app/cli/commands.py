from .app import app
from discrete_app.graphs import *
from discrete_app.io import *
import graph_ext
from pathlib import Path

import typer

def _sanitize_output_prefix(output_arg: str) -> str:
    raw_path = Path(output_arg)

    base_name = raw_path.stem

    return base_name

@app.command()
def color(
        json_file: str = typer.Option("color_graph.json", "--json", "-f",
                                      help="O nome do arquivo JSON que armazena o grafo"),
        output_file: str = typer.Option("color_graph.svg", "--output", "-o",
                                        help="O nome do arquivo de saída")
) -> None:
    try:
        target_json = _sanitize_output_prefix(json_file) + ".json"
        target_svg = _sanitize_output_prefix(output_file) + ".svg"

        graph_data = parse_graph_json(target_json)

        graph_cpp, labels = to_cpp_graph(graph_data)

        colors = graph_ext.find_chromatic_coloring(graph_cpp)

        graph_nx = to_network_graph(graph_data)

        plot_chromatic_result(graph_nx, colors, labels, target_svg)
    except FileNotFoundError as e:
        print(f"{e}")

@app.command()
def tarjan(
        json_file: str = typer.Option("tarjan_graph.json", "--json", "-f",
                                      help="O nome do arquivo JSON que armazena o grafo"),
        output_file: str = typer.Option("tarjan_graph", "--output", "-o",
                                        help="O nome do arquivo de saída"),
        all: bool = typer.Option(False, "--all", "-a",)
) -> None:
    try:
        target_json = _sanitize_output_prefix(json_file) + ".json"
        graph_data = parse_graph_json(target_json)

        graph_cpp, labels = to_cpp_graph(graph_data)

        cutted_points = graph_ext.find_articulation_points(graph_cpp)
        graph_nx = to_network_graph(graph_data)

        if all:
            target_svg = _sanitize_output_prefix(output_file) + ".svg"

            connected_components = graph_ext.get_connected_components(graph_cpp, cutted_points)

            plot_tarjan_result(graph_nx, cutted_points, connected_components, labels, target_svg)
        else:
            target_path = _sanitize_output_prefix(output_file)
            base_path = Path(target_path)

            folder = Path(target_path)

            for point in cutted_points:
                connected_component = graph_ext.get_connected_components(graph_cpp, [point])

                node_label = labels.get(point, str(point))
                specific_output = folder / f"{base_path}_{node_label}.svg"

                plot_tarjan_result(graph_nx, [point], connected_component, labels, specific_output)

    except FileNotFoundError as e:
        print(f"{e}")