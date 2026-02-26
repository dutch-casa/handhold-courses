# Lab 24 — Network Shortest Path

Implement three classical shortest-path algorithms on weighted directed graphs and use them to solve a network delay problem.

## Tasks

- [ ] Implement `dijkstra(graph, n, start)` — return shortest distances from `start` to every node (0-indexed); nodes unreachable stay `INT_MAX`
- [ ] Implement `bellman_ford(edges, n, start)` — return shortest distances using edge relaxation; set `result[0] = -1` if a negative cycle is reachable
- [ ] Implement `network_delay(times, n, k)` — given directed weighted edges `{u, v, w}`, return the time for a signal from node `k` to reach all `n` nodes, or `-1` if impossible

## Hints

- `dijkstra`: use a min-priority queue of `{distance, node}`; skip stale entries where recorded distance < popped distance
- `bellman_ford`: relax all edges `n-1` times; on the nth pass, any further relaxation means a negative cycle exists
- `network_delay`: run Dijkstra from node `k` (1-indexed); take the max of all distances; if any remain `INT_MAX` return `-1`

Use `Run Tests` to check your progress.
