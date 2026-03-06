#include <chrono>
#include "solution.h"

DbResult<BenchmarkResult> benchmark(const std::vector<BenchmarkQuery>& queries) {
  // An empty benchmark provides no useful data — treat it as a caller error.
  if (queries.empty()) return std::unexpected(DbError::kNotFound);

  BenchmarkResult out{};
  auto t0 = std::chrono::high_resolution_clock::now();

  for (const BenchmarkQuery& q : queries) {
    // Count every row visited: this is "rows examined", not "rows returned".
    // Profiling cares about work done, not result set size.
    for (const Row& row : q.rows) {
      (void)row;
      ++out.rows_scanned;
    }
    ++out.query_count;
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  out.total_ns = static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());

  return out;
}
