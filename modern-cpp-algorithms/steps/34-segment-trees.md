---
title: Segment Trees and Fenwick Trees
---

# Range query problem

{{show: range-viz grow 0.5s spring}} You have an array and need to answer many range queries — sum, min, or max over arbitrary subarrays [l, r]. You also need to handle point updates. Brute force is O(n) per query; that's too slow when queries number in the millions.

{{focus: naive}} {{annotate: naive "O(n) per query"}} Scanning [l, r] for each query is O(n). With Q queries, that's O(nQ). For n = Q = 10^5, that's 10^10 operations. Unusable.

{{focus: prefix-sums}} {{annotate: prefix-sums "O(1) query, O(n) update"}} Prefix sums answer sum queries in O(1): prefix[r] - prefix[l-1]. But updating a single element forces you to rebuild all prefix sums — O(n) per update.

{{focus: segment-tree}} {{annotate: segment-tree "O(log n) both"}} Segment trees and Fenwick trees both achieve O(log n) for both query and update. The tradeoff: O(n) build time, O(n) space, then O(log n) per operation.

{{focus: none}} The choice between segment tree and Fenwick tree depends on the operation. Fenwick is simpler and faster in practice for prefix sums. Segment tree handles arbitrary range operations.

```data:range-viz type=array
["[3|1|4|1|5|9|2|6] sum(2,5) = ?", "Naive: scan 4 elements → O(n)", "Prefix: O(1) sum, O(n) update", "Segment tree: O(log n) both"]
---
naive: 1
prefix-sums: 2
segment-tree: 3
```

# Segment tree build

{{clear: slide}}

{{show: build-code typewriter 2s linear}} Build the segment tree bottom-up. Each internal node stores the aggregate (sum, min, max) of its range. The tree is stored in a flat array — node at index i has children at 2i and 2i+1.

{{focus: array-repr}} {{zoom: 1.2x}} {{annotate: array-repr "Implicit tree"}} The flat array representation uses 1-based indexing. Root is index 1. Left child of node i is 2i, right child is 2i+1. Parent of i is i/2. No pointers needed — the indices encode the tree structure.

{{zoom: 1x}} {{focus: build-recurse}} {{zoom: 1.3x}} {{annotate: build-recurse "Post-order build"}} Build recursively: leaf nodes store the original values. Internal nodes store the combination of their two children. Build bottom-up, so children are always computed before parents.

{{zoom: 1x}} {{focus: size}} {{zoom: 1.2x}} {{annotate: size "4n is safe"}} Allocate 4n elements for the tree array. In the worst case (n not a power of 2), the tree height is ceil(log2(n)) + 1 and leaf count can reach 2^(ceil(log2(n))). 4n always covers it.

{{zoom: 1x}} {{focus: none}} The flat array + implicit indexing is a beautiful trick. Same structure as a binary heap. Zero pointer overhead, excellent cache locality, trivial to implement.

```code:build-code lang=cpp
#include <vector>
#include <functional>

class SegTree {
    int n_;
    std::vector<int> tree_;  // 1-indexed, size 4*n

    void build(const std::vector<int>& arr, int node, int start, int end) {
        if (start == end) {
            tree_[node] = arr[start];
            return;
        }
        const int mid = (start + end) / 2;
        build(arr, 2 * node,     start, mid);
        build(arr, 2 * node + 1, mid + 1, end);
        tree_[node] = tree_[2 * node] + tree_[2 * node + 1];
    }

public:
    explicit SegTree(const std::vector<int>& arr)
        : n_(static_cast<int>(arr.size())), tree_(4 * arr.size(), 0) {
        build(arr, 1, 0, n_ - 1);
    }
};
---
array-repr: 6
size: 18
build-recurse: 7-15
```

# Segment tree query

{{clear: slide}}

{{show: query-code typewriter 2s linear}} Query returns the aggregate over [l, r]. The recursive descent checks three cases: the node's range is fully outside, fully inside, or partially overlapping the query range.

{{focus: outside}} {{zoom: 1.2x}} {{annotate: outside "Disjoint: return identity"}} If the node's range [start, end] doesn't intersect [l, r], return the identity value for the operation. For sum: 0. For min: INT_MAX. For max: INT_MIN. This range contributes nothing.

