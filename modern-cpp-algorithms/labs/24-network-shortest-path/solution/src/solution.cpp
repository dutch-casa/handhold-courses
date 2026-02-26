#include "solution.h"
#include <vector>
#include <queue>
#include <climits>

std::vector<int> dijkstra(const std::vector<Edge>& graph, int n, int start) {
    // Build adjacency list: adj[u] = list of {v, weight}
    std::vector<std::vector<std::pair<int,int>>> adj(n);
    for (const auto& e : graph) {
        adj[e.from].emplace_back(e.to, e.weight);
    }

    std::vector<int> dist(n, INT_MAX);
    dist[start] = 0;

    // Min-heap: {distance, node}
    std::priority_queue<std::pair<int,int>,
                        std::vector<std::pair<int,int>>,
                        std::greater<>> pq;
    pq.emplace(0, start);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // Stale entry — a shorter path was already processed
        if (d > dist[u]) continue;

        for (auto [v, w] : adj[u]) {
            if (dist[u] == INT_MAX) continue;
            int nd = dist[u] + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.emplace(nd, v);
            }
        }
    }
    return dist;
}

std::vector<int> bellman_ford(const std::vector<Edge>& edges, int n, int start) {
    std::vector<int> dist(n, INT_MAX);
    dist[start] = 0;

    // Relax all edges n-1 times
    for (int i = 0; i < n - 1; ++i) {
        for (const auto& e : edges) {
            if (dist[e.from] == INT_MAX) continue;
            int nd = dist[e.from] + e.weight;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
            }
        }
    }

    // nth pass: if any distance still decreases, a negative cycle exists
    for (const auto& e : edges) {
        if (dist[e.from] == INT_MAX) continue;
        if (dist[e.from] + e.weight < dist[e.to]) {
            dist[0] = -1;
            return dist;
        }
    }
    return dist;
}

int network_delay(const std::vector<std::vector<int>>& times, int n, int k) {
    // Build adjacency list (1-indexed nodes, shift to 0-indexed internally)
    std::vector<std::vector<std::pair<int,int>>> adj(n + 1);
    for (const auto& t : times) {
        adj[t[0]].emplace_back(t[1], t[2]);
    }

    std::vector<int> dist(n + 1, INT_MAX);
    dist[k] = 0;

    std::priority_queue<std::pair<int,int>,
                        std::vector<std::pair<int,int>>,
                        std::greater<>> pq;
    pq.emplace(0, k);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            int nd = dist[u] + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.emplace(nd, v);
            }
        }
    }

    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        if (dist[i] == INT_MAX) return -1;
        ans = std::max(ans, dist[i]);
    }
    return ans;
}
