---
title: Two pointers pattern
---

# The idea

{{show: complexity-viz grow 0.5s spring}} Two pointers replaces a nested loop with a single pass. O of n squared becomes O of n. The trick: move pointers based on the problem's structure, not brute force.

{{focus: brute}} {{annotate: brute "O(n^2)"}} Brute force: for every element, scan every other element. Two nested loops. Quadratic.

{{focus: smart}} {{annotate: smart "O(n)"}} Two pointers: place pointers at strategic positions. Move them based on what you learn from each comparison. Each pointer travels the array at most once. Linear.

{{focus: none}} The pointers aren't random. They exploit sorted order, or partitioning, or a specific invariant. That structure is what makes the elimination safe.

```data:complexity-viz type=array
["Brute: O(n^2) — check all pairs", "Two pointers: O(n) — eliminate intelligently"]
---
brute: 0
smart: 1
```

# Opposite-direction pointers

{{clear: slide}}

{{show: two-sum grow 0.5s spring}} Two pointers from opposite ends. Classic: two-sum on a sorted array. Find two numbers that add to the target.

{{focus: start}} {{annotate: start "Both ends"}} Left pointer at the beginning, right pointer at the end. Sum them.

{{focus: too-small}} {{annotate: too-small "Move left →"}} Sum is too small? The left element is the smallest available. Moving right pointer left makes it smaller — wrong direction. Move left pointer right to increase the sum.

{{focus: too-big}} {{annotate: too-big "← Move right"}} Sum is too big? Move right pointer left to decrease it.

{{focus: found}} {{annotate: found "Match"}} Sum equals target? Done. The pointers converged on the answer.

{{focus: none}} Each step eliminates an entire row or column of the pair matrix. That's why it's linear.

{{clear: slide}}

{{show: twosum-code typewriter 1.5s linear}} The implementation is tight. Sort guarantees the elimination logic works.

{{focus: init}} {{zoom: 1.2x}} {{annotate: init "Endpoints"}} Left at zero, right at end. The widest possible window.

{{zoom: 1x}} {{focus: sum-check}} {{zoom: 1.3x}} {{annotate: sum-check "Three cases"}} Three outcomes: match, too small, too big. Each one moves exactly one pointer by exactly one position.

{{zoom: 1x}} {{focus: converge}} {{zoom: 1.2x}} {{annotate: converge "They meet"}} The pointers move toward each other. At most n steps total. When they meet, all pairs have been considered.

{{zoom: 1x}} {{focus: none}} Sorted input plus opposite-direction pointers. The first pattern to try when a problem involves pairs in sorted data.

```data:two-sum type=array
["[1|3|5|7|9|11] ^L=0 ^R=5 sum=12 >target", "[1|3|5|7|9|11] ^L=0 ^R=4 sum=10 <target", "[1|3|5|7|9|11] ^L=1 ^R=4 sum=12 >target", "[1|3|5|7|9|11] ^L=1 ^R=3 sum=10 =target FOUND"]
---
start: 0
too-big: 0
too-small: 1
found: 3
```

```code:twosum-code lang=cpp
#include <vector>
#include <utility>

std::pair<int, int> two_sum_sorted(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;

    while (left < right) {
        int sum = arr[left] + arr[right];
        if (sum == target) {
            return {left, right};
        }
        if (sum < target) {
            ++left;
        } else {
            --right;
        }
    }
    return {-1, -1};
}
---
init: 5-6
sum-check: 9-17
converge: 8
```

# Fast and slow pointers

{{clear: slide}}

{{show: fast-slow grow 0.5s spring}} Same-direction pointers moving at different speeds. The classic application: cycle detection in a linked list.

{{focus: no-cycle}} {{annotate: no-cycle "Fast hits null"}} If there's no cycle, the fast pointer reaches the end. It hits null. Done — no cycle.

{{focus: cycle}} {{annotate: cycle "They meet"}} If there's a cycle, the fast pointer enters it first. The slow pointer follows. Fast gains one node per step. Eventually it laps slow, and they collide inside the cycle.

