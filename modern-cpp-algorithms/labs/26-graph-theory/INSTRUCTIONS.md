# Lab 26 — Graph Theory Practice

Implement three foundational graph algorithms: all-pairs shortest paths, bipartite checking, and bridge detection.

## Tasks

- [ ] Implement `floyd_warshall(n, edges)` — return an n×n matrix of shortest distances between all pairs; use `INT_MAX/2` for no-path to avoid overflow on addition
- [ ] Implement `is_bipartite(n, edges)` — return true if the undirected graph can be 2-colored (no odd cycles)
- [ ] Implement `find_bridges(n, edges)` — return all edges whose removal increases the number of connected components; use Tarjan's bridge-finding algorithm

## Hints

- `floyd_warshall`: initialize `dist[i][i] = 0` and `dist[u][v] = weight`; then triple-loop `k, i, j` updating `dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])`
- `is_bipartite`: BFS/DFS coloring — assign color 0 to start, color 1 to neighbors; if a neighbor has the same color as current, not bipartite; check every component
- `find_bridges`: Tarjan's: track `disc[u]` (discovery time) and `low[u]` (lowest disc reachable); edge `u-v` is a bridge if `low[v] > disc[u]`

Use `Run Tests` to check your progress.
