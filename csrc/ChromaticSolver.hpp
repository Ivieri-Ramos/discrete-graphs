#pragma once

#include <cstdint>
#include <vector>
#include "Graphs.hpp"

/// @brief calcula o numero cromatico exato do grafo
/// @param graph o grafo a ser processado
/// @return vetor onde o índice é o vértice e o valor é a cor atribuída a ele
[[nodiscard]] std::vector<int64_t> find_chromatic_coloring(const Graph& graph);