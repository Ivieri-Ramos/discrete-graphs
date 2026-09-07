#include <cstdint>
#include <vector>

#include "Graphs.hpp"

Graph::Graph(const int64_t total_vertices) {
    this->total_vertices = total_vertices;

    vertices.resize(total_vertices);
}

void Graph::add_edge(const int64_t from, const int64_t to) {
    vertices.at(from).push_back(to);
    vertices.at(to).push_back(from);
}

const std::vector<int64_t>& Graph::get_neighbors(const int64_t id) const {
    return vertices.at(id);
}

int64_t Graph::get_total_vertices() const {
    return total_vertices;
}

uint64_t Graph::get_vertex_degree(const int64_t id) const {
    return vertices.at(id).size();
}
