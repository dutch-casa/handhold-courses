#include "solution.h"

DbResult<std::vector<ResultRow>> execute_select(
    const std::vector<Row>& rows,
    const SelectStmt& stmt) {
  // TODO: see INSTRUCTIONS.md
  (void)rows;
  (void)stmt;
  return std::unexpected(DbError::kNotFound);
}

bool eval_predicate(const ResultRow& row, const Predicate& pred) {
  // TODO: see INSTRUCTIONS.md
  (void)row;
  (void)pred;
  return false;
}

DbResult<std::vector<ResultRow>> execute_select_where(
    const std::vector<Row>& rows,
    const SelectStmt& stmt) {
  // TODO: see INSTRUCTIONS.md
  (void)rows;
  (void)stmt;
  return std::unexpected(DbError::kNotFound);
}
