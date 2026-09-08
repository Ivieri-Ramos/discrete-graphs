#pragma once

#include <cstdint>
#include <vector>

/// @brief estrutura base para representação de grafos não direcionados
/// @details utiliza listas de adjacência para armazenar as conexões
class Graph {
    int64_t total_vertices;
    std::vector<std::vector<int64_t>> vertices;

public:
    /// @brief inicializa o grafo com uma quantidade fixa de vértices
    /// @param total_vertices número de vértices que o grafo terá
    explicit Graph(int64_t total_vertices);

    /// @brief adiciona uma aresta bidirecional entre dois vértices
    /// @param from vértice de origem
    /// @param to vértice de destino
    void add_edge(int64_t from, int64_t to);

    /// @brief retorna os vizinhos conectados diretamente a um vértice
    /// @param id identificador do vértice consultado
    /// @return referência para o vetor de vizinhos
    [[nodiscard]] const std::vector<int64_t>& get_neighbors(int64_t id) const;

    /// @brief informa o número total de vértices no grafo
    /// @return quantidade total de vértices
    [[nodiscard]] int64_t get_total_vertices() const;

    /// @brief calcula o grau do vértice
    /// @param id identificador do vértice
    /// @return o grau do vértice
    [[nodiscard]] uint64_t get_vertex_degree(int64_t id) const;
};