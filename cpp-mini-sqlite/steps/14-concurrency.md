---
title: Concurrent Readers
---

# The reader-writer problem

{{clear: slide}}

{{show: rw-diagram typewriter 2s linear}} The reader-writer problem asks: how do you allow many concurrent readers while ensuring a single writer has exclusive access?

{{focus: readers-node}} {{annotate: readers-node "Many readers allowed"}} Multiple threads can read the same data simultaneously without corrupting it. Reads do not change the data, so they do not interfere with each other. {{focus: writer-node}} {{annotate: writer-node "One writer, exclusive"}} A write modifies the data. While a write is in progress, no other thread — reader or writer — may access the data. {{annotate: writer-node "Mutual exclusion"}} This is mutual exclusion at the write boundary.

{{focus: conflict-node}} {{annotate: conflict-node "Read-write conflict"}} A reader and a writer active at the same time is a data race. One thread reads a partially written value. The program has undefined behavior. {{focus: write-write-node}} {{annotate: write-write-node "Write-write conflict"}} Two concurrent writers produce a torn write — the final value is an interleaving of both writes, which is neither one. {{focus: readers-node}} {{annotate: readers-node "Read-read: safe"}} Two concurrent readers are always safe.

{{focus: rw-table}} {{annotate: rw-table "State table"}} The legal state table is: multiple concurrent readers, or exactly one writer, never both. This is the invariant that `std::shared_mutex` enforces.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```diagram:rw-diagram
readers-node [service: Readers (shared_lock)]
writer-node [service: Writer (unique_lock)]
conflict-node [service: Read-Write race — UB]
write-write-node [service: Write-Write race — torn write]
rw-table [service: State: many readers OR one writer]
readers-node --> conflict-node: concurrent with write
writer-node --> conflict-node: concurrent with read
writer-node --> write-write-node: concurrent with write
{Safe: readers-node}
{Dangerous: conflict-node, write-write-node}
---
readers-node: readers-node
writer-node: writer-node
conflict-node: conflict-node
write-write-node: write-write-node
rw-table: rw-table
```

# std::shared_mutex

{{clear: slide}}

{{show: shared-mutex-example typewriter 2s linear}} `std::shared_mutex` provides two lock modes to enforce the reader-writer invariant.

{{focus: shared-lock-line}} {{zoom: 1.2x}} {{annotate: shared-lock-line "shared_lock for readers"}} `std::shared_lock<std::shared_mutex>` acquires the mutex in shared mode. Multiple threads can hold shared locks simultaneously. {{annotate: shared-lock-line "No reader contention"}} A thread acquiring a shared lock succeeds immediately as long as no writer holds or is waiting for an exclusive lock.

{{zoom: 1x}} {{focus: unique-lock-line}} {{zoom: 1.2x}} {{annotate: unique-lock-line "unique_lock for writers"}} `std::unique_lock<std::shared_mutex>` acquires the mutex in exclusive mode. {{annotate: unique-lock-line "Writer exclusivity"}} While a unique lock is held, every other thread — reader or writer — blocks on acquisition. {{annotate: unique-lock-line "RAII release"}} Both lock types release the mutex in their destructor, so they are safe across early returns and exceptions.

{{zoom: 1x}} {{focus: guarded-read}} {{zoom: 1.2x}} {{annotate: guarded-read "Read path"}} `read_all` acquires a shared lock, copies the row vector, and returns. The copy is safe because no writer can mutate the vector while the shared lock is held. {{zoom: 1x}} {{focus: guarded-write}} {{zoom: 1.2x}} {{annotate: guarded-write "Write path"}} `append_row` acquires an exclusive lock before calling `push_back`. No reader can observe a partial state.

{{zoom: 1x}} {{focus: member-mutex}} {{zoom: 1.2x}} {{annotate: member-mutex "mutable for const methods"}} The mutex is declared `mutable` so that `read_all` — a logically const method — can still acquire the lock. Logical constness and physical constness diverge here.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:shared-mutex-example lang=cpp
#include <shared_mutex>
#include <vector>

class RowTable {
 public:
  // Multiple threads may call read_all concurrently.
  std::vector<Row> read_all() const {
    std::shared_lock lock(mutex_);  // shared (reader) lock
    return rows_;                   // copy while lock is held
  }

  // Only one thread may call append_row at a time;
  // all readers block until append_row returns.
  void append_row(Row row) {
    std::unique_lock lock(mutex_);  // exclusive (writer) lock
    rows_.push_back(std::move(row));
  }

