---
title: Minimum Spanning Trees
---

# MST definition

{{show: mst-graph grow 0.5s spring}} A spanning tree connects all V vertices with exactly V-1 edges and no cycles. A minimum spanning tree (MST) does this with the smallest possible total edge weight. Two classic algorithms: Kruskal and Prim.

{{focus: spanning-property}} {{annotate: spanning-property "All vertices"}} Every vertex must be included. The tree spans the entire graph — no islands, no isolated nodes.

{{focus: no-cycle}} {{annotate: no-cycle "V-1 edges"}} A tree with V vertices has exactly V-1 edges. Add any more and you create a cycle. Remove any and the graph disconnects.

{{focus: min-weight}} {{annotate: min-weight "Minimum cost"}} Among all spanning trees, we want the one with the smallest sum of edge weights. Applications: cheapest network wiring, road building, clustering.

{{focus: none}} The cut property justifies greedy: the minimum weight edge crossing any cut of the graph is always safe to include in an MST. That's the foundation both algorithms build on.

```data:mst-graph type=graph
0 -- 1 : 4
0 -- 2 : 3
1 -- 2 : 1
1 -- 3 : 2
2 -- 3 : 4
3 -- 4 : 2
---
spanning-property: 0, 1, 2, 3, 4
no-cycle: 1-2, 1-3, 3-4, 0-2
min-weight: 1-2, 1-3, 3-4, 0-2
```

# Kruskal's algorithm

{{clear: slide}}

{{show: kruskal-graph grow 0.5s spring}} Kruskal sorts all edges by weight and greedily adds the cheapest edge that doesn't form a cycle. Union-Find tracks which vertices are already connected so cycle detection is near O(1).

{{focus: sort-edges}} {{annotate: sort-edges "Sort by weight"}} Sort all edges ascending by weight. Process them in order — always try the cheapest next.

{{focus: safe-edge}} {{annotate: safe-edge "No cycle"}} An edge is safe to add if its two endpoints are in different components. Adding it merges those components without creating a cycle.

{{focus: cycle-edge}} {{annotate: cycle-edge "Skip cycle"}} If both endpoints are in the same component, adding this edge creates a cycle. Skip it.

{{focus: done}} {{annotate: done "V-1 edges"}} Stop when you've added V-1 edges. The MST is complete. You don't need to process remaining edges.

{{focus: none}} Kruskal's total complexity: O(E log E) for sorting plus near-linear Union-Find. Sorting dominates.

```data:kruskal-graph type=graph
0 -- 1 : 4
0 -- 2 : 3
1 -- 2 : 1
1 -- 3 : 2
2 -- 3 : 4
3 -- 4 : 2
---
sort-edges: 1-2, 1-3, 3-4, 0-2, 0-1, 2-3
safe-edge: 1-2, 1-3, 3-4, 0-2
cycle-edge: 0-1, 2-3
done: 1-2, 1-3, 3-4, 0-2
```

{{clear: slide}}

{{show: kruskal-code typewriter 2s linear}} Kruskal in code: sort edges, iterate, Union-Find for cycle detection. Clean and direct.

{{focus: sort-step}} {{zoom: 1.2x}} {{annotate: sort-step "Sort ascending"}} Sort edges by weight. `std::sort` with a lambda comparing the weight field.

{{zoom: 1x}} {{focus: union-check}} {{zoom: 1.3x}} {{annotate: union-check "Find + union"}} For each edge: find the roots of both endpoints. Different roots? Safe edge — union them and add to MST. Same root? Cycle — skip.

{{zoom: 1x}} {{focus: mst-build}} {{zoom: 1.2x}} {{annotate: mst-build "Collect edges"}} Push accepted edges into the result vector. Stop early when V-1 edges are collected.

{{zoom: 1x}} {{focus: none}} Kruskal is edge-centric. Sort once, iterate once, Union-Find per edge. No adjacency list required.

```code:kruskal-code lang=cpp
#include <vector>
#include <algorithm>
#include <tuple>
#include <numeric>

using Edge = std::tuple<int, int, int>;  // {weight, u, v}

struct UnionFind {
    std::vector<int> parent, rank;
    UnionFind(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rank[a] < rank[b]) std::swap(a, b);
        parent[b] = a;
        if (rank[a] == rank[b]) ++rank[a];
        return true;
    }
};

std::vector<Edge> kruskal(int n, std::vector<Edge> edges) {
    std::sort(edges.begin(), edges.end());  // sort by weight (first field)
    UnionFind uf(n);
    std::vector<Edge> mst;

    for (auto [w, u, v] : edges) {
        if (uf.unite(u, v)) {
            mst.emplace_back(w, u, v);
            if (static_cast<int>(mst.size()) == n - 1) break;
        }
    }
    return mst;
}
---
sort-step: 27
union-check: 30-34
mst-build: 33-34
```

