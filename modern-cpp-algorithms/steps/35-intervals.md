---
title: Intervals
---

# Interval representation

{{show: interval-struct typewriter 2s linear}} An interval is just two numbers: a start and an end. You can use `std::pair<int,int>` or a named struct. The struct wins on readability when you're passing intervals around.

{{focus: pair-form}} {{annotate: pair-form "std::pair shorthand"}} `pair<int,int>` works for quick solutions. `p.first` and `p.second` are legal but noisy. Fine for a coding interview, not great for production code.

{{focus: struct-form}} {{zoom: 1.2x}} {{annotate: struct-form "Named struct"}} The struct spells out intent. `iv.start`, `iv.end` read naturally. Add `bool overlaps(Interval o)` if you're computing this repeatedly.

{{zoom: 1x}} {{focus: sort-key}} {{annotate: sort-key "Sort by start"}} Nearly every interval algorithm sorts by start time first. That's why start comes first in the struct. `std::ranges::sort(ivs, {}, &Interval::start)` — a projection, not a comparator lambda.

{{focus: none}} Get the representation right upfront. Sorting by start collapses most interval problems into a linear scan.

```code:interval-struct lang=cpp
#include <algorithm>
#include <vector>

// Option 1: pair — compact but opaque
using Iv = std::pair<int, int>;  // {start, end}

// Option 2: named struct — preferred
struct Interval {
    int start;
    int end;

    bool overlaps(const Interval& o) const noexcept {
        return start <= o.end && o.start <= end;
    }
};

// Sort a list of intervals by start time
void sort_by_start(std::vector<Interval>& ivs) {
    std::ranges::sort(ivs, {}, &Interval::start);
}
---
pair-form: 4-5
struct-form: 8-14
sort-key: 17-19
```

```data:intervals type=array
[{"label": "[1,4]", "start": 1, "end": 4}, {"label": "[2,6]", "start": 2, "end": 6}, {"label": "[8,10]", "start": 8, "end": 10}, {"label": "[9,11]", "start": 9, "end": 11}]
---
region: 0
```

# Merge overlapping intervals

{{clear: slide}}

{{show: merge-code typewriter 2s linear}} Merging intervals: sort by start, then do one linear scan. If the current interval overlaps the last merged one, extend it. Otherwise push a new one.

{{show: intervals grow 0.5s spring}} {{focus: sort-step}} {{annotate: sort-step "Sort first"}} The sort is mandatory. Without it you'd need O(n²) comparisons. After sorting by start, any overlap can only happen with the previous interval in your result list.

{{focus: overlap-check}} {{zoom: 1.2x}} {{annotate: overlap-check "back().end >= start"}} The overlap condition after sorting: `result.back().end >= iv.start`. If the last merged interval's end reaches into the current interval's start, they overlap.

{{zoom: 1x}} {{focus: extend}} {{zoom: 1.2x}} {{annotate: extend "Extend or push"}} Overlap: extend by taking `max(back().end, iv.end)` — the current interval might be fully contained. No overlap: push a fresh interval. That's the entire algorithm.

{{zoom: 1x}} {{focus: none}} O(n log n) for the sort, O(n) for the scan. The scan is the textbook pattern for any merge-type interval problem.

```code:merge-code lang=cpp
// Merge all overlapping intervals.
// Input need not be sorted — we sort it here.
std::vector<Interval> merge(std::vector<Interval> ivs) {
    if (ivs.size() <= 1) return ivs;

    std::ranges::sort(ivs, {}, &Interval::start);

    std::vector<Interval> result;
    result.push_back(ivs[0]);

    for (int i = 1; i < static_cast<int>(ivs.size()); ++i) {
        Interval& back = result.back();
        const Interval& iv = ivs[i];

        if (back.end >= iv.start) {
            back.end = std::max(back.end, iv.end);  // extend
        } else {
            result.push_back(iv);                   // no overlap, push
        }
    }
    return result;
}
---
sort-step: 6
overlap-check: 14
extend: 15-16
```

```data:intervals type=array
[{"label": "[1,4]", "start": 1, "end": 4}, {"label": "[2,6]", "start": 2, "end": 6}, {"label": "[8,10]", "start": 8, "end": 10}, {"label": "[9,11]", "start": 9, "end": 11}]
---
region: 0
```

# Insert interval

{{clear: slide}}

{{show: insert-code typewriter 2s linear}} Insert a new interval into a sorted, non-overlapping list. Three phases: collect intervals that end before the new one starts, merge all overlapping ones, then append the rest.

{{focus: phase-before}} {{zoom: 1.2x}} {{annotate: phase-before "Before the gap"}} While `ivs[i].end < new_iv.start`, push directly. These intervals end before the new one begins — no overlap possible.

{{zoom: 1x}} {{focus: phase-merge}} {{zoom: 1.2x}} {{annotate: phase-merge "Absorb overlaps"}} While `ivs[i].start <= new_iv.end`, the current interval overlaps the new one. Expand new_iv to contain both. Keep expanding until no more overlaps.

{{zoom: 1x}} {{focus: phase-after}} {{zoom: 1.2x}} {{annotate: phase-after "Tail copy"}} Push the merged interval, then copy all remaining intervals as-is. They all start after the merged region ends.

{{zoom: 1x}} {{focus: none}} Three phases, one pass, O(n). The key insight: after sorting, all overlapping intervals form a contiguous block.

