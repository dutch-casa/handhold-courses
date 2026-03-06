#pragma once
#include <cstdint>
#include <cstring>
#include <expected>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Core error type
// ---------------------------------------------------------------------------

enum class DbError { kNotFound, kCorrupted, kOutOfMemory, kIo, kSyntax, kFull };

template <typename T>
using DbResult = std::expected<T, DbError>;

// ---------------------------------------------------------------------------
// Row storage type
// ---------------------------------------------------------------------------

static constexpr size_t kNameMax = 32;
static constexpr size_t kRowSize = 40;

struct Row {
  uint32_t id;
  uint8_t  null_mask;  // bit 0: name is SQL NULL
  char     name[kNameMax];
};

// ---------------------------------------------------------------------------
// Result type (projected interface)
// ---------------------------------------------------------------------------

struct ResultRow {
  uint32_t    id;
  bool        name_null;
  std::string name;  // empty when name_null is true
};

// ---------------------------------------------------------------------------
// Benchmark types
// ---------------------------------------------------------------------------

struct BenchmarkResult {
  uint32_t query_count;  // number of queries executed
  uint64_t total_ns;     // total wall time in nanoseconds
  uint64_t rows_scanned; // total rows examined across all queries
};

struct BenchmarkQuery {
  std::string      sql;   // carried for context; not parsed in this lab
  std::vector<Row> rows;  // row table for this query
};

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

// Run all queries and aggregate timing + scan counts.
// Returns kNotFound if queries is empty (an empty benchmark is meaningless).
DbResult<BenchmarkResult> benchmark(const std::vector<BenchmarkQuery>& queries);
