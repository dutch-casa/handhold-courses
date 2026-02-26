# Shortest Paths — BFS

BFS on an unweighted graph finds shortest paths. Multi-source BFS handles problems where multiple starting points spread simultaneously.

## Tasks

- [ ] Implement `shortest_path(adj, start, end)` — return the shortest path length in an unweighted graph given an adjacency list. Return `-1` if no path exists. `adj[i]` is the list of neighbors of node `i`.
- [ ] Implement `oranges_rotting(grid)` — return the minimum minutes until all oranges are rotten, or `-1` if impossible. `0` = empty, `1` = fresh, `2` = rotten. Each minute, rotten oranges infect adjacent fresh ones.

## Hints

- `shortest_path`: standard BFS from `start`. Track distance. Return distance when `end` is reached.
- `oranges_rotting`: enqueue all initially rotten oranges (multi-source BFS). Process level by level, counting minutes. After BFS, check if any fresh oranges remain.

Use `Run Tests` to check your progress.
