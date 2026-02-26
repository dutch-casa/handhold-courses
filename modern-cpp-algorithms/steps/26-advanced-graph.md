---
title: Advanced Graph Algorithms
---

# Floyd-Warshall

{{show: fw-matrix grow 0.5s spring}} Floyd-Warshall solves all-pairs shortest paths in one shot. The result: a 2D matrix where `dist[i][j]` is the shortest path from i to j. O(V³) time, O(V²) space. The triple loop is the whole algorithm.

{{focus: init-matrix}} {{annotate: init-matrix "Direct edges"}} Initialize: `dist[i][j]` = edge weight if the edge exists, 0 on the diagonal, INF otherwise. Each cell starts at the best direct connection.

{{focus: relax-step}} {{annotate: relax-step "Via vertex k"}} The core: for each intermediate vertex k, for each pair (i, j), check if going i→k→j is shorter than the current i→j. If so, update.

{{focus: filled}} {{annotate: filled "All paths"}} After all V rounds of k, every `dist[i][j]` holds the shortest path considering all possible intermediate vertices. No single-source restriction — all pairs simultaneously.

{{focus: none}} Floyd-Warshall is beautifully simple: three nested loops, one comparison. The k loop must be outermost — that ordering is the correctness invariant.

```data:fw-matrix type=matrix
[[0, 3, INF, 7],
 [8, 0, 2, INF],
 [5, INF, 0, 1],
 [2, INF, INF, 0]]
---
init-matrix: [0][0], [0][1], [1][2], [2][3], [3][0]
relax-step: [0][2], [0][3], [1][3]
filled: [0][0], [0][1], [0][2], [0][3]
```

{{clear: slide}}

{{show: fw-code typewriter 2s linear}} Floyd-Warshall in code. The outer loop index is k — the intermediate vertex. Don't swap the loop order.

{{focus: init-code}} {{zoom: 1.2x}} {{annotate: init-code "Seed matrix"}} Initialize from the edge list. Zero the diagonal. Everything unreachable starts at INF.

{{zoom: 1x}} {{focus: triple-loop}} {{zoom: 1.3x}} {{annotate: triple-loop "k outermost"}} k must be the outermost loop. That ordering ensures that when we use `dist[i][k]` and `dist[k][j]`, those values already account for all intermediate vertices with index less than k.

{{zoom: 1x}} {{focus: fw-relax}} {{zoom: 1.2x}} {{annotate: fw-relax "Single update"}} The update: if going through k is cheaper, take it. Guard against INF overflow by checking before adding.

{{zoom: 1x}} {{focus: neg-cycle-fw}} {{zoom: 1.2x}} {{annotate: neg-cycle-fw "Diagonal check"}} Negative cycle detection: if `dist[i][i]` becomes negative after running, there's a negative cycle reachable from i. The algorithm is still correct for graphs without negative cycles.

{{zoom: 1x}} {{focus: none}} Three loops, one update. Floyd-Warshall is the most compact all-pairs algorithm.

```code:fw-code lang=cpp
#include <vector>
#include <limits>
#include <tuple>

constexpr int INF = std::numeric_limits<int>::max() / 2;

std::vector<std::vector<int>> floyd_warshall(
    int n, const std::vector<std::tuple<int,int,int>>& edges)
{
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, INF));

    for (int i = 0; i < n; ++i) dist[i][i] = 0;
    for (auto [u, v, w] : edges) {
        dist[u][v] = w;
        dist[v][u] = w;  // undirected
    }

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    return dist;
}
---
init-code: 10-14
triple-loop: 16-24
fw-relax: 19-21
neg-cycle-fw: 11
```

# Tarjan's SCC

{{clear: slide}}

{{show: scc-graph grow 0.5s spring}} Strongly Connected Components (SCCs) in a directed graph: maximal sets where every vertex can reach every other. Tarjan finds all SCCs in one DFS pass using discovery times and low-link values.

