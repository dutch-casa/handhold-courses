---
title: Binary search variations
---

# Search on the answer space

{{show: answer-space grow 0.5s spring}} Sometimes you don't search the array — you search the answer. The input is a range of possible answers, and you binary search for the best one.

{{focus: idea}} {{annotate: idea "Search answers"}} The pattern: define a range of candidate answers. For each candidate, check if it's feasible. The feasibility function is monotonic — at some point, candidates go from infeasible to feasible. Binary search that boundary.

{{focus: example}} {{annotate: example "Ship packages"}} Classic example: ship packages within D days with minimum capacity. The answer is a capacity value. Too small — can't fit. Big enough — works. Binary search on the capacity.

{{focus: none}} This flips your thinking. You're not searching data. You're searching the space of possible answers.

{{clear: slide}}

{{show: answer-code typewriter 2s linear}} Here's the template. Binary search the answer range, check feasibility at each midpoint.

{{focus: bounds}} {{zoom: 1.2x}} {{annotate: bounds "Answer range"}} lo is the smallest possible answer. hi is the largest. For shipping capacity: lo is the max single package weight, hi is the total weight of all packages.

{{zoom: 1x}} {{focus: mid-check}} {{zoom: 1.3x}} {{annotate: mid-check "Feasible?"}} At each midpoint, call the feasibility function. Can we ship all packages in D days with capacity mid? If yes, try smaller. If no, go bigger.

{{zoom: 1x}} {{focus: feasible-fn}} {{zoom: 1.2x}} {{annotate: feasible-fn "Greedy check"}} The feasibility check is usually greedy. Pack packages greedily until the day is full, start a new day. Count the days. Are they within the limit?

{{zoom: 1x}} {{focus: none}} Answer space search turns optimization problems into decision problems. "Find the minimum X" becomes "is X feasible?"

```data:answer-space type=array
["Answer range: [lo ............. hi]", "Check mid: feasible? → shrink right", "Check mid: infeasible? → shrink left", "Converge on optimal answer"]
---
idea: 0
example: 1, 2
```

```code:answer-code lang=cpp
#include <algorithm>
#include <vector>
#include <numeric>

bool can_ship(const std::vector<int>& weights, int days, int capacity) {
    int current_load = 0;
    int needed_days = 1;
    for (int w : weights) {
        if (current_load + w > capacity) {
            ++needed_days;
            current_load = 0;
        }
        current_load += w;
    }
    return needed_days <= days;
}

int min_capacity(const std::vector<int>& weights, int days) {
    int lo = *std::max_element(weights.begin(), weights.end());
    int hi = std::accumulate(weights.begin(), weights.end(), 0);

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (can_ship(weights, days, mid)) {
            hi = mid;           // feasible — try smaller
        } else {
            lo = mid + 1;       // infeasible — need more capacity
        }
    }
    return lo;
}
---
bounds: 19-20
mid-check: 23-28
feasible-fn: 5-15
```

# Rotated sorted array

{{clear: slide}}

{{show: rotated-array grow 0.5s spring}} A sorted array that's been rotated. The sorted order is broken at one point, but both halves are still sorted.

{{focus: original}} {{annotate: original "Sorted"}} Original sorted array. Clean ascending order.

{{focus: rotated}} {{annotate: rotated "Rotated at pivot"}} After rotation, the array wraps around. Two sorted halves with a break in the middle. The smallest element is the pivot point.

{{focus: none}} Binary search still works — you just need to figure out which half is sorted at each step.

{{clear: slide}}

{{show: rotated-code typewriter 2s linear}} The trick: at any midpoint, one half is guaranteed sorted. Determine which half, then check if the target lives there.

{{focus: sorted-left}} {{zoom: 1.2x}} {{annotate: sorted-left "Left is sorted?"}} If the left element is less than or equal to mid, the left half is sorted. Check if the target falls in that range. If yes, search left. If no, search right.