# Union-Find with path compression

{{clear: slide}}

{{show: union-find-tree grow 0.5s spring}} Union-Find (disjoint set union) tracks which elements share a component. Two operations: `find` returns the root of a component, `union` merges two components. Path compression and union by rank make both nearly O(1).

{{focus: initial-state}} {{annotate: initial-state "Each alone"}} Initially, every element is its own parent. Five elements, five singleton components.

{{focus: after-union}} {{annotate: after-union "Merge 0,1,2"}} After uniting 0-1 and 1-2: one tree rooted at the winner. The rank determines who becomes root.

{{focus: path-compress}} {{annotate: path-compress "Flatten path"}} Path compression: when `find` walks up to the root, flatten the path — set every node's parent directly to the root. Future `find` calls on those nodes are O(1).

{{focus: union-rank}} {{annotate: union-rank "Stay short"}} Union by rank: always attach the shorter tree under the taller one. Keeps tree height logarithmic.

{{focus: none}} Together, path compression and union by rank give amortized inverse-Ackermann time per operation — effectively constant for any realistic input.

```data:union-find-tree type=tree
{id: 0, parent: 0}
{id: 1, parent: 0}
{id: 2, parent: 0}
{id: 3, parent: 3}
{id: 4, parent: 3}
---
initial-state: 0, 1, 2, 3, 4
after-union: 0, 1, 2
path-compress: 1, 2
union-rank: 0, 3
```

{{clear: slide}}

{{show: uf-code typewriter 2s linear}} Path compression in one line. Union by rank in four. The whole structure is under 20 lines.

{{focus: find-compress}} {{zoom: 1.2x}} {{annotate: find-compress "One-liner"}} `find` with path compression: if the node isn't its own parent, recurse and assign the result directly to `parent[x]`. One assignment flattens the entire path.

{{zoom: 1x}} {{focus: unite-logic}} {{zoom: 1.3x}} {{annotate: unite-logic "Rank decides"}} `unite`: find both roots. Same root means already connected. Attach smaller rank under larger. Increment rank only on a tie — that's the only case that increases height.

{{zoom: 1x}} {{focus: none}} Find and unite: the whole API. Everything Kruskal needs. No other data structure required.

```code:uf-code lang=cpp
#include <vector>
#include <numeric>

struct UnionFind {
    std::vector<int> parent, rank;

    explicit UnionFind(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);  // path compression
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;           // same component
        if (rank[a] < rank[b]) std::swap(a, b);
        parent[b] = a;                      // attach b under a
        if (rank[a] == rank[b]) ++rank[a];  // tie: increase rank
        return true;
    }
};
---
find-compress: 11-13
unite-logic: 17-24
```

# Prim's algorithm

{{clear: slide}}

{{show: prim-graph grow 0.5s spring}} Prim grows an MST one vertex at a time. Start from any vertex. Repeatedly add the cheapest edge that crosses from the current tree to an unvisited vertex. Stop when all vertices are in the tree.

{{focus: start-vertex}} {{annotate: start-vertex "Seed the tree"}} Pick any starting vertex. Mark it in the tree. Its edges are candidates.

{{focus: crossing-edges}} {{annotate: crossing-edges "Cheapest crossing"}} Among all edges from tree vertices to non-tree vertices, pick the cheapest. That's the min-cut edge — always safe by the cut property.

{{focus: grow-tree}} {{annotate: grow-tree "Add vertex"}} Add the new vertex to the tree. Add its outgoing edges to the candidate pool. Repeat.

{{focus: none}} Prim with a binary heap: O(E log V). Same asymptotic as Dijkstra — the algorithms are structurally nearly identical. Prim tracks key[v] (cheapest edge into v), Dijkstra tracks dist[v] (total path cost).

```data:prim-graph type=graph
0 -- 1 : 4
0 -- 2 : 3
1 -- 2 : 1
1 -- 3 : 2
2 -- 3 : 4
3 -- 4 : 2
---
start-vertex: 0
crossing-edges: 0-1, 0-2
grow-tree: 0, 2, 1, 3, 4
```

{{clear: slide}}

{{show: prim-code typewriter 2s linear}} Prim with a min-heap. The structure mirrors Dijkstra: extract min, check stale, relax neighbors.

{{focus: key-array}} {{zoom: 1.2x}} {{annotate: key-array "Edge cost to tree"}} `key[v]` holds the cheapest known edge cost connecting v to the current tree. Initialize to INF. Source node gets 0.

