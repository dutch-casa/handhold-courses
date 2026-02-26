---
title: Advanced BFS
---

# Bidirectional BFS

{{show: bidir-viz grow 0.5s spring}} Bidirectional BFS launches two simultaneous wavefronts — one from the source, one from the target. They meet in the middle. This halves the effective search depth and reduces the number of nodes explored dramatically.

{{focus: forward-wave}} {{annotate: forward-wave "From source"}} The forward frontier expands outward from the source. Standard BFS.

{{focus: backward-wave}} {{annotate: backward-wave "From target"}} The backward frontier expands outward from the target — following edges in reverse, or treating the graph as undirected.

{{focus: meeting-point}} {{annotate: meeting-point "They meet"}} When a node appears in both frontiers, the path is complete. The shortest path length is the sum of the two frontier distances.

{{focus: none}} Standard BFS explores O of b to the d nodes, where b is the branching factor and d is the distance. Bidirectional BFS explores O of b to the d over 2 from each end. The difference is enormous on dense graphs.

{{clear: slide}}

{{show: bidir-code typewriter 2s linear}} Alternate between frontiers. Expand the smaller one first.

{{focus: two-fronts}} {{zoom: 1.2x}} {{annotate: two-fronts "Two visited sets"}} Two unordered sets track each frontier's visited nodes. When a node appears in both, the frontiers have met.

{{zoom: 1x}} {{focus: expand-smaller}} {{zoom: 1.3x}} {{annotate: expand-smaller "Balance the growth"}} Always expand the smaller frontier. This balances the two wavefronts and minimizes total work. If one frontier is much smaller, it's cheaper to grow it.

{{zoom: 1x}} {{focus: check-overlap}} {{zoom: 1.2x}} {{annotate: check-overlap "Intersection test"}} After each expansion step, check if any newly added node is in the other frontier. Intersection means the path is found.

{{zoom: 1x}} {{focus: none}} Bidirectional BFS shines when the target is known in advance. One-directional BFS is simpler for unknown targets or multi-source problems.

```data:bidir-viz type=graph
0 -- 1
1 -- 2
2 -- 3
3 -- 4
4 -- 5
---
forward-wave: 0, 1, 2
backward-wave: 5, 4, 3
meeting-point: 2, 3
```

```code:bidir-code lang=cpp
#include <vector>
#include <unordered_set>
#include <queue>

int bidir_bfs(int src, int dst,
              const std::vector<std::vector<int>>& adj) {
    if (src == dst) return 0;

    std::unordered_set<int> front_visited = {src};
    std::unordered_set<int> back_visited  = {dst};

    std::queue<int> front_q;
    std::queue<int> back_q;
    front_q.push(src);
    back_q.push(dst);

    int steps = 0;

    auto expand = [&](std::queue<int>& q,
                      std::unordered_set<int>& visited,
                      const std::unordered_set<int>& other) -> bool {
        int level_size = static_cast<int>(q.size());
        for (int i = 0; i < level_size; ++i) {
            int node = q.front(); q.pop();
            for (int neighbor : adj[node]) {
                if (other.count(neighbor)) return true;
                if (!visited.count(neighbor)) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                }
            }
        }
        return false;
    };

    while (!front_q.empty() && !back_q.empty()) {
        ++steps;
        if (front_q.size() <= back_q.size()) {
            if (expand(front_q, front_visited, back_visited)) return steps;
        } else {
            if (expand(back_q, back_visited, front_visited)) return steps;
        }
    }
    return -1;
}
---
two-fronts: 9-10
expand-smaller: 33-37
check-overlap: 24-25
```

# BFS with state encoding

{{clear: slide}}

{{show: state-code typewriter 2s linear}} Sometimes the "node" in BFS isn't a single integer — it's a complex state. Encode it as a hashable key. The BFS mechanics stay identical; only the state representation changes.

{{focus: state-type}} {{zoom: 1.2x}} {{annotate: state-type "What is a node?"}} Define what constitutes a unique state. For the word ladder problem: the current word is the state. For a puzzle: the board configuration.

{{zoom: 1x}} {{focus: encode}} {{zoom: 1.3x}} {{annotate: encode "Hashable key"}} Encode the state as something you can put in an unordered set. Strings work naturally. For boards, serialize to a string. For positions plus flags, pack into an int with bit manipulation.