{{zoom: 1x}} {{focus: inside}} {{zoom: 1.2x}} {{annotate: inside "Fully covered: return node"}} If [start, end] is completely inside [l, r], return tree_[node] directly. This is the key optimization — don't recurse further when the whole range is useful.

{{zoom: 1x}} {{focus: partial}} {{zoom: 1.3x}} {{annotate: partial "Partial: recurse both"}} Partial overlap: recurse on both children and combine results. At most O(log n) nodes are in the "partial" category — the rest hit the outside or inside cases.

{{zoom: 1x}} {{focus: none}} The depth of the recursion is O(log n). At each level, at most 4 nodes are accessed. Total work per query: O(log n) — a clean theoretical bound that holds in practice.

```code:query-code lang=cpp
int query(int node, int start, int end, int l, int r) const {
    if (r < start || end < l) {
        return 0;  // identity for sum; use INT_MAX for min
    }
    if (l <= start && end <= r) {
        return tree_[node];  // fully covered
    }
    const int mid = (start + end) / 2;
    const int left  = query(2 * node,     start, mid,     l, r);
    const int right = query(2 * node + 1, mid + 1, end,   l, r);
    return left + right;
}

// Public wrapper
int query(int l, int r) const {
    return query(1, 0, n_ - 1, l, r);
}
---
outside: 2-3
inside: 5-6
partial: 8-10
```

# Segment tree update

{{clear: slide}}

{{show: update-code typewriter 1.5s linear}} Point update: change a single element and propagate the change upward through the tree. The path from leaf to root is O(log n) nodes.

{{focus: leaf-update}} {{zoom: 1.2x}} {{annotate: leaf-update "Update leaf"}} Descend to the leaf that holds index pos. Set its value. This is straightforward — follow the same left/right branching as in build.

{{zoom: 1x}} {{focus: propagate}} {{zoom: 1.2x}} {{annotate: propagate "Recompute parents"}} On the way back up, each internal node recomputes its value from its two children. The path from leaf to root is the only part of the tree that changes. All other nodes are unaffected.

{{zoom: 1x}} {{focus: none}} Update and query are symmetric traversals. Both descend, both touch O(log n) nodes. The segment tree's elegance is in this symmetry.

```code:update-code lang=cpp
void update(int node, int start, int end, int pos, int val) {
    if (start == end) {
        tree_[node] = val;  // update leaf
        return;
    }
    const int mid = (start + end) / 2;
    if (pos <= mid) {
        update(2 * node,     start, mid,     pos, val);
    } else {
        update(2 * node + 1, mid + 1, end,   pos, val);
    }
    // Propagate: recompute this node from children
    tree_[node] = tree_[2 * node] + tree_[2 * node + 1];
}

void update(int pos, int val) {
    update(1, 0, n_ - 1, pos, val);
}
---
leaf-update: 3
propagate: 11
```

# Fenwick tree

{{clear: slide}}

{{show: fenwick-code typewriter 2s linear}} A Fenwick tree — Binary Indexed Tree — supports prefix sum queries and point updates in O(log n) with half the code of a segment tree. Each index stores a partial sum over a range defined by the lowest set bit.

{{focus: idea}} {{zoom: 1.2x}} {{annotate: idea "Implicit range per index"}} fenwick[i] stores the sum of elements from index (i - lowbit(i) + 1) to i, where lowbit(i) = i & (-i). This implicit range assignment is what makes both operations O(log n).

{{zoom: 1x}} {{focus: update}} {{zoom: 1.2x}} {{annotate: update "Walk up: add lowbit"}} To update index i, add the delta to fenwick[i], then move to i + lowbit(i), and repeat. This bubbles the change up to all ranges that include i.

{{zoom: 1x}} {{focus: query}} {{zoom: 1.3x}} {{annotate: query "Walk down: subtract lowbit"}} To query prefix sum [1, i], add fenwick[i], then move to i - lowbit(i), and repeat. This collects exactly the non-overlapping ranges that together cover [1, i].

{{zoom: 1x}} {{focus: none}} The Fenwick tree's simplicity is deceptive. It took Fenwick years to discover this encoding. Once you see it, you can implement it in ten lines from memory.

