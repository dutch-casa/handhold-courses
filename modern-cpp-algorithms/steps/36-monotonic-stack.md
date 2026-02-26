---
title: Monotonic Stack
---

# The monotonic stack pattern

{{show: mono-intro typewriter 2s linear}} A monotonic stack maintains elements in sorted order — either always increasing or always decreasing from bottom to top. When a new element violates that order, you pop until it doesn't. The pops are where the work happens.

{{show: mono-stack grow 0.5s spring}} {{focus: invariant}} {{annotate: invariant "Stack invariant"}} The invariant: every element on the stack is in order. Monotone increasing means each element is greater than the one below it. You enforce this by popping smaller elements before pushing.

{{focus: amortized}} {{annotate: amortized "O(n) amortized"}} Each element is pushed once and popped at most once. Total pushes + pops = 2n. O(n) amortized regardless of how many pops happen per step.

{{focus: two-flavors}} {{annotate: two-flavors "Increasing vs decreasing"}} Increasing stack: pop when current > top → good for finding next greater element. Decreasing stack: pop when current < top → good for next smaller. Pick based on what you're computing when you pop.

{{focus: none}} The algorithm skeleton is always the same: iterate, maintain the invariant by popping, do work on pop, push. Everything else is details.

```code:mono-intro lang=cpp
// Monotone increasing stack skeleton
// "While top of stack is >= current element, pop"
template<typename T>
void process_increasing(const std::vector<T>& nums) {
    std::stack<int> stk;  // stores indices, not values

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        // Maintain increasing order: pop elements smaller than nums[i]
        while (!stk.empty() && nums[stk.top()] < nums[i]) {
            int idx = stk.top(); stk.pop();
            // nums[idx]'s "next greater element" is nums[i]
            // — do work here
        }
        stk.push(i);
    }
    // Anything left on stack has no next greater element
}
---
invariant: 9-12
amortized: 8-13
two-flavors: 3-14
```

```data:mono-stack type=stack
[4, 2, 6]
---
region: 0
```

# Next greater element

{{clear: slide}}

{{show: nge-code typewriter 2s linear}} For each element in the array, find the first element to its right that is strictly greater. Classic monotone increasing stack — we pop when we find a larger element, and the larger element is the answer for the popped index.

{{show: mono-stack grow 0.5s spring}} {{focus: pop-is-answer}} {{zoom: 1.2x}} {{annotate: pop-is-answer "Pop = answer found"}} When `nums[i] > nums[stk.top()]`, the element being popped now has its answer: it's `nums[i]`. Store it in the result array at the popped index.

{{zoom: 1x}} {{focus: remainder}} {{zoom: 1.2x}} {{annotate: remainder "Stack remainder = -1"}} Elements still on the stack after the loop have no next greater element to their right. Set their result to -1.

{{zoom: 1x}} {{focus: circular}} {{zoom: 1.2x}} {{annotate: circular "Circular variant"}} For a circular array, run the loop twice (or use `i % n`). The second pass lets earlier elements see later ones as their "next greater."

{{zoom: 1x}} {{focus: none}} The result array and the stack are maintained in parallel. The stack holds unsettled indices; the pop settles them.

```code:nge-code lang=cpp
// For each element, find the next strictly greater element to its right.
// Returns -1 if no such element exists.
std::vector<int> next_greater(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    std::vector<int> result(n, -1);
    std::stack<int> stk;  // indices of unsettled elements

    for (int i = 0; i < n; ++i) {
        // Pop all elements smaller than nums[i]; nums[i] is their answer
        while (!stk.empty() && nums[stk.top()] < nums[i]) {
            result[stk.top()] = nums[i];
            stk.pop();
        }
        stk.push(i);
    }
    // Anything left: no next greater, result already -1
    return result;
}

// Circular variant: run two passes over the array
std::vector<int> next_greater_circular(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    std::vector<int> result(n, -1);
    std::stack<int> stk;

    for (int i = 0; i < 2 * n; ++i) {
        while (!stk.empty() && nums[stk.top()] < nums[i % n]) {
            result[stk.top()] = nums[i % n];
            stk.pop();
        }
        if (i < n) stk.push(i);
    }
    return result;
}
---
pop-is-answer: 10-12
remainder: 13-14
circular: 18-29
```

