---
title: Heap Internals
---

# Complete binary tree

{{show: heap-array grow 0.5s spring}} A heap is a complete binary tree stored in an array. "Complete" means every level is filled except possibly the last, which fills left to right. The array layout is the heap — no pointers, no allocation.

{{focus: level-0}} {{annotate: level-0 "Root at 0"}} The root sits at index 0. Level 0: one node. Level 1: two nodes. Level 2: four nodes. The tree is full left to right at every level.

{{focus: index-math}} {{annotate: index-math "Parent/child math"}} The index arithmetic: for a node at index i, its left child is at 2i+1, right child at 2i+2, parent at (i-1)/2. No pointers needed — the array position encodes the structure.

{{focus: complete-shape}} {{annotate: complete-shape "Left to right"}} Completeness guarantees height is exactly floor(log₂ n). That keeps all heap operations O(log n). No degenerate trees.

{{focus: none}} The array layout beats pointer-based trees in cache behavior. Every level's nodes are contiguous. Parent and children are at predictable offsets. Prefetcher loves it.

```data:heap-array type=array
[50, 30, 40, 20, 25, 35, 15, 10, 8]
---
level-0: 0
index-math: 0, 1, 2
complete-shape: 0, 1, 2, 3, 4, 5, 6, 7, 8
```

{{clear: slide}}

{{show: heap-tree grow 0.5s spring}} The same data visualized as a tree. The array index maps directly to tree position.

{{focus: tree-root}} {{annotate: tree-root "Index 0"}} Index 0 is the root: 50. Its left child (index 1) is 30. Right child (index 2) is 40.

{{focus: tree-level-2}} {{annotate: tree-level-2 "Indices 3-6"}} Level 2: indices 3 through 6. Children of 30: indices 3 (20) and 4 (25). Children of 40: indices 5 (35) and 6 (15).

{{focus: tree-leaves}} {{annotate: tree-leaves "Indices 7-8"}} Leaves: indices 7 and 8. Children of 20. The last level fills from the left.

{{focus: none}} The tree and array are the same data structure. Use whichever view helps reasoning. The array is what actually lives in memory.

```data:heap-tree type=tree
{id: 0, val: 50, left: 1, right: 2}
{id: 1, val: 30, left: 3, right: 4}
{id: 2, val: 40, left: 5, right: 6}
{id: 3, val: 20, left: 7, right: 8}
{id: 4, val: 25}
{id: 5, val: 35}
{id: 6, val: 15}
{id: 7, val: 10}
{id: 8, val: 8}
---
tree-root: 0
tree-level-2: 3, 4, 5, 6
tree-leaves: 7, 8
```

# Heap property

{{clear: slide}}

{{show: max-heap-tree grow 0.5s spring}} The heap property: every parent is >= its children (max-heap) or <= its children (min-heap). The root is always the maximum (or minimum). This invariant holds at every node, not just the root.

{{focus: max-root}} {{annotate: max-root "Global max"}} The root holds the global maximum. That's the guarantee. Extracting the maximum is O(1) — just read the root.

{{focus: local-order}} {{annotate: local-order "Local only"}} The heap property is local: parent >= children. Siblings have no ordering relationship. The heap is not sorted. Left subtree values can be smaller or larger than right subtree values.

{{focus: partial-order}} {{annotate: partial-order "Partial order"}} Heap order is a partial order. You know the max, but finding the k-th largest requires more work. The heap is optimized for one thing: rapid access to the extreme value.

{{focus: none}} Max-heap for access to largest. Min-heap for access to smallest. The property maintains itself through push and pop operations.

```data:max-heap-tree type=tree
{id: 0, val: 50, left: 1, right: 2}
{id: 1, val: 30, left: 3, right: 4}
{id: 2, val: 40, left: 5, right: 6}
{id: 3, val: 20}
{id: 4, val: 25}
{id: 5, val: 35}
{id: 6, val: 15}
---
max-root: 0
local-order: 1, 2, 3, 4, 5, 6
partial-order: 1, 2
```

