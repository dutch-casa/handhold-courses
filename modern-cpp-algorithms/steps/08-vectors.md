---
title: Vectors and arrays
---

# std::vector

{{show: vec-code typewriter 2s linear}} vector is the workhorse container. A dynamic array that grows automatically.

{{focus: create}} {{zoom: 1.2x}} {{annotate: create "Initialize"}} Brace initialization fills the vector with values. The vector allocates exactly enough space.

{{zoom: 1x}} {{focus: push-back}} {{zoom: 1.2x}} {{annotate: push-back "Append"}} push back adds an element to the end. If there's capacity, it's O of 1. If the vector is full, it reallocates.

{{zoom: 1x}} {{focus: access}} {{zoom: 1.2x}} {{annotate: access "Random access"}} Bracket access is O of 1. Constant time. Just pointer arithmetic — the elements are contiguous in memory.

{{zoom: 1x}} {{focus: range-for}} {{zoom: 1.2x}} {{annotate: range-for "Iterate"}} Range-for iterates every element. Use const reference to avoid copies.

{{zoom: 1x}} {{focus: size-cap}} {{annotate: size-cap "Size vs capacity"}} Size is how many elements you have. Capacity is how many fit before reallocation. They're different.

{{zoom: 1x}} {{focus: none}} vector is your default container. Use it unless you have a specific reason not to.

```code:vec-code lang=cpp
#include <vector>

std::vector<int> nums = {10, 20, 30, 40, 50};

nums.push_back(60);                     // append
nums.emplace_back(70);                  // construct in place

int first = nums[0];                    // 10 (no bounds check)
int safe = nums.at(0);                  // 10 (throws if out of range)

for (const auto& n : nums) {
    // process each element
}

auto sz = nums.size();                  // 7
auto cap = nums.capacity();             // implementation-defined, >= 7
---
create: 3
push-back: 5-6
access: 8-9
range-for: 11-13
size-cap: 15-16
```

# Growth strategy

{{clear: slide}}

{{show: growth-viz grow 0.5s spring}} When a vector runs out of capacity, it allocates a new array with double the capacity, copies everything over, and frees the old one.

{{focus: phase-1}} {{annotate: phase-1 "Capacity: 4"}} Four elements. Full. The next push back triggers reallocation.

{{focus: phase-2}} {{annotate: phase-2 "Capacity: 8"}} New array with capacity 8. Four elements copied. Four slots empty. The old array is freed.

{{focus: phase-3}} {{annotate: phase-3 "Amortized O(1)"}} Doubling means you copy N elements after N insertions. Amortized cost per insertion: O of 1.

{{focus: none}} This is why vector is fast despite occasional reallocations. The doubling strategy amortizes the cost.

```data:growth-viz type=array
["[10|20|30|40]", "[10|20|30|40|50|_|_|_]", "[10|20|30|40|50|60|70|80]"]
---
phase-1: 0
phase-2: 1
phase-3: 2
```

# std::array and std::span

{{clear: slide}}

{{show: array-code typewriter 1.5s linear}} When the size is known at compile time, use array. When you need a view into contiguous memory, use span.

{{focus: std-array}} {{zoom: 1.2x}} {{annotate: std-array "Fixed size"}} std array is a fixed-size array on the stack. No heap allocation. The size is part of the type — array of 4 ints is a different type from array of 5 ints.

{{zoom: 1x}} {{focus: span-view}} {{zoom: 1.3x}} {{annotate: span-view "Non-owning view"}} span is to arrays what string view is to strings. A non-owning view. It doesn't care whether the underlying data is a vector, an array, or a C array. It just sees contiguous elements.

{{zoom: 1x}} {{focus: span-param}} {{zoom: 1.2x}} {{annotate: span-param "Universal parameter"}} Use span as a function parameter when you accept any contiguous container. One function handles vector, array, and C arrays.

{{zoom: 1x}} {{focus: none}} array for fixed-size stack data. span for non-owning views. vector for everything else.

```code:array-code lang=cpp
#include <array>
#include <span>
#include <vector>

std::array<int, 4> fixed = {1, 2, 3, 4};        // stack-allocated, fixed size

// span: non-owning view into contiguous memory
void process(std::span<const int> data) {
    for (int n : data) { /* ... */ }
}

std::vector<int> vec = {10, 20, 30};
process(vec);                                     // works
process(fixed);                                   // works
int raw[] = {5, 6, 7};
process(raw);                                     // works
---
std-array: 5
span-view: 8-10
span-param: 12-16
```

# Iterators

{{clear: slide}}

{{show: iter-code typewriter 1.5s linear}} Iterators are the bridge between containers and algorithms. Every container has them.

{{focus: begin-end}} {{zoom: 1.2x}} {{annotate: begin-end "Half-open range"}} begin points to the first element. end points one past the last. The range is half-open: begin is included, end is excluded.

