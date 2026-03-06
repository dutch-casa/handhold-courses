#include "solution.h"
#include <cstring>

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

// ── BTree helpers ─────────────────────────────────────────────────────────────

// Returns a mutable BTreeNode reference from a cached (const) Page.
// The const_cast is intentional: the cache's backing std::list holds non-const
// Page objects; PageCache::get exposes them as const to prevent accidental
// unsynchronized writes. Callers that write through this reference MUST flush
// the page to disk via flush() before returning.
BTreeNode& BTree::as_node(const Page* p) {
  return *reinterpret_cast<BTreeNode*>(const_cast<uint8_t*>(p->data.data()));
}

DbResult<PageId> BTree::alloc_page() {
  PageId id = next_page_id_++;
  Page page{id, {}};
  page.data.fill(0);
  auto result = pm_.write(page);
  if (!result) return std::unexpected(result.error());
  return id;
}

// Pre-implemented search.
DbResult<uint32_t> BTree::search(uint32_t key) const {
  PageId node_id = root_id_;
  while (true) {
    auto result = cache_.get(node_id);
    if (!result) return std::unexpected(result.error());
    auto& node = *reinterpret_cast<const BTreeNode*>((*result)->data.data());

    uint32_t i = 0;
    while (i < node.num_keys && key > node.keys[i]) ++i;

    if (node.is_leaf) {
      if (i < node.num_keys && node.keys[i] == key) return node.children[i];
      return std::unexpected(DbError::kNotFound);
    }
    node_id = node.children[i];
  }
}
