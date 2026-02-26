---
title: Graph DFS
---

# Adjacency list in C++

{{show: adj-code typewriter 2s linear}} Graphs are nodes and edges. The adjacency list maps each node to its neighbors. Two common representations.

{{focus: vec-vec}} {{zoom: 1.2x}} {{annotate: vec-vec "Dense IDs"}} vector of vectors: when nodes are numbered 0 through n minus 1. Index i holds the list of i's neighbors. Compact. Cache-friendly. No hashing overhead.

{{zoom: 1x}} {{focus: map-based}} {{zoom: 1.3x}} {{annotate: map-based "Sparse IDs"}} unordered map: when node IDs are sparse or non-numeric. Strings, large integers, whatever. Each key maps to a vector of neighbors.

{{zoom: 1x}} {{focus: build-edges}} {{zoom: 1.2x}} {{annotate: build-edges "Add edges"}} Building the adjacency list: iterate over edges, push each neighbor. For undirected graphs, push both directions.

{{zoom: 1x}} {{focus: none}} vector of vectors is the default. Use the map when your node IDs don't fit neatly into 0 through n minus 1.

```code:adj-code lang=cpp
#include <vector>
#include <unordered_map>

// Option 1: vector<vector<int>> — nodes 0 to n-1
int n = 5;
std::vector<std::vector<int>> adj(n);
adj[0].push_back(1);    // edge 0 → 1
adj[0].push_back(2);    // edge 0 → 2
adj[1].push_back(3);    // edge 1 → 3

// Option 2: unordered_map — sparse or string IDs
std::unordered_map<std::string, std::vector<std::string>> graph;
graph["A"].push_back("B");
graph["A"].push_back("C");
graph["B"].push_back("D");

// Building from edge list (undirected):
std::vector<std::pair<int, int>> edges = {{0,1}, {0,2}, {1,3}, {2,4}};
for (auto [u, v] : edges) {
    adj[u].push_back(v);
    adj[v].push_back(u);     // undirected: both directions
}
---
vec-vec: 4-8
map-based: 11-14
build-edges: 17-21
```

# DFS on graphs

{{clear: slide}}

{{show: graph-dfs grow 0.5s spring}} Graph DFS is tree DFS with one addition: a visited set. Trees have no cycles, so you never revisit a node. Graphs can cycle, so you must track what you've seen.

{{focus: start}} {{annotate: start "Begin at 0"}} Start at node 0. Mark it visited. Explore its neighbors.

{{focus: explore}} {{annotate: explore "Go deeper"}} Visit unvisited neighbors depth-first. Same idea as tree DFS — go as deep as possible before backtracking.

{{focus: backtrack}} {{annotate: backtrack "All visited"}} When all neighbors of a node are visited, backtrack. The visited set prevents revisiting.

{{focus: none}} Without the visited set, cycles cause infinite loops. With it, each node is processed exactly once.

{{clear: slide}}

{{show: dfs-code typewriter 2s linear}} Recursive and iterative. Both use a visited vector.

{{focus: recursive}} {{zoom: 1.2x}} {{annotate: recursive "Natural recursion"}} Recursive DFS: mark visited, process, recurse on unvisited neighbors. Clean and readable. Use when recursion depth is bounded.

{{zoom: 1x}} {{focus: iterative}} {{zoom: 1.3x}} {{annotate: iterative "Explicit stack"}} Iterative DFS: push the start node. Pop, check visited, mark, push neighbors. The explicit stack replaces the call stack.

{{zoom: 1x}} {{focus: visited-check}} {{zoom: 1.2x}} {{annotate: visited-check "Prevent revisit"}} In the iterative version, check visited after popping, not before pushing. Nodes can be pushed multiple times from different neighbors — deduplication happens at pop time.

{{zoom: 1x}} {{focus: none}} Recursive for clarity, iterative for deep graphs. Both are O of V plus E — each vertex and edge visited once.

```data:graph-dfs type=graph
0 -- 1
0 -- 2
1 -- 3
2 -- 4
3 -- 4
---
start: 0
explore: 1, 3
backtrack: 4, 2
```

```code:dfs-code lang=cpp
#include <vector>
#include <stack>

// Recursive DFS
void dfs_recursive(int node, const std::vector<std::vector<int>>& adj,
                   std::vector<bool>& visited) {
    visited[node] = true;
    // process node here

    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            dfs_recursive(neighbor, adj, visited);
        }
    }
}

// Iterative DFS
std::vector<int> dfs_iterative(int start, const std::vector<std::vector<int>>& adj) {
    int n = static_cast<int>(adj.size());
    std::vector<bool> visited(n, false);
    std::vector<int> order;
    std::stack<int> stk;
    stk.push(start);

    while (!stk.empty()) {
        int node = stk.top();
        stk.pop();
        if (visited[node]) continue;
        visited[node] = true;
        order.push_back(node);

        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                stk.push(neighbor);
            }
        }
    }
    return order;
}
---
recursive: 5-14
iterative: 17-37
visited-check: 27
```