# Daily temperatures

{{clear: slide}}

{{show: temps-code typewriter 2s linear}} Daily temperatures: given an array of temperatures, for each day return how many days until a warmer temperature. Same pattern as next greater — but we store the gap, not the value.

{{focus: gap-not-value}} {{zoom: 1.2x}} {{annotate: gap-not-value "Distance, not the element"}} When we pop index `j` because `temps[i] > temps[j]`, the answer for day `j` is `i - j` — the distance between them, not the temperature itself. One-line change from the NGE template.

{{zoom: 1x}} {{focus: result-calc}} {{zoom: 1.2x}} {{annotate: result-calc "i - stk.top()"}} `result[stk.top()] = i - stk.top()`. Current index minus the stored index gives the number of days to wait.

{{zoom: 1x}} {{focus: none}} This problem is pure pattern recognition. See "days until" or "how long until" in a problem? Think monotonic stack. The distance formula is always current_index - stored_index.

```code:temps-code lang=cpp
// For each day, how many days until a warmer temperature?
// Returns 0 if no warmer day exists to the right.
std::vector<int> daily_temperatures(const std::vector<int>& temps) {
    const int n = static_cast<int>(temps.size());
    std::vector<int> result(n, 0);
    std::stack<int> stk;  // indices of days awaiting a warmer day

    for (int i = 0; i < n; ++i) {
        while (!stk.empty() && temps[stk.top()] < temps[i]) {
            const int j = stk.top(); stk.pop();
            result[j] = i - j;  // days until warmer
        }
        stk.push(i);
    }
    // Remaining indices keep result[i] = 0 (no warmer day)
    return result;
}
---
gap-not-value: 9-11
result-calc: 11
```

# Largest rectangle in histogram

{{clear: slide}}

{{show: hist-code typewriter 2s linear}} Largest rectangle in histogram: for each bar, find the widest rectangle that uses that bar as its shortest bar. This requires knowing the nearest shorter bar on each side — monotone increasing stack.

{{show: histogram grow 0.5s spring}} {{focus: extend-left}} {{zoom: 1.2x}} {{annotate: extend-left "Nearest smaller left"}} When we pop index `j` because `heights[i] < heights[j]`, `i` is the first bar to j's right that is shorter. The stack element below `j` is the first bar to j's left that is shorter. Between them is the full width for height `heights[j]`.

{{zoom: 1x}} {{focus: width-calc}} {{zoom: 1.2x}} {{annotate: width-calc "Width formula"}} Width = `i - stk.top() - 1`. If the stack is empty after popping, the left boundary is index -1, so width = `i`. Area = `heights[j] * width`.

{{zoom: 1x}} {{focus: sentinel}} {{zoom: 1.2x}} {{annotate: sentinel "Sentinel flush"}} Appending a 0-height bar at the end forces all remaining stack elements to flush. Cleaner than a separate post-loop.

{{zoom: 1x}} {{focus: none}} This is the hardest of the monotonic stack problems. The width formula is the tricky part — derive it from "nearest smaller on each side" and it falls into place.

```code:hist-code lang=cpp
// Returns the area of the largest rectangle in the histogram.
int largest_rect_histogram(std::vector<int> heights) {
    heights.push_back(0);  // sentinel: flushes all remaining bars
    const int n = static_cast<int>(heights.size());
    std::stack<int> stk;   // indices, increasing heights
    int max_area = 0;

    for (int i = 0; i < n; ++i) {
        // Maintain increasing: pop bars that are taller than heights[i]
        while (!stk.empty() && heights[stk.top()] > heights[i]) {
            const int h = heights[stk.top()]; stk.pop();
            // Width: from the new top of stack (exclusive) to i (exclusive)
            const int w = stk.empty() ? i : i - stk.top() - 1;
            max_area = std::max(max_area, h * w);
        }
        stk.push(i);
    }
    return max_area;
}
---
extend-left: 9-12
width-calc: 12-13
sentinel: 2
```

