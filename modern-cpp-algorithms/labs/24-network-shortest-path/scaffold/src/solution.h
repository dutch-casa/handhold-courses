#pragma once
#include <vector>
#include <climits>

struct Edge {
    int from, to, weight;
};

// Dijkstra: shortest distances from start to all nodes (0-indexed).
// graph = list of directed edges {from, to, weight}, all weights >= 0.
// Returns vector of size n; unreachable nodes have distance INT_MAX.
std::vector<int> dijkstra(const std::vector<Edge>& graph, int n, int start);

// Bellman-Ford: shortest distances from start to all nodes.
// Returns vector of size n; unreachable nodes have distance INT_MAX.
// If a negative cycle is reachable, sets result[0] = -1.
std::vector<int> bellman_ford(const std::vector<Edge>& edges, int n, int start);

// Network delay: time for a signal from node k (1-indexed) to reach all n nodes.
// times[i] = {u, v, w} meaning edge u->v with delay w.
// Returns the maximum distance across all nodes, or -1 if any node is unreachable.
int network_delay(const std::vector<std::vector<int>>& times, int n, int k);
