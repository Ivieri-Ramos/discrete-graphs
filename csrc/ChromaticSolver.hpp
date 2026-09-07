#pragma once

#include <cstdint>
#include <vector>
#include "Graphs.hpp"

[[nodiscard]] std::vector<int64_t> find_chromatic_coloring(const Graph& graph);