{{zoom: 1x}} {{focus: neighbors}} {{zoom: 1.2x}} {{annotate: neighbors "State transitions"}} Generate neighboring states — all valid states reachable in one step. For word ladder: all words one letter different that exist in the dictionary.

{{zoom: 1x}} {{focus: visited-state}} {{zoom: 1.2x}} {{annotate: visited-state "Seen states"}} The visited set holds encoded states, not node IDs. unordered_set handles lookup in amortized O of 1.

{{zoom: 1x}} {{focus: none}} State-space BFS is BFS on an implicit graph. The graph is never built explicitly — you generate neighbors on the fly. The pattern scales to enormous state spaces.

```code:state-code lang=cpp
#include <string>
#include <vector>
#include <unordered_set>
#include <queue>

// Word ladder: minimum steps to transform begin_word into end_word
int word_ladder(const std::string& begin_word, const std::string& end_word,
                const std::unordered_set<std::string>& word_list) {
    if (!word_list.count(end_word)) return 0;

    std::unordered_set<std::string> visited = {begin_word};
    std::queue<std::string> q;
    q.push(begin_word);
    int steps = 1;

    while (!q.empty()) {
        int level_size = static_cast<int>(q.size());
        for (int i = 0; i < level_size; ++i) {
            std::string word = q.front(); q.pop();

            // Generate all neighbors: one letter at a time
            for (int j = 0; j < static_cast<int>(word.size()); ++j) {
                char original = word[j];
                for (char c = 'a'; c <= 'z'; ++c) {
                    word[j] = c;
                    if (word == end_word) return steps + 1;
                    if (word_list.count(word) && !visited.count(word)) {
                        visited.insert(word);
                        q.push(word);
                    }
                }
                word[j] = original;
            }
        }
        ++steps;
    }
    return 0;
}
---
state-type: 8-9
encode: 14-15
neighbors: 21-31
visited-state: 14
```

# BFS on abstract states

{{clear: slide}}

{{show: abstract-code typewriter 2s linear}} Abstract BFS: the state isn't a physical location. It's whatever configuration fully describes the problem's current situation. Puzzle solving, game states, state machines — all reducible to BFS on a state graph.

{{focus: state-def}} {{zoom: 1.2x}} {{annotate: state-def "State = config"}} Define the state completely. For the 8-puzzle: the board arrangement. For coin change: amount remaining. For lock combination: current digits.

{{zoom: 1x}} {{focus: transitions}} {{zoom: 1.3x}} {{annotate: transitions "Valid moves"}} Generate all valid transitions from the current state. These are the edges in the implicit graph. Each move produces a new state.

{{zoom: 1x}} {{focus: target-state}} {{zoom: 1.2x}} {{annotate: target-state "Goal test"}} The termination condition is reaching the target state — not a physical node. Check for goal state when dequeuing.

{{zoom: 1x}} {{focus: none}} Abstract BFS solves any problem phrased as "minimum operations to reach goal state" — sliding puzzles, coin change (counting coins), minimum genetic mutation. The state encoding is the hard part; BFS itself is mechanical.

```code:abstract-code lang=cpp
#include <string>
#include <unordered_set>
#include <queue>

// Open a lock: deadend combinations are forbidden.
// Find minimum turns to reach target from "0000".
int open_lock(const std::vector<std::string>& deadends,
              const std::string& target) {
    std::unordered_set<std::string> dead(deadends.begin(), deadends.end());
    if (dead.count("0000")) return -1;

    std::unordered_set<std::string> visited = {"0000"};
    std::queue<std::string> q;
    q.push("0000");
    int turns = 0;

    while (!q.empty()) {
        int level_size = static_cast<int>(q.size());
        for (int i = 0; i < level_size; ++i) {
            std::string state = q.front(); q.pop();
            if (state == target) return turns;

            for (int d = 0; d < 4; ++d) {
                for (int delta : {1, -1}) {
                    std::string next = state;
                    next[d] = static_cast<char>(
                        '0' + (next[d] - '0' + delta + 10) % 10);
                    if (!dead.count(next) && !visited.count(next)) {
                        visited.insert(next);
                        q.push(next);
                    }
                }
            }
        }
        ++turns;
    }
    return -1;
}
---
state-def: 8-9
transitions: 22-30
target-state: 21
```

# Kahn's topological sort

{{clear: slide}}

