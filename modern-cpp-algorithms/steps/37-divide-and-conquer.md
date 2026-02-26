---
title: Divide and Conquer
---

# The pattern

{{show: dc-intro typewriter 2s linear}} Divide and conquer: split the problem into independent subproblems, solve each recursively, combine the results. Three parts — divide, conquer, combine. The combine step is where the real work often lives.

{{show: dc-diagram grow 0.5s spring}} {{focus: divide-step}} {{annotate: divide-step "Divide"}} Split the input roughly in half. The split should be O(1) or O(n) — never O(n log n), or you've hidden the recursion inside the divide step.

{{focus: conquer-step}} {{annotate: conquer-step "Conquer"}} Recurse on each half. Base case: when the input is small enough to solve directly (size 0 or 1).

{{focus: combine-step}} {{annotate: combine-step "Combine"}} Merge the two sub-answers into one. This step determines the recurrence. Merge sort: O(n) combine → T(n) = 2T(n/2) + O(n) → O(n log n). Quick sort: O(n) partition but combine is trivial → same recurrence, different constant.

{{focus: none}} Divide and conquer differs from DP in one crucial way: subproblems don't overlap. You're not caching; you're just recursing on genuinely disjoint pieces.

```diagram:dc-diagram
Problem(n) -> Left(n/2)
Problem(n) -> Right(n/2)
Left(n/2) -> Combine
Right(n/2) -> Combine
---
divide-step: Problem(n)
conquer-step: Left(n/2)
combine-step: Combine
```

# Merge sort

{{clear: slide}}

{{show: merge-sort-code typewriter 2s linear}} Merge sort is the cleanest divide-and-conquer algorithm. Split in half, sort each half, merge. O(n log n) guaranteed regardless of input. The merge step is worth understanding deeply.

{{show: merge-sort-data grow 0.5s spring}} {{focus: split}} {{zoom: 1.2x}} {{annotate: split "Midpoint split"}} `mid = left + (right - left) / 2`. Always this form, never `(left + right) / 2` — that can overflow with large indices.

{{zoom: 1x}} {{focus: merge-step}} {{zoom: 1.3x}} {{annotate: merge-step "Two-pointer merge"}} Merge two sorted halves with two pointers. Compare heads, take the smaller, advance that pointer. When one side is exhausted, append the other. This is O(n) and stable.

{{zoom: 1x}} {{focus: stability}} {{zoom: 1.2x}} {{annotate: stability "Stable sort"}} The `<=` in the comparison (`left[li] <= right[ri]`) ensures stability: equal elements from the left half come before those from the right. Change to `<` for instability.

{{zoom: 1x}} {{focus: none}} Merge sort is the baseline O(n log n) sort. It's stable, predictable, and easy to extend — counting inversions modifies only the merge step.

```code:merge-sort-code lang=cpp
#include <vector>

// Merge two sorted halves [left..mid] and [mid+1..right] in-place.
void merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> tmp;
    tmp.reserve(right - left + 1);

    int li = left, ri = mid + 1;
    while (li <= mid && ri <= right) {
        if (arr[li] <= arr[ri]) tmp.push_back(arr[li++]);
        else                    tmp.push_back(arr[ri++]);
    }
    while (li <= mid)    tmp.push_back(arr[li++]);
    while (ri <= right)  tmp.push_back(arr[ri++]);

    std::ranges::copy(tmp, arr.begin() + left);
}

void merge_sort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;  // base case
    const int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}
---
split: 18-19
merge-step: 8-13
stability: 9
```

```data:merge-sort-data type=array
[5, 3, 8, 1, 9, 2, 7, 4]
---
region: 0
```

# Quick sort

{{clear: slide}}

{{show: quick-sort-code typewriter 2s linear}} Quick sort partitions around a pivot, then recurses on each side. Average O(n log n), worst-case O(n²) with bad pivots. Median-of-three pivot selection pushes worst case to near-zero probability on real data.

{{focus: partition}} {{zoom: 1.2x}} {{annotate: partition "Lomuto partition"}} Lomuto partition: pick the last element as pivot. Walk a pointer `i` from left to right. When you find an element ≤ pivot, swap it to position `i` and advance. Pivot ends up at its final sorted position.

{{zoom: 1x}} {{focus: pivot-choice}} {{zoom: 1.2x}} {{annotate: pivot-choice "Median of three"}} Median-of-three: look at first, middle, and last elements, sort them, use the median. Eliminates worst-case behavior on sorted/reverse-sorted input. One function call before partitioning.

{{zoom: 1x}} {{focus: recurse}} {{zoom: 1.2x}} {{annotate: recurse "Recurse around pivot"}} After partition, pivot index `p` is in its final position. Recurse on `[left, p-1]` and `[p+1, right]`. The combine step is nothing — the array is already in place.

{{zoom: 1x}} {{focus: none}} Quick sort's advantage over merge sort: in-place, better cache performance, lower constant factor. Its risk: pivot choice. Use `std::sort` in production — it's typically introsort (quick + heap sort fallback).

