---
title: Ranges and views
---

# Iterator-pair problems

{{show: iter-problem typewriter 2s linear}} The classic algorithm API takes two iterators. It works, but it has problems.

{{focus: verbose}} {{zoom: 1.2x}} {{annotate: verbose "Noisy syntax"}} Every call repeats begin and end. Sort takes begin-comma-end. Find takes begin-comma-end-comma-value. The container name appears twice per call. This is visual noise.

{{zoom: 1x}} {{focus: mismatch}} {{zoom: 1.2x}} {{annotate: mismatch "Mismatched pairs"}} Nothing prevents passing begin from one container and end from another. The compiler won't catch it. You'll get undefined behavior at runtime.

{{zoom: 1x}} {{focus: compose}} {{zoom: 1.2x}} {{annotate: compose "Can't compose"}} Chaining operations requires intermediate containers. Filter even numbers, then square them, then take five — that's three separate passes with three temporary vectors. Wasteful.

{{zoom: 1x}} {{focus: none}} Iterator pairs are a 1990s abstraction. Ranges, introduced in C++ 20, solve all three problems.

```code:iter-problem lang=cpp
#include <algorithm>
#include <vector>

std::vector<int> v = {5, 3, 8, 1, 9, 2, 7, 4, 6};

// Verbose: container name repeated
std::sort(v.begin(), v.end());
auto it = std::find(v.begin(), v.end(), 8);

// Dangerous: mismatched iterators compile fine
std::vector<int> other = {10, 20};
// std::sort(v.begin(), other.end());  // UB! compiles silently

// Can't compose: need intermediate storage
std::vector<int> evens;
std::copy_if(v.begin(), v.end(), std::back_inserter(evens),
    [](int x) { return x % 2 == 0; });
std::vector<int> squared;
std::transform(evens.begin(), evens.end(),
    std::back_inserter(squared),
    [](int x) { return x * x; });
// Two allocations, two passes, lots of noise
---
verbose: 7-8
mismatch: 12
compose: 15-22
```

# The ranges abstraction

{{clear: slide}}

{{show: ranges-code typewriter 2s linear}} A range is anything with begin and end. Ranges algorithms take a single range argument instead of two iterators.

{{focus: range-sort}} {{zoom: 1.2x}} {{annotate: range-sort "One argument"}} ranges sort takes the container directly. No begin-end ceremony. One argument. The range carries both boundaries.

{{zoom: 1x}} {{focus: range-find}} {{zoom: 1.2x}} {{annotate: range-find "Cleaner find"}} ranges find takes the container and the value. Two arguments instead of three. Reads like English.

{{zoom: 1x}} {{focus: projections}} {{zoom: 1.3x}} {{annotate: projections "Projections"}} Projections extract a comparison key without wrapping the entire comparator. Sort by a member field with a single projection argument. No lambda needed for simple member access.

{{zoom: 1x}} {{focus: safe}} {{annotate: safe "No mismatch"}} Mismatched iterators are impossible. You pass one range. The begin and end are always from the same source.

{{zoom: 1x}} {{focus: none}} Ranges algorithms are drop-in replacements for classic algorithms. Same performance. Better syntax. Stronger safety.

```code:ranges-code lang=cpp
#include <algorithm>
#include <ranges>
#include <vector>
#include <string>

std::vector<int> v = {5, 3, 8, 1, 9, 2, 7, 4, 6};

// ranges::sort: pass the container directly
std::ranges::sort(v);

// ranges::find: container + value
auto it = std::ranges::find(v, 8);

// Projections: sort structs by member
struct Student { std::string name; int grade; };
std::vector<Student> students = {{"Alice", 92}, {"Bob", 78}, {"Eve", 95}};
std::ranges::sort(students, {}, &Student::grade);
// sorted by grade ascending — no lambda needed

// Mismatched iterators: impossible with ranges
// std::ranges::sort takes ONE argument, not two
---
range-sort: 9
range-find: 12
projections: 16-18
safe: 21
```

