#pragma once

#include <cstdint>
#include <vector>
#include "Graphs.hpp"

/// @brief encontra os pontos de articulação do grafo
/// @param graph o grafo que será analisado
/// @return vetor contendo os identificadores dos vértices de corte
std::vector<int64_t> find_articulation_points(const Graph& graph);

/// @brief divide o grafo em componentes conexas, ignorando vértices específicos
/// @param graph o grafo base para a busca
/// @param excluded_vertices lista de vértices que devem ser tratados como já visitados/removidos
/// @return lista onde cada item é uma componente conexa contendo seus respectivos vértices
std::vector<std::vector<int64_t>> get_connected_components(
    const Graph& graph,
    const std::vector<int64_t>& excluded_vertices);