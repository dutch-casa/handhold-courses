#include "solution.h"
#include <queue>
#include <vector>

int shortest_path(const std::vector<std::vector<int>>& adj, int start, int end) {
    if (start == end) return 0;
    int n = static_cast<int>(adj.size());
    std::vector<int> dist(n, -1);
    dist[start] = 0;
    std::queue<int> q;
    q.push(start);
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (int neighbor : adj[node]) {
            if (dist[neighbor] != -1) continue;
            dist[neighbor] = dist[node] + 1;
            if (neighbor == end) return dist[neighbor];
            q.push(neighbor);
        }
    }
    return -1;
}

int oranges_rotting(std::vector<std::vector<int>>& grid) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    std::queue<std::pair<int, int>> q;
    int fresh = 0;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == 2) q.push({r, c});
            else if (grid[r][c] == 1) fresh++;
        }
    }

    if (fresh == 0) return 0;

    int minutes = 0;
    int dirs[] = {0, 1, 0, -1, 0};

    while (!q.empty()) {
        int size = static_cast<int>(q.size());
        bool rotted_any = false;
        for (int i = 0; i < size; i++) {
            auto [r, c] = q.front();
            q.pop();
            for (int d = 0; d < 4; d++) {
                int nr = r + dirs[d], nc = c + dirs[d + 1];
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                if (grid[nr][nc] != 1) continue;
                grid[nr][nc] = 2;
                fresh--;
                rotted_any = true;
                q.push({nr, nc});
            }
        }
        if (rotted_any) minutes++;
    }

    return fresh == 0 ? minutes : -1;
}
