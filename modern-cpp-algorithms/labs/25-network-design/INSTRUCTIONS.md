# Lab 25 — Network Design

Build minimum spanning tree algorithms to find the cheapest way to connect a network of nodes.

## Tasks

- [ ] Implement `kruskal_mst(n, edges)` — return the total cost of the MST using Kruskal's algorithm with union-find
- [ ] Implement `prim_mst(n, adj)` — return the MST cost using Prim's algorithm with a min-priority queue
- [ ] Implement `min_cost_connect(n, connections)` — given a list of `{city1, city2, cost}` connections, return the minimum cost to connect all cities, or `-1` if impossible

## Hints

- `kruskal_mst`: sort edges by weight; use union-find (path compression + union by rank) to skip edges that form cycles; stop after adding `n-1` edges
- `prim_mst`: start from node 0; use a min-heap of `{edge_weight, neighbor}`; track which nodes are already in the MST
- `min_cost_connect`: this is exactly Kruskal or Prim on the given connection list; if fewer than `n-1` edges are added, return `-1`

Use `Run Tests` to check your progress.
