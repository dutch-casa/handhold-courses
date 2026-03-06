---
title: Profiling
---

# Why profile

{{clear: slide}}

{{show: amdahl-table typewriter 2s linear}} Before optimizing, you must measure.

{{focus: blind-row}} {{annotate: blind-row "Blind optimization cost"}} Optimizing without profiling is guessing. You will spend a week making the fast path 10 percent faster when 90 percent of time is spent in the slow path. {{focus: amdahl-row}} {{annotate: amdahl-row "Amdahl's law"}} Amdahl's law states that the maximum speedup is bounded by the fraction of work that can be parallelized or accelerated. {{annotate: amdahl-row "The bottleneck first"}} If 80 percent of time is in function A, then no amount of work on function B will yield more than a 20 percent total improvement. {{focus: profile-first-row}} {{annotate: profile-first-row "Measurement first"}} Find the bottleneck first. Only then write a single line of optimization.

{{focus: flamegraph-row}} {{annotate: flamegraph-row "Flamegraph overview"}} A flamegraph is the fastest way to find where time goes. {{annotate: flamegraph-row "One screen"}} In a single glance you can see which functions own the largest fraction of CPU time and how deep the call stacks go. {{focus: profile-first-row}} The two-step discipline is iron: profile first, then optimize the widest frame.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```data:amdahl-table type=array
["blind optimization  │  wastes engineering time on non-bottlenecks", "Amdahl's law        │  speedup = 1 / (1 - p + p/n) where p = parallel fraction", "profile first       │  find the actual bottleneck before writing a single optimization", "flamegraph          │  fastest tool for identifying which functions own the most CPU"]
---
blind-row: 0
amdahl-row: 1
profile-first-row: 2
flamegraph-row: 3
```

# The profiling tools

{{clear: slide}}

{{show: tools-table typewriter 2s linear}} Different platforms have different profiling tools, but they all answer the same question: where does time go?

{{focus: perf-row}} {{annotate: perf-row "Linux perf"}} On Linux, `perf stat` counts hardware performance counters — cache misses, branch mispredictions, instructions per cycle. {{focus: perf-record-row}} {{annotate: perf-record-row "Low overhead"}} `perf record` samples the call stack at a fixed rate. {{annotate: perf-record-row "perf report view"}} `perf report` renders an interactive call-graph view. The overhead is typically below 1 percent.