```code:quick-sort-code lang=cpp
int partition(std::vector<int>& arr, int left, int right) {
    // Median-of-three pivot: sort first/mid/last, use mid as pivot
    const int mid = left + (right - left) / 2;
    if (arr[left] > arr[mid])  std::swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) std::swap(arr[left], arr[right]);
    if (arr[mid]  > arr[right]) std::swap(arr[mid], arr[right]);
    // arr[mid] is the median — move it to right-1 as pivot
    std::swap(arr[mid], arr[right]);
    const int pivot = arr[right];

    int i = left - 1;
    for (int j = left; j < right; ++j) {
        if (arr[j] <= pivot) std::swap(arr[++i], arr[j]);
    }
    std::swap(arr[i + 1], arr[right]);
    return i + 1;
}

void quick_sort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;
    const int p = partition(arr, left, right);
    quick_sort(arr, left, p - 1);
    quick_sort(arr, p + 1, right);
}
---
partition: 10-14
pivot-choice: 2-8
recurse: 18-21
```

# Count inversions

{{clear: slide}}

{{show: inversions-code typewriter 2s linear}} An inversion is a pair (i, j) where i < j but arr[i] > arr[j]. Count all inversions using modified merge sort. The trick: during merge, when we take from the right half, all remaining left elements are greater — they each form an inversion.

{{focus: inversion-count}} {{zoom: 1.2x}} {{annotate: inversion-count "Cross-inversions in merge"}} When `arr[ri] < arr[li]` during merge, every element from `li` to `mid` is an inversion partner for `arr[ri]`. That's `mid - li + 1` inversions added at once — O(1) per such event.

{{zoom: 1x}} {{focus: accumulate}} {{zoom: 1.2x}} {{annotate: accumulate "Accumulate in merge"}} Pass an `inversions` counter by reference into the merge function. Add to it during the right-side-wins branch. The rest of merge sort is unchanged.

{{zoom: 1x}} {{focus: none}} This is the power of divide and conquer: you can augment the combine step to compute additional information without changing the O(n log n) bound.

```code:inversions-code lang=cpp
// Modified merge that counts cross-inversions
long long merge_count(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> tmp;
    tmp.reserve(right - left + 1);

    long long inversions = 0;
    int li = left, ri = mid + 1;

    while (li <= mid && ri <= right) {
        if (arr[li] <= arr[ri]) {
            tmp.push_back(arr[li++]);
        } else {
            // arr[ri] < arr[li]: all remaining left elements > arr[ri]
            inversions += (mid - li + 1);
            tmp.push_back(arr[ri++]);
        }
    }
    while (li <= mid)   tmp.push_back(arr[li++]);
    while (ri <= right) tmp.push_back(arr[ri++]);

    std::ranges::copy(tmp, arr.begin() + left);
    return inversions;
}

long long count_inversions(std::vector<int>& arr, int left, int right) {
    if (left >= right) return 0;
    const int mid = left + (right - left) / 2;
    long long inv = count_inversions(arr, left, mid)
                  + count_inversions(arr, mid + 1, right)
                  + merge_count(arr, left, mid, right);
    return inv;
}
---
inversion-count: 12-14
accumulate: 3-19
```

# Closest pair of points

{{clear: slide}}

{{show: closest-pair-code typewriter 2s linear}} Closest pair of points in 2D: naive is O(n²). Divide and conquer gives O(n log n). Split by median x-coordinate, solve each half, then check the strip where cross-half pairs could be closer than the current best.

{{focus: halves}} {{zoom: 1.2x}} {{annotate: halves "Split on x"}} Sort points by x. Split at the midpoint. Recursively find the closest pair in each half. Let `d = min(d_left, d_right)`.

{{zoom: 1x}} {{focus: strip-check}} {{zoom: 1.3x}} {{annotate: strip-check "Strip width = d"}} The only cross-half candidates are points within distance `d` of the dividing line. Collect them, sort by y. For each strip point, check at most 7 neighbors — it's proven that no more than 7 points fit in a 2d × d box.

{{zoom: 1x}} {{focus: strip-bound}} {{annotate: strip-bound "7 comparisons max"}} The 7-neighbor bound is geometric: in a d×2d rectangle split by the midline, you can pack at most 8 points with mutual distance ≥ d. So the inner loop is effectively O(1) per point — the strip check is O(n).

{{focus: none}} This is the canonical 2D divide and conquer example. The strip optimization is the insight that makes it O(n log n). Without it you'd still have O(n²) in the combine step.

```code:closest-pair-code lang=cpp
#include <cmath>
#include <algorithm>

struct Point { double x, y; };

double dist(const Point& a, const Point& b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

double closest_strip(std::vector<Point>& strip, double d) {
    std::ranges::sort(strip, {}, &Point::y);
    double best = d;
    for (int i = 0; i < static_cast<int>(strip.size()); ++i) {
        // At most 7 points ahead can improve d
        for (int j = i + 1; j < static_cast<int>(strip.size())
                          && (strip[j].y - strip[i].y) < best; ++j) {
            best = std::min(best, dist(strip[i], strip[j]));
        }
    }
    return best;
}

double closest_rec(std::vector<Point>& pts, int l, int r) {
    if (r - l <= 3) {
        double mn = std::numeric_limits<double>::max();
        for (int i = l; i < r; ++i)
            for (int j = i+1; j < r; ++j) mn = std::min(mn, dist(pts[i], pts[j]));
        std::ranges::sort(pts.begin() + l, pts.begin() + r, {}, &Point::y);
        return mn;
    }
    const int m = l + (r - l) / 2;
    const double mx = pts[m].x;
    double d = std::min(closest_rec(pts, l, m), closest_rec(pts, m, r));

    std::vector<Point> strip;
    for (int i = l; i < r; ++i)
        if (std::abs(pts[i].x - mx) < d) strip.push_back(pts[i]);

    return std::min(d, closest_strip(strip, d));
}
---
halves: 21-28
strip-check: 9-17
strip-bound: 13-16
```
