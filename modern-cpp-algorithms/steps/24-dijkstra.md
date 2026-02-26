---
title: Weighted Graphs and Dijkstra
---

# Weighted adjacency list

{{show: weighted-adj typewriter 2s linear}} Weighted graphs add a cost to each edge. Every neighbor entry is now a pair: the neighbor node and the edge weight. `vector<vector<pair<int,int>>>` is the standard representation.

{{focus: pair-type}} {{zoom: 1.2x}} {{annotate: pair-type "node, weight"}} Each pair holds the destination node first, the weight second. Structured bindings make it readable at the call site.

{{zoom: 1x}} {{focus: build-weighted}} {{zoom: 1.3x}} {{annotate: build-weighted "Both directions"}} Undirected weighted graphs push both directions. The weight is the same either way. For directed graphs, push one direction only.

{{zoom: 1x}} {{focus: example-graph}} {{zoom: 1.2x}} {{annotate: example-graph "5-node graph"}} A five-node example. Node 0 connects to 1 with cost 4 and to 2 with cost 1. These asymmetric weights are what Dijkstra exploits.

{{zoom: 1x}} {{focus: none}} The data structure is the same as unweighted. The pair's second field is the only addition. Everything else carries over.

```code:weighted-adj lang=cpp
#include <vector>
#include <utility>

// adj[u] = list of {neighbor, weight}
using Graph = std::vector<std::vector<std::pair<int, int>>>;

Graph build_graph(int n, const std::vector<std::tuple<int,int,int>>& edges) {
    Graph adj(n);
    for (auto [u, v, w] : edges) {
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);  // undirected
    }
    return adj;
}

// Example: 5-node graph
// 0 --4-- 1
// |       |
// 1   2   3
// |       |
// 2 --5-- 3
//     |
//     4 (cost 2 from node 2)
auto g = build_graph(5, {
    {0, 1, 4}, {0, 2, 1},
    {2, 1, 2}, {1, 3, 1},
    {2, 3, 5}, {3, 4, 3}
});
---
pair-type: 4
build-weighted: 7-12
example-graph: 15-24
```

# Dijkstra's algorithm

{{clear: slide}}

{{show: dijkstra-graph grow 0.5s spring}} Dijkstra finds the shortest path from a source to all other nodes. The key insight: always process the closest unvisited node next. Once a node is finalized, its shortest distance is fixed.

{{focus: source-node}} {{annotate: source-node "Start: dist 0"}} The source starts at distance 0. Everything else starts at infinity.

{{focus: relax-edges}} {{annotate: relax-edges "Relax neighbors"}} For each processed node, try to improve the distance of its neighbors. If going through the current node is cheaper, update.

{{focus: finalized}} {{annotate: finalized "Locked in"}} The greedy invariant: when we extract a node from the min-heap, its distance is final. No future edge can improve it — all remaining edges have non-negative weight.

{{focus: none}} The greedy choice holds because weights are non-negative. That's the one requirement Dijkstra imposes. Negative weights break the invariant.

```data:dijkstra-graph type=graph
0 -- 1 : 4
0 -- 2 : 1
2 -- 1 : 2
1 -- 3 : 1
2 -- 3 : 5
3 -- 4 : 3
---
source-node: 0
relax-edges: 1, 2, 3
finalized: 0, 2
```

# Priority queue with greater<>

{{clear: slide}}

{{show: pq-code typewriter 2s linear}} C++'s `priority_queue` is a max-heap by default. Dijkstra needs a min-heap — always extract the smallest distance. Flip it with `greater<pair<int,int>>` as the comparator.

{{focus: type-sig}} {{zoom: 1.2x}} {{annotate: type-sig "Min-heap type"}} The full type: `priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>>`. Verbose but standard. Store pairs as `{distance, node}` so the heap sorts by distance first.

{{zoom: 1x}} {{focus: push-op}} {{zoom: 1.3x}} {{annotate: push-op "Push {dist, node}"}} Push distance first, node second. That ordering means the comparator uses distance for heap ordering automatically.

{{zoom: 1x}} {{focus: pop-op}} {{zoom: 1.2x}} {{annotate: pop-op "Extract min"}} `top()` gives the pair with the smallest distance. `pop()` removes it. Structured bindings unpack both fields cleanly.

{{zoom: 1x}} {{focus: alias}} {{zoom: 1.2x}} {{annotate: alias "Type alias"}} A `using` alias makes the declaration readable. Declare it once, use it everywhere.

{{zoom: 1x}} {{focus: none}} `greater<>` flips the comparison. Min-heap. Smallest distance always at top. That's all it takes.

