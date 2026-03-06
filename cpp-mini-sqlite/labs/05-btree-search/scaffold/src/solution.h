#pragma once
#include <array>
#include <cstdint>
#include <cstdio>
#include <expected>
#include <list>
#include <unordered_map>

// ── Primitives ───────────────────────────────────────────────────────────────

using PageId = uint32_t;
constexpr uint32_t kPageSize = 4096;

// Error codes for all database operations. Not every code applies to each lab.
enum class DbError { kNotFound, kCorrupted, kOutOfMemory, kIo, kSyntax, kFull };

template <typename T>
using DbResult = std::expected<T, DbError>;

struct Page {
  PageId id;
  alignas(4) std::array<uint8_t, kPageSize> data;
};

// ── B-tree node ──────────────────────────────────────────────────────────────

// Invariant: num_keys <= kOrder at all times.
constexpr uint32_t kOrder = 510;  // max keys per node; fits in one 4096-byte page

struct BTreeNode {
  uint32_t num_keys;              // 0..kOrder (current key count)
  uint32_t is_leaf;               // uint32_t keeps 4-byte alignment; no padding before keys[]
  uint32_t keys[kOrder];          // sorted key array
  // Internal: children[i] is the PageId of the subtree left of keys[i];
  //           children[num_keys] is the rightmost child.
  // Leaf:     children[i] is the value associated with keys[i].
  uint32_t children[kOrder + 1];
};
static_assert(sizeof(BTreeNode) <= kPageSize, "BTreeNode exceeds page size");

// ── PageManager — implemented in helpers.cpp ─────────────────────────────────

// Owns a file and maps PageIds to byte offsets (offset = id * kPageSize).
class PageManager {
 public:
  explicit PageManager(const char* path);
  ~PageManager();
  PageManager(const PageManager&) = delete;
  PageManager& operator=(const PageManager&) = delete;

  DbResult<Page>  read(PageId id);
  DbResult<void>  write(const Page& page);

 private:
  std::FILE* file_;
};

// ── PageCache — implemented in helpers.cpp ───────────────────────────────────

// LRU page cache. Evicts least recently used page when at capacity.
class PageCache {
 public:
  explicit PageCache(PageManager& pm, size_t capacity = 64);
  PageCache(const PageCache&) = delete;
  PageCache& operator=(const PageCache&) = delete;

  DbResult<const Page*> get(PageId id);

 private:
  PageManager& pm_;
  size_t capacity_;
  std::list<Page> lru_;
  std::unordered_map<PageId, std::list<Page>::iterator> index_;
};

// ── BTree ────────────────────────────────────────────────────────────────────

// B-tree backed by a PageCache.
// Invariant: page root_id_ always holds the root BTreeNode.
class BTree {
 public:
  explicit BTree(PageCache& cache, PageId root_id = 0);

  // Iterative descent from root to leaf. Returns kNotFound if key is absent.
  DbResult<uint32_t> search(uint32_t key) const;

 private:
  PageCache& cache_;
  PageId root_id_;
};