{{zoom: 1x}} {{focus: sorted-right}} {{zoom: 1.3x}} {{annotate: sorted-right "Right is sorted?"}} Otherwise the right half is sorted. Same logic — check if the target is in the sorted right half. If yes, search right. If no, search left.

{{zoom: 1x}} {{focus: none}} One extra comparison per step. Still O of log n. The rotation doesn't change the complexity — it just adds a branch.

```data:rotated-array type=array
["[1|3|5|7|9|11|13]", "[9|11|13|1|3|5|7]"]
---
original: 0
rotated: 1
```

```code:rotated-code lang=cpp
int search_rotated(const std::vector<int>& arr, int target) {
    int lo = 0;
    int hi = static_cast<int>(arr.size()) - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;

        if (arr[lo] <= arr[mid]) {
            // Left half is sorted
            if (arr[lo] <= target && target < arr[mid]) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } else {
            // Right half is sorted
            if (arr[mid] < target && target <= arr[hi]) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }
    return -1;
}
---
sorted-left: 9-15
sorted-right: 17-22
```

# 2D matrix search

{{clear: slide}}

{{show: matrix-search grow 0.5s spring}} A sorted matrix — rows are sorted, and the first element of each row is greater than the last element of the previous row. It's really a sorted array folded into rows.

{{focus: flat-view}} {{annotate: flat-view "Virtually flat"}} Treat the matrix as a single sorted array of rows times columns elements. Index k maps to row k divided by columns, column k modulo columns.

{{focus: cell-access}} {{annotate: cell-access "Row and column"}} This mapping lets you run standard binary search on a 2D matrix without actually flattening it. Zero extra space.

{{focus: none}} The matrix is sorted in disguise. Unfold it mentally, search it normally.

{{clear: slide}}

{{show: matrix-code typewriter 1.5s linear}} Standard binary search, but index into 2D using division and modulo.

{{focus: dims}} {{zoom: 1.2x}} {{annotate: dims "Total elements"}} Compute rows and columns. The total element count is rows times cols — that's your search space.

{{zoom: 1x}} {{focus: mapping}} {{zoom: 1.3x}} {{annotate: mapping "1D → 2D"}} Convert the 1D midpoint to 2D coordinates. mid divided by cols gives the row. mid modulo cols gives the column. Standard index decomposition.

{{zoom: 1x}} {{focus: compare}} {{zoom: 1.2x}} {{annotate: compare "Normal search"}} After the coordinate mapping, it's the same binary search. Compare, eliminate half, repeat.

{{zoom: 1x}} {{focus: none}} Any time a 2D structure has a global sorted order, flatten it mentally and binary search.

```data:matrix-search type=matrix
[[ 1,  3,  5,  7],
 [10, 11, 16, 20],
 [23, 30, 34, 60]]
---
flat-view: [0][0], [0][1], [0][2], [0][3], [1][0], [1][1]
cell-access: [1][2]
```

```code:matrix-code lang=cpp
bool search_matrix(const std::vector<std::vector<int>>& matrix, int target) {
    if (matrix.empty()) return false;
    int rows = static_cast<int>(matrix.size());
    int cols = static_cast<int>(matrix[0].size());
    int lo = 0;
    int hi = rows * cols - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int val = matrix[mid / cols][mid % cols];

        if (val == target) return true;
        if (val < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return false;
}
---
dims: 3-6
mapping: 9-10
compare: 12-17
```

# Minimize-max and maximize-min

{{clear: slide}}

{{show: minmax-code typewriter 2s linear}} "Split the array into k parts to minimize the maximum sum." This is answer space search with a feasibility check.

{{focus: search-range}} {{zoom: 1.2x}} {{annotate: search-range "Answer bounds"}} The smallest possible max-sum is the largest single element. The largest is the total sum. Binary search between them.

{{zoom: 1x}} {{focus: feasible}} {{zoom: 1.3x}} {{annotate: feasible "Greedy partition"}} Can we split into at most k parts where no part exceeds mid? Greedily accumulate elements. When adding the next would exceed mid, start a new part. Count the parts.

