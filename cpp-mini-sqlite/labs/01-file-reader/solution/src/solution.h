#pragma once

#include <cstdint>
#include <expected>
#include <vector>

// Error codes for all database operations. kIo covers any syscall failure.
enum class DbError {
  kNotFound,
  kCorrupted,
  kOutOfMemory,
  kIo,
  kSyntax,
  kFull,
};

template <typename T>
using DbResult = std::expected<T, DbError>;

// Reads all bytes from the file at `path` in binary mode.
// Returns kIo on any syscall failure (open, seek, size query, read).
DbResult<std::vector<uint8_t>> read_file(const char* path);
