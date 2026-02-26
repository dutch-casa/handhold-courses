---
title: BFS pattern
---

# BFS vs DFS

{{show: bfs-dfs-viz grow 0.5s spring}} Two traversal strategies, opposite priorities. DFS goes deep immediately — it commits to one path and follows it to the end before trying another. BFS explores by distance — it visits everything one hop away before moving to two hops.

{{focus: dfs-path}} {{annotate: dfs-path "Depth first"}} DFS: start at the root, go left, keep going, hit a leaf, backtrack, go right. It discovers distant nodes before nearby siblings.

{{focus: bfs-wave}} {{annotate: bfs-wave "Level by level"}} BFS: start at the root, visit all neighbors, then all their neighbors, expanding outward like a wavefront. Distance from the source is the invariant.

{{focus: tradeoffs}} {{annotate: tradeoffs "When to pick"}} DFS: better for path finding, topological sort, cycle detection, problems that need the whole tree. BFS: better for shortest paths, minimum steps, level-order output, reachability in unweighted graphs.

{{focus: none}} DFS uses a stack — either the call stack or an explicit one. BFS uses a queue. The data structure is the strategy.

```data:bfs-dfs-viz type=array
["DFS: goes deep fast — path 1→2→4→5→3", "BFS: explores by distance — level 0: [1], level 1: [2,3], level 2: [4,5]"]
---
dfs-path: 0
bfs-wave: 1
tradeoffs: 0, 1
```

# Queue-based BFS

{{clear: slide}}

{{show: bfs-template typewriter 2s linear}} The BFS template. Queue, visited set, level-by-level expansion. Memorize this shape.

{{focus: init}} {{zoom: 1.2x}} {{annotate: init "Seed the queue"}} Push the start node and mark it visited. Marking at enqueue time prevents duplicates — never mark at dequeue time, or you'll enqueue the same node many times.

{{zoom: 1x}} {{focus: outer-loop}} {{zoom: 1.3x}} {{annotate: outer-loop "Until empty"}} Process until the queue is empty. Each iteration processes one node. When the queue drains, all reachable nodes have been visited.

{{zoom: 1x}} {{focus: dequeue}} {{zoom: 1.2x}} {{annotate: dequeue "Front of line"}} Dequeue from the front. BFS always processes in arrival order — FIFO. This guarantees distance ordering.

{{zoom: 1x}} {{focus: enqueue}} {{zoom: 1.3x}} {{annotate: enqueue "Discover neighbors"}} For each unvisited neighbor, mark it visited and enqueue it. Marking before enqueuing is critical — prevents redundant work.

{{zoom: 1x}} {{focus: none}} Queue, mark-at-enqueue, FIFO. These three properties make BFS correct and efficient. O of V plus E — every vertex and edge processed once.

```code:bfs-template lang=cpp
#include <vector>
#include <queue>

void bfs(int start, const std::vector<std::vector<int>>& adj) {
    int n = static_cast<int>(adj.size());
    std::vector<bool> visited(n, false);
    std::queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        // process node here

        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}
---
init: 9-10
outer-loop: 12
dequeue: 13-14
enqueue: 17-20
```

# Tree level-order

{{clear: slide}}

{{show: level-tree grow 0.5s spring}} BFS on a binary tree gives level-order traversal — all nodes at depth 0, then depth 1, then depth 2. The trick for grouping by level: process the queue in batches.

{{focus: level-0}} {{annotate: level-0 "Root"}} Level 0: just the root. One node.

{{focus: level-1}} {{annotate: level-1 "Children"}} Level 1: the root's two children. Two nodes.

{{focus: level-2}} {{annotate: level-2 "Grandchildren"}} Level 2: the four grandchildren. Four nodes.

{{focus: none}} Each level's nodes are in the queue together. Process them all before pushing the next level.

{{clear: slide}}

{{show: levelorder-code typewriter 2s linear}} Capture queue size at the start of each level iteration. Process exactly that many nodes, then start the next level.

{{focus: level-list}} {{zoom: 1.2x}} {{annotate: level-list "This level's nodes"}} Collect all nodes from this level into a local vector before pushing their children.

{{zoom: 1x}} {{focus: level-size}} {{zoom: 1.3x}} {{annotate: level-size "Frozen count"}} Capture the queue size before processing. Only process that many — don't include the next level's nodes that get enqueued during this loop.

{{zoom: 1x}} {{focus: push-children}} {{zoom: 1.2x}} {{annotate: push-children "Next level"}} Push non-null children. They form the next batch. The outer while loop will handle them next iteration.

{{zoom: 1x}} {{focus: none}} The frozen level size is the key. It's what separates level-order from simple BFS. One extra variable, dramatically different output structure.

```data:level-tree type=tree
     1
    / \
   2   3
  / \ / \
 4  5 6  7
---
level-0: 1
level-1: 2, 3
level-2: 4, 5, 6, 7
```

