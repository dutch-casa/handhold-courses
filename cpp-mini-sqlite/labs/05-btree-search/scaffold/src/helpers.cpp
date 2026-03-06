#include "solution.h"

// ── PageManager ──────────────────────────────────────────────────────────────

PageManager::PageManager(const char* path) : file_(std::fopen(path, "r+b")) {}

PageManager::~PageManager() {
  if (file_) std::fclose(file_);
}

DbResult<Page> PageManager::read(PageId id) {
  if (!file_) return std::unexpected(DbError::kIo);
  size_t offset = static_cast<size_t>(id) * kPageSize;
  // Note: long is 64-bit on macOS/Linux; safe for files up to ~8 EB.
  if (std::fseek(file_, static_cast<long>(offset), SEEK_SET) != 0)
    return std::unexpected(DbError::kIo);
  Page page{id, {}};
  size_t n = std::fread(page.data.data(), 1, kPageSize, file_);
  if (n != kPageSize) return std::unexpected(DbError::kIo);
  return page;
}

DbResult<void> PageManager::write(const Page& page) {
  if (!file_) return std::unexpected(DbError::kIo);
  size_t offset = static_cast<size_t>(page.id) * kPageSize;
  if (std::fseek(file_, static_cast<long>(offset), SEEK_SET) != 0)
    return std::unexpected(DbError::kIo);
  size_t n = std::fwrite(page.data.data(), 1, kPageSize, file_);
  if (n != kPageSize) return std::unexpected(DbError::kIo);
  return {};
}

// ── PageCache ────────────────────────────────────────────────────────────────

PageCache::PageCache(PageManager& pm, size_t capacity)
    : pm_(pm), capacity_(capacity) {}

// Returns a pointer to the cached Page. The pointer is valid until the next
// call to get() that causes an eviction (i.e., when the cache is full).
// Callers must not hold the returned pointer across a subsequent get() call.
DbResult<const Page*> PageCache::get(PageId id) {
  auto it = index_.find(id);
  if (it != index_.end()) {
    lru_.splice(lru_.begin(), lru_, it->second);
    return &lru_.front();
  }
  if (lru_.size() == capacity_) {
    index_.erase(lru_.back().id);
    lru_.pop_back();
  }
  auto result = pm_.read(id);
  if (!result) return std::unexpected(result.error());
  lru_.push_front(std::move(*result));
  index_[id] = lru_.begin();
  return &lru_.front();
}