{{zoom: 1x}} {{focus: converge}} {{zoom: 1.2x}} {{annotate: converge "Shrink range"}} If k-or-fewer parts suffice, mid might be the answer — try smaller. If too many parts, mid is too small — go bigger.

{{zoom: 1x}} {{focus: none}} Minimize-the-maximum and maximize-the-minimum are the same pattern with flipped comparisons. Spot the keywords "minimize the maximum" or "maximize the minimum" and think binary search immediately.

```code:minmax-code lang=cpp
#include <algorithm>
#include <numeric>
#include <vector>

bool can_split(const std::vector<int>& arr, int k, int max_sum) {
    int parts = 1;
    int current = 0;
    for (int x : arr) {
        if (current + x > max_sum) {
            ++parts;
            current = 0;
        }
        current += x;
    }
    return parts <= k;
}

int minimize_max_sum(const std::vector<int>& arr, int k) {
    int lo = *std::max_element(arr.begin(), arr.end());
    int hi = std::accumulate(arr.begin(), arr.end(), 0);

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (can_split(arr, k, mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}
---
search-range: 19-20
feasible: 5-15
converge: 22-28
```

# Floating-point binary search

{{clear: slide}}

{{show: float-code typewriter 1.5s linear}} Binary search on real numbers. No integer indices — just narrow a floating-point range until it's precise enough.

{{focus: precision}} {{zoom: 1.2x}} {{annotate: precision "Epsilon guard"}} Instead of lo less-than-or-equal hi, loop while the gap exceeds epsilon. Or run a fixed number of iterations — 100 iterations give you roughly 30 decimal digits of precision.

{{zoom: 1x}} {{focus: mid-real}} {{zoom: 1.3x}} {{annotate: mid-real "Real midpoint"}} The midpoint is just the average of lo and hi. No overflow concern with typical floating-point ranges.

{{zoom: 1x}} {{focus: check}} {{zoom: 1.2x}} {{annotate: check "Narrow"}} Check the condition at mid. Narrow the range. Same logic as integer binary search, but the domain is continuous.

{{zoom: 1x}} {{focus: none}} Square roots, cube roots, any monotonic continuous function — floating-point binary search finds the root in about 100 iterations regardless of the range.

```code:float-code lang=cpp
double my_sqrt(double x) {
    double lo = 0.0;
    double hi = x;
    if (x < 1.0) hi = 1.0;  // handle x in (0, 1)

    for (int i = 0; i < 100; ++i) {
        double mid = lo + (hi - lo) / 2.0;
        if (mid * mid <= x) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    return lo;
}
---
precision: 6
mid-real: 7
check: 8-12
```

# Pattern recognition

{{clear: slide}}

{{show: decision grow 0.5s spring}} Binary search shows up in disguise. Here's how to unmask it.

{{focus: direct}} {{annotate: direct "Sorted + lookup"}} Direct search: sorted array, find an element. Textbook binary search. Use lower bound.

{{focus: answer}} {{annotate: answer "Optimize"}} Answer space: "minimize X such that..." or "maximize Y such that..." Binary search the answer, feasibility check at each step.

{{focus: rotated}} {{annotate: rotated "Modified sorted"}} Modified sorted: rotated arrays, arrays with duplicates, peak finding. The sorted property is bent but not broken.

{{focus: predicate}} {{annotate: predicate "True/false boundary"}} Predicate boundary: any monotonic true/false sequence. First bad version. Minimum speed to arrive on time. partition point handles these.

{{focus: none}} The common thread: a monotonic condition that lets you eliminate half the space. When you see that structure, binary search is the tool.

```data:decision type=array
["Direct: sorted array + element lookup", "Answer space: optimize with feasibility", "Modified sorted: rotated, peaks, dupes", "Predicate: monotonic true→false boundary"]
---
direct: 0
answer: 1
rotated: 2
predicate: 3
```