# Views — lazy and composable

{{clear: slide}}

{{show: views-code typewriter 2s linear}} Views are lightweight, non-owning, lazy range adaptors. They don't copy data. They don't allocate. They transform elements on demand.

{{focus: filter-view}} {{zoom: 1.2x}} {{annotate: filter-view "Lazy filter"}} filter view skips elements that don't match the predicate. No intermediate vector. Elements are evaluated one at a time as you iterate.

{{zoom: 1x}} {{focus: transform-view}} {{zoom: 1.2x}} {{annotate: transform-view "Lazy transform"}} transform view applies a function to each element on access. The function runs when you dereference the iterator, not when you create the view.

{{zoom: 1x}} {{focus: no-alloc}} {{zoom: 1.2x}} {{annotate: no-alloc "Zero allocation"}} Views don't own data. They hold a reference to the source range plus the adaptor logic. Creating a view is O of 1 regardless of range size.

{{zoom: 1x}} {{focus: materialize}} {{annotate: materialize "Pull into vector"}} To materialize a view into a real container, pipe it into ranges to vector (C++ 23) or use the constructor. Until then, elements exist only as computation.

{{zoom: 1x}} {{focus: none}} Views are the functional programming backbone of C++ 20. Lazy evaluation means you only compute what you consume.

```code:views-code lang=cpp
#include <ranges>
#include <vector>

std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// filter: lazy, no allocation
auto evens = data
    | std::views::filter([](int x) { return x % 2 == 0; });
// nothing computed yet

// transform: lazy, no allocation
auto squared = data
    | std::views::transform([](int x) { return x * x; });
// nothing computed yet

// No allocation — views are O(1) to create
for (int x : evens) {
    // 2, 4, 6, 8, 10 — computed on demand
}

// Materialize into a vector (C++23)
auto vec = std::ranges::to<std::vector>(squared);
// vec = {1, 4, 9, 16, 25, 36, 49, 64, 81, 100}
---
filter-view: 7-9
transform-view: 12-14
no-alloc: 17-19
materialize: 22-23
```

# Pipe syntax

{{clear: slide}}

{{show: lazy-pipeline grow 0.5s spring}} {{show: pipe-code typewriter 2s linear}} The pipe operator chains views into a data pipeline. Data flows left to right through filter, transform, take.

{{focus: source}} {{annotate: source "Source data"}} Data enters the pipeline from the source range. A vector, an array, or any range.

{{focus: filter-stage}} {{annotate: filter-stage "Filter"}} The filter stage skips elements that fail the predicate. Only matching elements pass through.

{{focus: transform-stage}} {{annotate: transform-stage "Transform"}} The transform stage applies a function to each surviving element. The output type can differ from the input.

{{focus: take-stage}} {{annotate: take-stage "Take N"}} The take stage limits output to the first N elements. Once N elements have been consumed, iteration stops. The rest of the pipeline never executes for remaining elements.

{{focus: simple-pipe}} {{zoom: 1.2x}} {{annotate: simple-pipe "Left to right"}} Read the pipe left to right. Start with data. Filter to evens. Square them. Take three. The pipeline reads like a sentence.

{{zoom: 1x}} {{focus: complex-pipe}} {{zoom: 1.2x}} {{annotate: complex-pipe "Compose freely"}} Pipes compose without limit. Each view adds a layer of lazy computation. No intermediate allocations. Elements flow through one at a time.

{{zoom: 1x}} {{focus: none}} {{zoom: 1x}} Pipes replace nested function calls with linear, readable chains. The data flows in one direction. Each stage has one job.

```diagram:lazy-pipeline
source [database]
filter [service]
transform [service]
take [service]
output [cache]
source --elements--> filter
filter --matching--> transform
transform --mapped--> take
take --first N--> output
{Pipeline: source, filter, transform, take, output}
---
source: source
filter-stage: filter
transform-stage: transform
take-stage: take
```

