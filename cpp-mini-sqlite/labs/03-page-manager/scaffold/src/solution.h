#pragma once
#include <array>
#include <cstdint>
#include <cstdio>
#include <expected>

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

using PageId = uint32_t;
constexpr uint32_t kPageSize = 4096;

struct Page {
  PageId id;
  std::array<uint8_t, kPageSize> data;
};

// Owns a file handle. Maps PageId to byte offset, performs reads and writes.
// Invariant: every page lives at byte offset id * kPageSize.
class PageManager {
 public:
  explicit PageManager(const char* path);
  ~PageManager();

  DbResult<Page>  read(PageId id);
  DbResult<void>  write(const Page& page);

 private:
  FILE* file_;
};
