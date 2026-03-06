#include "solution.h"

DbResult<PageHeader> read_page_header(std::string_view path) {
  return std::unexpected(DbError::kIo);
}
