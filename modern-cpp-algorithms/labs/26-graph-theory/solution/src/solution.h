#pragma once
#include <vector>
#include <utility>

// Floyd-Warshall: all-pairs shortest paths.
// n = number of nodes (0-indexed); edges = list of {u, v, weight} (directed).
// Returns n×n matrix; dist[i][j] is the shortest path from i to j,
// or INT_MAX/2 if no path exists.
std::vector<std::vector<int>> floyd_warshall(int n, const std::vector<std::tuple<int,int,int>>& edges);

// Bipartite check: can the undirected graph be 2-colored?
// n = number of nodes (0-indexed); edges = list of {u, v} (undirected, unweighted).
bool is_bipartite(int n, const std::vector<std::pair<int,int>>& edges);

// Find bridges: edges whose removal disconnects the graph.
// n = number of nodes (0-indexed); edges = list of {u, v} (undirected, unweighted).
// Returns list of bridge edges; each pair has the smaller node first.
std::vector<std::pair<int,int>> find_bridges(int n, const std::vector<std::pair<int,int>>& edges);