{{focus: disc-low}} {{annotate: disc-low "Two numbers"}} Each vertex gets two numbers: `disc` (discovery time) and `low` (lowest disc reachable via DFS tree + one back edge). Low tracks how far back we can reach.

{{focus: scc-found}} {{annotate: scc-found "Root detection"}} A vertex u is the root of an SCC when `low[u] == disc[u]`. No back edge can escape this SCC to an older vertex. Pop the stack up to u — that's the SCC.

{{focus: stack-usage}} {{annotate: stack-usage "On-stack set"}} The DFS stack collects the current SCC's candidates. When the root is detected, pop all members of that SCC off the stack.

{{focus: none}} Tarjan: one DFS, two arrays, one stack. O(V+E). Every vertex and edge touched exactly once.

```data:scc-graph type=graph
0 -> 1
1 -> 2
2 -> 0
2 -> 3
3 -> 4
4 -> 3
---
disc-low: 0, 1, 2, 3, 4
scc-found: 0, 3
stack-usage: 0, 1, 2
```

{{clear: slide}}

{{show: tarjan-code typewriter 2s linear}} Tarjan's implementation. The low-link update is the key — two cases: back edge and tree edge.

{{focus: tarjan-entry}} {{zoom: 1.2x}} {{annotate: tarjan-entry "Init on enter"}} On entry: assign disc and low equal to the timer, increment timer, push onto stack, mark on-stack.

{{zoom: 1x}} {{focus: low-update}} {{zoom: 1.3x}} {{annotate: low-update "Propagate low"}} Two update cases: if a neighbor is on-stack, it's a back edge — update low with neighbor's disc. If it's an unvisited neighbor (tree edge), recurse and pull back the low value.

{{zoom: 1x}} {{focus: root-pop}} {{zoom: 1.2x}} {{annotate: root-pop "Pop SCC"}} Root check: `low[u] == disc[u]`. Pop the stack until u is popped. Those vertices form one SCC.

{{zoom: 1x}} {{focus: none}} The on-stack check distinguishes back edges from cross edges. Cross edges don't participate in SCC formation — back edges do.

```code:tarjan-code lang=cpp
#include <vector>
#include <stack>
#include <algorithm>

struct Tarjan {
    const std::vector<std::vector<int>>& adj;
    int n, timer = 0;
    std::vector<int> disc, low;
    std::vector<bool> on_stack;
    std::stack<int> stk;
    std::vector<std::vector<int>> sccs;

    explicit Tarjan(const std::vector<std::vector<int>>& g)
        : adj(g), n(static_cast<int>(g.size())),
          disc(n, -1), low(n), on_stack(n, false) {}

    void dfs(int u) {
        disc[u] = low[u] = timer++;
        stk.push(u);
        on_stack[u] = true;

        for (int v : adj[u]) {
            if (disc[v] == -1) {
                dfs(v);
                low[u] = std::min(low[u], low[v]);  // tree edge
            } else if (on_stack[v]) {
                low[u] = std::min(low[u], disc[v]);  // back edge
            }
        }

        if (low[u] == disc[u]) {  // u is root of SCC
            sccs.emplace_back();
            while (true) {
                int v = stk.top(); stk.pop();
                on_stack[v] = false;
                sccs.back().push_back(v);
                if (v == u) break;
            }
        }
    }

    std::vector<std::vector<int>> run() {
        for (int i = 0; i < n; ++i)
            if (disc[i] == -1) dfs(i);
        return sccs;
    }
};
---
tarjan-entry: 17-20
low-update: 22-26
root-pop: 29-36
```

# Bridges and articulation points

{{clear: slide}}

{{show: bridge-graph grow 0.5s spring}} Bridges are edges whose removal disconnects the graph. Articulation points are vertices whose removal disconnects it. Both are critical infrastructure in the graph.