```code:fenwick-code lang=cpp
class Fenwick {
    int n_;
    std::vector<int> tree_;  // 1-indexed

    static int lowbit(int i) { return i & (-i); }

public:
    explicit Fenwick(int n) : n_(n), tree_(n + 1, 0) {}

    // Add delta to position i (1-indexed)
    void update(int i, int delta) {
        for (; i <= n_; i += lowbit(i))
            tree_[i] += delta;
    }

    // Prefix sum [1, i]
    int query(int i) const {
        int sum = 0;
        for (; i > 0; i -= lowbit(i))
            sum += tree_[i];
        return sum;
    }

    // Range sum [l, r]
    int query(int l, int r) const {
        return query(r) - query(l - 1);
    }
};
---
idea: 3-4
update: 9-12
query: 15-19
```

# The i & (-i) trick

{{clear: slide}}

{{show: lowbit-viz grow 0.5s spring}} The i & (-i) operation isolates the lowest set bit of i. That single bit defines how far each Fenwick node's range extends backward and how far update must propagate forward.

{{focus: how}} {{annotate: how "Two's complement"}} In two's complement, -i flips all bits and adds 1. The result: all bits above the lowest set bit are flipped, all bits below are cleared, and the lowest set bit is preserved. AND with i keeps only that one bit.

{{focus: examples}} {{annotate: examples "Concrete values"}} i=6 (110₂): -6 = 010₂, so 6 & (-6) = 010₂ = 2. Range covers 2 elements. i=8 (1000₂): -8 = 1000₂, so 8 & (-8) = 8. Range covers 8 elements. The lowest set bit equals the range length.

{{focus: update-path}} {{annotate: update-path "Update: i += lowbit(i)"}} Adding lowbit(i) to i flips the lowest set bit up — like incrementing the binary counter one "level". Updating index 3 (011) → 4 (100) → 8 (1000) → 16 → ... walks up the tree covering progressively larger ranges.

{{focus: query-path}} {{annotate: query-path "Query: i -= lowbit(i)"}} Subtracting lowbit(i) clears the lowest set bit. Querying prefix [1,7]: 7 (111) → 6 (110) → 4 (100) → 0. Three lookups collect non-overlapping ranges that exactly cover [1,7].

{{focus: none}} This bit trick is the entire Fenwick tree. Once you internalize it, the update and query loops write themselves. The tree is implicit in the bit structure of the indices.

```data:lowbit-viz type=array
["i=1 (001): lowbit=1, covers [1,1]", "i=2 (010): lowbit=2, covers [1,2]", "i=3 (011): lowbit=1, covers [3,3]", "i=4 (100): lowbit=4, covers [1,4]", "i=6 (110): lowbit=2, covers [5,6]", "i=8 (1000): lowbit=8, covers [1,8]"]
---
how: 0
examples: 1
update-path: 3
query-path: 5
```

# When to use which

{{clear: slide}}

{{show: comparison grow 0.5s spring}} Segment tree or Fenwick tree? The answer depends on what you need the tree to do.

{{focus: fenwick-wins}} {{annotate: fenwick-wins "Fenwick: prefix operations"}} Use Fenwick when the operation supports "undo" — when you can compute range [l, r] as prefix(r) - prefix(l-1). That covers sum, XOR, and any group operation with inverses. Fenwick is 2x faster, half the code, and easier to get right under pressure.

{{focus: seg-wins}} {{annotate: seg-wins "Segment: arbitrary ranges"}} Use segment tree when the operation has no inverse — min, max, GCD, LCM. You can't compute range_min(l, r) from two prefix_min values. Segment tree handles these directly. Also use segment tree for lazy propagation (range updates, not just point updates).

{{focus: lazy}} {{annotate: lazy "Lazy propagation"}} For range updates — "add 5 to all elements in [l, r]" — you need lazy propagation in the segment tree. Each node carries a pending update that's pushed down when the node is split. This keeps range update + range query both O(log n).

{{focus: none}} Fenwick for prefix-invertible operations. Segment tree for everything else. When in doubt, segment tree is the general solution — more code, but handles all cases.

```data:comparison type=array
["Fenwick: prefix sum/XOR — O(n) space, ~10 lines", "Segment: min/max/GCD — O(4n) space, ~30 lines", "Lazy segment: range update + range query", "Merge sort tree: frequency queries in O(log^2 n)"]
---
fenwick-wins: 0
seg-wins: 1
lazy: 2
```
