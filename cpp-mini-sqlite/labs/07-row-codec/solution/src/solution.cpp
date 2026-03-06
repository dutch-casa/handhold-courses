#include <cassert>
#include <cstring>
#include "solution.h"

void encode_row(const Row& row, std::span<uint8_t> out) {
  assert(out.size() >= kRowSize);
  std::memset(out.data(), 0, kRowSize);
  std::memcpy(out.data() + kOffId,       &row.id,        sizeof(row.id));
  std::memcpy(out.data() + kOffNullMask, &row.null_mask, sizeof(row.null_mask));
  std::memcpy(out.data() + kOffName,      row.name,       kNameMax);
}

DbResult<Row> decode_row(std::span<const uint8_t> in) {
  if (in.size() < kRowSize) return std::unexpected(DbError::kCorrupted);
  Row row{};
  std::memcpy(&row.id,        in.data() + kOffId,       sizeof(row.id));
  std::memcpy(&row.null_mask, in.data() + kOffNullMask, sizeof(row.null_mask));
  std::memcpy(row.name,       in.data() + kOffName,     kNameMax);
  return row;
}
