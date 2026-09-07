#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Graphs.hpp"
#include "TarjanSolver.hpp"
#include "ChromaticSolver.hpp"

namespace py = pybind11;

PYBIND11_MODULE(graph_ext, m) {
    m.doc() = "Modulo C++ para Teoria dos Grafos";

    py::class_<Graph>(m, "Graph", "Estrutura de adjacência para grafos não direcionados")
        .def(py::init<int64_t>(), py::arg("total_vertices"),
             "Instancia um grafo não direcionado com V vertices (indices de 0 a V - 1)")

        .def("add_edge", &Graph::add_edge, py::arg("u"), py::arg("v"),
             "Insere uma aresta não direcionada entre os vertices u e v")

        .def("get_total_vertices", &Graph::get_total_vertices,
             "Retorna o número total de vertices do grafo")

        .def("get_neighbors", &Graph::get_neighbors, py::arg("u"),
             "Retorna a lista de vizinhos do vértice u")

        .def("get_vertex_degree", &Graph::get_vertex_degree, py::arg("u"),
             "Retorna o grau do vértice u");

    m.def("find_articulation_points", &find_articulation_points,
          py::arg("graph"),
          "Calcula os pontos de articulação (vértices de corte) usando o Algoritmo de Tarjan O(V + E)");

    m.def("get_connected_components", &get_connected_components,
          py::arg("graph"),
          py::arg("excluded_vertices"),
          "Particiona o grafo nas componentes conexas resultantes ignorando os vértices informados");

    m.def("find_chromatic_coloring", &find_chromatic_coloring,
          py::arg("graph"),
          "Calcula a atribuição exata de cores/slots e atesta o número cromático chi(G)");
}