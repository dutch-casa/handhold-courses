#include "solution.h"

DbResult<BenchmarkResult> benchmark(const std::vector<BenchmarkQuery>& queries) {
  (void)queries;
  return std::unexpected(DbError::kNotFound);
}