# Connected components

{{clear: slide}}

{{show: components-code typewriter 2s linear}} An undirected graph might be disconnected. Connected components are the isolated subgraphs. DFS from an unvisited node discovers its entire component.

{{focus: outer}} {{zoom: 1.2x}} {{annotate: outer "Scan all nodes"}} Iterate over every node. If it's unvisited, it starts a new component. Run DFS from it.

{{zoom: 1x}} {{focus: dfs-call}} {{zoom: 1.3x}} {{annotate: dfs-call "Discover group"}} The DFS marks everything reachable from the start node. All marked nodes belong to the same component.

{{zoom: 1x}} {{focus: count}} {{zoom: 1.2x}} {{annotate: count "Increment"}} Each time the outer loop finds an unvisited node, increment the component count. The DFS handles the rest.

{{zoom: 1x}} {{focus: none}} Number of connected components: count DFS calls. Component membership: label each node with its component ID. Same DFS, different bookkeeping.

```code:components-code lang=cpp
#include <vector>

void dfs(int node, const std::vector<std::vector<int>>& adj,
         std::vector<bool>& visited) {
    visited[node] = true;
    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            dfs(neighbor, adj, visited);
        }
    }
}

int count_components(int n, const std::vector<std::vector<int>>& adj) {
    std::vector<bool> visited(n, false);
    int components = 0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfs(i, adj, visited);
            ++components;
        }
    }
    return components;
}
---
outer: 17-22
dfs-call: 19
count: 20
```

# Cycle detection

{{clear: slide}}

{{show: cycle-detect grow 0.5s spring}} Detecting cycles in a directed graph. Three colors: white (unvisited), gray (in current DFS path), black (fully processed).

{{focus: white}} {{annotate: white "Unvisited"}} White: the node hasn't been touched yet. DFS hasn't reached it.

{{focus: gray}} {{annotate: gray "Active path"}} Gray: the node is on the current recursion stack. We're exploring its descendants right now.

{{focus: black}} {{annotate: black "Done"}} Black: the node and all its descendants are fully explored. No cycle passes through it.

{{focus: cycle-edge}} {{annotate: cycle-edge "Gray → gray"}} The cycle signal: you encounter a gray node from a gray node. That means you've found a back edge — a path from a descendant back to an ancestor. Cycle confirmed.

{{focus: none}} White to gray to black. If you see gray while exploring, there's a cycle.

{{clear: slide}}

{{show: cycle-code typewriter 2s linear}} Three-color DFS in code.

{{focus: colors}} {{zoom: 1.2x}} {{annotate: colors "0/1/2"}} Use an int vector: 0 for white, 1 for gray, 2 for black. Clean and fast.

{{zoom: 1x}} {{focus: enter}} {{zoom: 1.3x}} {{annotate: enter "Go gray"}} When entering a node, mark it gray. We're now exploring its subtree.

{{zoom: 1x}} {{focus: detect}} {{zoom: 1.2x}} {{annotate: detect "Back edge"}} If a neighbor is gray, we've found a back edge. Return true — there's a cycle.

{{zoom: 1x}} {{focus: finish}} {{zoom: 1.2x}} {{annotate: finish "Go black"}} When all neighbors are processed, mark the node black. It's fully explored.

{{zoom: 1x}} {{focus: none}} Three-color DFS detects cycles in O of V plus E. It's the standard technique for directed graphs.

```data:cycle-detect type=graph
0 -- 1
1 -- 2
2 -- 0
2 -- 3
---
white: 3
gray: 0, 1, 2
black: 3
cycle-edge: 2, 0
```

```code:cycle-code lang=cpp
#include <vector>

bool has_cycle_dfs(int node, const std::vector<std::vector<int>>& adj,
                   std::vector<int>& color) {
    color[node] = 1;  // gray

    for (int neighbor : adj[node]) {
        if (color[neighbor] == 1) return true;   // back edge → cycle
        if (color[neighbor] == 0) {
            if (has_cycle_dfs(neighbor, adj, color)) return true;
        }
    }

    color[node] = 2;  // black
    return false;
}

bool has_cycle(int n, const std::vector<std::vector<int>>& adj) {
    std::vector<int> color(n, 0);  // all white
    for (int i = 0; i < n; ++i) {
        if (color[i] == 0) {
            if (has_cycle_dfs(i, adj, color)) return true;
        }
    }
    return false;
}
---
colors: 19
enter: 5
detect: 8
finish: 14
```

# Topological sort

{{clear: slide}}

{{show: topo-code typewriter 2s linear}} Topological sort orders vertices so that every edge points forward. If A depends on B, B comes first. Only works on directed acyclic graphs.

