#include "solution.h"

#include <cstdio>

DbResult<std::vector<uint8_t>> read_file(const char* path) {
  FILE* f = std::fopen(path, "rb");
  if (!f) return std::unexpected(DbError::kIo);

  // Seek to end to determine file size.
  if (std::fseek(f, 0, SEEK_END) != 0) {
    std::fclose(f);
    return std::unexpected(DbError::kIo);
  }

  long size = std::ftell(f);
  if (size < 0) {
    std::fclose(f);
    return std::unexpected(DbError::kIo);
  }

  if (std::fseek(f, 0, SEEK_SET) != 0) {
    std::fclose(f);
    return std::unexpected(DbError::kIo);
  }

  std::vector<uint8_t> buf(static_cast<size_t>(size));

  // A zero-size file is valid — fread with count=0 is a no-op.
  if (size > 0) {
    size_t bytes_read = std::fread(buf.data(), 1, static_cast<size_t>(size), f);
    if (bytes_read != static_cast<size_t>(size)) {
      std::fclose(f);
      return std::unexpected(DbError::kIo);
    }
  }

  std::fclose(f);
  return buf;
}