{{focus: bridge-edge}} {{annotate: bridge-edge "No bypass"}} A bridge exists when there's no back edge bypassing it — removing it leaves some vertices stranded. Low-link detects it: edge (u,v) is a bridge if `low[v] > disc[u]`.

{{focus: artic-point}} {{annotate: artic-point "Cut vertex"}} An articulation point has a child in the DFS tree with no back edge reaching above the articulation point. `low[v] >= disc[u]` for some child v of u (u not being root).

{{focus: root-special}} {{annotate: root-special "Root: 2+ children"}} The DFS root is an articulation point iff it has 2 or more children in the DFS tree. Special-case it.

{{focus: none}} Same DFS machinery as Tarjan — disc, low, one pass. Bridges and articulation points are found simultaneously in O(V+E).

```data:bridge-graph type=graph
0 -- 1
1 -- 2
2 -- 3
3 -- 4
4 -- 2
---
bridge-edge: 0-1, 1-2
artic-point: 1, 2
root-special: 0
```

{{clear: slide}}

{{show: bridge-code typewriter 2s linear}} Bridge finding. The critical condition is one comparison after the DFS call returns.

{{focus: bridge-init}} {{zoom: 1.2x}} {{annotate: bridge-init "Track disc/low"}} Same setup as Tarjan: discovery time and low-link per vertex. A timer, initialized once, increments on each visit.

{{zoom: 1x}} {{focus: bridge-check}} {{zoom: 1.3x}} {{annotate: bridge-check "low > disc"}} After recursing into v, if `low[v] > disc[u]`, the edge (u,v) is a bridge. No back edge from v's subtree reaches u or above — this edge is the only connection.

{{zoom: 1x}} {{focus: parent-skip}} {{zoom: 1.2x}} {{annotate: parent-skip "Skip tree parent"}} Skip the parent in the low-link update. We track the parent explicitly for this reason — we don't want to count the tree edge upward as a back edge.

{{zoom: 1x}} {{focus: none}} Bridge detection: one extra condition after DFS returns. The same code modified for articulation points checks `low[v] >= disc[u]` instead.

```code:bridge-code lang=cpp
#include <vector>

struct BridgeFinder {
    const std::vector<std::vector<int>>& adj;
    int n, timer = 0;
    std::vector<int> disc, low;
    std::vector<std::pair<int,int>> bridges;

    explicit BridgeFinder(const std::vector<std::vector<int>>& g)
        : adj(g), n(static_cast<int>(g.size())), disc(n, -1), low(n) {}

    void dfs(int u, int parent) {
        disc[u] = low[u] = timer++;

        for (int v : adj[u]) {
            if (v == parent) continue;             // skip tree parent
            if (disc[v] == -1) {
                dfs(v, u);
                low[u] = std::min(low[u], low[v]);
                if (low[v] > disc[u])              // bridge condition
                    bridges.emplace_back(u, v);
            } else {
                low[u] = std::min(low[u], disc[v]);
            }
        }
    }

    std::vector<std::pair<int,int>> run() {
        for (int i = 0; i < n; ++i)
            if (disc[i] == -1) dfs(i, -1);
        return bridges;
    }
};
---
bridge-init: 6-7
bridge-check: 19-20
parent-skip: 14
```

# Bipartite checking

{{clear: slide}}

{{show: bipartite-graph grow 0.5s spring}} A graph is bipartite if its vertices can be split into two groups where all edges go between groups — never within. Equivalently: it's 2-colorable. Equivalently: it has no odd-length cycles.

{{focus: two-color}} {{annotate: two-color "Color 0 and 1"}} BFS-color the graph: assign 0 to the source, 1 to all neighbors, 0 to their unvisited neighbors, and so on. If you ever need to assign a color that conflicts with an existing assignment, it's not bipartite.

{{focus: conflict}} {{annotate: conflict "Same color"}} A conflict: two adjacent vertices share the same color. That means they're in the same group — not bipartite. An odd cycle forced it.

