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