```code:levelorder-code lang=cpp
#include <vector>
#include <queue>

std::vector<std::vector<int>> level_order(const TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) return result;

    std::queue<const TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int level_size = static_cast<int>(q.size());
        std::vector<int> level;

        for (int i = 0; i < level_size; ++i) {
            const auto* node = q.front();
            q.pop();
            level.push_back(node->val);

            if (node->left)  q.push(node->left.get());
            if (node->right) q.push(node->right.get());
        }

        result.push_back(std::move(level));
    }
    return result;
}
---
level-list: 13
level-size: 11
push-children: 19-20
```

# Graph BFS

{{clear: slide}}

{{show: graph-bfs grow 0.5s spring}} BFS on an unweighted graph finds the shortest path by node count. The first time you reach a node, you've taken the fewest hops. That's BFS's guarantee.

{{focus: source}} {{annotate: source "Distance 0"}} The source node is at distance 0. It's in the queue first.

{{focus: dist-1}} {{annotate: dist-1 "Distance 1"}} All nodes adjacent to the source are at distance 1. They're processed second.

{{focus: dist-2}} {{annotate: dist-2 "Distance 2"}} All nodes two hops away are at distance 2. BFS expands the wavefront outward.

{{focus: none}} Track distances with a vector initialized to -1. Set the distance when you enqueue. When you reach the target, you have its shortest path length.

{{clear: slide}}

{{show: graph-bfs-code typewriter 2s linear}} BFS with distance tracking for shortest path.

{{focus: dist-vec}} {{zoom: 1.2x}} {{annotate: dist-vec "Distances"}} A distance vector tracks hops from the source. Initialized to -1 (unvisited). The source gets distance 0.

{{zoom: 1x}} {{focus: relax}} {{zoom: 1.3x}} {{annotate: relax "One more hop"}} When you enqueue a neighbor, its distance is the current node's distance plus one. This is the BFS distance formula.

{{zoom: 1x}} {{focus: target-check}} {{zoom: 1.2x}} {{annotate: target-check "Early exit"}} If you're looking for a specific target, check at dequeue. Once found, the stored distance is the shortest path length.

{{zoom: 1x}} {{focus: none}} BFS on unweighted graphs: shortest path by hop count. For weighted graphs, use Dijkstra. BFS is Dijkstra with all edge weights equal to one.

```data:graph-bfs type=graph
0 -- 1
0 -- 2
1 -- 3
2 -- 3
3 -- 4
---
source: 0
dist-1: 1, 2
dist-2: 3
```

```code:graph-bfs-code lang=cpp
#include <vector>
#include <queue>

std::vector<int> bfs_distances(int start, int target,
                               const std::vector<std::vector<int>>& adj) {
    int n = static_cast<int>(adj.size());
    std::vector<int> dist(n, -1);
    std::queue<int> q;

    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        if (node == target) return dist;

        for (int neighbor : adj[node]) {
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[node] + 1;
                q.push(neighbor);
            }
        }
    }
    return dist;
}
---
dist-vec: 8-10
relax: 20-21
target-check: 17
```

# Grid BFS

{{clear: slide}}

{{show: maze-bfs grow 0.5s spring}} BFS on a grid solves maze problems. Cells are nodes, passable cells are connected. BFS finds the minimum number of steps from start to end.

{{focus: wave-0}} {{annotate: wave-0 "Start cell"}} The start cell is at distance 0. Enqueue it.

{{focus: wave-1}} {{annotate: wave-1 "One step"}} All cells one step away that aren't walls. Distance 1.

{{focus: wave-n}} {{annotate: wave-n "Wavefront"}} The wavefront expands outward. The first time you reach the end cell, that's the shortest path.

{{focus: none}} Grid BFS and graph BFS are identical. The adjacency list is replaced by the four-direction loop. The visited array uses the same indexing as the grid.

{{clear: slide}}

{{show: maze-code typewriter 2s linear}} Grid BFS for shortest path in a maze.

{{focus: dirs}} {{zoom: 1.2x}} {{annotate: dirs "Four neighbors"}} The directions array encodes up, down, left, right. Loop over it to reach all four neighbors of each cell.

{{zoom: 1x}} {{focus: bounds-wall}} {{zoom: 1.3x}} {{annotate: bounds-wall "Gate check"}} Skip cells that are out of bounds, walls (value 1), or already visited. Three conditions, one compound check.

{{zoom: 1x}} {{focus: dist-update}} {{zoom: 1.2x}} {{annotate: dist-update "Propagate distance"}} Set the neighbor's distance to the current cell's distance plus one. Mark as visited by updating the distance array.

{{zoom: 1x}} {{focus: reach-end}} {{zoom: 1.2x}} {{annotate: reach-end "First arrival wins"}} When you dequeue the end cell, its distance is the shortest path. Return it.

{{zoom: 1x}} {{focus: none}} The grid acts as both the map and the visited array. No separate boolean vector needed when you track distances.

```data:maze-bfs type=matrix
[[0,0,0,0,1],
 [1,0,1,0,0],
 [0,0,0,1,0],
 [0,1,0,0,0],
 [0,0,0,0,0]]
---
wave-0: [0][0]
wave-1: [0][1], [1][1]
wave-n: [0][2], [2][0], [2][1], [2][2]
```

