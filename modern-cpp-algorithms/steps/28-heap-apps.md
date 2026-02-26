---
title: Heap Applications
---

# Top-K pattern

{{show: topk-code typewriter 2s linear}} Finding the top-k largest elements from n items. The naive approach sorts everything: O(n log n). The heap approach maintains a min-heap of size k: O(n log k). When k << n, the difference is significant.

{{focus: heap-invariant}} {{zoom: 1.2x}} {{annotate: heap-invariant "Size-k min-heap"}} The invariant: the heap always holds exactly the k largest elements seen so far. The minimum of those k elements is at the heap top.

{{zoom: 1x}} {{focus: compare-step}} {{zoom: 1.3x}} {{annotate: compare-step "Top vs new"}} For each new element: compare it with the heap's top (the smallest of the current top-k). If the new element is larger, pop the top and push the new element.

{{zoom: 1x}} {{focus: evict-step}} {{zoom: 1.2x}} {{annotate: evict-step "Evict minimum"}} Evicting the current minimum to make room for a larger element keeps the heap holding exactly the k largest. The heap top is always the boundary — elements smaller than it can never be in the top-k.

{{zoom: 1x}} {{focus: result}} {{zoom: 1.2x}} {{annotate: result "Heap is answer"}} After processing all n elements, the heap contains the top-k. Pop them all for sorted order (ascending — it's a min-heap). Or just return the heap as-is.

{{zoom: 1x}} {{focus: none}} Top-k with a size-k min-heap: O(n log k). Streaming-friendly — works on data arriving one at a time. Memory is O(k) instead of O(n).

```code:topk-code lang=cpp
#include <vector>
#include <queue>

// Returns k largest elements (unordered)
std::vector<int> top_k(const std::vector<int>& nums, int k) {
    // Min-heap of size k: top is the smallest of the k largest
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;

    for (int x : nums) {
        pq.push(x);
        if (static_cast<int>(pq.size()) > k) {
            pq.pop();  // evict the smallest — it can't be top-k
        }
    }

    std::vector<int> result;
    while (!pq.empty()) {
        result.push_back(pq.top());
        pq.pop();
    }
    return result;
}

// O(n log k) time, O(k) space
// vs std::sort: O(n log n) time, O(1) extra space
// vs nth_element: O(n) time but result is unordered
---
heap-invariant: 7
compare-step: 9-12
evict-step: 11
result: 15-19
```

# Median maintenance

{{clear: slide}}

{{show: dual-heap grow 0.5s spring}} The running median problem: given a stream of numbers, report the median after each insertion. Dual-heap approach: one max-heap for the lower half, one min-heap for the upper half. The median lives at the boundary between them.

{{focus: lower-heap}} {{annotate: lower-heap "Max-heap: lower"}} The lower heap is a max-heap. Its top is the largest element in the lower half — the left neighbor of the median.

{{focus: upper-heap}} {{annotate: upper-heap "Min-heap: upper"}} The upper heap is a min-heap. Its top is the smallest element in the upper half — the right neighbor of the median.

{{focus: balance-rule}} {{annotate: balance-rule "Sizes differ by ≤1"}} Balance invariant: the two heaps differ in size by at most 1. If they're equal size, the median is the average of both tops. If one is larger, its top is the median.

{{focus: none}} Two heaps, one median. Each insertion is O(log n). Rebalancing is O(log n). Reading the median is O(1). Far better than re-sorting on every insertion.

```data:dual-heap type=tree
{id: "lower-root", val: 5, left: "l1", right: "l2"}
{id: "l1", val: 3}
{id: "l2", val: 4}
{id: "upper-root", val: 7, left: "u1", right: "u2"}
{id: "u1", val: 9}
{id: "u2", val: 11}
---
lower-heap: lower-root, l1, l2
upper-heap: upper-root, u1, u2
balance-rule: lower-root, upper-root
```

{{clear: slide}}

{{show: median-code typewriter 2s linear}} Median maintenance in code. The rebalancing logic is the key.

{{focus: two-heaps}} {{zoom: 1.2x}} {{annotate: two-heaps "Two pq members"}} Two `priority_queue` members: `lower` (max-heap) and `upper` (min-heap). Both hold `int`.

{{zoom: 1x}} {{focus: add-logic}} {{zoom: 1.3x}} {{annotate: add-logic "Route new element"}} Route each new element: if it's <= the lower max, push to lower. Otherwise push to upper. Then rebalance.

{{zoom: 1x}} {{focus: rebalance}} {{zoom: 1.2x}} {{annotate: rebalance "Move top across"}} Rebalancing: if the size difference exceeds 1, move the top of the larger heap to the smaller heap. One move restores balance.

{{zoom: 1x}} {{focus: median-read}} {{zoom: 1.2x}} {{annotate: median-read "O(1) median"}} Reading the median: equal sizes → average of both tops. Unequal → top of the larger heap.

{{zoom: 1x}} {{focus: none}} This pattern generalizes: two heaps maintain any order statistic at a boundary. Not just the median — any quantile.

```code:median-code lang=cpp
#include <queue>
#include <vector>

class MedianFinder {
    std::priority_queue<int> lower;                        // max-heap
    std::priority_queue<int,std::vector<int>,std::greater<int>> upper; // min-heap

public:
    void add(int num) {
        if (lower.empty() || num <= lower.top()) {
            lower.push(num);
        } else {
            upper.push(num);
        }

        // Rebalance: sizes must differ by at most 1
        if (static_cast<int>(lower.size()) > static_cast<int>(upper.size()) + 1) {
            upper.push(lower.top()); lower.pop();
        } else if (static_cast<int>(upper.size()) > static_cast<int>(lower.size()) + 1) {
            lower.push(upper.top()); upper.pop();
        }
    }

    double median() const {
        if (lower.size() == upper.size())
            return (lower.top() + upper.top()) / 2.0;
        return lower.size() > upper.size() ? lower.top() : upper.top();
    }
};
---
two-heaps: 4-5
add-logic: 8-12
rebalance: 15-19
median-read: 22-24
```

# Merge K sorted lists

{{clear: slide}}

{{show: merge-k-code typewriter 2s linear}} Merging K sorted arrays into one sorted array. The naive approach concatenates and sorts: O(n log n). The heap approach: maintain a min-heap of size K holding one element from each list. Always extract the global minimum, then push the next element from that list.

{{focus: heap-entry}} {{zoom: 1.2x}} {{annotate: heap-entry "{val, list, idx}"}} Each heap entry holds three fields: the value, which list it came from, and its index in that list. This lets us efficiently pull the next element from the right list.

{{zoom: 1x}} {{focus: seed-heap}} {{zoom: 1.3x}} {{annotate: seed-heap "Seed with heads"}} Initialize the heap with the first element from each list. K initial pushes, all O(log K).

{{zoom: 1x}} {{focus: extract-min}} {{zoom: 1.2x}} {{annotate: extract-min "Always global min"}} Each `top()` gives the global minimum across all remaining elements. Extract it, add to result, then push the next element from the same list (if any).

{{zoom: 1x}} {{focus: complexity}} {{zoom: 1.2x}} {{annotate: complexity "O(n log K)"}} Total complexity: O(n log K) where n is total elements across all lists. The heap never exceeds size K. Each of the n elements is pushed and popped exactly once.

{{zoom: 1x}} {{focus: none}} This is the online merge: memory is O(K), not O(n). Works for streaming inputs where you can't load all lists at once.

```code:merge-k-code lang=cpp
#include <vector>
#include <queue>
#include <tuple>

std::vector<int> merge_k_sorted(const std::vector<std::vector<int>>& lists) {
    using Entry = std::tuple<int, int, int>;  // {val, list_idx, elem_idx}
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;

    // Seed with first element from each non-empty list
    for (int i = 0; i < static_cast<int>(lists.size()); ++i) {
        if (!lists[i].empty()) {
            pq.push({lists[i][0], i, 0});
        }
    }

    std::vector<int> result;
    while (!pq.empty()) {
        auto [val, li, ei] = pq.top(); pq.pop();
        result.push_back(val);

        int next_ei = ei + 1;
        if (next_ei < static_cast<int>(lists[li].size())) {
            pq.push({lists[li][next_ei], li, next_ei});
        }
    }
    return result;
}
---
heap-entry: 6
seed-heap: 9-13
extract-min: 16-21
complexity: 6
```

# Task scheduling

{{clear: slide}}

{{show: schedule-code typewriter 2s linear}} Task scheduling with a priority queue: process tasks in priority order, handling cooldown constraints. Classic CPU scheduling and system design problem.

{{focus: task-struct}} {{zoom: 1.2x}} {{annotate: task-struct "Priority + time"}} Each task has a priority and an available-at time. It can only run when the current time >= available time. Cooldown means a task can't run again for some duration after executing.

{{zoom: 1x}} {{focus: ready-queue}} {{zoom: 1.3x}} {{annotate: ready-queue "Max-heap on priority"}} The max-heap holds ready tasks — those whose available time has passed. Always run the highest-priority ready task.

{{zoom: 1x}} {{focus: cooldown-logic}} {{zoom: 1.2x}} {{annotate: cooldown-logic "Requeue with delay"}} After running a task, if it repeats, push it back with `available_at = current_time + cooldown`. It won't be dequeued again until the cooldown expires.

{{zoom: 1x}} {{focus: idle-step}} {{zoom: 1.2x}} {{annotate: idle-step "Advance time"}} If no task is ready, advance time to the earliest available task. Don't spin — jump directly.

{{zoom: 1x}} {{focus: none}} This pattern handles the LeetCode "Task Scheduler" problem and real OS scheduling. The heap gives O(log n) selection, time-jumping avoids idle loops.

```code:schedule-code lang=cpp
#include <queue>
#include <vector>
#include <tuple>

struct Task {
    int priority;
    int available_at;
    int id;
};

auto task_cmp = [](const Task& a, const Task& b) {
    return a.priority < b.priority;  // max-heap on priority
};

std::vector<int> schedule(std::vector<Task> tasks, int cooldown) {
    using PQ = std::priority_queue<Task, std::vector<Task>, decltype(task_cmp)>;
    PQ ready(task_cmp);
    std::sort(tasks.begin(), tasks.end(),
        [](const Task& a, const Task& b){ return a.available_at < b.available_at; });

    int time = 0, i = 0;
    std::vector<int> order;

    while (i < static_cast<int>(tasks.size()) || !ready.empty()) {
        while (i < static_cast<int>(tasks.size()) && tasks[i].available_at <= time) {
            ready.push(tasks[i++]);
        }
        if (ready.empty()) { time = tasks[i].available_at; continue; }

        Task t = ready.top(); ready.pop();
        order.push_back(t.id);
        ++time;
        t.available_at = time + cooldown;
        ready.push(t);  // requeue with cooldown
    }
    return order;
}
---
task-struct: 3-7
ready-queue: 10-12
cooldown-logic: 32-33
idle-step: 30
```

# Huffman coding

{{clear: slide}}

{{show: huffman-tree grow 0.5s spring}} Huffman coding builds an optimal prefix-free encoding from character frequencies. High-frequency characters get short codes, rare characters get long codes. The structure emerges from a greedy min-heap construction.

{{focus: freq-init}} {{annotate: freq-init "Leaf nodes"}} Start with one leaf node per character, weighted by frequency. Push all of them into a min-heap.

{{focus: merge-step}} {{annotate: merge-step "Combine two mins"}} Repeatedly extract the two nodes with the smallest frequencies, create an internal node with their combined frequency, and push it back. The internal node's left/right children are the two extracted nodes.

{{focus: final-tree}} {{annotate: final-tree "One tree"}} After n-1 merges, one node remains — the root. The tree encodes every character. Left edges are 0, right edges are 1. Path from root to a leaf is that character's code.

{{focus: optimality}} {{annotate: optimality "Provably optimal"}} Huffman is provably optimal for symbol-by-symbol prefix-free encoding. The greedy choice — always merge the two smallest — produces the minimum expected code length.

{{focus: none}} Applications: zlib, PNG, JPEG, ZIP, HTTP/2 HPACK. Huffman is one of the most deployed algorithms in existence.

```data:huffman-tree type=tree
{id: "root", val: 15, left: "n1", right: "n2"}
{id: "n1", val: 6, left: "a", right: "b"}
{id: "a", val: 2, label: "a:2"}
{id: "b", val: 4, label: "b:4"}
{id: "n2", val: 9, left: "c", right: "d"}
{id: "c", val: 4, label: "c:4"}
{id: "d", val: 5, label: "d:5"}
---
freq-init: a, b, c, d
merge-step: n1, n2
final-tree: root
optimality: root, n1, n2
```

{{clear: slide}}

{{show: huffman-code typewriter 2s linear}} Huffman construction in code. The min-heap drives the greedy merges.

{{focus: node-struct}} {{zoom: 1.2x}} {{annotate: node-struct "Tree node"}} Each node holds a frequency, an optional character (leaves only), and left/right children. `unique_ptr` owns the children.

{{zoom: 1x}} {{focus: heap-init}} {{zoom: 1.3x}} {{annotate: heap-init "Seed leaves"}} Create one leaf per character, push all to the min-heap ordered by frequency.

{{zoom: 1x}} {{focus: merge-loop}} {{zoom: 1.2x}} {{annotate: merge-loop "Merge until one"}} Merge loop: extract two minimums, create their parent with combined frequency, push back. n-1 iterations for n characters.

{{zoom: 1x}} {{focus: code-extract}} {{zoom: 1.2x}} {{annotate: code-extract "DFS for codes"}} Extract codes with a DFS: append '0' going left, '1' going right. Leaf nodes record their code string.

{{zoom: 1x}} {{focus: none}} Huffman: O(n log n) build time. The codes are optimal. Real implementations also need a serialized tree header so the decoder can reconstruct the tree.

```code:huffman-code lang=cpp
#include <queue>
#include <memory>
#include <string>
#include <unordered_map>

struct HuffNode {
    int freq;
    char ch = '\0';
    std::unique_ptr<HuffNode> left, right;
    bool is_leaf() const { return !left && !right; }
};

auto cmp = [](const HuffNode* a, const HuffNode* b){
    return a->freq > b->freq;
};

std::unordered_map<char, std::string>
huffman_codes(const std::unordered_map<char, int>& freqs) {
    std::priority_queue<HuffNode*, std::vector<HuffNode*>, decltype(cmp)> pq(cmp);

    for (auto [ch, f] : freqs) {
        auto* node = new HuffNode{f, ch};
        pq.push(node);
    }

    while (pq.size() > 1) {
        auto* left  = pq.top(); pq.pop();
        auto* right = pq.top(); pq.pop();
        auto* parent = new HuffNode{left->freq + right->freq};
        parent->left.reset(left);
        parent->right.reset(right);
        pq.push(parent);
    }

    std::unordered_map<char, std::string> codes;
    std::function<void(HuffNode*, std::string)> dfs =
        [&](HuffNode* node, std::string code) {
            if (!node) return;
            if (node->is_leaf()) { codes[node->ch] = code; return; }
            dfs(node->left.get(),  code + '0');
            dfs(node->right.get(), code + '1');
        };
    dfs(pq.top(), "");
    return codes;
}
---
node-struct: 6-11
heap-init: 17-21
merge-loop: 23-30
code-extract: 32-39
```

# Lazy deletion

{{clear: slide}}

{{show: lazy-del-code typewriter 2s linear}} Sometimes you need to "delete" elements from a heap that don't support efficient arbitrary removal. Lazy deletion: don't actually remove them. Mark them as deleted. When you pop, skip any element marked deleted. Works beautifully with the stale-entry pattern from Dijkstra.

{{focus: tombstone}} {{zoom: 1.2x}} {{annotate: tombstone "Mark, don't remove"}} Use a set or hash set of "deleted" IDs. When a deletion is requested, add the ID to the set. The heap still holds the element — we just know to ignore it later.

{{zoom: 1x}} {{focus: skip-on-pop}} {{zoom: 1.3x}} {{annotate: skip-on-pop "Skip at pop time"}} Before processing a popped element, check if it's in the deleted set. If it is, discard it and pop again. Keep skipping until you find a valid element.

{{zoom: 1x}} {{focus: amortized}} {{zoom: 1.2x}} {{annotate: amortized "Still O(log n)"}} Amortized cost: each element is pushed once and popped once. Even with lazy deletion, the total heap operations are O(n log n) for n elements. No element is skipped more than once.

{{zoom: 1x}} {{focus: cleanup}} {{zoom: 1.2x}} {{annotate: cleanup "Remove from set"}} When a deleted element is popped and discarded, remove it from the deleted set. Memory doesn't accumulate.

{{zoom: 1x}} {{focus: none}} Lazy deletion turns a heap into a mutable priority queue without implementing decrease-key. Used in Dijkstra (stale entries are implicitly deleted), event simulation, and any heap where updates are needed.

```code:lazy-del-code lang=cpp
#include <queue>
#include <unordered_set>
#include <optional>

class LazyHeap {
    std::priority_queue<int> pq;                // max-heap
    std::unordered_set<int> deleted;

public:
    void push(int val) {
        pq.push(val);
    }

    void remove(int val) {
        deleted.insert(val);                    // mark deleted
    }

    std::optional<int> top() {
        while (!pq.empty() && deleted.count(pq.top())) {
            deleted.erase(pq.top());            // cleanup
            pq.pop();                           // skip stale
        }
        if (pq.empty()) return std::nullopt;
        return pq.top();
    }

    void pop() {
        top();             // skip to valid top first
        if (!pq.empty()) pq.pop();
    }

    bool empty() { return !top().has_value(); }
};
---
tombstone: 7, 13-14
skip-on-pop: 17-21
amortized: 9, 13
cleanup: 18-19
```
