#include "solution.h"

PageCache::PageCache(PageManager& pm, size_t capacity)
    : pm_(pm), capacity_(capacity) {}

DbResult<const Page*> PageCache::get(PageId id) {
  // Cache hit: move to front, return pointer.
  auto it = index_.find(id);
  if (it != index_.end()) {
    lru_.splice(lru_.begin(), lru_, it->second);
    return &lru_.front();
  }
  // Cache miss: evict LRU if at capacity.
  if (lru_.size() == capacity_) {
    index_.erase(lru_.back().id);
    lru_.pop_back();
  }
  // Load from disk, push to front.
  auto result = pm_.read(id);
  if (!result) {
    return std::unexpected(result.error());
  }
  lru_.push_front(std::move(*result));
  index_[id] = lru_.begin();
  return &lru_.front();
}