{{focus: multi-component}} {{annotate: multi-component "All components"}} Iterate over all vertices and launch BFS from unvisited ones. Bipartite checking applies to all connected components.

{{focus: none}} Two-coloring with BFS: O(V+E). The color array doubles as the visited array. Clean single-pass algorithm.

```data:bipartite-graph type=graph
0 -- 1
0 -- 3
1 -- 2
2 -- 3
---
two-color: 0, 1, 2, 3
conflict: none
multi-component: 0, 1, 2, 3
```

{{clear: slide}}

{{show: bipartite-code typewriter 2s linear}} BFS two-coloring. The conflict check is one line inside the BFS loop.

{{focus: color-array}} {{zoom: 1.2x}} {{annotate: color-array "0, 1, unset"}} Color array initialized to -1 (unset). 0 and 1 are the two groups. We check and assign in the same loop.

{{zoom: 1x}} {{focus: bfs-color}} {{zoom: 1.3x}} {{annotate: bfs-color "Flip color"}} When enqueuing a neighbor, assign the opposite color: `1 - color[u]`. Simple integer flip.

{{zoom: 1x}} {{focus: conflict-check}} {{zoom: 1.2x}} {{annotate: conflict-check "Same? Not bipartite"}} If a neighbor is already colored and its color matches the current node, conflict — return false immediately.

{{zoom: 1x}} {{focus: none}} Bipartite check: BFS with color flipping and conflict detection. One function, one pass, complete.

```code:bipartite-code lang=cpp
#include <vector>
#include <queue>

bool is_bipartite(int n, const std::vector<std::vector<int>>& adj) {
    std::vector<int> color(n, -1);

    for (int start = 0; start < n; ++start) {
        if (color[start] != -1) continue;
        color[start] = 0;
        std::queue<int> q;
        q.push(start);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false;          // conflict
                }
            }
        }
    }
    return true;
}
---
color-array: 5
bfs-color: 16
conflict-check: 18-20
```

# Euler and Hamilton paths

{{clear: slide}}

{{show: euler-graph grow 0.5s spring}} Two famous path problems with completely different hardness. Euler circuits visit every edge once. Hamiltonian cycles visit every vertex once. One is polynomial, the other NP-hard.

{{focus: euler-condition}} {{annotate: euler-condition "Degree parity"}} Euler circuit exists iff the graph is connected and every vertex has even degree. Euler path (not circuit) exists iff exactly two vertices have odd degree — those are the start and end. Check degrees: O(V+E).

{{focus: hamilton-hard}} {{annotate: hamilton-hard "NP-complete"}} Hamiltonian cycle: visit every vertex exactly once and return to start. No efficient algorithm is known. Checking all permutations is O(V!). Even determining existence is NP-complete.

{{focus: hierholzer}} {{annotate: hierholzer "Follow edges"}} Finding an Euler circuit: Hierholzer's algorithm. Walk the graph, removing edges as you go. When stuck, backtrack and splice in the new circuit. O(E) time.

{{focus: none}} Euler: solvable in linear time with degree check. Hamiltonian: intractable in general. The difference looks minor but is fundamental — edge-cover vs vertex-cover hardness gap.

```data:euler-graph type=graph
0 -- 1
1 -- 2
2 -- 3
3 -- 0
0 -- 2
---
euler-condition: 0, 1, 2, 3
hamilton-hard: 0, 1, 2, 3
hierholzer: 0, 1, 2, 3, 0
```

{{clear: slide}}

{{show: hierholzer-code typewriter 2s linear}} Hierholzer's algorithm for Euler circuits. Stack-based, destroys edges as it walks.

{{focus: degree-check}} {{zoom: 1.2x}} {{annotate: degree-check "Prevalidate"}} Check all vertex degrees before starting. Odd degree means no Euler circuit exists — fail fast.

