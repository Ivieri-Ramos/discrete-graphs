#pragma once

#include <cstdint>
#include <vector>
#include "Graphs.hpp"

std::vector<int64_t> find_articulation_points(const Graph& graph);

std::vector<std::vector<int64_t>> get_connected_components(
    const Graph& graph,
    const std::vector<int64_t>& excluded_vertices);