{{focus: none}} This is Floyd's tortoise and hare. It detects cycles in O of n time and O of 1 space. No hash set needed.

{{clear: slide}}

{{show: cycle-code typewriter 1.5s linear}} Two pointers, same start, different speeds.

{{focus: ptrs}} {{zoom: 1.2x}} {{annotate: ptrs "Two speeds"}} Slow advances one step. Fast advances two steps. Both start at head.

{{zoom: 1x}} {{focus: loop}} {{zoom: 1.3x}} {{annotate: loop "Chase"}} Loop while fast and fast next are valid. If they're ever equal (after at least one step), there's a cycle.

{{zoom: 1x}} {{focus: meet}} {{zoom: 1.2x}} {{annotate: meet "Collision"}} When slow equals fast, they've met inside the cycle. Return true.

{{zoom: 1x}} {{focus: none}} Floyd's algorithm works for any sequence with potential cycles — linked lists, function iteration, random number sequences.

```data:fast-slow type=linked-list
[3] -> [2] -> [0] -> [-4] -> [2]
---
no-cycle: [3], [2], [0], [-4]
cycle: [2], [0], [-4]
```

```code:cycle-code lang=cpp
struct ListNode {
    int val;
    ListNode* next;
};

bool has_cycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return true;
        }
    }
    return false;
}
---
ptrs: 7-8
loop: 10-15
meet: 13-14
```

# Iterator arithmetic

{{clear: slide}}

{{show: iter-code typewriter 1.5s linear}} C++ iterators support pointer-like arithmetic. The standard library gives you precise control over positioning.

{{focus: distance-fn}} {{zoom: 1.2x}} {{annotate: distance-fn "Gap between"}} std distance returns the number of hops between two iterators. For random-access iterators, it's constant time. For forward iterators, it walks.

{{zoom: 1x}} {{focus: advance-fn}} {{zoom: 1.3x}} {{annotate: advance-fn "Move by n"}} std advance moves an iterator by n positions. Modifies in place. Negative values work for bidirectional iterators.

{{zoom: 1x}} {{focus: next-prev}} {{zoom: 1.2x}} {{annotate: next-prev "Copy + move"}} std next and std prev return a new iterator without modifying the original. Useful when you need to peek ahead without losing your current position.

{{zoom: 1x}} {{focus: none}} These four — distance, advance, next, prev — are your iterator toolkit. They work with every iterator category.

```code:iter-code lang=cpp
#include <iterator>
#include <vector>

std::vector<int> v = {10, 20, 30, 40, 50};

auto a = v.begin();
auto b = v.begin() + 3;

auto gap = std::distance(a, b);         // 3

auto it = v.begin();
std::advance(it, 2);                    // it now points to 30

auto peek = std::next(v.begin(), 4);    // points to 50, begin unchanged
auto back = std::prev(v.end());         // points to 50 (last element)
---
distance-fn: 9
advance-fn: 11-12
next-prev: 14-15
```

# String two pointers

{{clear: slide}}

{{show: palindrome-code typewriter 1.5s linear}} Strings are arrays of characters. Two pointers work naturally. Classic: palindrome check.

{{focus: ends}} {{zoom: 1.2x}} {{annotate: ends "Outside in"}} Left at the front, right at the back. Compare characters. If they differ, it's not a palindrome.

{{zoom: 1x}} {{focus: skip}} {{zoom: 1.3x}} {{annotate: skip "Filter noise"}} Real-world twist: ignore non-alphanumeric characters and case. Advance pointers past junk, then compare lowercase versions.

{{zoom: 1x}} {{focus: converge}} {{zoom: 1.2x}} {{annotate: converge "Meet in middle"}} When the pointers cross, every pair matched. It's a palindrome.

{{zoom: 1x}} {{focus: none}} Reverse a string, compress spaces, remove duplicates — all two-pointer problems on character arrays.