```code:pipe-code lang=cpp
#include <ranges>
#include <vector>

std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// Pipe: filter → transform → take
auto result = data
    | std::views::filter([](int x) { return x % 2 == 0; })
    | std::views::transform([](int x) { return x * x; })
    | std::views::take(3);

for (int x : result) {
    // 4, 16, 36 — first 3 even squares
}

// More complex pipeline
auto pipeline = data
    | std::views::filter([](int x) { return x > 3; })
    | std::views::transform([](int x) { return x * 10; })
    | std::views::take(4)
    | std::views::reverse;

for (int x : pipeline) {
    // 70, 60, 50, 40 — reversed
}
---
simple-pipe: 7-10
complex-pipe: 17-21
```

# take, drop, and reverse views

{{clear: slide}}

{{show: slice-code typewriter 2s linear}} Slicing views select subsequences without copying. take from the front, drop from the front, reverse the order.

{{focus: take-view}} {{zoom: 1.2x}} {{annotate: take-view "First N"}} take produces the first N elements. If the range has fewer than N, it yields everything. Lazy — no allocation.

{{zoom: 1x}} {{focus: drop-view}} {{zoom: 1.2x}} {{annotate: drop-view "Skip N"}} drop skips the first N elements and yields the rest. Pair take and drop to slice windows out of a range.

{{zoom: 1x}} {{focus: take-while}} {{zoom: 1.2x}} {{annotate: take-while "Until predicate"}} take while yields elements until the predicate fails. Then it stops. No further elements are evaluated.

{{zoom: 1x}} {{focus: drop-while}} {{zoom: 1.2x}} {{annotate: drop-while "Skip until"}} drop while skips elements until the predicate fails. Then it yields everything after that point.

{{zoom: 1x}} {{focus: reverse-view}} {{annotate: reverse-view "Reverse order"}} reverse produces elements in reverse order. It requires a bidirectional range — no forward-only iterators.

{{zoom: 1x}} {{focus: none}} These views are the building blocks for pagination, windowing, and prefix/suffix extraction. All lazy. All zero-copy.

```code:slice-code lang=cpp
#include <ranges>
#include <vector>

std::vector<int> v = {10, 20, 30, 40, 50, 60, 70, 80};

// take: first 3
auto first3 = v | std::views::take(3);
// 10, 20, 30

// drop: skip first 3
auto after3 = v | std::views::drop(3);
// 40, 50, 60, 70, 80

// take_while: elements while < 50
auto small = v | std::views::take_while([](int x) { return x < 50; });
// 10, 20, 30, 40

// drop_while: skip while < 50
auto big = v | std::views::drop_while([](int x) { return x < 50; });
// 50, 60, 70, 80

// reverse
auto rev = v | std::views::reverse;
// 80, 70, 60, 50, 40, 30, 20, 10
---
take-view: 7-8
drop-view: 11-12
take-while: 15-16
drop-while: 19-20
reverse-view: 23-24
```

# split and join views

{{clear: slide}}

{{show: split-code typewriter 2s linear}} split breaks a range into subranges at a delimiter. join flattens nested ranges into a single sequence.

{{focus: split-str}} {{zoom: 1.2x}} {{annotate: split-str "Split by delimiter"}} split view breaks a string view at every occurrence of the delimiter. Each piece is a subrange — a lazy view into the original data.

{{zoom: 1x}} {{focus: split-val}} {{zoom: 1.2x}} {{annotate: split-val "Split by value"}} split works on any range, not just strings. Split a vector of ints at every zero. The zeros are consumed — they don't appear in the output subranges.

{{zoom: 1x}} {{focus: join-flat}} {{zoom: 1.2x}} {{annotate: join-flat "Flatten nested"}} join view flattens one level of nesting. A vector of vectors becomes a single sequence. A range of subranges becomes a flat range.