```code:pq-code lang=cpp
#include <queue>
#include <vector>
#include <functional>

// Min-heap: smallest distance at top
// pair: {distance, node}
using MinHeap = std::priority_queue<
    std::pair<int, int>,
    std::vector<std::pair<int, int>>,
    std::greater<std::pair<int, int>>
>;

void demo() {
    MinHeap pq;

    pq.push({0, 0});    // {dist=0, node=0}
    pq.push({4, 1});    // {dist=4, node=1}
    pq.push({1, 2});    // {dist=1, node=2}

    auto [dist, node] = pq.top();  // {1, 2} — min distance
    pq.pop();

    // next: {0, 0}, then {4, 1}
}
---
type-sig: 7-11
alias: 7
push-op: 15-17
pop-op: 19-20
```

# Implementation walkthrough

{{clear: slide}}

{{show: dijkstra-impl typewriter 2s linear}} Full Dijkstra with distance array, min-heap, and edge relaxation. Every part has a job. Read it as a loop: extract min, check stale, relax neighbors, push updates.

{{focus: init}} {{zoom: 1.2x}} {{annotate: init "Setup"}} Initialize distances to infinity, source to 0. Push the source onto the heap. The INF sentinel means unreachable.

{{zoom: 1x}} {{focus: extract}} {{zoom: 1.3x}} {{annotate: extract "Extract min"}} Pull the closest node. The key check: if this distance is worse than what we already know, this entry is stale — skip it.

{{zoom: 1x}} {{focus: stale-check}} {{zoom: 1.2x}} {{annotate: stale-check "Skip stale"}} We push duplicate entries when we find a shorter path. Old entries remain in the heap. The stale check filters them out cheaply.

{{zoom: 1x}} {{focus: relax}} {{zoom: 1.3x}} {{annotate: relax "Relax edge"}} For each neighbor: compute new distance through current node. If it's shorter than the known distance, update and push the new entry.

{{zoom: 1x}} {{focus: none}} The stale entry pattern is essential. It's lazy deletion — we don't remove old entries from the heap, we just skip them when popped.

```code:dijkstra-impl lang=cpp
#include <vector>
#include <queue>
#include <limits>

using Graph = std::vector<std::vector<std::pair<int, int>>>;
constexpr int INF = std::numeric_limits<int>::max();

std::vector<int> dijkstra(const Graph& adj, int src) {
    int n = static_cast<int>(adj.size());
    std::vector<int> dist(n, INF);
    dist[src] = 0;

    std::priority_queue<std::pair<int,int>,
        std::vector<std::pair<int,int>>,
        std::greater<>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;  // stale entry

        for (auto [v, w] : adj[u]) {
            int new_dist = dist[u] + w;
            if (new_dist < dist[v]) {
                dist[v] = new_dist;
                pq.push({new_dist, v});
            }
        }
    }
    return dist;
}
---
init: 9-16
extract: 18-21
stale-check: 22
relax: 24-29
```

# Path reconstruction

{{clear: slide}}

{{show: path-code typewriter 2s linear}} Distance tells you *how far*. The parent array tells you *how to get there*. Store the parent of each node when you relax an edge. Backtrack from destination to source to reconstruct the path.

{{focus: parent-init}} {{zoom: 1.2x}} {{annotate: parent-init "Track parents"}} Initialize parent to -1 for all nodes. -1 means no parent yet — node is unreachable or is the source.

{{zoom: 1x}} {{focus: parent-update}} {{zoom: 1.3x}} {{annotate: parent-update "Record on relax"}} When you relax an edge and update `dist[v]`, also set `parent[v] = u`. The path leading to v now goes through u.

{{zoom: 1x}} {{focus: backtrack}} {{zoom: 1.2x}} {{annotate: backtrack "Walk back"}} Reconstruct: start at destination, follow parents until you reach the source. The path is built in reverse — flip it at the end.

{{zoom: 1x}} {{focus: none}} Two arrays, one traversal. `dist` for costs, `parent` for the path. Dijkstra gives you both in one pass.

```code:path-code lang=cpp
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

constexpr int INF = std::numeric_limits<int>::max();
using Graph = std::vector<std::vector<std::pair<int, int>>>;

std::pair<std::vector<int>, std::vector<int>>
dijkstra_with_path(const Graph& adj, int src) {
    int n = static_cast<int>(adj.size());
    std::vector<int> dist(n, INF);
    std::vector<int> parent(n, -1);
    dist[src] = 0;

    std::priority_queue<std::pair<int,int>,
        std::vector<std::pair<int,int>>,
        std::greater<>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return {dist, parent};
}

std::vector<int> reconstruct(const std::vector<int>& parent, int dst) {
    std::vector<int> path;
    for (int cur = dst; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
}
---
parent-init: 13
parent-update: 26
backtrack: 34-40
```

# Negative weights and Bellman-Ford

{{clear: slide}}