```code:palindrome-code lang=cpp
#include <string>
#include <cctype>

bool is_palindrome(const std::string& s) {
    int left = 0;
    int right = static_cast<int>(s.size()) - 1;

    while (left < right) {
        while (left < right && !std::isalnum(s[left])) ++left;
        while (left < right && !std::isalnum(s[right])) --right;

        if (std::tolower(s[left]) != std::tolower(s[right])) {
            return false;
        }
        ++left;
        --right;
    }
    return true;
}
---
ends: 5-6
skip: 9-10
converge: 8, 15-16
```

# Prefix sums

{{clear: slide}}

{{show: prefix-code typewriter 2s linear}} Prefix sums aren't two pointers, but they pair naturally. Precompute cumulative sums, then answer range queries in O of 1.

{{focus: build}} {{zoom: 1.2x}} {{annotate: build "Accumulate"}} Build the prefix sum array. prefix at i equals the sum of elements 0 through i minus 1. prefix at 0 is zero — the empty prefix.

{{zoom: 1x}} {{focus: query}} {{zoom: 1.3x}} {{annotate: query "Subtract"}} Sum of elements from index l to r? prefix at r plus 1 minus prefix at l. One subtraction. Constant time. No loop.

{{zoom: 1x}} {{focus: subarray}} {{zoom: 1.2x}} {{annotate: subarray "Range sum"}} This is the foundation for subarray sum problems. "Does a subarray with sum k exist?" becomes "do two prefix sums differ by k?" — that's a hash map lookup.

{{zoom: 1x}} {{focus: none}} Prefix sums trade O of n build time for O of 1 range queries. Any problem involving subarray sums should trigger this reflex.

```code:prefix-code lang=cpp
#include <vector>

std::vector<int> build_prefix(const std::vector<int>& arr) {
    std::vector<int> prefix(arr.size() + 1, 0);
    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        prefix[i + 1] = prefix[i] + arr[i];
    }
    return prefix;
}

// Sum of arr[l..r] inclusive:
int range_sum(const std::vector<int>& prefix, int l, int r) {
    return prefix[r + 1] - prefix[l];
}

// Usage:
// arr    = {3, 1, 4, 1, 5}
// prefix = {0, 3, 4, 8, 9, 14}
// range_sum(prefix, 1, 3) = prefix[4] - prefix[1] = 9 - 3 = 6
---
build: 3-8
query: 12-14
subarray: 17-19
```

# Dutch national flag

{{clear: slide}}

{{show: dnf-code typewriter 2s linear}} Three-way partition. Sort an array of 0s, 1s, and 2s in a single pass using three pointers.

{{focus: pointers}} {{zoom: 1.2x}} {{annotate: pointers "Three regions"}} lo marks the boundary of zeros. mid scans. hi marks the boundary of twos. Everything before lo is 0. Everything after hi is 2. Between lo and hi — unsorted.

{{zoom: 1x}} {{focus: zero-case}} {{zoom: 1.3x}} {{annotate: zero-case "Swap to front"}} Mid sees a 0? Swap it to the lo position. Advance both lo and mid. The zero zone grows.

{{zoom: 1x}} {{focus: one-case}} {{zoom: 1.2x}} {{annotate: one-case "Leave it"}} Mid sees a 1? It's already in the right zone. Just advance mid.

{{zoom: 1x}} {{focus: two-case}} {{zoom: 1.3x}} {{annotate: two-case "Swap to back"}} Mid sees a 2? Swap it to the hi position. Decrement hi. Don't advance mid — the swapped-in element is unexamined.

{{zoom: 1x}} {{focus: none}} Single pass, constant space. The Dutch national flag is three-way partitioning — useful for quicksort variants and any problem with three categories.

```code:dnf-code lang=cpp
#include <vector>
#include <algorithm>

void dutch_flag(std::vector<int>& arr) {
    int lo = 0;
    int mid = 0;
    int hi = static_cast<int>(arr.size()) - 1;

    while (mid <= hi) {
        if (arr[mid] == 0) {
            std::swap(arr[lo], arr[mid]);
            ++lo;
            ++mid;
        } else if (arr[mid] == 1) {
            ++mid;
        } else {
            std::swap(arr[mid], arr[hi]);
            --hi;
        }
    }
}
---
pointers: 5-7
zero-case: 10-13
one-case: 14-15
two-case: 17-18
```
