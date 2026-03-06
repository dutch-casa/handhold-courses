#include <cstring>
#include "solution.h"

DbResult<std::vector<ResultRow>> execute_select(
    const std::vector<Row>& rows,
    const SelectStmt& stmt) {
  (void)stmt;

  std::vector<ResultRow> result;
  result.reserve(rows.size());

  for (const Row& row : rows) {
    bool name_null = (row.null_mask & 1u) != 0;
    std::string name;
    if (!name_null) {
      name = std::string(row.name, strnlen(row.name, kNameMax));
    }
    result.push_back(ResultRow{row.id, name_null, std::move(name)});
  }

  return result;
}

bool eval_predicate(const ResultRow& row, const Predicate& pred) {
  if (pred.col == "id") {
    uint32_t rhs = pred.rhs.integer;
    switch (pred.op) {
      case TokenKind::kEq: return row.id == rhs;
      case TokenKind::kLt: return row.id <  rhs;
      case TokenKind::kGt: return row.id >  rhs;
      case TokenKind::kLe: return row.id <= rhs;
      case TokenKind::kGe: return row.id >= rhs;
      case TokenKind::kNe: return row.id != rhs;
      default:             return false;
    }
  }

  if (pred.col == "name") {
    if (pred.op == TokenKind::kIs) {
      // negate=false → IS NULL; negate=true → IS NOT NULL
      return row.name_null == !pred.negate;
    }
    const std::string& rhs = pred.rhs.text;
    switch (pred.op) {
      case TokenKind::kEq: return row.name == rhs;
      case TokenKind::kLt: return row.name <  rhs;
      case TokenKind::kGt: return row.name >  rhs;
      case TokenKind::kLe: return row.name <= rhs;
      case TokenKind::kGe: return row.name >= rhs;
      case TokenKind::kNe: return row.name != rhs;
      default:             return false;
    }
  }

  // Unknown column — conservative failure.
  return false;
}

DbResult<std::vector<ResultRow>> execute_select_where(
    const std::vector<Row>& rows,
    const SelectStmt& stmt) {
  auto all = execute_select(rows, stmt);
  if (!all) return std::unexpected(all.error());

  if (!stmt.where) return all;

  const Predicate& pred = *stmt.where;
  std::vector<ResultRow> filtered;
  for (ResultRow& row : *all) {
    if (eval_predicate(row, pred)) {
      filtered.push_back(std::move(row));
    }
  }
  return filtered;
}
