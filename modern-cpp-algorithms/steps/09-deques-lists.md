---
title: Deques, lists, and stacks
---

# std::deque

{{show: deque-code typewriter 2s linear}} deque — double-ended queue. Pronounced "deck." Fast insertion at both front and back.

{{focus: push-ops}} {{zoom: 1.2x}} {{annotate: push-ops "Both ends O(1)"}} push front and push back are both amortized O of 1. Unlike vector, push front doesn't shift elements. The deque allocates in blocks and manages an internal map of block pointers.

{{zoom: 1x}} {{focus: random-access}} {{zoom: 1.2x}} {{annotate: random-access "Indexed access"}} Bracket access works. It's O of 1 but slightly slower than vector — the deque must compute which block and which offset within that block.

{{zoom: 1x}} {{focus: no-contiguous}} {{annotate: no-contiguous "Not contiguous"}} Unlike vector, deque memory is not contiguous. You can't get a flat pointer to all elements. That kills span compatibility and hurts cache performance.

{{zoom: 1x}} {{focus: none}} Use deque when you need fast push and pop at both ends. Use vector for everything else.

```code:deque-code lang=cpp
#include <deque>

std::deque<int> dq = {10, 20, 30};

dq.push_front(5);              // {5, 10, 20, 30}
dq.push_back(40);              // {5, 10, 20, 30, 40}
dq.pop_front();                // {10, 20, 30, 40}

int val = dq[2];               // 30 (O(1), block + offset lookup)
auto sz = dq.size();           // 4

// No contiguous guarantee — can't do std::span<int>(dq)
---
push-ops: 5-7
random-access: 9-10
no-contiguous: 12
```

# Deque block layout

{{clear: slide}}

{{show: deque-blocks grow 0.5s spring}} A deque stores data in fixed-size blocks. An internal map of pointers connects them.

{{focus: block-a}} {{annotate: block-a "Block A"}} The first block holds the front elements. When you push front and this block is full, the deque allocates a new block before it.

{{focus: block-b}} {{annotate: block-b "Block B"}} Middle blocks are full. Elements fill from front to back within each block.

{{focus: block-c}} {{annotate: block-c "Block C"}} The last block holds the back elements. push back fills from the left. When full, a new block is appended.

{{focus: none}} This block architecture means iterators are more complex than vector iterators. Each iterator stores four values: a pointer to the current element, the block start, the block end, and a pointer into the map.

```data:deque-blocks type=array
["Block A: [_|_|5|10]", "Block B: [20|30|40|50]", "Block C: [60|70|_|_]"]
---
block-a: 0
block-b: 1
block-c: 2
```

# std::list and std::forward_list

{{clear: slide}}

{{show: list-code typewriter 2s linear}} list is a doubly-linked list. forward list is singly-linked. Both provide O of 1 insert and erase at any iterator position.

{{focus: splice-op}} {{zoom: 1.2x}} {{annotate: splice-op "O(1) splice"}} Splice moves elements from one list to another in constant time. No copying. Just pointer reassignment. This is the killer feature of list.

{{zoom: 1x}} {{focus: insert-erase}} {{zoom: 1.2x}} {{annotate: insert-erase "Stable iterators"}} Insert and erase at an iterator are O of 1. More importantly, other iterators remain valid. Vector invalidates iterators on insert. List does not.

{{zoom: 1x}} {{focus: forward}} {{zoom: 1.2x}} {{annotate: forward "Minimal overhead"}} forward list is singly-linked. One pointer per node instead of two. It only supports forward traversal and insertion after a position.

{{zoom: 1x}} {{focus: no-random}} {{annotate: no-random "No bracket access"}} Neither list supports random access. No brackets. No constant-time jump to the Nth element. You must walk the chain.

{{zoom: 1x}} {{focus: none}} Lists trade cache performance for iterator stability and O of 1 splicing. That trade is rarely worth it.

```code:list-code lang=cpp
#include <list>
#include <forward_list>

std::list<int> lst = {10, 20, 30, 40};

// Splice: move elements between lists in O(1)
std::list<int> other = {99, 100};
lst.splice(lst.begin(), other);    // {99, 100, 10, 20, 30, 40}

// Insert and erase at iterator: O(1)
auto it = std::next(lst.begin(), 2);
lst.insert(it, 15);                // {99, 100, 15, 10, 20, 30, 40}
lst.erase(it);                     // erase element at 'it'

// forward_list: singly-linked, insert_after only
std::forward_list<int> fl = {1, 2, 3};
fl.push_front(0);                  // {0, 1, 2, 3}

// No random access: lst[2] won't compile
---
splice-op: 7-8
insert-erase: 10-13
forward: 16-17
no-random: 19
```

# Stack and queue adaptors

{{clear: slide}}

{{show: stack-adaptor grow 0.5s spring}} {{show: adaptor-code typewriter 1.5s linear}} stack, queue, and priority queue are not containers. They're adaptors — thin wrappers around a real container.