{{focus: valgrind-row}} {{annotate: valgrind-row "Callgrind tool"}} Valgrind's Callgrind tool instruments every instruction and produces a precise call-count and instruction-count profile. {{annotate: valgrind-row "Exact but slow"}} The process runs 10 to 50 times slower under Callgrind. Use it when you need precision, not speed. {{focus: instruments-row}} {{annotate: instruments-row "macOS Instruments"}} On macOS, Instruments provides a graphical flamegraph and memory profiler. Attach to any process without recompilation. {{focus: gprof-row}} {{annotate: gprof-row "gprof tool"}} `gprof` requires recompiling with `-pg` and is the oldest tool. It has flat-profile and call-graph modes. {{annotate: gprof-row "Less accurate"}} Sampling frequency limitations make it less accurate than `perf`. It is portable across platforms that support it.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```data:tools-table type=array
["perf stat     │  Linux  │  hardware counters: IPC, cache misses, branch mispredictions", "perf record   │  Linux  │  call-stack sampling at ~1 kHz, <1% overhead", "callgrind     │  Linux/macOS  │  exact instruction counts, 10-50x slowdown", "Instruments   │  macOS  │  graphical flamegraph, memory, no recompile needed", "gprof         │  portable  │  requires -pg flag, flat profile + call graph"]
---
perf-row: 0
perf-record-row: 1
valgrind-row: 2
instruments-row: 3
gprof-row: 4
```

# Reading a flamegraph

{{clear: slide}}

{{show: flamegraph-diagram typewriter 2s linear}} A flamegraph has two axes and one key rule.

{{focus: x-axis-node}} {{annotate: x-axis-node "Alphabetical, not time"}} The x-axis is sorted alphabetically by function name within each stack level. It is not a timeline. Left-to-right position tells you nothing about when a function ran. {{focus: y-axis-node}} {{annotate: y-axis-node "Call depth"}} The y-axis is call depth: the bottom row is the entry point and each row above it is a callee. A tall stack means deep nesting. {{focus: width-node}} {{annotate: width-node "Width = time spent"}} Width represents the fraction of total CPU time spent in that function and everything it calls. A wide frame is the bottleneck. {{focus: leaf-node}} {{annotate: leaf-node "Flat top = leaf work"}} A frame that reaches the top of the graph without being called by anything else is a leaf function. That is where the CPU actually executes instructions. {{annotate: leaf-node "Optimization target"}} Optimize the widest leaf first.

{{focus: plateau-node}} {{annotate: plateau-node "Plateau pattern"}} A wide frame with a narrow top means most time is in the function itself, not in its callees. That is the hot function. {{focus: tower-node}} {{annotate: tower-node "Tower pattern"}} A narrow frame that fans into many callees means the function is a dispatcher. Narrow towers are rarely the bottleneck.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```diagram:flamegraph-diagram
[entry-point] -> [execute_select_where]
[execute_select_where] -> [execute_select]
[execute_select_where] -> [eval_predicate]
[execute_select] -> [strnlen]
[execute_select] -> [string-alloc]
[string-alloc] -> [malloc]
---
x-axis-node: entry-point
y-axis-node: execute_select_where
width-node: execute_select
leaf-node: string-alloc
plateau-node: strnlen
tower-node: eval_predicate
```

# Hotspots in our database

{{clear: slide}}

{{show: scan-loop typewriter 2s linear}} The scan loop is where the database spends most of its time.

{{focus: strnlen-call}} {{zoom: 1.2x}} {{annotate: strnlen-call "strnlen per row"}} Every row calls `strnlen(row.name, kNameMax)`. {{annotate: strnlen-call "Up to 32 bytes scanned"}} For a name that fills all 32 bytes, this walks the entire buffer looking for a null terminator that is not there. {{annotate: strnlen-call "O(n) scan cost"}} With one million rows, that is one million `strnlen` calls.

{{zoom: 1x}} {{focus: string-alloc}} {{zoom: 1.2x}} {{annotate: string-alloc "Heap allocation"}} `std::string(row.name, len)` allocates on the heap for names longer than the small-string optimization threshold — typically 15 to 22 characters depending on the standard library implementation. {{annotate: string-alloc "Allocation per row"}} One heap allocation per non-null row means one `malloc` and one `free` per row in the result. {{annotate: string-alloc "Primary bottleneck"}} Profiling consistently shows string allocation as the widest leaf in the flamegraph.

{{zoom: 1x}} {{focus: push-back}} {{zoom: 1.2x}} {{annotate: push-back "Struct copy"}} `push_back` with `std::move` avoids copying the string but still moves the struct. {{zoom: 1x}} {{focus: reserve-call}} {{zoom: 1.2x}} {{annotate: reserve-call "Pre-allocated capacity"}} `reserve` prevents vector reallocation as rows accumulate. Without it, the vector doubles its capacity several times, copying all existing elements each time.

{{zoom: 1x}} {{focus: scan-loop}} {{zoom: 1.2x}} The three-part hotspot is: `strnlen` per row, heap allocation per row, and struct move into the result vector. The next lessons address each of these. {{annotate: scan-loop "Three hotspots"}}

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:scan-loop lang=cpp
DbResult<std::vector<ResultRow>> execute_select(
    const std::vector<Row>& rows,
    const SelectStmt& stmt) {
  (void)stmt;

  std::vector<ResultRow> result;
  result.reserve(rows.size());  // eliminates realloc

  for (const Row& row : rows) {
    bool name_null = (row.null_mask & 1u) != 0;
    std::string name;
    if (!name_null) {
      // strnlen walks up to kNameMax bytes per row
      size_t len = strnlen(row.name, kNameMax);
      // std::string allocates on heap if len > SSO threshold (~15 chars)
      name = std::string(row.name, len);
    }
    result.push_back(ResultRow{row.id, name_null, std::move(name)});
  }

  return result;
}
---
reserve-call: 7
strnlen-call: 14
string-alloc: 16
push-back: 18
scan-loop: 9
```
