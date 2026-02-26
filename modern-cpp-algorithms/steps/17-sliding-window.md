---
title: Sliding window
---

# Fixed-size window

{{show: fixed-window grow 0.5s spring}} A window of size k slides across the array. At each position, you know the sum (or max, or whatever) of exactly k elements.

{{focus: pos-0}} {{annotate: pos-0 "Window [0..2]"}} The window starts at the left. Sum the first k elements. That's your initial answer.

{{focus: pos-1}} {{annotate: pos-1 "Slide right"}} Slide one step right. Add the new element entering the window. Subtract the old element leaving. The sum updates in O of 1. No re-scanning.

{{focus: pos-2}} {{annotate: pos-2 "Keep sliding"}} Every step: one addition, one subtraction. The window slides across n elements in O of n time total. Not O of n times k.

{{focus: none}} Fixed window is the simplest sliding window. Perfect for "maximum sum of k consecutive elements" or "average of every k-length subarray."

{{clear: slide}}

{{show: fixed-code typewriter 1.5s linear}} Build the initial window, then slide.

{{focus: init-sum}} {{zoom: 1.2x}} {{annotate: init-sum "First window"}} Sum the first k elements. This is the seed.

{{zoom: 1x}} {{focus: slide}} {{zoom: 1.3x}} {{annotate: slide "Add and drop"}} For each new position, add the element entering on the right, subtract the element leaving on the left. Update the running answer.

{{zoom: 1x}} {{focus: track}} {{zoom: 1.2x}} {{annotate: track "Best so far"}} Track the best answer seen so far. Here it's the maximum sum, but it could be minimum, count, average — whatever the problem asks.

{{zoom: 1x}} {{focus: none}} Two lines in the loop body. That's the entire pattern.

```data:fixed-window type=array
["[{3|1|4}|1|5|9|2] sum=8", "[3|{1|4|1}|5|9|2] sum=6", "[3|1|{4|1|5}|9|2] sum=10"]
---
pos-0: 0
pos-1: 1
pos-2: 2
```

```code:fixed-code lang=cpp
#include <vector>
#include <algorithm>

int max_sum_k(const std::vector<int>& arr, int k) {
    int window_sum = 0;
    for (int i = 0; i < k; ++i) {
        window_sum += arr[i];
    }

    int best = window_sum;
    for (int i = k; i < static_cast<int>(arr.size()); ++i) {
        window_sum += arr[i] - arr[i - k];
        best = std::max(best, window_sum);
    }
    return best;
}
---
init-sum: 5-8
slide: 11-12
track: 13
```

# Variable-size window

{{clear: slide}}

{{show: variable-viz grow 0.5s spring}} The window grows and shrinks. The right pointer expands the window. The left pointer contracts it when a constraint is violated.

{{focus: expand}} {{annotate: expand "Grow right"}} Expand: move the right pointer right. Include the new element. The window grows.

{{focus: shrink}} {{annotate: shrink "Shrink left"}} Shrink: if the window violates the constraint, move the left pointer right until the constraint holds again.

{{focus: optimal}} {{annotate: optimal "Track best"}} At every valid window, update the answer. The longest, shortest, or best valid window is your result.

{{focus: none}} Both pointers only move right. Total work: at most 2n pointer moves. O of n.

{{clear: slide}}

{{show: variable-code typewriter 2s linear}} Longest subarray with sum at most target. Classic variable window.

{{focus: right}} {{zoom: 1.2x}} {{annotate: right "Expand"}} The right pointer iterates through every element. Each element enters the window exactly once.

{{zoom: 1x}} {{focus: shrink-loop}} {{zoom: 1.3x}} {{annotate: shrink-loop "Contract"}} While the window sum exceeds the target, subtract the left element and advance left. The inner while loop contracts the window.

{{zoom: 1x}} {{focus: answer}} {{zoom: 1.2x}} {{annotate: answer "Best valid"}} After shrinking, the window is valid. Check if it's the longest seen so far.

