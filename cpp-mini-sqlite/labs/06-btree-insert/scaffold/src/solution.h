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
  uint32_t num_keys;  // 0..kOrder (current key count)
  uint32_t is_leaf;   // uint32_t keeps 4-byte alignment; no padding before keys[]
  uint32_t keys[kOrder];
  // Internal: children[i] is the PageId of the subtree left of keys[i];
  //           children[num_keys] is the rightmost child.
  // Leaf:     children[i] is the value associated with keys[i].
  uint32_t children[kOrder + 1];
};
static_assert(sizeof(BTreeNode) <= kPageSize, "BTreeNode exceeds page size");

// ── PageManager — implemented in helpers.cpp ─────────────────────────────────

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

// ── Free function — student implements ───────────────────────────────────────

// Splits the full child at parent.children[idx].
// right receives the right half; parent gains the promoted median key.
// Precondition: child.num_keys == kOrder.
void split_child(BTreeNode& parent, uint32_t idx,
                 BTreeNode& child, PageId right_id, BTreeNode& right);

// ── BTree ────────────────────────────────────────────────────────────────────

// B-tree backed by PageManager (writes) and PageCache (reads).
// Invariant: page root_id_ always holds the root BTreeNode.
class BTree {
 public:
  // page_count: total pages currently in the file; used as the next free page ID.
  BTree(PageManager& pm, PageCache& cache, PageId root_id, uint32_t page_count);

  // Pre-implemented in helpers.cpp.
  DbResult<uint32_t> search(uint32_t key) const;

  // Student implements this.
  DbResult<void> insert(uint32_t key, uint32_t value);

 private:
  PageManager& pm_;
  PageCache& cache_;
  PageId root_id_;
  uint32_t next_page_id_;  // bump-pointer page allocator

  // Reinterpret a const Page* as a mutable BTreeNode. Implemented in helpers.cpp.
  static BTreeNode& as_node(const Page* p);

  // Allocate and zero a new page. Implemented in helpers.cpp.
  DbResult<PageId> alloc_page();
};