```data:histogram type=array
[2, 1, 5, 6, 2, 3]
---
region: 2
```

# Trapping rain water

{{clear: slide}}

{{show: rain-code typewriter 2s linear}} Trapping rain water: given an elevation map, compute how much water it holds after rain. Two approaches — prefix max arrays (clean) or monotone stack (teaches the pattern). We'll show both.

{{focus: prefix-max-approach}} {{zoom: 1.2x}} {{annotate: prefix-max-approach "Prefix max arrays"}} For each position, water level = min(max_left[i], max_right[i]). Water trapped = that level minus the height. Two passes to build max arrays, one pass to sum. O(n) time, O(n) space.

{{zoom: 1x}} {{focus: stack-approach}} {{zoom: 1.3x}} {{annotate: stack-approach "Stack: horizontal slabs"}} The stack approach computes water in horizontal slabs. When a taller bar is found, the popped bar forms a trough. Width is determined by the current bar and the new stack top. Height is `min(left_wall, right_wall) - trough_floor`.

{{zoom: 1x}} {{focus: none}} Both are O(n) time. The prefix max approach is clearer. The stack approach is O(1) extra space and worth knowing as a variant of the histogram pattern.

```code:rain-code lang=cpp
// Approach 1: prefix max arrays — cleaner
int trap_prefix(const std::vector<int>& h) {
    const int n = static_cast<int>(h.size());
    std::vector<int> lmax(n), rmax(n);

    lmax[0] = h[0];
    for (int i = 1; i < n; ++i) lmax[i] = std::max(lmax[i-1], h[i]);

    rmax[n-1] = h[n-1];
    for (int i = n-2; i >= 0; --i) rmax[i] = std::max(rmax[i+1], h[i]);

    int water = 0;
    for (int i = 0; i < n; ++i)
        water += std::min(lmax[i], rmax[i]) - h[i];
    return water;
}

// Approach 2: monotone stack — horizontal slabs
int trap_stack(const std::vector<int>& h) {
    std::stack<int> stk;
    int water = 0;
    for (int i = 0; i < static_cast<int>(h.size()); ++i) {
        while (!stk.empty() && h[stk.top()] < h[i]) {
            const int floor_idx = stk.top(); stk.pop();
            if (stk.empty()) break;
            const int width    = i - stk.top() - 1;
            const int height   = std::min(h[stk.top()], h[i]) - h[floor_idx];
            water += width * height;
        }
        stk.push(i);
    }
    return water;
}
---
prefix-max-approach: 2-15
stack-approach: 18-30
```

# Recognizing the pattern

{{clear: slide}}

{{show: recognition typewriter 2s linear}} Monotonic stack problems have a tell: they ask about the nearest greater or smaller element, or about areas/volumes defined by boundary elements. Once you see the tell, the code is almost mechanical.

{{show: tell-data grow 0.5s spring}} {{focus: next-greater-tell}} {{annotate: next-greater-tell "Next greater/smaller"}} "For each element, find the next larger/smaller one" → monotone increasing/decreasing stack respectively. The pop event is the answer event.

{{focus: area-volume-tell}} {{annotate: area-volume-tell "Area/volume with boundaries"}} "Largest rectangle / water trapped / largest container" → the pop determines a boundary. Width comes from the index gap, height from the popped element. Area = width × height.

{{focus: span-tell}} {{annotate: span-tell "Days/distance until condition"}} "How many days until warmer / stock span" → same as next greater, but you want `i - stored_index`, not `nums[i]`.

{{focus: none}} If the naive solution is O(n²) because you scan left or right from each element, a monotone stack probably drops it to O(n). That's the clue.

```data:tell-data type=array
["Next greater element → monotone increasing stack", "Next smaller element → monotone decreasing stack", "Days/distance until condition → store index, answer = i - j", "Largest rectangle → increasing stack, width formula on pop", "Trapping rain water → prefix max (clean) or stack (O(1) space)"]
---
next-greater-tell: 0
area-volume-tell: 3
span-tell: 2
```