```code:maze-code lang=cpp
#include <vector>
#include <queue>
#include <array>

int shortest_path(std::vector<std::vector<int>>& grid,
                  std::pair<int,int> start, std::pair<int,int> end) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, -1));
    std::queue<std::pair<int,int>> q;

    auto [sr, sc] = start;
    dist[sr][sc] = 0;
    q.push(start);

    constexpr std::array<std::pair<int,int>, 4> dirs = {{{-1,0},{1,0},{0,-1},{0,1}}};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (std::pair{r,c} == end) return dist[r][c];

        for (auto [dr, dc] : dirs) {
            int nr = r + dr;
            int nc = c + dc;
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (grid[nr][nc] == 1 || dist[nr][nc] != -1) continue;

            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }
    return -1;  // unreachable
}
---
dirs: 17
bounds-wall: 26-27
dist-update: 29-30
reach-end: 23
```

# Multi-source BFS

{{clear: slide}}

{{show: multisrc-code typewriter 2s linear}} Multi-source BFS starts from multiple nodes simultaneously. Enqueue all sources at distance 0. The wavefront expands from all of them in parallel.

{{focus: multi-init}} {{zoom: 1.2x}} {{annotate: multi-init "All sources"}} Enqueue every source node and mark them all at distance 0. The queue starts with multiple entries.

{{zoom: 1x}} {{focus: single-wave}} {{zoom: 1.3x}} {{annotate: single-wave "One wavefront"}} From here, BFS runs identically to single-source. The wavefront just happens to start from multiple points. The algorithm doesn't need to know about multiple sources.

{{zoom: 1x}} {{focus: dist-meaning}} {{zoom: 1.2x}} {{annotate: dist-meaning "Nearest source"}} The resulting distances are the distance from each cell to its nearest source. Multi-source BFS computes all nearest-source distances in one pass.

{{zoom: 1x}} {{focus: none}} Classic problems: distance from any 0-cell in a grid, rotting oranges, nearest exit. All multi-source BFS — seed the queue with all starting nodes, run BFS once.

```code:multisrc-code lang=cpp
#include <vector>
#include <queue>
#include <array>

// Grid problem: distance from each cell to nearest 0
std::vector<std::vector<int>> distance_to_zero(std::vector<std::vector<int>>& grid) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, -1));
    std::queue<std::pair<int,int>> q;

    // Seed all zero-cells as sources at distance 0
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 0) {
                dist[r][c] = 0;
                q.push({r, c});
            }
        }
    }

    constexpr std::array<std::pair<int,int>, 4> dirs = {{{-1,0},{1,0},{0,-1},{0,1}}};
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (auto [dr, dc] : dirs) {
            int nr = r + dr;
            int nc = c + dc;
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (dist[nr][nc] != -1) continue;

            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }
    return dist;
}
---
multi-init: 14-21
single-wave: 23-35
dist-meaning: 35
```

# 0-1 BFS

{{clear: slide}}

{{show: deque-code typewriter 2s linear}} Graphs with edge weights of only 0 or 1. BFS with a deque instead of a queue. Zero-weight edges go to the front, one-weight edges go to the back.

{{focus: why-deque}} {{zoom: 1.2x}} {{annotate: why-deque "Order by cost"}} A regular queue loses the distance ordering when edges have varying weights. A deque with zero-weight edges at the front restores it. Zero-cost neighbors are processed immediately — they're at the same distance as the current node.

{{zoom: 1x}} {{focus: push-front}} {{zoom: 1.3x}} {{annotate: push-front "Zero cost"}} Zero-weight edge: push the neighbor to the front. It has the same distance as the current node and must be processed before one-weight neighbors.

{{zoom: 1x}} {{focus: push-back}} {{zoom: 1.2x}} {{annotate: push-back "Unit cost"}} One-weight edge: push the neighbor to the back. It's one step further and should be processed later.

{{zoom: 1x}} {{focus: dist-check}} {{zoom: 1.2x}} {{annotate: dist-check "Relaxation"}} Only enqueue if the new distance improves on the stored distance. This prevents redundant processing.

{{zoom: 1x}} {{focus: none}} 0-1 BFS is Dijkstra specialized for binary edge weights. Deque instead of priority queue. O of V plus E instead of O of (V plus E) log V.

```code:deque-code lang=cpp
#include <vector>
#include <deque>
#include <climits>

// Each edge has weight 0 or 1
std::vector<int> bfs_01(int start,
    const std::vector<std::vector<std::pair<int,int>>>& adj) {
    int n = static_cast<int>(adj.size());
    std::vector<int> dist(n, INT_MAX);
    std::deque<int> dq;

    dist[start] = 0;
    dq.push_back(start);

    while (!dq.empty()) {
        int node = dq.front();
        dq.pop_front();

        for (auto [neighbor, weight] : adj[node]) {
            int new_dist = dist[node] + weight;
            if (new_dist >= dist[neighbor]) continue;

            dist[neighbor] = new_dist;
            if (weight == 0) {
                dq.push_front(neighbor);   // zero cost: front
            } else {
                dq.push_back(neighbor);    // unit cost: back
            }
        }
    }
    return dist;
}
---
why-deque: 9-10
push-front: 23-24
push-back: 25-26
dist-check: 20-21
```