{{zoom: 1x}} {{focus: prim-extract}} {{zoom: 1.3x}} {{annotate: prim-extract "Extract cheapest"}} Extract the vertex with the smallest key. If already in MST, skip — stale entry.

{{zoom: 1x}} {{focus: prim-relax}} {{zoom: 1.2x}} {{annotate: prim-relax "Update key"}} For each neighbor not yet in MST: if the edge weight is less than its current key, update and push. We care about the edge weight, not the accumulated distance.

{{zoom: 1x}} {{focus: none}} Prim and Dijkstra share the same heap loop pattern. The difference: Dijkstra accumulates path cost, Prim only cares about the single edge into each vertex.

```code:prim-code lang=cpp
#include <vector>
#include <queue>
#include <limits>

using Graph = std::vector<std::vector<std::pair<int, int>>>;
constexpr int INF = std::numeric_limits<int>::max();

int prim_mst_cost(const Graph& adj, int src = 0) {
    int n = static_cast<int>(adj.size());
    std::vector<int> key(n, INF);
    std::vector<bool> in_mst(n, false);
    key[src] = 0;

    std::priority_queue<std::pair<int,int>,
        std::vector<std::pair<int,int>>,
        std::greater<>> pq;
    pq.push({0, src});

    int total = 0;
    while (!pq.empty()) {
        auto [cost, u] = pq.top(); pq.pop();
        if (in_mst[u]) continue;
        in_mst[u] = true;
        total += cost;

        for (auto [v, w] : adj[u]) {
            if (!in_mst[v] && w < key[v]) {
                key[v] = w;
                pq.push({w, v});
            }
        }
    }
    return total;
}
---
key-array: 10-12
prim-extract: 20-22
prim-relax: 26-30
```

# Kruskal vs Prim

{{clear: slide}}

{{show: comparison-table grow 0.5s spring}} Two algorithms, same problem, different trade-offs. The choice depends on graph density.

{{focus: kruskal-row}} {{annotate: kruskal-row "Sparse graphs"}} Kruskal is edge-centric. Sort all edges, run Union-Find. Complexity: O(E log E). For sparse graphs where E ≈ V, this is O(V log V). Excellent on sparse inputs.

{{focus: prim-row}} {{annotate: prim-row "Dense graphs"}} Prim is vertex-centric. For dense graphs where E ≈ V², a simple array priority queue gives O(V²). With a binary heap it's O(E log V) — better than Kruskal on dense graphs.

{{focus: impl-row}} {{annotate: impl-row "Implementation"}} Kruskal needs an edge list and Union-Find. Prim needs an adjacency list and a min-heap. Kruskal is often simpler to code in competitive programming.

{{focus: none}} Sparse graph: Kruskal. Dense graph: Prim with array priority queue. Modern hardware: just use Kruskal — Union-Find cache behavior is excellent.

```data:comparison-table type=hash-map
{"Property": "Data structure", "Kruskal": "Edge list", "Prim": "Adjacency list"}
{"Property": "Complexity", "Kruskal": "O(E log E)", "Prim": "O(E log V) heap"}
{"Property": "Best for", "Kruskal": "Sparse graphs", "Prim": "Dense graphs"}
{"Property": "Core operation", "Kruskal": "Union-Find", "Prim": "Min-heap extract"}
---
kruskal-row: 1, 2
prim-row: 1, 2
impl-row: 0, 3
```

# Applications

{{clear: slide}}

{{show: app-graph grow 0.5s spring}} MST appears everywhere minimum-cost connectivity is the goal. Three killer applications.

{{focus: network-design}} {{annotate: network-design "Cable routing"}} Network design: lay cables, pipes, or roads connecting all nodes at minimum cost. Direct translation of the MST problem. Used in real infrastructure planning.

{{focus: clustering}} {{annotate: clustering "Remove heavy edges"}} Clustering: build the MST, then remove the k-1 heaviest edges to get k clusters. The clusters minimize intra-cluster edge weight — a form of single-linkage hierarchical clustering.

{{focus: approx}} {{annotate: approx "2x optimal TSP"}} Approximation algorithms: the MST is the backbone of the 2-approximation for the metric Traveling Salesman Problem. The MST cost is always ≤ OPT. A DFS tour of the MST visits each edge twice — total ≤ 2 * OPT.

{{focus: none}} MST isn't just a graph theory concept. It's a building block for network optimization, unsupervised learning, and approximation algorithms for NP-hard problems.

```data:app-graph type=graph
A -- B : 5
A -- C : 3
B -- D : 2
C -- D : 4
C -- E : 6
D -- E : 1
---
network-design: A-C, B-D, D-E, A-B
clustering: C-E
approx: A, B, C, D, E
```