```code:insert-code lang=cpp
std::vector<Interval> insert(std::vector<Interval> ivs, Interval nw) {
    std::vector<Interval> result;
    int i = 0;
    const int n = static_cast<int>(ivs.size());

    // Phase 1: intervals that end before nw starts
    while (i < n && ivs[i].end < nw.start) {
        result.push_back(ivs[i++]);
    }

    // Phase 2: merge all overlapping intervals into nw
    while (i < n && ivs[i].start <= nw.end) {
        nw.start = std::min(nw.start, ivs[i].start);
        nw.end   = std::max(nw.end,   ivs[i].end);
        ++i;
    }
    result.push_back(nw);

    // Phase 3: intervals that start after nw ends
    while (i < n) result.push_back(ivs[i++]);

    return result;
}
---
phase-before: 7-9
phase-merge: 12-16
phase-after: 19-20
```

# Interval scheduling

{{clear: slide}}

{{show: schedule-code typewriter 2s linear}} Interval scheduling maximization: select the maximum number of non-overlapping intervals. The greedy solution is optimal: always pick the interval with the earliest end time.

{{show: activity-data grow 0.5s spring}} {{focus: sort-end}} {{annotate: sort-end "Sort by end time"}} Sort by end time, not start. The interval that ends earliest leaves the most room for future intervals. This is the greedy choice property in action.

{{focus: greedy-choice}} {{zoom: 1.2x}} {{annotate: greedy-choice "Compatible check"}} An interval is compatible if its start is strictly after the last selected interval's end. Compatible? Select it and advance the boundary. Otherwise skip.

{{zoom: 1x}} {{focus: none}} The exchange argument proves this: swapping any selected interval for a later-ending one can only reduce or maintain the count, never improve it.

```code:schedule-code lang=cpp
// Returns the maximum number of non-overlapping intervals.
int schedule(std::vector<Interval> ivs) {
    std::ranges::sort(ivs, {}, &Interval::end);

    int count = 0;
    int last_end = std::numeric_limits<int>::min();

    for (const Interval& iv : ivs) {
        if (iv.start >= last_end) {  // compatible
            ++count;
            last_end = iv.end;
        }
    }
    return count;
}
---
sort-end: 3
greedy-choice: 8-11
```

```data:activity-data type=array
[{"label": "[1,3]"}, {"label": "[2,5]"}, {"label": "[3,6]"}, {"label": "[5,7]"}, {"label": "[6,8]"}]
---
region: 0
```

# Meeting rooms — sweep line

{{clear: slide}}

{{show: sweep-code typewriter 2s linear}} Meeting rooms II: given meeting intervals, find the minimum number of conference rooms required. Sweep line on events — treat each start and end as a timestamped event.

{{focus: event-model}} {{zoom: 1.2x}} {{annotate: event-model "Events not intervals"}} Convert each interval into two events: `{start, +1}` and `{end, -1}`. Sort all events by time. Ties: process ends before starts (a room freed at time t is available for a meeting starting at t).

{{zoom: 1x}} {{focus: sweep}} {{zoom: 1.2x}} {{annotate: sweep "Running maximum"}} Sweep through events, maintaining a running count of active meetings. The answer is the maximum value that count ever reaches.

{{zoom: 1x}} {{focus: tie-break}} {{annotate: tie-break "End before start on ties"}} Sort key: `{time, type}` where end=-1 < start=+1. This ensures that if a meeting ends and another starts at the same minute, the freed room is counted before the new meeting claims one.

{{focus: none}} Sweep line decouples the structure from the query. You stop thinking in intervals and start thinking in events at a timeline.

```code:sweep-code lang=cpp
int min_meeting_rooms(const std::vector<Interval>& ivs) {
    // Each event: {time, delta}. End events sort before start events on ties.
    std::vector<std::pair<int, int>> events;
    events.reserve(ivs.size() * 2);

    for (const Interval& iv : ivs) {
        events.push_back({iv.start, +1});  // meeting starts
        events.push_back({iv.end,   -1});  // meeting ends
    }

    // Sort by time; ends (-1) before starts (+1) on ties
    std::ranges::sort(events);

    int active = 0, peak = 0;
    for (auto [time, delta] : events) {
        active += delta;
        peak = std::max(peak, active);
    }
    return peak;
}
---
event-model: 3-9
sweep: 12-16
tie-break: 11
```

# Pattern recognition

{{clear: slide}}

{{show: pattern-summary typewriter 2s linear}} Interval problems share a small vocabulary of operations. Match the problem shape to the algorithm and you're 90% done.

{{show: patterns-data grow 0.5s spring}} {{focus: sort-signal}} {{annotate: sort-signal "Always sort first"}} If the problem involves multiple intervals and asks about overlaps, merges, or coverage — sort by start time first. Almost without exception.

{{focus: sweep-signal}} {{annotate: sweep-signal "Count → sweep line"}} If the question is "how many X overlap at any point?" — that's a sweep line. Turn intervals into events, sort, scan, track a running count.

{{focus: greedy-signal}} {{annotate: greedy-signal "Maximize count → greedy by end"}} If you want the maximum number of non-overlapping intervals, sort by end time and greedily select compatible ones. Provably optimal.

{{focus: none}} The trap is reaching for DP when greedy suffices. Intervals with the earliest-end greedy always beats DP here — the exchange argument closes it.

```data:patterns-data type=array
["Merge intervals → sort by start, linear scan", "Insert interval → 3-phase scan (before, merge, after)", "Schedule max intervals → sort by end, greedy pick", "Min rooms / max overlap → sweep line on events", "Coverage / gap detection → sort by start, track coverage boundary"]
---
sort-signal: 0
sweep-signal: 3
greedy-signal: 2
```
