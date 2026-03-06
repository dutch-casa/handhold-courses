#pragma once
#include <cstdint>
#include <expected>
#include <string>
#include <vector>

enum class DbError { kNotFound, kCorrupted, kOutOfMemory, kIo, kSyntax, kFull };

template <typename T>
using DbResult = std::expected<T, DbError>;

constexpr uint32_t kNameMax = 32;

// Wire-format row from the storage layer.
struct Row {
  uint32_t id;
  uint8_t  null_mask;  // bit 0: name is SQL NULL
  char     name[kNameMax];
};

// Parsed SELECT statement (where field is ignored in this lab).
struct SelectStmt {
  // Minimal definition — only the fields execute_select needs.
  // In a full build this would import from the parser header.
};

// A projected result row: stable, string-owning public interface type.
struct ResultRow {
  uint32_t    id;
  bool        name_null;
  std::string name;  // empty when name_null is true
};

// Execute SELECT * FROM table (no WHERE) over a decoded row set.
// Returns all rows projected to ResultRow. stmt is accepted for interface
// consistency; its where field is not consulted.
DbResult<std::vector<ResultRow>> execute_select(
    const std::vector<Row>& rows,
    const SelectStmt& stmt);
