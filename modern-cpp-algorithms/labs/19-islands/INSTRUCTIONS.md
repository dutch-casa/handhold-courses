# Islands and Components — Graph Traversal

Grids are graphs. Each cell connects to its neighbors. DFS flood-fill counts connected regions.

## Tasks

- [ ] Implement `num_islands(grid)` — count the number of islands in a 2D grid. `'1'` is land, `'0'` is water. Connected horizontally/vertically.
- [ ] Implement `count_components(n, edges)` — count connected components in an undirected graph with `n` nodes (0-indexed) and the given edges.
- [ ] Implement `can_finish(num_courses, prereqs)` — return `true` if all courses can be finished given prerequisite pairs `{course, prereq}`. This is cycle detection in a directed graph.

## Hints

- `num_islands`: iterate cells, DFS/BFS flood-fill each unvisited `'1'`, mark visited cells as `'0'`.
- `count_components`: build adjacency list, DFS from each unvisited node, count DFS launches.
- `can_finish`: build a directed adjacency list, detect cycles with DFS coloring (white/gray/black) or Kahn's algorithm.

Use `Run Tests` to check your progress.
