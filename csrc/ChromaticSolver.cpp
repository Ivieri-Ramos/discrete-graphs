#include "ChromaticSolver.hpp"

#include <algorithm>
#include <numeric>

namespace {
    class ChromaticContext {
        const Graph& graph;
        const int64_t total_vertices;

        std::vector<int64_t> colors;
        std::vector<int64_t> order;

        [[nodiscard]] bool is_safe(const int64_t u, const int64_t color) const {
            for (const int64_t neighbor : graph.get_neighbors(u)) {
                if (colors[neighbor] == color) {
                    return false;
                }
            }

            return true;
        }

        bool backtrack(const size_t index, const int64_t max_color) {
            if (index == static_cast<size_t>(total_vertices)) {
                return true;
            }

            const int64_t u = order[index];

            for (int64_t color = 0; color < max_color; color++) {
                if (is_safe(u, color)) {
                    colors[u] = color;

                    if (backtrack(index + 1, max_color)) {
                        return true;
                    }

                    colors[u] = -1;
                }
            }

            return false;
        }

    public:
        explicit ChromaticContext(const Graph& graph)
            :graph(graph),
            total_vertices(graph.get_total_vertices()),
            colors(graph.get_total_vertices(), - 1),
            order(graph.get_total_vertices()) {

            std::iota(order.begin(), order.end(), 0);

            std::sort(order.begin(), order.end(), [&graph](const int64_t u, const int64_t v) {
                return graph.get_vertex_degree(u) > graph.get_vertex_degree(v);
            });
        }

        std::vector<int64_t> solve() {
            if (total_vertices == 0) {
                return {};
            }

            for (int64_t num_colors = 1; num_colors <= total_vertices; num_colors++) {
                std::fill(colors.begin(), colors.end(), -1);

                if (backtrack(0, num_colors)) {
                    break;
                }
            }

            return colors;
        }
    };

}

std::vector<int64_t> find_chromatic_coloring(const Graph& graph) {
    ChromaticContext context(graph);
    return context.solve();
}
