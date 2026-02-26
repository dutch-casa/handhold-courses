#include "solution.h"
#include <vector>
#include <climits>

// TODO: implement Dijkstra using a min-priority queue
std::vector<int> dijkstra(const std::vector<Edge>& graph, int n, int start) {
    // TODO: build adjacency list, run priority queue relaxation
    return std::vector<int>(n, 0);
}

// TODO: implement Bellman-Ford with negative cycle detection
std::vector<int> bellman_ford(const std::vector<Edge>& edges, int n, int start) {
    // TODO: relax all edges n-1 times; check for negative cycle on nth pass
    return std::vector<int>(n, 0);
}

// TODO: implement network_delay using Dijkstra on 1-indexed nodes
int network_delay(const std::vector<std::vector<int>>& times, int n, int k) {
    // TODO: run Dijkstra from k, return max distance or -1
    return 0;
}
