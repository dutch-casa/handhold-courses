---
title: The binary search pattern
---

# The sorted invariant

{{show: sorted-viz grow 0.5s spring}} Binary search works because the data is sorted. That's the invariant. If the array is sorted, you can eliminate half the search space with one comparison.

{{focus: unsorted}} {{annotate: unsorted "Linear scan"}} Unsorted data? You check every element. O of n. No shortcut exists because any element could be anywhere.

{{focus: sorted}} {{annotate: sorted "Halving"}} Sorted data? Compare the middle element to your target. If the target is smaller, it's in the left half. If larger, the right half. You just eliminated fifty percent in one step.

{{focus: none}} Repeat. Each step halves the space. N elements become log n comparisons. That's the power of the sorted invariant.

```data:sorted-viz type=array
["[3|8|1|5|9|2|7]", "[1|2|3|5|7|8|9]"]
---
unsorted: 0
sorted: 1
```

# The template

{{clear: slide}}

{{show: template-code typewriter 2s linear}} Every binary search has the same skeleton. Two pointers, a loop, a midpoint.

{{focus: init}} {{zoom: 1.2x}} {{annotate: init "Search bounds"}} lo starts at zero, hi starts at size minus one. These define the search window. Everything outside is eliminated.

{{zoom: 1x}} {{focus: loop}} {{zoom: 1.2x}} {{annotate: loop "Converge"}} The loop runs while lo is less than or equal to hi. When they cross, the search is exhausted.

{{zoom: 1x}} {{focus: mid-calc}} {{zoom: 1.3x}} {{annotate: mid-calc "Avoid overflow"}} Calculate mid as lo plus half the distance. Don't use lo plus hi divided by two — that overflows for large indices. This is the safe formula.

{{zoom: 1x}} {{focus: branch}} {{zoom: 1.2x}} {{annotate: branch "Eliminate half"}} Three cases. Found it — return. Target is smaller — move hi left. Target is larger — move lo right. Each iteration cuts the space in half.

{{zoom: 1x}} {{focus: not-found}} {{annotate: not-found "Exhausted"}} If the loop ends without finding the target, return negative one. The element isn't in the array.

{{zoom: 1x}} {{focus: none}} Memorize this shape. Every binary search problem is a variation of it.

```code:template-code lang=cpp
int binary_search(const std::vector<int>& arr, int target) {
    int lo = 0;
    int hi = static_cast<int>(arr.size()) - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}
---
init: 2-3
loop: 5
mid-calc: 6
branch: 8-15
not-found: 17
```

# Classic binary search

{{clear: slide}}

{{show: binary-search-trace grow 0.5s spring}} Let's trace it. Array of seven sorted values, searching for 23.

{{focus: step-0}} {{annotate: step-0 "Full range"}} lo at index 0, hi at index 6. Mid is index 3, value 15. Target 23 is larger — move lo to 4.

{{focus: step-1}} {{annotate: step-1 "Right half"}} lo at 4, hi at 6. Mid is 5, value 27. Target 23 is smaller — move hi to 4.

{{focus: step-2}} {{annotate: step-2 "Found"}} lo at 4, hi at 4. Mid is 4, value 23. Match. Return index 4.

{{focus: none}} Three comparisons to search seven elements. Log base 2 of 7 is about 2.8. That's binary search — logarithmic.

```data:binary-search-trace type=array
["[2|5|8|15|23|27|36] ^lo=0 ^hi=6 ^mid=3", "[2|5|8|15|23|27|36] ^lo=4 ^hi=6 ^mid=5", "[2|5|8|15|23|27|36] ^lo=4 ^hi=4 ^mid=4 FOUND"]
---
step-0: 0
step-1: 1
step-2: 2
```

# std::lower_bound

{{clear: slide}}

{{show: lower-code typewriter 1.5s linear}} lower bound finds the first element greater than or equal to the target. It returns an iterator, not an index.

{{focus: call}} {{zoom: 1.2x}} {{annotate: call "First >= target"}} Pass begin, end, and the target value. It returns an iterator to the first element that's not less than the target.

{{zoom: 1x}} {{focus: check}} {{zoom: 1.3x}} {{annotate: check "Verify hit"}} Always check two things. First: is the iterator valid (not end)? Second: does the value actually equal what you want? lower bound gives you "at least as big" — that might not be an exact match.

{{zoom: 1x}} {{focus: index}} {{zoom: 1.2x}} {{annotate: index "Iterator to index"}} Subtract begin to get the index. std distance works too, but subtraction is cleaner for random-access iterators.

{{zoom: 1x}} {{focus: none}} lower bound is the workhorse. Most binary search problems in the standard library reduce to it.

```code:lower-code lang=cpp
#include <algorithm>
#include <vector>

std::vector<int> v = {1, 3, 3, 5, 7, 9, 11};

auto it = std::lower_bound(v.begin(), v.end(), 5);
// it points to first element >= 5 → v[3] = 5

if (it != v.end() && *it == 5) {
    auto idx = it - v.begin();  // 3
}

// If target not present:
auto it2 = std::lower_bound(v.begin(), v.end(), 4);
// it2 points to 5 (first element >= 4)
---
call: 6
check: 9-11
index: 10
```