 private:
  mutable std::shared_mutex mutex_;
  std::vector<Row>          rows_;
};
---
shared-lock-line: 8
unique-lock-line: 15
guarded-read: 7
guarded-write: 14
member-mutex: 20
```

# Lock-free reads with std::atomic

{{clear: slide}}

{{show: atomic-example typewriter 2s linear}} For a single integer field like the row count, a full mutex is heavier than necessary.

{{focus: atomic-decl}} {{zoom: 1.2x}} {{annotate: atomic-decl "Atomic counter"}} `std::atomic<size_t>` allows any thread to read or write the value without a mutex. {{annotate: atomic-decl "Lock-free on most platforms"}} On x86-64 and ARM64, atomic operations on `size_t` are lock-free: they compile to a single load or store instruction with an appropriate memory barrier.

{{zoom: 1x}} {{focus: relaxed-load}} {{zoom: 1.2x}} {{annotate: relaxed-load "Relaxed load for readers"}} `row_count_.load(std::memory_order_relaxed)` reads the current count without any synchronization constraint. {{annotate: relaxed-load "No barrier needed"}} If the only invariant is "the count is approximately current", relaxed ordering is sufficient and cheapest.

{{zoom: 1x}} {{focus: release-store}} {{zoom: 1.2x}} {{annotate: release-store "Release store for writer"}} `row_count_.store(rows_.size(), std::memory_order_release)` publishes the new count with a release barrier. {{annotate: release-store "Happens-before guarantee"}} Any reader that subsequently loads with `memory_order_acquire` is guaranteed to see all writes that happened before the store. {{annotate: release-store "Acquire-release pairing"}} The release store on the writer pairs with an acquire load on the reader to establish a happens-before relationship across threads.

{{zoom: 1x}} {{focus: combine-pattern}} {{zoom: 1.2x}} {{annotate: combine-pattern "Split-lock strategy"}} The pattern: use `shared_mutex` to guard the row vector (requires RAII lock protocol) and `std::atomic` for the count (read with a single instruction, no lock needed). This gives readers a lock-free fast path for count queries.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:atomic-example lang=cpp
#include <atomic>
#include <shared_mutex>

class RowTable {
 public:
  size_t row_count() const {
    // Lock-free: a single atomic load instruction.
    return row_count_.load(std::memory_order_relaxed);
  }

  void append_row(Row row) {
    std::unique_lock lock(mutex_);
    rows_.push_back(std::move(row));
    // Publish the new count with a release barrier so readers see the update.
    row_count_.store(rows_.size(), std::memory_order_release);
  }

 private:
  mutable std::shared_mutex mutex_;
  std::vector<Row>          rows_;
  std::atomic<size_t>       row_count_{0};
};
---
atomic-decl: 21
relaxed-load: 8
release-store: 15
combine-pattern: 4
```

# The costs

{{clear: slide}}

{{show: cost-table typewriter 2s linear}} Concurrency primitives are not free.

{{focus: mutex-latency-row}} {{annotate: mutex-latency-row "Mutex acquisition cost"}} An uncontended `std::shared_mutex` acquire costs roughly 10 to 30 nanoseconds — comparable to an L3 cache miss. Under contention the cost rises to microseconds or more as threads spin or sleep. {{focus: false-sharing-mutex-row}} {{annotate: false-sharing-mutex-row "Mutex false sharing"}} The `std::shared_mutex` object itself is 40 to 56 bytes depending on the platform. If multiple mutexes are packed in an array or struct, they may share cache lines and cause false sharing between the threads that lock them.

{{focus: writer-starvation-row}} {{annotate: writer-starvation-row "Writer starvation"}} Under heavy read load, a writer requesting an exclusive lock may wait indefinitely because new readers keep arriving and acquiring the shared lock. Some platforms bias toward writers; others do not. {{annotate: writer-starvation-row "Platform dependent"}} Starvation behaviour is not standardized. Test under your target workload.

{{focus: atomic-latency-row}} {{annotate: atomic-latency-row "Atomic cost"}} A lock-free `std::atomic<size_t>` load with `memory_order_relaxed` costs 1 to 3 nanoseconds on x86-64 — essentially a regular load with a compiler fence. An acquire-release pair is 5 to 15 nanoseconds. Far cheaper than a mutex for a single scalar.

{{focus: write-spike-row}} {{annotate: write-spike-row "Latency spike during writes"}} When a writer holds the exclusive lock, every reader blocks. If a write takes 50 microseconds — for example, reallocating a large vector — every concurrent read query stalls for 50 microseconds. {{annotate: write-spike-row "Short write discipline"}} Keep writes short: append one row at a time, hold the lock for microseconds, not milliseconds.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```data:cost-table type=array
["shared_mutex acquire (uncontended) │  10–30 ns  │  comparable to L3 cache miss", "shared_mutex acquire (contended)   │  1–100 µs  │  thread may sleep and be rescheduled", "mutex false sharing                │  up to 10x │  pack mutexes with alignas(64)", "writer starvation                  │  unbounded │  platform-dependent, test under load", "atomic<size_t> relaxed load        │  1–3 ns    │  single load instruction with compiler fence", "write spike (vector realloc)       │  50+ µs    │  all readers block; keep writes short"]
---
mutex-latency-row: 0
false-sharing-mutex-row: 2
writer-starvation-row: 3
atomic-latency-row: 4
write-spike-row: 5
```