{{focus: stack-viz}} {{annotate: stack-viz "LIFO"}} Stack is last-in first-out. push adds to the top. pop removes from the top. top peeks at the top element. That's the entire interface.

{{focus: stack-ops}} {{zoom: 1.2x}} {{annotate: stack-ops "Three operations"}} The adaptor restricts the interface. You can't iterate. You can't index. You get push, pop, and top. That's discipline enforced by the type system.

{{zoom: 1x}} {{focus: queue-ops}} {{zoom: 1.2x}} {{annotate: queue-ops "FIFO"}} Queue is first-in first-out. push adds to the back. pop removes from the front. front peeks at the front.

{{zoom: 1x}} {{focus: underlying}} {{annotate: underlying "deque by default"}} By default, both stack and queue wrap a deque. You can swap in a vector or list as the underlying container.

{{zoom: 1x}} {{focus: none}} Adaptors are about restricting access, not adding functionality. They make invalid operations unrepresentable.

```data:stack-adaptor type=stack
["push 10", "push 20", "push 30", "top → 30", "pop → 30"]
^top=2
---
stack-viz: 0, 1, 2, 3, 4
```

```code:adaptor-code lang=cpp
#include <stack>
#include <queue>

std::stack<int> stk;
stk.push(10);
stk.push(20);
stk.push(30);
int t = stk.top();        // 30
stk.pop();                 // removes 30

std::queue<int> q;
q.push(10);
q.push(20);
q.push(30);
int f = q.front();         // 10
q.pop();                   // removes 10

// Custom underlying container
std::stack<int, std::vector<int>> vec_stack;
---
stack-ops: 4-9
queue-ops: 11-16
underlying: 19
```

# When to use what

{{clear: slide}}

{{show: decision-guide grow 0.5s spring}} The container decision is almost always "use vector." Here's when it's not.

{{focus: vec-default}} {{annotate: vec-default "Default"}} Vector: random access, contiguous memory, fast iteration. Your default for ninety-five percent of cases.

{{focus: deque-case}} {{annotate: deque-case "Both-end push"}} Deque: you need fast push front. A task scheduler where work arrives at both ends. Rare in practice.

{{focus: list-case}} {{annotate: list-case "Stable iterators"}} List: you hold iterators and need them to survive insertions and erasures. An LRU cache with O of 1 promotion. Genuinely rare.

{{focus: stack-case}} {{annotate: stack-case "Restricted access"}} Stack or queue: you want to enforce LIFO or FIFO discipline at the type level. BFS uses queue. DFS uses stack. Parsing uses stack.

{{focus: none}} When in doubt, start with vector. Profile before switching.

```data:decision-guide type=array
["vector<T> — default, contiguous, random access", "deque<T> — fast push_front + push_back", "list<T> — stable iterators, O(1) splice", "stack<T> / queue<T> — restricted LIFO / FIFO"]
---
vec-default: 0
deque-case: 1
list-case: 2
stack-case: 3
```

# Performance reality

{{clear: slide}}

{{show: cache-code typewriter 1.5s linear}} Algorithmic complexity lies. Cache behavior tells the truth.

{{focus: vec-sum}} {{zoom: 1.2x}} {{annotate: vec-sum "Contiguous"}} Summing a vector walks contiguous memory. The CPU prefetcher predicts the access pattern. Cache lines bring neighbors for free. This is fast.

{{zoom: 1x}} {{focus: list-sum}} {{zoom: 1.2x}} {{annotate: list-sum "Scattered"}} Summing a list chases pointers across the heap. Every node is a separate allocation. Every access is a potential cache miss. Same O of n complexity. Ten times slower in practice.

{{zoom: 1x}} {{focus: deque-sum}} {{zoom: 1.2x}} {{annotate: deque-sum "Block-local"}} Deque sits in between. Access within a block is contiguous. Crossing block boundaries costs a pointer chase. Better than list, worse than vector.

{{zoom: 1x}} {{focus: none}} {{zoom: 1x}} Bjarne Stroustrup tested this. For almost every workload — even with frequent mid-container insertions — vector beats list. The cache penalty of pointer chasing overwhelms the algorithmic advantage of O of 1 insertion. Measure before you reach for a linked list.

```code:cache-code lang=cpp
#include <vector>
#include <list>
#include <deque>
#include <numeric>

// Vector: contiguous, prefetcher-friendly
std::vector<int> vec(1'000'000, 1);
auto v_sum = std::accumulate(vec.begin(), vec.end(), 0);

// List: pointer-chasing, cache-hostile
std::list<int> lst(1'000'000, 1);
auto l_sum = std::accumulate(lst.begin(), lst.end(), 0);

// Deque: block-contiguous, middle ground
std::deque<int> dq(1'000'000, 1);
auto d_sum = std::accumulate(dq.begin(), dq.end(), 0);

// Same O(n) complexity. Vastly different wall-clock time.
// Typical ratio: vector 1x, deque 1.5x, list 5-10x
---
vec-sum: 7-8
list-sum: 11-12
deque-sum: 15-16
```
