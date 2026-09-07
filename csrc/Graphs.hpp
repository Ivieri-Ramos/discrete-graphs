#pragma once

#include <cstdint>
#include <vector>

class Graph {
    int64_t total_vertices;
    std::vector<std::vector<int64_t>> vertices;

public:
    explicit Graph(int64_t total_vertices);

    void add_edge(int64_t from, int64_t to);

    [[nodiscard]] const std::vector<int64_t>& get_neighbors(int64_t id) const;

    [[nodiscard]] int64_t get_total_vertices() const;

    [[nodiscard]] uint64_t get_vertex_degree(int64_t id) const;
};