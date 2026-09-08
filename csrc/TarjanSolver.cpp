#include "TarjanSolver.hpp"

namespace {
    class TarjanContext {
        const Graph& graph;
        int64_t timer = 0;

        struct NodeState {
            int64_t tin = -1;
            int64_t low = -1;
            bool is_cut = false;
        };

        std::vector<NodeState> states;

        [[nodiscard]] std::vector<int64_t> get_cut_vertices() const {
            std::vector<int64_t> cut_vertices;

            for (int64_t vertex = 0; vertex < graph.get_total_vertices(); vertex++) {
                if (states[vertex].is_cut) {
                    cut_vertices.push_back(vertex);
                }
            }

            return cut_vertices;
        }

        void dfs_tarjan(const int64_t u, const int64_t parent) {
            // a busca em profundidade (dfs) percorre o grafo registrando o momento exato da descoberta de cada vértice em uma variável tin
            timer++;
            states[u].tin = timer;
            
            // simultaneamente, um rastreador low é inicializado armazenando o mesmo tempo de descoberta
            states[u].low = timer;

            int64_t children = 0;

            for (const int64_t v : graph.get_neighbors(u)) {
                if (v == parent) {
                    continue;
                }

                if (states[v].tin != -1) {
                    // é atualizado apenas se o vértice ou seus descendentes conseguirem acessar um vértice mais antigo através de uma rota alternativa
                    states[u].low = std::min(states[u].low, states[v].tin);
                }
                else {
                    children++;

                    dfs_tarjan(v, u);

                    states[u].low = std::min(states[u].low, states[v].low);

                    // no retorno da recursão, ocorre o teste de isolamento: se o valor low de um vértice filho não for menor que o valor tin do vértice pai, constata-se que não há caminhos alternativos para escapar. a falha na rota de escape caracteriza o pai como um ponto de corte iminente
                    if (parent != -1 && states[v].low >= states[u].tin) {
                        states[u].is_cut = true;
                    }
                }
            }

            // a raiz da árvore de busca só é promovida a ponto de articulação  se originar dois ou mais ramos totalmente independentes de pesquisa
            if (parent == -1 && children > 1) {
                states[u].is_cut = true;
            }
        }

    public:
        explicit TarjanContext(const Graph& graph)
            : graph(graph), states(graph.get_total_vertices()) {}

        std::vector<int64_t> run_tarjan() {
            for (int64_t u = 0; u < graph.get_total_vertices(); u++) {
                if (states[u].tin == -1) {
                    dfs_tarjan(u, -1);
                }
            }

            return get_cut_vertices();
        }
    };

    class ConnectivityContext
    {
        const Graph& graph;
        std::vector<bool> visited;

    public:
        // o estado inicial é montado marcando preventivamente os vértices da lista de exclusão como regiões já visitadas
        ConnectivityContext(const Graph& graph, const std::vector<int64_t>& excluded_components)
            : graph(graph), visited(graph.get_total_vertices(), false) {

            for (const int64_t u : excluded_components) {
                visited.at(u) = true;
            }
        }

        [[nodiscard]] bool is_visited(const int64_t u) const {
            return visited[u];
        }

        // a travessia local agrupa os vizinhos alcançáveis em cadeia, recuando naturalmente quando a busca esbarra nos vértices da lista de exclusão (já marcados previamente)
        void explore(const int64_t u, std::vector<int64_t>& component) {
            visited[u] = true;
            component.push_back(u);

            for (const int64_t v : graph.get_neighbors(u)) {
                if (!visited[v]) {
                    explore(v, component);
                }
            }
        }
    };
}

std::vector<int64_t> find_articulation_points(const Graph& graph) {
    TarjanContext context(graph);

    return context.run_tarjan();
}

std::vector<std::vector<int64_t>> get_connected_components(const Graph& graph, const std::vector<int64_t>& excluded_vertices) {
    ConnectivityContext context(graph, excluded_vertices);
    std::vector<std::vector<int64_t>> components;

    // um laço sequencial verifica cada vértice disponível na estrutura e dispara uma nova travessia quando encontra um alvo não processado
    for (int64_t u = 0; u < graph.get_total_vertices(); u++) {
        if (!context.is_visited(u)) {
            std::vector<int64_t> current_component;

            context.explore(u, current_component);

            // cada ramificação esgotada gera um subgrafo fechado e isolado de vértices conectados entre si, que é salvo como uma componente conexa final
            components.push_back(std::move(current_component));
        }
    }

    return components;
}