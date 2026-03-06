#include "solution.h"

DbResult<PageHeader> read_page_header(std::string_view path) {
  auto bytes = read_file(std::string(path).c_str());
  if (!bytes) {
    return std::unexpected(bytes.error());
  }
  if (bytes->size() < 8) {
    return std::unexpected(DbError::kCorrupted);
  }
  const auto& data = *bytes;
  uint32_t cell_count =
    (static_cast<uint32_t>(data[4]) << 24) |
    (static_cast<uint32_t>(data[5]) << 16) |
    (static_cast<uint32_t>(data[6]) <<  8) |
     static_cast<uint32_t>(data[7]);
  return PageHeader{data[0], {}, cell_count};
}