# Insertion (bubble up)

{{clear: slide}}

{{show: bubble-up-tree grow 0.5s spring}} {{show: bubble-up-array grow 0.5s spring}} Inserting into a heap: append the new element at the end of the array (next available position in the complete tree), then bubble it up by swapping with its parent until the heap property holds.

{{focus: insert-pos}} {{annotate: insert-pos "Append first"}} New element goes at index n — the next slot. This maintains the complete tree shape. Now restore the heap property.

{{focus: compare-parent}} {{annotate: compare-parent "Check parent"}} Compare the new element with its parent at (i-1)/2. If the new element is larger (max-heap), swap them. Repeat until the new element is in the right place.

{{focus: bubble-path}} {{annotate: bubble-path "Swap upward"}} The bubble-up path is at most O(log n) swaps — the tree height. Each swap moves the element one level up.

{{focus: final-pos}} {{annotate: final-pos "Property restored"}} Once the element is smaller than or equal to its parent (or it's the root), stop. The heap property is restored globally.

{{focus: none}} Insertion: O(log n). Append, then bubble up. At most log n comparisons and swaps.

```data:bubble-up-tree type=tree
{id: 0, val: 50, left: 1, right: 2}
{id: 1, val: 30, left: 3, right: 4}
{id: 2, val: 40, left: 5, right: 6}
{id: 3, val: 20}
{id: 4, val: 25}
{id: 5, val: 35}
{id: 6, val: 45}
---
insert-pos: 6
compare-parent: 6, 2
bubble-path: 6, 2
final-pos: 6
```

```data:bubble-up-array type=array
[50, 30, 40, 20, 25, 35, 45]
---
insert-pos: 6
compare-parent: 6
bubble-path: 2, 6
final-pos: 6
```

{{clear: slide}}

{{show: insert-code typewriter 2s linear}} Bubble-up in code. A simple while loop tracking the current index.

{{focus: push-back}} {{zoom: 1.2x}} {{annotate: push-back "Append element"}} Push the new value to the end of the vector. The complete tree shape is maintained.

{{zoom: 1x}} {{focus: bubble-loop}} {{zoom: 1.3x}} {{annotate: bubble-loop "While not root"}} Loop while the current index has a parent (index > 0). Compare with parent. Swap if the new element belongs higher.

{{zoom: 1x}} {{focus: parent-calc}} {{zoom: 1.2x}} {{annotate: parent-calc "(i-1)/2"}} Parent index: integer division of (i-1)/2. Left child uses 2i+1, right child 2i+2. The formula is always the same.

{{zoom: 1x}} {{focus: none}} Three lines of real logic: push, compare, swap. The rest is index arithmetic.

```code:insert-code lang=cpp
#include <vector>
#include <utility>

void heap_push(std::vector<int>& heap, int val) {
    heap.push_back(val);
    int i = static_cast<int>(heap.size()) - 1;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[parent] >= heap[i]) break;  // heap property satisfied
        std::swap(heap[parent], heap[i]);
        i = parent;
    }
}

// Manual max-heap example:
// heap_push(h, 45) on [50,30,40,20,25,35]:
// Append:  [50,30,40,20,25,35,45]
// i=6, parent=2: 45 > 40 → swap → [50,30,45,20,25,35,40]
// i=2, parent=0: 45 < 50 → stop
---
push-back: 4-5
bubble-loop: 7-11
parent-calc: 8
```

# Extraction (bubble down)

{{clear: slide}}

{{show: bubble-down-tree grow 0.5s spring}} {{show: bubble-down-array grow 0.5s spring}} Extracting the max: swap root with the last element, shrink the array by one, then bubble down the new root until the heap property holds.

{{focus: swap-root}} {{annotate: swap-root "Root ↔ last"}} Swap the root (max) with the last element. Then remove the last element — the max is now safely extracted. The new root is almost certainly out of place.

{{focus: sift-down}} {{annotate: sift-down "Find largest child"}} Bubble down: compare the current node with its two children. If a child is larger, swap with the larger child. Repeat.

{{focus: choose-child}} {{annotate: choose-child "Larger child"}} Always swap with the *larger* of the two children. Swapping with the smaller child would violate the heap property for the other child.

{{focus: sift-stop}} {{annotate: sift-stop "Both smaller"}} Stop when both children are smaller than or equal to the current node. Or when you reach a leaf. The heap property is restored.

{{focus: none}} Extraction: O(log n). The tree height bounds the number of swaps. Swap root to end, remove, sift down.

```data:bubble-down-tree type=tree
{id: 0, val: 8, left: 1, right: 2}
{id: 1, val: 30, left: 3, right: 4}
{id: 2, val: 40, left: 5}
{id: 3, val: 20}
{id: 4, val: 25}
{id: 5, val: 35}
---
swap-root: 0
sift-down: 0, 2
choose-child: 2, 5
sift-stop: 5
```

```data:bubble-down-array type=array
[8, 30, 40, 20, 25, 35]
---
swap-root: 0
sift-down: 0
choose-child: 2
sift-stop: 5
```

{{clear: slide}}

{{show: extract-code typewriter 2s linear}} Sift-down (bubble-down) in code. The inner loop always moves to the larger child.

{{focus: extract-root}} {{zoom: 1.2x}} {{annotate: extract-root "Save max"}} Save the root value, move the last element to the root, pop the last slot, then sift down.

{{zoom: 1x}} {{focus: sift-loop}} {{zoom: 1.3x}} {{annotate: sift-loop "Find larger"}} Find the larger child. If neither child is larger than the current node, stop. Otherwise swap with the larger child and continue from there.

{{zoom: 1x}} {{focus: bounds-guard}} {{zoom: 1.2x}} {{annotate: bounds-guard "Check exists"}} Check that children exist before comparing. Right child might not exist if the array size is even. Always check left before right.

{{zoom: 1x}} {{focus: none}} Sift-down is the harder half of heap maintenance. Always swap with the larger child, always check bounds.

```code:extract-code lang=cpp
#include <vector>
#include <utility>

int heap_pop(std::vector<int>& heap) {
    int max_val = heap[0];
    heap[0] = heap.back();
    heap.pop_back();

    int i = 0;
    int n = static_cast<int>(heap.size());
    while (true) {
        int left  = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < n && heap[left] > heap[largest])
            largest = left;
        if (right < n && heap[right] > heap[largest])
            largest = right;

        if (largest == i) break;
        std::swap(heap[i], heap[largest]);
        i = largest;
    }
    return max_val;
}
---
extract-root: 4-7
sift-loop: 13-21
bounds-guard: 14-17
```

# std::priority_queue

{{clear: slide}}

{{show: pq-usage typewriter 2s linear}} `std::priority_queue` wraps a heap. Max-heap by default. Flip to min-heap with `greater<>`. Three operations: `push`, `pop`, `top`. Nothing else.

{{focus: max-pq}} {{zoom: 1.2x}} {{annotate: max-pq "Default max-heap"}} Default: `priority_queue<int>`. Top gives the maximum. Push and pop maintain the heap property automatically.

{{zoom: 1x}} {{focus: min-pq}} {{zoom: 1.3x}} {{annotate: min-pq "greater<> flips"}} Min-heap: `priority_queue<int, vector<int>, greater<int>>`. Top gives the minimum. Same interface, opposite ordering.

{{zoom: 1x}} {{focus: pq-ops}} {{zoom: 1.2x}} {{annotate: pq-ops "push/pop/top"}} The API: `push(val)` inserts, `top()` reads the extreme (does not remove), `pop()` removes the extreme. `empty()` and `size()` as expected.

{{zoom: 1x}} {{focus: custom-cmp}} {{zoom: 1.2x}} {{annotate: custom-cmp "Custom order"}} Custom comparator for complex types. Lambda or struct with `operator()`. The comparator takes two elements and returns true if the first should be *lower* in priority.

{{zoom: 1x}} {{focus: none}} `priority_queue` hides the array, the sift operations, everything. Use it for all heap needs — roll your own only when you need `decrease-key`.

```code:pq-usage lang=cpp
#include <queue>
#include <vector>
#include <functional>

// Max-heap (default)
std::priority_queue<int> max_pq;
max_pq.push(3); max_pq.push(1); max_pq.push(4);
int top = max_pq.top();  // 4
max_pq.pop();            // removes 4, top is now 3

// Min-heap
std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
min_pq.push(3); min_pq.push(1); min_pq.push(4);
int min_top = min_pq.top();  // 1

// Custom comparator (min-heap on second field of pair)
auto cmp = [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
    return a.second > b.second;  // greater → min-heap on second
};
std::priority_queue<std::pair<int,int>,
    std::vector<std::pair<int,int>>, decltype(cmp)> custom_pq(cmp);
---
max-pq: 5-9
min-pq: 11-14
pq-ops: 8-9, 13
custom-cmp: 16-21
```

# make_heap, push_heap, pop_heap

{{clear: slide}}

{{show: heap-ops-code typewriter 2s linear}} The `<algorithm>` heap functions operate in-place on any random-access range. They let you turn an existing vector into a heap and manage it manually. More flexible than `priority_queue` when you need access to the underlying container.

{{focus: make-heap}} {{zoom: 1.2x}} {{annotate: make-heap "Heapify in O(n)"}} `std::make_heap` converts an unsorted range into a max-heap. O(n) — faster than pushing elements one by one (which would be O(n log n)). The trick: sift down from the middle, not push-up from scratch.

{{zoom: 1x}} {{focus: push-heap}} {{zoom: 1.3x}} {{annotate: push-heap "After push_back"}} `std::push_heap`: add a new element with `push_back`, then call `push_heap` to bubble it up. The range [begin, end-1) must already be a heap.

{{zoom: 1x}} {{focus: pop-heap}} {{zoom: 1.2x}} {{annotate: pop-heap "Before pop_back"}} `std::pop_heap`: moves the max to the back, leaving [begin, end-1) as a valid heap. Then call `pop_back` to remove it. Two-step but gives you access to the removed element.

{{zoom: 1x}} {{focus: sort-heap}} {{zoom: 1.2x}} {{annotate: sort-heap "Heap sort"}} `std::sort_heap`: if a range is a heap, this sorts it in ascending order. It's heap sort. O(n log n). Destroys the heap property.

{{zoom: 1x}} {{focus: none}} These functions are the underlying machinery `priority_queue` wraps. Use them when you need direct vector access alongside heap operations.

```code:heap-ops-code lang=cpp
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

    // Build heap in O(n)
    std::make_heap(v.begin(), v.end());
    // v is now a valid max-heap, v[0] == 9

    // Add element
    v.push_back(7);
    std::push_heap(v.begin(), v.end());  // bubble 7 up
    // v[0] still 9

    // Remove max
    std::pop_heap(v.begin(), v.end());   // max moved to back
    int max_val = v.back();
    v.pop_back();                        // actually remove it
    // max_val == 9, v[0] == 7

    // Sort (destroys heap property)
    std::sort_heap(v.begin(), v.end());  // ascending
}
---
make-heap: 7-8
push-heap: 11-12
pop-heap: 15-18
sort-heap: 21
```

# nth_element

{{clear: slide}}

{{show: nth-code typewriter 2s linear}} `std::nth_element` partially sorts a range so that the element at position n is the element that would be there in a fully sorted range. Everything before it is ≤ it, everything after is ≥ it. O(n) average.

{{focus: nth-guarantee}} {{zoom: 1.2x}} {{annotate: nth-guarantee "Position correct"}} After `nth_element`, the nth position holds the correct value. The two partitions (left and right of nth) are unordered internally. Only the nth element's position is guaranteed.

{{zoom: 1x}} {{focus: kth-largest}} {{zoom: 1.3x}} {{annotate: kth-largest "Kth largest"}} To find the kth largest: use iterator `begin() + (n - k)` as the nth position. After the call, that position holds the kth largest element.

{{zoom: 1x}} {{focus: nth-complexity}} {{zoom: 1.2x}} {{annotate: nth-complexity "O(n) average"}} Introselect (the typical implementation): O(n) average, O(n) worst case. Much faster than full sort when you only need one order statistic.

{{zoom: 1x}} {{focus: none}} Use `nth_element` for median finding, top-k without sorting, percentile computation. It's underused — most reach for `sort` when they only need one position.

```code:nth-code lang=cpp
#include <vector>
#include <algorithm>

int kth_largest(std::vector<int> v, int k) {
    // Put kth-largest at position (n-k)
    int n = static_cast<int>(v.size());
    auto nth = v.begin() + (n - k);
    std::nth_element(v.begin(), nth, v.end());
    return *nth;
}

int median(std::vector<int> v) {
    int n = static_cast<int>(v.size());
    auto mid = v.begin() + n / 2;
    std::nth_element(v.begin(), mid, v.end());
    return *mid;  // lower median for even n
}

// Example: [3,1,4,1,5,9,2,6], k=3 → 5
// nth_element puts 5 at index 5: [?, ?, ?, ?, ?, 5, ?, ?]
// left side ≤ 5, right side ≥ 5 (unordered)
---
nth-guarantee: 6-8
kth-largest: 4-9
nth-complexity: 6
```

# partial_sort

{{clear: slide}}

{{show: partial-sort-code typewriter 2s linear}} `std::partial_sort` sorts only the first k elements. The range [begin, middle) is sorted. The range [middle, end) contains the remaining elements in unspecified order. O(n log k).

{{focus: partial-sig}} {{zoom: 1.2x}} {{annotate: partial-sig "Three iterators"}} Three iterators: begin, middle, end. Elements [begin, middle) are the k smallest, fully sorted. Everything else is dumped into [middle, end).

{{zoom: 1x}} {{focus: why-heap}} {{zoom: 1.3x}} {{annotate: why-heap "Heap-based"}} Implementation: build a max-heap of size k from the first k elements. Scan the rest — if smaller than the heap max, replace it and sift down. After scanning, extract from the heap in sorted order. O(n log k).

{{zoom: 1x}} {{focus: nth-vs-partial}} {{zoom: 1.2x}} {{annotate: nth-vs-partial "Sorted vs unsorted"}} `nth_element` is faster (O(n)) but the k elements aren't sorted relative to each other. `partial_sort` is O(n log k) but the top-k are fully sorted. Choose based on need.

{{zoom: 1x}} {{focus: none}} `partial_sort` is the right tool when you need the top-k in rank order: leaderboards, ranked results, priority lists. Faster than full sort for small k relative to n.

```code:partial-sort-code lang=cpp
#include <vector>
#include <algorithm>

// Top k smallest, sorted
std::vector<int> top_k_sorted(std::vector<int> v, int k) {
    auto mid = v.begin() + k;
    std::partial_sort(v.begin(), mid, v.end());
    return {v.begin(), mid};
}

// Top k largest, sorted descending
std::vector<int> top_k_largest(std::vector<int> v, int k) {
    auto mid = v.begin() + k;
    std::partial_sort(v.begin(), mid, v.end(),
        std::greater<int>{});
    return {v.begin(), mid};
}

// Comparison:
// Full sort:       O(n log n) — all n elements sorted
// partial_sort:    O(n log k) — first k sorted, rest unordered
// nth_element:     O(n)       — kth found, neither partition sorted
---
partial-sig: 6
why-heap: 6
nth-vs-partial: 18-20
```