{{focus: dfs-approach}} {{zoom: 1.2x}} {{annotate: dfs-approach "Post-order"}} DFS-based topo sort: run DFS. When a node finishes (all descendants explored), push it to a result list. The finishing order, reversed, is the topological order.

{{zoom: 1x}} {{focus: push-finish}} {{zoom: 1.3x}} {{annotate: push-finish "After children"}} Push the node after the recursive calls return. This guarantees all dependents are already in the list.

{{zoom: 1x}} {{focus: reverse}} {{zoom: 1.2x}} {{annotate: reverse "Flip it"}} Reverse the result. DFS finish order is reverse topological order. The first node to finish has no outgoing edges that matter — it goes last.

{{zoom: 1x}} {{focus: multi-start}} {{annotate: multi-start "Cover all"}} Iterate over all nodes. Some may be unreachable from any single start. Launch DFS from every unvisited node.

{{zoom: 1x}} {{focus: none}} DFS topo sort: finish order reversed. O of V plus E. If the graph has a cycle, topo sort is impossible — detect it with the three-color method.

```code:topo-code lang=cpp
#include <vector>
#include <algorithm>

void topo_dfs(int node, const std::vector<std::vector<int>>& adj,
              std::vector<bool>& visited, std::vector<int>& order) {
    visited[node] = true;

    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            topo_dfs(neighbor, adj, visited, order);
        }
    }

    order.push_back(node);  // post-order
}

std::vector<int> topological_sort(int n, const std::vector<std::vector<int>>& adj) {
    std::vector<bool> visited(n, false);
    std::vector<int> order;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            topo_dfs(i, adj, visited, order);
        }
    }

    std::reverse(order.begin(), order.end());
    return order;
}
---
dfs-approach: 4-15
push-finish: 14
reverse: 27
multi-start: 21-25
```

# Grid DFS

{{clear: slide}}

{{show: grid-dfs grow 0.5s spring}} A 2D grid is an implicit graph. Each cell is a node. Neighbors are the four adjacent cells — up, down, left, right.

{{focus: land}} {{annotate: land "Island cells"}} In the islands problem, '1' is land, '0' is water. An island is a connected region of land cells.

{{focus: flood}} {{annotate: flood "DFS flood fill"}} DFS from a land cell marks every connected land cell as visited. One DFS call discovers one island.

{{focus: none}} Count the DFS calls. Each one discovers a new island. Same logic as connected components, but on a grid.

{{clear: slide}}

{{show: grid-code typewriter 2s linear}} Grid DFS: directions array, bounds check, mark visited.

{{focus: dirs}} {{zoom: 1.2x}} {{annotate: dirs "Four neighbors"}} The directions array encodes up, down, left, right as coordinate offsets. Loop over it to visit all four neighbors.

{{zoom: 1x}} {{focus: bounds}} {{zoom: 1.3x}} {{annotate: bounds "Stay in grid"}} Check bounds before recursing. Out of range? Skip. Water? Skip. Already visited? Skip.

{{zoom: 1x}} {{focus: mark}} {{zoom: 1.2x}} {{annotate: mark "Sink the land"}} Mark visited by changing '1' to '0'. No separate visited array needed — the grid itself is the bookkeeping.

{{zoom: 1x}} {{focus: count-islands}} {{zoom: 1.2x}} {{annotate: count-islands "Count DFS calls"}} Scan every cell. Unvisited land starts a DFS and increments the island count.

{{zoom: 1x}} {{focus: none}} Grid DFS is graph DFS with implicit edges. The directions array and bounds check replace the adjacency list.

```data:grid-dfs type=matrix
[["1","1","0","0","0"],
 ["1","1","0","0","0"],
 ["0","0","1","0","0"],
 ["0","0","0","1","1"]]
---
land: [0][0], [0][1], [1][0], [1][1]
flood: [0][0], [0][1], [1][0], [1][1]
```

```code:grid-code lang=cpp
#include <vector>
#include <array>

void dfs_grid(std::vector<std::vector<char>>& grid, int r, int c) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    if (grid[r][c] != '1') return;

    grid[r][c] = '0';  // mark visited

    constexpr std::array<std::pair<int,int>, 4> dirs = {{{-1,0},{1,0},{0,-1},{0,1}}};
    for (auto [dr, dc] : dirs) {
        dfs_grid(grid, r + dr, c + dc);
    }
}

int count_islands(std::vector<std::vector<char>>& grid) {
    int count = 0;
    for (int r = 0; r < static_cast<int>(grid.size()); ++r) {
        for (int c = 0; c < static_cast<int>(grid[0].size()); ++c) {
            if (grid[r][c] == '1') {
                dfs_grid(grid, r, c);
                ++count;
            }
        }
    }
    return count;
}
---
dirs: 12
bounds: 7-8
mark: 10
count-islands: 18-27
```