{{zoom: 1x}} {{focus: deref}} {{zoom: 1.2x}} {{annotate: deref "Like a pointer"}} Dereference an iterator with star. Advance with plus plus. Compare with not-equals. They work like pointers.

{{zoom: 1x}} {{focus: algo-use}} {{zoom: 1.2x}} {{annotate: algo-use "Algorithm input"}} Algorithms take iterator pairs. sort takes begin and end. find takes begin, end, and a value. Every standard algorithm works this way.

{{zoom: 1x}} {{focus: none}} Iterators decouple algorithms from containers. Any algorithm works with any container.

```code:iter-code lang=cpp
#include <vector>
#include <algorithm>

std::vector<int> v = {30, 10, 50, 20, 40};

auto it = v.begin();        // points to 30
auto end = v.end();         // points past 40

*it;                        // 30 (dereference)
++it;                       // advance to next element
it != end;                  // true until we reach the end

std::sort(v.begin(), v.end());       // {10, 20, 30, 40, 50}
auto found = std::find(v.begin(), v.end(), 30);  // iterator to 30
---
begin-end: 6-7
deref: 9-11
algo-use: 13-14
```

# Common operations

{{clear: slide}}

{{show: ops-code typewriter 2s linear}} The operations you'll use constantly.

{{focus: insert-erase}} {{zoom: 1.2x}} {{annotate: insert-erase "Modify"}} Insert and erase take iterators. Insert before the position. Erase at the position. Both shift elements — O of n for mid-array operations.

{{zoom: 1x}} {{focus: reserve}} {{zoom: 1.2x}} {{annotate: reserve "Preallocate"}} Reserve preallocates capacity without adding elements. Use it when you know the final size. It prevents reallocations during push back.

{{zoom: 1x}} {{focus: clear-resize}} {{zoom: 1.2x}} {{annotate: clear-resize "Size management"}} Clear removes all elements but keeps the capacity. Resize changes the size — growing adds defaults, shrinking removes from the end.

{{zoom: 1x}} {{focus: empty-check}} {{annotate: empty-check "Always check empty"}} Use empty, not size equals zero. They do the same thing, but empty communicates intent.

{{zoom: 1x}} {{focus: none}} These operations plus push back and bracket access cover ninety percent of vector usage.

```code:ops-code lang=cpp
std::vector<int> v = {10, 20, 30, 40, 50};

// Insert and erase
v.insert(v.begin() + 2, 25);     // {10, 20, 25, 30, 40, 50}
v.erase(v.begin());               // {20, 25, 30, 40, 50}

// Reserve capacity
v.reserve(100);                   // capacity >= 100, size unchanged

// Clear and resize
v.clear();                        // size = 0, capacity unchanged
v.resize(5, 0);                   // size = 5, filled with zeros

// Check empty
if (!v.empty()) {
    auto front = v.front();       // first element
    auto back = v.back();         // last element
}
---
insert-erase: 4-5
reserve: 8
clear-resize: 11-12
empty-check: 15-18
```

# Cache behavior

{{clear: slide}}

{{show: cache-diagram grow 0.5s spring}} Vector elements are contiguous in memory. This makes iteration fast — not because of algorithmic complexity, but because of hardware.

{{focus: contiguous}} {{annotate: contiguous "Contiguous"}} A vector's elements sit next to each other in memory. When the CPU fetches one element, the cache line brings in the neighbors for free.

{{focus: cache-line}} {{annotate: cache-line "64 bytes"}} A typical cache line is 64 bytes. For 4-byte ints, that's 16 elements per cache line. Iterating a vector touches cold memory once every 16 elements.

{{focus: none}} This is mechanical sympathy. The data structure matches how the hardware works. Linked lists, by contrast, scatter nodes across the heap. Every access is a cache miss. Even when linked lists have the same algorithmic complexity, vectors often win by 10x on real hardware.

```diagram:cache-diagram
ram [database]
cache [cache]
cpu [service]
ram --cache line--> cache
cache --fast read--> cpu
{Memory Hierarchy: ram, cache, cpu}
---
contiguous: ram
cache-line: cache
```

# The container decision

{{clear: slide}}

{{show: decision grow 0.5s spring}} When to use what.

{{focus: vector-row}} {{annotate: vector-row "Default"}} Vector: your default. Random access, contiguous memory, amortized O of 1 append.

{{focus: array-row}} {{annotate: array-row "Compile-time size"}} Array: size known at compile time. Stack allocated. Zero overhead.

{{focus: span-row}} {{annotate: span-row "View parameter"}} Span: function parameter that accepts any contiguous container. Non-owning.

{{focus: none}} Start with vector. Switch to array when the size is fixed. Use span for function parameters.

```data:decision type=array
["vector<T> — dynamic, growable (default)", "array<T,N> — fixed size, stack", "span<T> — non-owning view"]
---
vector-row: 0
array-row: 1
span-row: 2
```
