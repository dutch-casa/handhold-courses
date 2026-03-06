---
title: Cache Lines
---

# The memory hierarchy

{{clear: slide}}

{{show: hierarchy-table typewriter 2s linear}} The CPU does not read memory one byte at a time.

{{focus: register-row}} {{annotate: register-row "Registers: 1 cycle"}} Register reads complete in a single clock cycle. There are a few dozen registers. All actual computation happens here. {{focus: l1-row}} {{annotate: l1-row "L1: 3–4 cycles"}} L1 cache is 32 to 64 kilobytes, private to each core. A miss here costs 3 to 4 cycles. {{focus: l2-row}} {{annotate: l2-row "L2 cache: 10 cycles"}} L2 is 256 kilobytes to 1 megabyte per core. A miss costs roughly 10 cycles. {{focus: l3-row}} {{annotate: l3-row "L3: 30–40 cycles"}} L3 is shared across cores, typically 8 to 32 megabytes. Misses cost 30 to 40 cycles. {{focus: ram-row}} {{annotate: ram-row "RAM: 200+ cycles"}} A cache miss that reaches main memory costs 200 or more cycles — a 200-fold slowdown relative to a register.

{{focus: none}} The gap between L1 and RAM is not 2x or 10x: it is 50x to 100x. {{annotate: ram-row "100x latency gap"}} The single most important optimization for a loop over many rows is keeping data in L1 or L2. That means fitting as many rows as possible into 64 bytes.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```data:hierarchy-table type=array
["registers  │     1 cycle  │  ~16 to 32 general-purpose registers", "L1 cache   │   3-4 cycles  │  32–64 KB, private per core", "L2 cache   │    10 cycles  │  256 KB–1 MB per core", "L3 cache   │  30-40 cycles  │  8–32 MB, shared across cores", "main RAM   │   200+ cycles  │  gigabytes, accessed via memory controller"]
---
register-row: 0
l1-row: 1
l2-row: 2
l3-row: 3
ram-row: 4
```

# Cache lines

{{clear: slide}}

{{show: cacheline-diagram typewriter 2s linear}} The CPU never fetches a single byte from RAM.

{{focus: line-box}} {{annotate: line-box "64-byte unit"}} The fetch unit is a cache line: exactly 64 contiguous bytes. When you access byte 0 of a struct, the CPU loads bytes 0 through 63 into cache simultaneously. {{focus: row-box}} {{annotate: row-box "Row fits in one line"}} Our `Row` struct is 40 bytes. It fits entirely in one cache line. {{annotate: row-box "One fetch per row"}} Accessing any field of `Row` requires only one cache line fetch — `id`, `null_mask`, and `name` are all in the same line.

{{focus: split-box}} {{annotate: split-box "Split load penalty"}} If a struct spans two cache lines — for example, a 40-byte struct starting at offset 40 of a cache line — the CPU must fetch two lines to read the entire struct. {{annotate: split-box "Two fetches, double cost"}} This doubles the memory latency for that access. {{focus: row-box}} {{annotate: row-box "Aligned layout benefit"}} `Row` objects in a `std::vector` are laid out contiguously. A vector of N rows requires N cache line fetches if each Row fits in one line. If any Row were 65 bytes, alternating rows would span two lines.

{{focus: size-note}} {{annotate: size-note "Deliberate 40-byte size"}} The 40-byte size of `Row` is deliberate: it is small enough to fit in one cache line, leaving 24 bytes of the line as headroom.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```diagram:cacheline-diagram
[cache-line-64-bytes] -> [row-box]
[cache-line-64-bytes] -> [unused-24-bytes]
[row-box] -> [id-field-4]
[row-box] -> [null-mask-1]
[row-box] -> [padding-3]
[row-box] -> [name-32]
[split-box]
[line-box]
[size-note]
---
line-box: cache-line-64-bytes
row-box: row-box
split-box: split-box
size-note: unused-24-bytes
```

# Struct layout for our Row

{{clear: slide}}

{{show: row-layout typewriter 2s linear}} The fields of `Row` are laid out in declaration order, with alignment padding inserted by the compiler.

