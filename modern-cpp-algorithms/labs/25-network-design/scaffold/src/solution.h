#pragma once
#include <vector>

// Kruskal's MST: returns the total weight of the minimum spanning tree.
// n = number of nodes (0-indexed); edges = list of {u, v, weight} (undirected).
// Returns INT_MAX if the graph is disconnected.
int kruskal_mst(int n, std::vector<std::tuple<int,int,int>> edges);

// Prim's MST: returns the total weight of the minimum spanning tree.
// n = number of nodes (0-indexed).
// adj[u] = list of {neighbor, weight} (undirected adjacency list).
// Returns INT_MAX if the graph is disconnected.
int prim_mst(int n, const std::vector<std::vector<std::pair<int,int>>>& adj);

// Min cost to connect all cities.
// connections[i] = {city1, city2, cost} (1-indexed cities, undirected).
// Returns minimum total cost, or -1 if impossible.
int min_cost_connect(int n, const std::vector<std::vector<int>>& connections);