{{zoom: 1x}} {{focus: none}} Right expands, left contracts. Both move in one direction. That's the variable window template.

```data:variable-viz type=array
["[{3|1|4}|1|5] sum=8 ≤10 ✓", "[{3|1|4|1}|5] sum=9 ≤10 ✓", "[3|{1|4|1|5}] sum=11 >10 shrink!", "[3|1|{4|1|5}] sum=10 ≤10 ✓"]
---
expand: 0, 1
shrink: 2
optimal: 3
```

```code:variable-code lang=cpp
#include <vector>
#include <algorithm>

int longest_subarray_sum(const std::vector<int>& arr, int target) {
    int left = 0;
    int current_sum = 0;
    int best_len = 0;

    for (int right = 0; right < static_cast<int>(arr.size()); ++right) {
        current_sum += arr[right];

        while (current_sum > target) {
            current_sum -= arr[left];
            ++left;
        }

        best_len = std::max(best_len, right - left + 1);
    }
    return best_len;
}
---
right: 9-10
shrink-loop: 12-15
answer: 17
```

# Window with counter

{{clear: slide}}

{{show: counter-code typewriter 2s linear}} When the window tracks character frequencies or element counts, add a hash map. Classic: longest substring with at most k distinct characters.

{{focus: map}} {{zoom: 1.2x}} {{annotate: map "Frequency table"}} An unordered map tracks how many times each character appears in the current window. The map's size tells you how many distinct characters you have.

{{zoom: 1x}} {{focus: add}} {{zoom: 1.3x}} {{annotate: add "Enter window"}} When the right pointer advances, increment the count for the new character.

{{zoom: 1x}} {{focus: remove}} {{zoom: 1.2x}} {{annotate: remove "Leave window"}} When the left pointer advances, decrement the count. If the count hits zero, erase the key. The map stays clean.

{{zoom: 1x}} {{focus: check}} {{zoom: 1.2x}} {{annotate: check "Constraint"}} The constraint: map size must not exceed k. Shrink until it doesn't.

{{zoom: 1x}} {{focus: none}} Map-backed windows handle any "at most k distinct" or "all characters present" problem. The map is the bookkeeping layer.

```code:counter-code lang=cpp
#include <string>
#include <unordered_map>
#include <algorithm>

int longest_k_distinct(const std::string& s, int k) {
    std::unordered_map<char, int> freq;
    int left = 0;
    int best = 0;

    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        ++freq[s[right]];

        while (static_cast<int>(freq.size()) > k) {
            --freq[s[left]];
            if (freq[s[left]] == 0) {
                freq.erase(s[left]);
            }
            ++left;
        }

        best = std::max(best, right - left + 1);
    }
    return best;
}
---
map: 6
add: 11
remove: 14-17
check: 13
```

# Minimum window substring

{{clear: slide}}

{{show: minwin-code typewriter 2s linear}} The hardest standard sliding window problem. Find the shortest substring of s that contains all characters of t. Two counters: one for what you need, one for what you have.

{{focus: need}} {{zoom: 1.2x}} {{annotate: need "Requirements"}} Build a frequency map of t. This is what the window must contain.

{{zoom: 1x}} {{focus: expand}} {{zoom: 1.3x}} {{annotate: expand "Collect characters"}} Expand right. When a character's count in the window meets the requirement, increment a "formed" counter. When formed equals the number of required distinct characters, the window is valid.

{{zoom: 1x}} {{focus: contract}} {{zoom: 1.2x}} {{annotate: contract "Shrink to min"}} Once valid, shrink from the left. Track the shortest valid window seen. Keep shrinking until the window is no longer valid.

{{zoom: 1x}} {{focus: result}} {{zoom: 1.2x}} {{annotate: result "Best window"}} The answer is the shortest valid window found during the entire scan.

