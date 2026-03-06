#include "solution.h"

DbResult<std::vector<uint8_t>> read_file(const char* path) {
  return std::unexpected(DbError::kIo);
}
