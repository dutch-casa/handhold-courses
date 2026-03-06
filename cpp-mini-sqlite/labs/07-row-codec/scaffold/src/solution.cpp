#include <cassert>
#include <cstring>
#include "solution.h"

void encode_row(const Row& row, std::span<uint8_t> out) {
  // TODO: see INSTRUCTIONS.md
  assert(out.size() >= kRowSize);
  (void)row;
  (void)out;
}

DbResult<Row> decode_row(std::span<const uint8_t> in) {
  // TODO: see INSTRUCTIONS.md
  (void)in;
  return std::unexpected(DbError::kCorrupted);
}