{{show: bellman-graph grow 0.5s spring}} Dijkstra breaks on negative weights. Once a node is finalized, a later negative edge might offer a shorter path — but Dijkstra won't revisit it. Bellman-Ford handles negatives by relaxing all edges V-1 times.

{{focus: negative-trap}} {{annotate: negative-trap "Greedy fails"}} Example: 0 to 2 via 0→1→2 costs 3+(-5) = -2. Dijkstra finalizes 0→2 via cost 1 first and never reconsiders. Wrong answer.

{{focus: relax-rounds}} {{annotate: relax-rounds "V-1 rounds"}} Bellman-Ford runs V-1 relaxation rounds. Each round can extend the shortest path by one more edge. After V-1 rounds, all paths are found.

{{focus: negative-cycle}} {{annotate: negative-cycle "Cycle check"}} A V-th round that still relaxes any edge means there's a negative cycle. Shortest paths don't exist — you can keep going around the cycle to decrease distance.

{{focus: none}} Bellman-Ford is O(V * E). Slower than Dijkstra's O((V+E) log V), but correct for negative weights. No negative cycles: use Dijkstra. Negative weights: use Bellman-Ford.

```data:bellman-graph type=graph
0 -- 1 : 3
0 -- 2 : 1
1 -- 2 : -5
---
negative-trap: 2
relax-rounds: 0, 1, 2
negative-cycle: 0, 1, 2
```

{{clear: slide}}

{{show: bellman-code typewriter 2s linear}} Bellman-Ford is simpler to implement than Dijkstra. Flat edge list, nested loop, done.

{{focus: edge-list}} {{zoom: 1.2x}} {{annotate: edge-list "Flat edges"}} Store edges as triples: source, destination, weight. No adjacency list needed — iterate over all edges directly.

{{zoom: 1x}} {{focus: rounds}} {{zoom: 1.3x}} {{annotate: rounds "V-1 passes"}} Outer loop runs V-1 times. Inner loop relaxes every edge. Each pass guarantees shortest paths using at most k edges where k is the pass number.

{{zoom: 1x}} {{focus: neg-cycle-detect}} {{zoom: 1.2x}} {{annotate: neg-cycle-detect "V-th round"}} One final pass: if any edge still relaxes, a negative cycle exists. Return early or signal the caller.

{{zoom: 1x}} {{focus: none}} Bellman-Ford: flat and simple. O(V * E) but handles what Dijkstra can't.

```code:bellman-code lang=cpp
#include <vector>
#include <limits>
#include <tuple>

constexpr int INF = std::numeric_limits<int>::max();
using Edge = std::tuple<int, int, int>;  // {u, v, w}

std::vector<int> bellman_ford(int n, const std::vector<Edge>& edges, int src) {
    std::vector<int> dist(n, INF);
    dist[src] = 0;

    for (int i = 0; i < n - 1; ++i) {
        for (auto [u, v, w] : edges) {
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    // V-th round: detect negative cycles
    for (auto [u, v, w] : edges) {
        if (dist[u] != INF && dist[u] + w < dist[v]) {
            return {};  // negative cycle detected
        }
    }
    return dist;
}
---
edge-list: 7
rounds: 11-17
neg-cycle-detect: 20-24
```

# Complexity analysis

{{clear: slide}}

{{show: complexity-table grow 0.5s spring}} Dijkstra with a binary heap is O((V+E) log V). Every vertex is extracted once from the heap — O(V log V). Every edge triggers at most one push — O(E log V). Total: O((V+E) log V).

{{focus: extract-cost}} {{annotate: extract-cost "V extractions"}} Each vertex is extracted from the heap exactly once. Each extraction costs O(log V). Total extraction cost: O(V log V).

{{focus: push-cost}} {{annotate: push-cost "E pushes"}} Each edge relaxation may push to the heap. In the worst case, every edge triggers a push. Each push costs O(log V). Total push cost: O(E log V).

{{focus: dense-case}} {{annotate: dense-case "Dense graphs"}} For dense graphs where E ≈ V², the heap version is O(V² log V). A simple array-based priority queue gives O(V²) — better for dense graphs.

{{focus: none}} For sparse graphs (E ≈ V), binary heap Dijkstra dominates. For dense graphs, consider the array implementation. Fibonacci heap gives O(E + V log V) but has high constant factors.

```data:complexity-table type=hash-map
{"Algorithm": "Dijkstra (binary heap)", "Time": "O((V+E) log V)", "Negative?": "No"}
{"Algorithm": "Dijkstra (array)", "Time": "O(V²)", "Negative?": "No"}
{"Algorithm": "Bellman-Ford", "Time": "O(V·E)", "Negative?": "Yes"}
{"Algorithm": "Floyd-Warshall", "Time": "O(V³)", "Negative?": "Yes"}
---
extract-cost: 0
push-cost: 1
dense-case: 2
```