{{show: kahn-code typewriter 2s linear}} Kahn's algorithm: BFS-based topological sort using in-degree counts. Nodes with in-degree zero have no prerequisites — they can go first.

{{focus: indegree}} {{zoom: 1.2x}} {{annotate: indegree "Count prereqs"}} Compute the in-degree of every node — the number of edges pointing into it. Nodes with in-degree zero are ready to process. Seed the queue with them.

{{zoom: 1x}} {{focus: process-zero}} {{zoom: 1.3x}} {{annotate: process-zero "Process, then remove"}} Dequeue a zero-in-degree node, add it to the topological order. For each of its outgoing edges, decrement the neighbor's in-degree. If a neighbor's in-degree hits zero, it's now ready — enqueue it.

{{zoom: 1x}} {{focus: cycle-detect}} {{zoom: 1.2x}} {{annotate: cycle-detect "Cycle check"}} If the result contains fewer nodes than the graph, there's a cycle — some nodes never reached in-degree zero. Kahn's gives cycle detection for free.

{{zoom: 1x}} {{focus: none}} Kahn's is intuitive: process nodes only when all their prerequisites are done. It's the natural BFS formulation of topological sort. The DFS-based approach (reverse postorder) gives the same result but is less obvious.

```data:kahn-code type=graph
0 --> 1
0 --> 2
1 --> 3
2 --> 3
3 --> 4
---
indegree: 0, 1, 2, 3, 4
process-zero: 0
cycle-detect: 4
```

```code:kahn-code lang=cpp
#include <vector>
#include <queue>

std::vector<int> kahn_topo(int n, const std::vector<std::vector<int>>& adj) {
    std::vector<int> indegree(n, 0);
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            ++indegree[v];
        }
    }

    std::queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (indegree[i] == 0) q.push(i);
    }

    std::vector<int> order;
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        order.push_back(node);

        for (int neighbor : adj[node]) {
            --indegree[neighbor];
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    // If order.size() != n, there's a cycle
    return order;
}
---
indegree: 5-10
process-zero: 18-27
cycle-detect: 30
```

# BFS vs DFS decision guide

{{clear: slide}}

{{show: decision-guide grow 0.5s spring}} The decision is almost never arbitrary. Problem structure tells you which to use.

{{focus: bfs-signals}} {{annotate: bfs-signals "BFS signals"}} Reach for BFS when the problem asks for shortest path, minimum steps, minimum operations, nearest target, or level-by-level output. BFS guarantees the first solution found is the shortest.

{{focus: dfs-signals}} {{annotate: dfs-signals "DFS signals"}} Reach for DFS when the problem asks to detect cycles, find all paths, generate all solutions, topological sort, or determine if a solution exists (not the shortest one). DFS is also the right tool for trees when you don't need level information.

{{focus: both-ok}} {{annotate: both-ok "Either works"}} Connected components and reachability can use either. Pick DFS for its simpler implementation, BFS if you want the added distance information.

{{focus: none}} The question to ask: does order of discovery matter? If you need the closest or fewest-hops, use BFS. If you need to exhaust all possibilities or a path exists anywhere, use DFS.

{{clear: slide}}

{{show: summary-table grow 0.5s spring}} The quick-reference table.

{{focus: bfs-row}} {{annotate: bfs-row "Queue + distance"}} BFS: queue, mark at enqueue, level-by-level. Guarantees shortest path in unweighted graphs. O of V plus E.

{{focus: dfs-row}} {{annotate: dfs-row "Stack + visited"}} DFS: stack or recursion, visited set, go deep first. Simpler code. Also O of V plus E, but explores differently.

{{focus: none}} Both traverse the same graph in the same time. The difference is the order of node discovery. Choose by what that order means for your problem.

```data:decision-guide type=array
["Shortest path / min steps → BFS", "All paths / generate solutions → DFS", "Cycle detection → DFS (3-color)", "Level-order / by distance → BFS", "Topological sort → DFS (postorder) or BFS (Kahn's)", "Connected components → either (DFS simpler)"]
---
bfs-signals: 0, 3
dfs-signals: 1, 2, 4
both-ok: 5
```

```data:summary-table type=array
["BFS: queue, mark-at-enqueue, O(V+E), shortest path guaranteed", "DFS: stack/recursion, visited set, O(V+E), all paths, cycle detection"]
---
bfs-row: 0
dfs-row: 1
```
