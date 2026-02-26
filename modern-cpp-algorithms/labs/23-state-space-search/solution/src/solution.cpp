#include "solution.h"
#include <queue>
#include <unordered_set>

int open_lock(const std::vector<std::string>& deadends, std::string target) {
    std::unordered_set<std::string> dead(deadends.begin(), deadends.end());
    std::string start = "0000";
    if (dead.count(start)) return -1;
    if (start == target) return 0;

    std::unordered_set<std::string> visited;
    visited.insert(start);
    std::queue<std::pair<std::string, int>> q;
    q.push({start, 0});

    while (!q.empty()) {
        auto [state, moves] = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            for (int d : {-1, 1}) {
                std::string next = state;
                next[i] = static_cast<char>((next[i] - '0' + d + 10) % 10 + '0');
                if (next == target) return moves + 1;
                if (!dead.count(next) && !visited.count(next)) {
                    visited.insert(next);
                    q.push({next, moves + 1});
                }
            }
        }
    }
    return -1;
}

std::vector<int> topo_sort(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<int>> adj(n);
    std::vector<int> in_degree(n, 0);
    for (auto& [from, to] : edges) {
        adj[from].push_back(to);
        in_degree[to]++;
    }

    std::queue<int> q;
    for (int i = 0; i < n; i++) {
        if (in_degree[i] == 0) q.push(i);
    }

    std::vector<int> order;
    order.reserve(n);
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        order.push_back(node);
        for (int neighbor : adj[node]) {
            in_degree[neighbor]--;
            if (in_degree[neighbor] == 0) q.push(neighbor);
        }
    }

    if (static_cast<int>(order.size()) != n) return {};
    return order;
}
