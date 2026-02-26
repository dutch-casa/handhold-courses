# State Space Search — BFS on Abstract States

BFS works on any state space, not just grids or graphs. Model each configuration as a node, transitions as edges, and search for the target state.

## Tasks

- [ ] Implement `open_lock(deadends, target)` — a lock starts at `"0000"`. Each move turns one wheel by one digit (wrapping 9->0 or 0->9). Return the minimum moves to reach `target`, or `-1` if impossible. Never visit a deadend state.
- [ ] Implement `topo_sort(n, edges)` — given `n` nodes (0-indexed) and directed edges `{from, to}`, return a valid topological ordering using Kahn's algorithm. Return an empty vector if a cycle exists.

## Hints

- `open_lock`: BFS from `"0000"`. Each state has 8 neighbors (4 wheels x 2 directions). Use a set for visited states and deadends.
- `topo_sort`: compute in-degrees, enqueue all nodes with in-degree 0, process BFS-style. If the result has fewer than `n` nodes, there is a cycle.

Use `Run Tests` to check your progress.
