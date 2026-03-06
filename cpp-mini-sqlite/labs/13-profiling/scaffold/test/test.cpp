#include <cstring>
#include <gtest/gtest.h>
#include "solution.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static Row make_row(uint32_t id, const char* name) {
  Row r{};
  r.id = id;
  r.null_mask = 0;
  if (name == nullptr) {
    r.null_mask = 1;
  } else {
    std::strncpy(r.name, name, kNameMax);
  }
  return r;
}

static std::vector<Row> make_rows(size_t count) {
  std::vector<Row> rows;
  rows.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    rows.push_back(make_row(static_cast<uint32_t>(i + 1), "testuser"));
  }
  return rows;
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

TEST(Benchmark, EmptyQueriesReturnError) {
  std::vector<BenchmarkQuery> queries;
  auto result = benchmark(queries);
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DbError::kNotFound);
}

TEST(Benchmark, SingleQueryPopulatesQueryCount) {
  std::vector<BenchmarkQuery> queries = {
    BenchmarkQuery{"SELECT * FROM users", make_rows(3)},
  };
  auto result = benchmark(queries);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->query_count, 1u);
}

TEST(Benchmark, MultipleQueriesAccumulateCount) {
  std::vector<BenchmarkQuery> queries = {
    BenchmarkQuery{"SELECT * FROM users", make_rows(2)},
    BenchmarkQuery{"SELECT * FROM users", make_rows(2)},
    BenchmarkQuery{"SELECT * FROM users", make_rows(2)},
  };
  auto result = benchmark(queries);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->query_count, 3u);
}

TEST(Benchmark, RowsScannedMatchesTotalRows) {
  std::vector<BenchmarkQuery> queries = {
    BenchmarkQuery{"SELECT * FROM users", make_rows(5)},
  };
  auto result = benchmark(queries);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->rows_scanned, 5u);
}

TEST(Benchmark, MultipleQueriesAccumulateRowsScanned) {
  std::vector<BenchmarkQuery> queries = {
    BenchmarkQuery{"SELECT * FROM users", make_rows(5)},
    BenchmarkQuery{"SELECT * FROM users", make_rows(5)},
  };
  auto result = benchmark(queries);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->rows_scanned, 10u);
}

TEST(Benchmark, TotalNsIsPositive) {
  // 50 queries × 10 000 rows = 500 000 row iterations — large enough that
  // any plausible high_resolution_clock resolution reports non-zero ns.
  std::vector<BenchmarkQuery> queries;
  for (int i = 0; i < 50; ++i) {
    queries.push_back(BenchmarkQuery{"SELECT * FROM users", make_rows(10000)});
  }
  auto result = benchmark(queries);
  ASSERT_TRUE(result.has_value());
  EXPECT_GT(result->total_ns, 0u);
}

TEST(Benchmark, LargeTableScansAllRows) {
  std::vector<BenchmarkQuery> queries = {
    BenchmarkQuery{"SELECT * FROM users", make_rows(1000)},
  };
  auto result = benchmark(queries);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->rows_scanned, 1000u);
}

TEST(Benchmark, QueryCountMatchesInputSize) {
  std::vector<BenchmarkQuery> queries;
  for (int i = 0; i < 7; ++i) {
    queries.push_back(BenchmarkQuery{"SELECT * FROM users", make_rows(10)});
  }
  auto result = benchmark(queries);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->query_count, 7u);
}
