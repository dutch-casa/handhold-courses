#include "solution.h"
#include <vector>
#include <tuple>
#include <utility>
#include <queue>
#include <climits>
#include <algorithm>

std::vector<std::vector<int>> floyd_warshall(int n, const std::vector<std::tuple<int,int,int>>& edges) {
    // Sentinel: INT_MAX/2 avoids overflow when adding two sentinels
    const int INF = INT_MAX / 2;
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, INF));

    for (int i = 0; i < n; ++i) dist[i][i] = 0;

    for (auto [u, v, w] : edges) {
        dist[u][v] = std::min(dist[u][v], w);
    }

    // Relax through intermediate node k
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    return dist;
}

bool is_bipartite(int n, const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> adj(n);
    for (auto [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // -1 = uncolored
    std::vector<int> color(n, -1);

    for (int start = 0; start < n; ++start) {
        if (color[start] != -1) continue;
        std::queue<int> q;
        q.push(start);
        color[start] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Tarjan's bridge-finding DFS
namespace {
    void dfs_bridge(int u, int parent,
                    int& timer,
                    std::vector<int>& disc,
                    std::vector<int>& low,
                    const std::vector<std::vector<int>>& adj,
                    std::vector<std::pair<int,int>>& bridges) {
        disc[u] = low[u] = timer++;
        for (int v : adj[u]) {
            if (v == parent) continue;
            if (disc[v] == -1) {
                dfs_bridge(v, u, timer, disc, low, adj, bridges);
                low[u] = std::min(low[u], low[v]);
                // u-v is a bridge when the subtree rooted at v cannot
                // reach u or any ancestor of u through a back edge
                if (low[v] > disc[u]) {
                    bridges.emplace_back(std::min(u, v), std::max(u, v));
                }
            } else {
                low[u] = std::min(low[u], disc[v]);
            }
        }
    }
}

std::vector<std::pair<int,int>> find_bridges(int n, const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> adj(n);
    for (auto [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<int> disc(n, -1), low(n, -1);
    std::vector<std::pair<int,int>> bridges;
    int timer = 0;

    for (int i = 0; i < n; ++i) {
        if (disc[i] == -1) {
            dfs_bridge(i, -1, timer, disc, low, adj, bridges);
        }
    }

    std::sort(bridges.begin(), bridges.end());
    return bridges;
}
