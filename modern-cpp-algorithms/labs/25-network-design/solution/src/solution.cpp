#include "solution.h"
#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#include <climits>
#include <numeric>

// Union-Find with path compression and union by rank
struct UnionFind {
    std::vector<int> parent, rank;

    explicit UnionFind(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    // Returns false if already in same component (cycle detected)
    bool unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return false;
        if (rank[ra] < rank[rb]) std::swap(ra, rb);
        parent[rb] = ra;
        if (rank[ra] == rank[rb]) ++rank[ra];
        return true;
    }
};

int kruskal_mst(int n, std::vector<std::tuple<int,int,int>> edges) {
    if (n == 1) return 0;

    std::sort(edges.begin(), edges.end(),
              [](const auto& a, const auto& b) {
                  return std::get<2>(a) < std::get<2>(b);
              });

    UnionFind uf(n);
    int total = 0;
    int added = 0;

    for (auto [u, v, w] : edges) {
        if (uf.unite(u, v)) {
            total += w;
            ++added;
            if (added == n - 1) return total;
        }
    }
    // Graph disconnected
    return INT_MAX;
}

int prim_mst(int n, const std::vector<std::vector<std::pair<int,int>>>& adj) {
    if (n == 1) return 0;

    std::vector<bool> inMST(n, false);
    // Min-heap: {edge_weight, node}
    std::priority_queue<std::pair<int,int>,
                        std::vector<std::pair<int,int>>,
                        std::greater<>> pq;

    pq.emplace(0, 0);
    int total = 0;
    int count = 0;

    while (!pq.empty() && count < n) {
        auto [w, u] = pq.top();
        pq.pop();
        if (inMST[u]) continue;
        inMST[u] = true;
        total += w;
        ++count;
        for (auto [v, ew] : adj[u]) {
            if (!inMST[v]) pq.emplace(ew, v);
        }
    }

    return (count == n) ? total : INT_MAX;
}

int min_cost_connect(int n, const std::vector<std::vector<int>>& connections) {
    if (n == 1) return 0;

    // Build edge list (convert 1-indexed to 0-indexed)
    std::vector<std::tuple<int,int,int>> edges;
    edges.reserve(connections.size());
    for (const auto& c : connections) {
        edges.emplace_back(c[0] - 1, c[1] - 1, c[2]);
    }

    int cost = kruskal_mst(n, std::move(edges));
    return (cost == INT_MAX) ? -1 : cost;
}
