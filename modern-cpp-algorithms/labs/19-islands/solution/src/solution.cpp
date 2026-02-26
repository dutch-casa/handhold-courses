#include "solution.h"
#include <vector>
#include <queue>

static void flood_fill(std::vector<std::vector<char>>& grid, int r, int c) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    if (r < 0 || r >= rows || c < 0 || c >= cols || grid[r][c] == '0') return;
    grid[r][c] = '0';
    flood_fill(grid, r + 1, c);
    flood_fill(grid, r - 1, c);
    flood_fill(grid, r, c + 1);
    flood_fill(grid, r, c - 1);
}

int num_islands(std::vector<std::vector<char>>& grid) {
    if (grid.empty()) return 0;
    int count = 0;
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == '1') {
                count++;
                flood_fill(grid, r, c);
            }
        }
    }
    return count;
}

int count_components(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<int>> adj(n);
    for (auto& [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    std::vector<bool> visited(n, false);
    int components = 0;

    for (int i = 0; i < n; i++) {
        if (visited[i]) continue;
        components++;
        std::queue<int> q;
        q.push(i);
        visited[i] = true;
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            for (int neighbor : adj[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
    }
    return components;
}

bool can_finish(int num_courses, const std::vector<std::pair<int, int>>& prereqs) {
    std::vector<std::vector<int>> adj(num_courses);
    std::vector<int> in_degree(num_courses, 0);
    for (auto& [course, prereq] : prereqs) {
        adj[prereq].push_back(course);
        in_degree[course]++;
    }

    std::queue<int> q;
    for (int i = 0; i < num_courses; i++) {
        if (in_degree[i] == 0) q.push(i);
    }

    int processed = 0;
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        processed++;
        for (int neighbor : adj[node]) {
            in_degree[neighbor]--;
            if (in_degree[neighbor] == 0) q.push(neighbor);
        }
    }
    return processed == num_courses;
}
