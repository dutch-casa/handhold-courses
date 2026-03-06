#pragma once

#include <cstdint>
#include <expected>
#include <string_view>
#include <vector>

// Error codes for all database operations. Not every code applies to each lab.
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

// On-disk page header: 8 bytes at the start of every page.
struct PageHeader {
  uint8_t  page_type;    // 0x0D = leaf, 0x05 = interior
  uint8_t  reserved[3];  // zero on write; ignored on read
  uint32_t cell_count;   // number of cells (native endian after decoding)
};

// Reads all bytes from the file at `path` in binary mode.
// Returns kIo on any syscall failure.
DbResult<std::vector<uint8_t>> read_file(const char* path);

// Reads and decodes the 8-byte page header from the start of the file at `path`.
// Returns kCorrupted if the file is shorter than 8 bytes.
// Returns kIo on any syscall failure.
DbResult<PageHeader> read_page_header(std::string_view path);
