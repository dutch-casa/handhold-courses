#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <expected>
#include <list>
#include <unordered_map>

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

// Holds up to `capacity` pages; evicts the least recently used when full.
// Invariant: lru_ and index_ are always in sync — every node in lru_ has an
// entry in index_, and every entry in index_ points to a live node in lru_.
class PageCache {
 public:
  explicit PageCache(PageManager& pm, size_t capacity);
  DbResult<const Page*> get(PageId id);

 private:
  PageManager& pm_;
  size_t capacity_;
  std::list<Page> lru_;                                          // front = most recent
  std::unordered_map<PageId, std::list<Page>::iterator> index_;
};