{{zoom: 1x}} {{focus: join-with}} {{zoom: 1.2x}} {{annotate: join-with "Insert delimiter"}} join with inserts a delimiter between each subrange. It's the inverse of split. Split then join with a different delimiter effectively replaces delimiters.

{{zoom: 1x}} {{focus: none}} split and join are complementary. Split decomposes. Join recomposes. Both are lazy and zero-copy.

```code:split-code lang=cpp
#include <ranges>
#include <string_view>
#include <vector>

// Split a string by delimiter
std::string_view csv = "alice,bob,charlie,diana";
for (auto word : csv | std::views::split(',')) {
    // subranges: "alice", "bob", "charlie", "diana"
    auto sv = std::string_view(word.begin(), word.end());
}

// Split a vector by value
std::vector<int> v = {1, 2, 0, 3, 4, 0, 5};
for (auto chunk : v | std::views::split(0)) {
    // subranges: {1,2}, {3,4}, {5}
}

// join: flatten nested ranges
std::vector<std::vector<int>> nested = {{1, 2}, {3, 4}, {5}};
auto flat = nested | std::views::join;
// 1, 2, 3, 4, 5

// join_with: insert delimiter between subranges (C++23)
std::vector<std::vector<int>> parts = {{1, 2}, {3, 4}};
auto joined = parts | std::views::join_with(0);
// 1, 2, 0, 3, 4
---
split-str: 7-10
split-val: 13-16
join-flat: 19-21
join-with: 24-26
```

# Ranges vs classic algorithms

{{clear: slide}}

{{show: compare-code typewriter 2s linear}} When do you use ranges and when do you stick with classic algorithms? The answer depends on composition.

{{focus: classic-way}} {{zoom: 1.2x}} {{annotate: classic-way "Imperative"}} The classic approach: multiple passes, intermediate containers, explicit iterator management. It works. It's verbose. Each step allocates.

{{zoom: 1x}} {{focus: ranges-way}} {{zoom: 1.3x}} {{annotate: ranges-way "Declarative"}} The ranges approach: one pipeline, lazy evaluation, zero intermediate allocation. The intent is front and center. The plumbing disappears.

{{zoom: 1x}} {{focus: when-classic}} {{zoom: 1.2x}} {{annotate: when-classic "Use classic when"}} Classic algorithms win for single-step operations. sort a vector? ranges sort is marginally cleaner but the difference is tiny. The real payoff comes with composition.

{{zoom: 1x}} {{focus: when-ranges}} {{zoom: 1.2x}} {{annotate: when-ranges "Use ranges when"}} Ranges win when you chain two or more operations. Filter then transform then take. The pipeline eliminates intermediate containers and makes the data flow visible.

{{zoom: 1x}} {{focus: none}} {{zoom: 1x}} Classic algorithms are battle-tested and universally supported. Ranges are the future — more expressive, more composable, and they compile to the same machine code. Use ranges for new code. Understand classic for reading existing code.

```code:compare-code lang=cpp
#include <algorithm>
#include <ranges>
#include <vector>
#include <numeric>

std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// Classic: filter evens, square, sum first 3
std::vector<int> evens;
std::copy_if(data.begin(), data.end(), std::back_inserter(evens),
    [](int x) { return x % 2 == 0; });
std::vector<int> squared;
std::transform(evens.begin(), evens.end(), std::back_inserter(squared),
    [](int x) { return x * x; });
squared.resize(3);
auto sum1 = std::accumulate(squared.begin(), squared.end(), 0);
// 3 allocations, 3 passes

// Ranges: same result, zero intermediate allocation
auto pipeline = data
    | std::views::filter([](int x) { return x % 2 == 0; })
    | std::views::transform([](int x) { return x * x; })
    | std::views::take(3);
// single lazy pass, zero allocation

// Classic: best for simple single-step operations
std::ranges::sort(data);

// Ranges: best for multi-step pipelines
// Prefer ranges for new code
---
classic-way: 9-17
ranges-way: 20-24
when-classic: 27
when-ranges: 20-24, 30
```
