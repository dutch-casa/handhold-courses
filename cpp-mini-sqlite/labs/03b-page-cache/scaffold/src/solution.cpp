#include "solution.h"

PageCache::PageCache(PageManager& pm, size_t capacity)
    : pm_(pm), capacity_(capacity) {}

DbResult<const Page*> PageCache::get(PageId) {
  return std::unexpected(DbError::kIo);
}