{{zoom: 1x}} {{focus: none}} This combines variable window with counter tracking. It's the template pushed to its limit.

```code:minwin-code lang=cpp
#include <string>
#include <unordered_map>
#include <climits>

std::string min_window(const std::string& s, const std::string& t) {
    std::unordered_map<char, int> need;
    for (char c : t) ++need[c];
    int required = static_cast<int>(need.size());

    std::unordered_map<char, int> have;
    int formed = 0;
    int left = 0;
    int best_len = INT_MAX;
    int best_start = 0;

    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        ++have[s[right]];
        if (need.count(s[right]) && have[s[right]] == need[s[right]]) {
            ++formed;
        }

        while (formed == required) {
            int window_len = right - left + 1;
            if (window_len < best_len) {
                best_len = window_len;
                best_start = left;
            }
            --have[s[left]];
            if (need.count(s[left]) && have[s[left]] < need[s[left]]) {
                --formed;
            }
            ++left;
        }
    }

    if (best_len == INT_MAX) return "";
    return s.substr(best_start, best_len);
}
---
need: 6-8
expand: 16-20
contract: 22-33
result: 36-37
```

# The sliding window template

{{clear: slide}}

{{show: template-code typewriter 1.5s linear}} Every sliding window problem follows one skeleton. Memorize this shape.

{{focus: outer}} {{zoom: 1.2x}} {{annotate: outer "Right expands"}} The outer for loop drives the right pointer. Every element enters the window exactly once.

{{zoom: 1x}} {{focus: update}} {{zoom: 1.3x}} {{annotate: update "Window state"}} After adding the right element, update the window's state. Sum, count, frequency map — whatever the problem tracks.

{{zoom: 1x}} {{focus: inner}} {{zoom: 1.2x}} {{annotate: inner "Left contracts"}} The inner while loop shrinks from the left when the window constraint is violated. The left pointer catches up.

{{zoom: 1x}} {{focus: record}} {{annotate: record "Capture answer"}} Record the answer at the right moment. For longest: record after shrinking (window is valid). For shortest: record during shrinking (window just became valid).

{{zoom: 1x}} {{focus: none}} Right expands, update state, left contracts, record answer. Four steps. Every sliding window problem maps onto this skeleton.

```code:template-code lang=cpp
// Sliding window template (pseudocode)
int sliding_window(const std::vector<int>& arr) {
    int left = 0;
    int answer = 0;
    // window state variables here

    for (int right = 0; right < static_cast<int>(arr.size()); ++right) {
        // Add arr[right] to window state

        while (/* window constraint violated */) {
            // Remove arr[left] from window state
            ++left;
        }

        // Update answer (longest: here; shortest: inside while)
    }
    return answer;
}
---
outer: 7
update: 8
inner: 10-13
record: 15
```

# When to use sliding window

{{clear: slide}}

{{show: signals grow 0.5s spring}} Sliding window signals. If two or more match, it's almost certainly the right pattern.

{{focus: contiguous}} {{annotate: contiguous "Subarray/substring"}} The problem asks about contiguous subarrays or substrings. Not subsequences — those aren't windows.

{{focus: optimal}} {{annotate: optimal "Longest/shortest"}} It asks for the longest, shortest, maximum, or minimum of a contiguous range.

{{focus: constraint}} {{annotate: constraint "Bounded"}} There's a constraint on the window: sum at most k, at most k distinct elements, must contain all characters of t.

{{focus: linear}} {{annotate: linear "O(n) hint"}} The expected time complexity is O of n or O of n log n. If the brute force is O of n squared, sliding window is the likely optimization.

{{focus: none}} Contiguous range, optimization, constraint, linear target. Those are your signals. When they line up, reach for the sliding window template.

```data:signals type=array
["Contiguous subarray or substring", "Longest / shortest / max / min", "Window constraint (sum, count, freq)", "O(n) expected complexity"]
---
contiguous: 0
optimal: 1
constraint: 2
linear: 3
```
