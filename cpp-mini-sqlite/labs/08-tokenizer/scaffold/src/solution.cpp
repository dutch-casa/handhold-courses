#include "solution.h"

DbResult<std::vector<Token>> tokenize(std::string_view sql) {
  // TODO: see INSTRUCTIONS.md
  (void)sql;
  return std::unexpected(DbError::kSyntax);
}