{{zoom: 1x}} {{focus: edge-consume}} {{zoom: 1.3x}} {{annotate: edge-consume "Walk and remove"}} Walk along unused edges. Use an index per vertex to track which edges remain — no actual deletion needed. The index advances past used edges.

{{zoom: 1x}} {{focus: circuit-build}} {{zoom: 1.2x}} {{annotate: circuit-build "Build in reverse"}} When stuck (all edges from current vertex used), push to result. The result is built in reverse order — flip at the end.

{{zoom: 1x}} {{focus: none}} Hierholzer: O(E). Stack-based. Each edge is traversed exactly once. The edge index approach avoids modifying the adjacency list.

```code:hierholzer-code lang=cpp
#include <vector>
#include <stack>
#include <algorithm>

std::vector<int> euler_circuit(int n, std::vector<std::vector<int>> adj) {
    // Check: all vertices must have even degree
    for (int i = 0; i < n; ++i) {
        if (adj[i].size() % 2 != 0) return {};  // no Euler circuit
    }

    std::vector<int> idx(n, 0);  // next unused edge index per vertex
    std::stack<int> stk;
    std::vector<int> circuit;
    stk.push(0);

    while (!stk.empty()) {
        int u = stk.top();
        if (idx[u] < static_cast<int>(adj[u].size())) {
            int v = adj[u][idx[u]++];
            stk.push(v);
        } else {
            circuit.push_back(u);
            stk.pop();
        }
    }

    std::reverse(circuit.begin(), circuit.end());
    return circuit;
}
---
degree-check: 7-8
edge-consume: 16-19
circuit-build: 20-22
```

# Algorithm decision tree

{{clear: slide}}

{{show: decision-table grow 0.5s spring}} Which graph algorithm for which problem? The decision depends on three questions: single-source or all-pairs, weighted or unweighted, negative weights or not.

{{focus: unweighted-row}} {{annotate: unweighted-row "BFS"}} Unweighted shortest paths: BFS. O(V+E). Don't reach for Dijkstra when edge weights are uniform.

{{focus: single-nonneg}} {{annotate: single-nonneg "Dijkstra"}} Single-source, non-negative weights: Dijkstra with a binary heap. O((V+E) log V). The workhorse of competitive programming and production systems.

{{focus: single-neg}} {{annotate: single-neg "Bellman-Ford"}} Single-source, negative weights, no negative cycles: Bellman-Ford. O(V*E). Slow but correct. Negative cycle detection is a bonus.

{{focus: all-pairs}} {{annotate: all-pairs "Floyd-Warshall"}} All-pairs: Floyd-Warshall. O(V³). Simple to implement, handles negative weights, detects negative cycles via the diagonal.

{{focus: connectivity}} {{annotate: connectivity "SCC/Bridge"}} Connectivity problems: Tarjan SCC, bridge finding, articulation points. All O(V+E). One DFS pass handles them all.

{{focus: none}} Know these patterns cold. Most graph problems reduce to one of these. The choice of algorithm is often the entire solution.

```data:decision-table type=hash-map
{"Problem": "Shortest path, no weights", "Algorithm": "BFS", "Complexity": "O(V+E)"}
{"Problem": "Single-source, non-negative", "Algorithm": "Dijkstra", "Complexity": "O((V+E) log V)"}
{"Problem": "Single-source, negative weights", "Algorithm": "Bellman-Ford", "Complexity": "O(V·E)"}
{"Problem": "All-pairs shortest paths", "Algorithm": "Floyd-Warshall", "Complexity": "O(V³)"}
{"Problem": "Minimum spanning tree", "Algorithm": "Kruskal / Prim", "Complexity": "O(E log E)"}
{"Problem": "Strongly connected components", "Algorithm": "Tarjan", "Complexity": "O(V+E)"}
{"Problem": "Bipartite check", "Algorithm": "BFS 2-color", "Complexity": "O(V+E)"}
---
unweighted-row: 0
single-nonneg: 1
single-neg: 2
all-pairs: 3
connectivity: 5
```
