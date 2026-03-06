# Lab 13: Profile Your Database

## Context

Before you can optimize, you need a benchmark harness. This lab builds one.

You will implement a `benchmark()` function that accepts a set of queries,
runs each query against its associated row table, measures wall time with
`std::chrono::high_resolution_clock`, and aggregates the results into a
`BenchmarkResult` struct.

This lab is deliberately narrow: no tokenizer, no parser. The `benchmark()`
function treats each `BenchmarkQuery` as a full table scan — it visits every
row in `q.rows` for every query. This mirrors what a real profiling harness
does: isolate one stage and measure it under controlled conditions.

## What to implement

Open `src/solution.cpp` and implement:

```
DbResult<BenchmarkResult> benchmark(const std::vector<BenchmarkQuery>& queries);
```

### Steps

1. Return `std::unexpected(DbError::kNotFound)` immediately if `queries` is
   empty. An empty benchmark is meaningless — treat it as an error.

2. Record the start time:
   ```cpp
   auto t0 = std::chrono::high_resolution_clock::now();
   ```

3. For each `BenchmarkQuery q` in `queries`:
   - Increment `out.query_count`.
   - For each `Row row` in `q.rows`: increment `out.rows_scanned`. This
     counts every row examined — the full scan count, not the filtered count.
     Real databases report "rows examined" in EXPLAIN ANALYZE for exactly this
     reason: it reveals whether an index was used.

4. Record the end time and compute elapsed nanoseconds:
   ```cpp
   auto t1 = std::chrono::high_resolution_clock::now();
   out.total_ns = static_cast<uint64_t>(
       std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
   ```

5. Return `out`.

### Invariants

- `query_count` must equal `queries.size()` for any non-empty input.
- `rows_scanned` must equal the sum of `q.rows.size()` across all queries.
- `total_ns` must be greater than zero for any non-trivial run.

## Running the tests

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -S . -Wno-dev
bash tap_runner.sh build
```

The stub returns `kNotFound` for all inputs, so all eight tests fail until
`benchmark()` is implemented. All eight must pass.
