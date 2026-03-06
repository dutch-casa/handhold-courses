#include "solution.h"

DbResult<std::vector<ResultRow>> execute_select(
    const std::vector<Row>& rows,
    const SelectStmt& stmt) {
  (void)rows;
  (void)stmt;
  return std::unexpected(DbError::kNotFound);
}
