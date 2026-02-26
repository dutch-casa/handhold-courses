#include "solution.h"
#include <vector>
#include <tuple>
#include <utility>

// TODO: initialize dist matrix, then run triple-loop relaxation
std::vector<std::vector<int>> floyd_warshall(int n, const std::vector<std::tuple<int,int,int>>& edges) {
    // TODO: build dist[n][n], apply k/i/j relaxation
    return {};
}

// TODO: BFS/DFS coloring; return false if any neighbor has the same color
bool is_bipartite(int n, const std::vector<std::pair<int,int>>& edges) {
    // TODO: build adjacency list, try 2-coloring each component
    return true;
}

// TODO: Tarjan's algorithm tracking disc[] and low[] values
std::vector<std::pair<int,int>> find_bridges(int n, const std::vector<std::pair<int,int>>& edges) {
    // TODO: DFS with discovery times; bridge when low[v] > disc[u]
    return {};
}