{{focus: id-field}} {{annotate: id-field "id at offset 0"}} `uint32_t id` starts at offset 0 and occupies 4 bytes. {{focus: null-mask-field}} {{annotate: null-mask-field "null_mask at offset 4"}} `uint8_t null_mask` starts at offset 4 and occupies 1 byte. {{focus: padding-region}} {{annotate: padding-region "3 bytes padding"}} The compiler inserts 3 bytes of padding after `null_mask` so that `char name[32]` starts at offset 8 — a natural alignment boundary. {{focus: name-field}} {{annotate: name-field "name at offset 8"}} `char name[32]` starts at offset 8 and occupies 32 bytes. Total: 40 bytes.

{{focus: pack-warning}} {{annotate: pack-warning "Harmful packing"}} If you used `__attribute__((packed))` to shrink the struct to 37 bytes, `name` would start at offset 5 — a non-aligned address. {{annotate: pack-warning "Split load on most rows"}} On a 64-byte cache line, a 37-byte struct starting at offset 0 ends at byte 36. The next struct starts at byte 37 — `name` begins at byte 37+5 = 42. Any struct that starts past byte 32 will have its `name` field straddle a cache line boundary. {{focus: id-field}} The current 40-byte layout is near-optimal: one cache line per row, no split loads, natural alignment throughout.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:row-layout lang=cpp
// sizeof(Row) == 40, alignof(Row) == 4
struct Row {
  uint32_t id;        // offset  0, size 4
  uint8_t  null_mask; // offset  4, size 1
                      // offset  5: 3 bytes implicit padding
  char     name[32];  // offset  8, size 32
  //               total: 40 bytes, fits in one 64-byte cache line
};

// Do NOT do this: __attribute__((packed)) shrinks to 37 bytes,
// causing split cache-line loads on most rows.
// struct __attribute__((packed)) Row { ... };  // AVOID
---
id-field: 3
null-mask-field: 4
padding-region: 5
name-field: 6
pack-warning: 11
```

# False sharing

{{clear: slide}}

{{show: false-sharing typewriter 2s linear}} False sharing is a multi-core performance hazard that looks like a correctness issue but is not one.

{{focus: thread-a-write}} {{zoom: 1.2x}} {{annotate: thread-a-write "Thread A counter"}} Thread A writes to its counter variable, which lives at some address in memory. {{zoom: 1x}} {{focus: thread-b-write}} {{zoom: 1.2x}} {{annotate: thread-b-write "Thread B counter"}} Thread B writes to its own counter variable. Both counters are declared as adjacent fields in a struct.

{{zoom: 1x}} {{focus: shared-line}} {{zoom: 1.2x}} {{annotate: shared-line "Same cache line"}} If thread A's counter and thread B's counter share a 64-byte cache line, every write by either thread invalidates the cache line in the other core's L1 cache. {{annotate: shared-line "Ping-pong effect"}} The cache line bounces between cores with every write. This can reduce throughput by 10x even though both threads are modifying separate variables.

{{zoom: 1x}} {{focus: alignas-fix}} {{zoom: 1.2x}} {{annotate: alignas-fix "alignas(64) isolation"}} The fix is `alignas(64)` on per-thread state. This forces each counter to start on a fresh cache line boundary, so writes by one thread never invalidate the other thread's cache line. {{annotate: alignas-fix "One line each"}} Each thread now owns its own exclusive cache line.

{{zoom: 1x}} {{focus: false-sharing-struct}} {{zoom: 1.2x}} {{annotate: false-sharing-struct "Pad to 64 bytes"}} The `PaddedCounter` struct contains the actual value plus padding to fill the cache line. Writes to `value` by one thread do not touch the cache line used by any other thread's `PaddedCounter`.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:false-sharing lang=cpp
// Bad: adjacent counters share a cache line.
struct SharedCounters {
  uint64_t thread_a_count;  // offset 0
  uint64_t thread_b_count;  // offset 8, SAME cache line as thread_a_count
};

// Fix: pad each counter to its own cache line.
struct alignas(64) PaddedCounter {
  uint64_t value;
  uint8_t  pad[64 - sizeof(uint64_t)];  // fill remaining 56 bytes
};

// Two PaddedCounters occupy two separate cache lines.
PaddedCounter counters[2];
//  counters[0].value → cache line 0
//  counters[1].value → cache line 1 (never invalidated by writes to counters[0])
---
thread-a-write: 3
thread-b-write: 4
shared-line: 4
alignas-fix: 8
false-sharing-struct: 8
```
