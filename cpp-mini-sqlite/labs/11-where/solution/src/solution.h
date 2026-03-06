#pragma once
#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

enum class DbError { kNotFound, kCorrupted, kOutOfMemory, kIo, kSyntax, kFull };

template <typename T>
using DbResult = std::expected<T, DbError>;

enum class TokenKind {
  kSelect, kInsert, kInto, kCreate, kTable, kFrom, kWhere, kValues, kNull, kNot, kIs, kAnd, kOr,
  kIdent, kInteger, kString,
  kLparen, kRparen, kComma, kStar, kSemicolon,
  kEq, kLt, kGt, kLe, kGe, kNe,
  kEof,
};

constexpr uint32_t kNameMax = 32;

// Wire-format row from the storage layer.
struct Row {
  uint32_t id;
  uint8_t  null_mask;  // bit 0: name is SQL NULL
  char     name[kNameMax];
};

// A single WHERE comparison: col op rhs, or col IS [NOT] NULL.
struct Literal {
  bool        is_null = false;
  uint32_t    integer = 0;
  std::string text;
};

struct Predicate {
  std::string_view col;
  TokenKind        op     = TokenKind::kEq;
  bool             negate = false;  // true for IS NOT NULL
  Literal          rhs;
};

// Parsed SELECT statement.
struct SelectStmt {
  std::string_view         table;
  std::optional<Predicate> where;
};

// Projected result row: stable, string-owning public interface type.
struct ResultRow {
  uint32_t    id;
  bool        name_null;
  std::string name;  // empty when name_null is true
};

// Execute SELECT * FROM table (no WHERE) over a decoded row set.
DbResult<std::vector<ResultRow>> execute_select(
    const std::vector<Row>& rows,
    const SelectStmt& stmt);

// Evaluate a predicate against a ResultRow.
// Returns true if the row satisfies the predicate.
bool eval_predicate(const ResultRow& row, const Predicate& pred);

// Execute SELECT * FROM table WHERE predicate.
// Calls execute_select then filters by eval_predicate.
// If stmt.where is absent, returns all rows.
DbResult<std::vector<ResultRow>> execute_select_where(
    const std::vector<Row>& rows,
    const SelectStmt& stmt);