# std::upper_bound

{{clear: slide}}

{{show: upper-code typewriter 1.5s linear}} upper bound finds the first element strictly greater than the target. Pair it with lower bound to find ranges of equal values.

{{focus: upper-call}} {{zoom: 1.2x}} {{annotate: upper-call "First > target"}} upper bound returns an iterator to the first element greater than the target. Not greater-or-equal — strictly greater.

{{zoom: 1x}} {{focus: range}} {{zoom: 1.3x}} {{annotate: range "Equal range"}} lower bound to upper bound gives you the range of all elements equal to the target. The count is the distance between them.

{{zoom: 1x}} {{focus: equal-range}} {{zoom: 1.2x}} {{annotate: equal-range "One call"}} std equal range returns both iterators in a pair. Slightly more efficient than calling lower bound and upper bound separately.

{{zoom: 1x}} {{focus: none}} lower bound gives the left edge. upper bound gives the right edge. Together they bracket the equal range.

```code:upper-code lang=cpp
#include <algorithm>
#include <vector>

std::vector<int> v = {1, 3, 3, 3, 5, 7, 9};

auto it = std::upper_bound(v.begin(), v.end(), 3);
// it points to 5 (first element > 3), index 4

// Count occurrences of 3:
auto lo = std::lower_bound(v.begin(), v.end(), 3);  // index 1
auto hi = std::upper_bound(v.begin(), v.end(), 3);  // index 4
int count = static_cast<int>(hi - lo);               // 3

// Or use equal_range:
auto [first, last] = std::equal_range(v.begin(), v.end(), 3);
int count2 = static_cast<int>(last - first);          // 3
---
upper-call: 6
range: 10-12
equal-range: 15-16
```

# binary_search and partition_point

{{clear: slide}}

{{show: misc-code typewriter 1.5s linear}} Two more tools. binary search tells you if an element exists. partition point generalizes to predicates.

{{focus: bin-search}} {{zoom: 1.2x}} {{annotate: bin-search "Existence only"}} std binary search returns a bool. Yes or no — is the value present? It doesn't tell you where. Use lower bound if you need the position.

{{zoom: 1x}} {{focus: part-point}} {{zoom: 1.3x}} {{annotate: part-point "Predicate search"}} partition point takes a predicate instead of a value. It finds the first element where the predicate returns false. The range must be partitioned — all trues before all falses.

{{zoom: 1x}} {{focus: custom}} {{zoom: 1.2x}} {{annotate: custom "Flexible"}} This is powerful. Any monotonic condition works. "Is this element less than 10?" "Is this square root less than the target?" Binary search on any yes/no boundary.

{{zoom: 1x}} {{focus: none}} binary search for existence checks. partition point when your condition is a predicate. lower bound for everything else.

```code:misc-code lang=cpp
#include <algorithm>
#include <vector>

std::vector<int> v = {1, 3, 5, 7, 9, 11, 13};

// Does 7 exist?
bool found = std::binary_search(v.begin(), v.end(), 7);  // true

// partition_point: first element where predicate is false
auto it = std::partition_point(v.begin(), v.end(),
    [](int x) { return x < 10; });
// it points to 11 (first element where x < 10 is false)

// Custom predicate binary search:
auto it2 = std::partition_point(v.begin(), v.end(),
    [](int x) { return x * x < 50; });
// it2 points to 9 (first element where x*x >= 50)
---
bin-search: 7
part-point: 10-12
custom: 15-17
```

# Recognizing binary search

{{clear: slide}}

{{show: signals grow 0.5s spring}} How do you know a problem wants binary search? Look for these signals.

{{focus: sig-sorted}} {{annotate: sig-sorted "Sorted input"}} The input is sorted, or can be sorted without breaking the problem. That's signal number one.

{{focus: sig-log}} {{annotate: sig-log "Log n required"}} The problem asks for better than O of n, or the input is too large for linear scan. A million elements? Binary search handles it in 20 steps.

{{focus: sig-monotonic}} {{annotate: sig-monotonic "Yes/no boundary"}} There's a monotonic property. Some condition that's true for small values and false for large values, or vice versa. A clean boundary you can search for.

{{focus: sig-minmax}} {{annotate: sig-minmax "Optimize"}} The problem says "find the minimum value that satisfies..." or "find the maximum..." That's binary search on the answer space. We'll cover that next lesson.

{{focus: none}} Sorted data, logarithmic requirement, monotonic boundary, optimization. Any of these — think binary search first.

```data:signals type=array
["Sorted input or sortable", "Need O(log n) or better", "Monotonic true→false boundary", "Minimize/maximize with feasibility check"]
---
sig-sorted: 0
sig-log: 1
sig-monotonic: 2
sig-minmax: 3
```
