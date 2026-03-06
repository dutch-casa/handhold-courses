---
title: Page Abstraction
---

# The Page struct

{{clear: slide}}

{{show: page-struct typewriter 2s linear}} A `Page` is two things: a `PageId` and a 4096-byte buffer. That's the entire abstraction. Everything the storage engine does is built on top of these two fields.

{{focus: pageid-type}} {{annotate: pageid-type "Strong alias, not raw int"}} `using PageId = uint32_t` — an alias, not a typedef. Every function that takes a page identifier uses this type. The alias makes the intent visible in every signature.

{{focus: kpagesize}} {{annotate: kpagesize "Compile-time constant"}} `constexpr uint32_t kPageSize = 4096` — a named constant, never a magic number. The number 4096 appears exactly once in the codebase. Change it here, and the entire engine adjusts.

{{focus: page-struct-body}} {{annotate: page-struct-body "Two fields, nothing else"}} `Page` has no methods, no heap allocations, no destructor. It is a named buffer with an identity. It's trivially copyable, stack-allocatable, and zero-overhead.

{{focus: none}} {{clear: slide}}

{{show: page-struct slide 0.3s}} Why 4096 bytes? That is the default OS virtual memory page size on x86 and ARM. A database page that matches the OS page fits entirely in one TLB entry, one cache line group, one filesystem block. The alignment is not accidental.

{{focus: data-field}} {{annotate: data-field "Fixed-size array, no heap"}} `std::array<uint8_t, kPageSize>` — not a `std::vector`. The buffer lives inline in the struct. No allocation, no pointer indirection. The whole `Page` is 4100 bytes on the stack.

{{focus: none}} There is no "empty" page, no null page ID, no optional state. Every `Page` value has exactly one ID and exactly 4096 bytes of data. Invalid states are unrepresentable.

{{focus: none}} {{clear: slide}}

```code:page-struct lang=cpp
#pragma once
#include <array>
#include <cstdint>

using PageId = uint32_t;
constexpr uint32_t kPageSize = 4096;

// A Page is a PageId and a 4096-byte buffer. Nothing more.
struct Page {
  PageId id;
  std::array<uint8_t, kPageSize> data;
};
---
pageid-type: 5
kpagesize: 6
page-struct-body: 9-12
data-field: 11
```

# PageManager — owning the file

{{clear: slide}}

{{show: page-manager-decl typewriter 2s linear}} `PageManager` owns a file handle. Its job is to translate `PageId` values into byte offsets and perform the actual reads and writes. It knows nothing about B-trees or rows.

{{focus: explicit-ctor}} {{annotate: explicit-ctor "Opens file on construct"}} The constructor takes a file path and opens it. The destructor closes it. The file handle is a private implementation detail — no caller ever touches `FILE*`.

{{focus: read-sig}} {{annotate: read-sig "Returns page or error"}} `read(PageId id)` returns `DbResult<Page>` — either a fully populated page or a `DbError`. The caller never checks `errno`. Errors come through the type.

{{focus: write-sig}} {{annotate: write-sig "Const ref, no copy"}} `write(const Page& page)` takes the page by const reference. No copy. `DbResult<void>` means "succeeded or failed" with no return value on the success path.

{{focus: file-member}} {{annotate: file-member "Single owner, private"}} `FILE* file_` is the only private member. The class is the owner. No shared handles, no raw pointer escapes, no dangling.

{{focus: none}} {{clear: slide}}

{{show: page-manager-decl slide 0.3s}} The formula `offset = id × kPageSize` maps any `PageId` to its byte position in the file. This class enforces that formula for every read and write. Callers never compute offsets — they hand over IDs.

{{focus: none}} One class. One file. One invariant: every page lives at `id × kPageSize`. The rest of the storage engine trusts that invariant completely.

{{focus: none}} {{clear: slide}}

```code:page-manager-decl lang=cpp
#pragma once
#include <cstdio>
#include "db_result.h"
#include "page.h"

// PageManager owns a file handle and maps PageIds to byte offsets.
class PageManager {
 public:
  explicit PageManager(const char* path);
  ~PageManager();

  DbResult<Page>  read(PageId id);
  DbResult<void>  write(const Page& page);

 private:
  FILE* file_;
};
---
explicit-ctor: 9-10
read-sig: 12
write-sig: 13
file-member: 16
```

# Reading and writing a page

{{clear: slide}}

{{show: read-write-impl typewriter 2s linear}} Here are the two implementations. Both follow the same shape: compute the offset, seek, transfer 4096 bytes, return a result.

{{focus: read-offset}} {{annotate: read-offset "Widened before multiply"}} `static_cast<size_t>(id) * kPageSize` — cast to `size_t` first to avoid 32-bit overflow on files larger than 4 GB. The `id` alone fits in `uint32_t`, but the product may not.

{{focus: fseek-check}} {{annotate: fseek-check "Seek fails on bad offset"}} `std::fseek` returns non-zero on failure. That failure becomes `DbError::kIo` immediately. The guard clause fires and returns — no else, no further work.

{{focus: fread-call}} {{annotate: fread-call "Exact byte count required"}} `std::fread` reads up to `kPageSize` bytes. If it returns fewer — truncated file, hardware fault — that is also `kIo`. A partial page is an error, not a partial success.

{{focus: write-fn}} {{zoom: 1.2x}} {{annotate: write-fn "Mirror of read"}} `write` is the structural mirror of `read`. Compute offset, seek, `std::fwrite`. Same guard pattern. If `fwrite` writes fewer than `kPageSize` bytes, return `kIo`.

{{focus: return-void}} {{annotate: return-void "DbResult<void> success"}} `return {}` constructs a `std::expected<void, DbError>` in its value state — the success path with no payload. Callers use `if (!result)` to detect failure.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:read-write-impl lang=cpp
#include <cstdio>
#include "page_manager.h"

DbResult<Page> PageManager::read(PageId id) {
  size_t offset = static_cast<size_t>(id) * kPageSize;
  if (std::fseek(file_, static_cast<long>(offset), SEEK_SET) != 0) {
    return std::unexpected(DbError::kIo);
  }
  Page page{id, {}};
  size_t n = std::fread(page.data.data(), 1, kPageSize, file_);
  if (n != kPageSize) {
    return std::unexpected(DbError::kIo);
  }
  return page;
}

DbResult<void> PageManager::write(const Page& page) {
  size_t offset = static_cast<size_t>(page.id) * kPageSize;
  if (std::fseek(file_, static_cast<long>(offset), SEEK_SET) != 0) {
    return std::unexpected(DbError::kIo);
  }
  size_t n = std::fwrite(page.data.data(), 1, kPageSize, file_);
  if (n != kPageSize) {
    return std::unexpected(DbError::kIo);
  }
  return {};
}
---
read-offset: 5
fseek-check: 6-8
fread-call: 10-13
write-fn: 17-27
return-void: 26
```

# The page cache — keeping pages in memory

{{clear: slide}}

{{show: lru-diagram grow 0.5s spring}} Every `read` call hits the disk. A query that traverses a B-tree may touch the same pages dozens of times. Without a cache, that's dozens of `fread` calls for pages already in memory.

{{focus: head-slot}} {{annotate: head-slot "Most recently used"}} The head of the list holds the most recently used page. Whenever a page is accessed, it moves to the front.

{{focus: tail-slot}} {{annotate: tail-slot "Evict this one next"}} The tail holds the least recently used page. When the cache is full and a new page must be loaded, the tail is discarded.

{{focus: none}} The eviction strategy is LRU — least recently used. The assumption is temporal locality: a page accessed now is likely to be accessed again soon. Pages that haven't been touched in a while are evicted first.

{{focus: none}} {{clear: slide}}

{{show: page-cache-decl typewriter 2s linear}} `PageCache` wraps a `PageManager` and adds a fixed-capacity LRU layer on top.

{{focus: pm-ref}} {{annotate: pm-ref "Does not own manager"}} `PageManager& pm_` — a reference, not an owner. The `PageManager` lives independently. `PageCache` is a layer, not a container.

{{focus: lru-list}} {{annotate: lru-list "Front is most recent"}} `std::list<Page> lru_` — a doubly-linked list. `splice` moves any node to the front in O(1), which is exactly what LRU promotion requires.

{{focus: index-map}} {{annotate: index-map "O(1) lookup by ID"}} `std::unordered_map<PageId, std::list<Page>::iterator> index_` — maps a `PageId` directly to its node in `lru_`. Hit detection and promotion are both O(1).

{{focus: none}} The two structures stay in sync. Every node in `lru_` has an entry in `index_`. Every entry in `index_` points to a live node. The invariant is maintained by `get` alone — there is no other writer.

{{focus: none}} {{clear: slide}}

```data:lru-diagram type=linked-list
(head "page_7") -> (p3 "page_3") -> (p1 "page_1") -> (tail "page_9") -> null
---
head-slot: head
tail-slot: tail
```

```code:page-cache-decl lang=cpp
#pragma once
#include <cstddef>
#include <list>
#include <unordered_map>
#include "page_manager.h"

// Holds up to N pages; evicts the least recently used when full.
class PageCache {
 public:
  explicit PageCache(PageManager& pm, size_t capacity);
  DbResult<const Page*> get(PageId id);

 private:
  PageManager& pm_;
  size_t capacity_;
  std::list<Page> lru_;                            // front = most recent
  std::unordered_map<PageId, std::list<Page>::iterator> index_;
};
---
pm-ref: 14
lru-list: 16
index-map: 17
```

# The get implementation — hit, evict, load

{{clear: slide}}

{{show: page-cache-get typewriter 2s linear}} `PageCache::get` has three cases: hit, miss with room to spare, and miss at capacity. Each case falls through to the next only when the previous doesn't apply.

{{focus: hit-check}} {{zoom: 1.2x}} {{annotate: hit-check "O(1) hash lookup"}} `index_.find(id)` is O(1). If the page is already cached, we move it to the front of `lru_` and return a pointer. No disk access, no allocation.

{{zoom: 1x}} {{focus: splice-call}} {{zoom: 1.2x}} {{annotate: splice-call "O(1) list promotion"}} `lru_.splice(lru_.begin(), lru_, it->second)` moves the node to the front without touching the `index_` map — the iterator in the map still points to the same node, now at a new position.

{{zoom: 1x}} {{focus: evict-block}} {{zoom: 1.2x}} {{annotate: evict-block "Discard least recent"}} If `lru_.size() == capacity_`, we erase the tail from `index_` and pop it from `lru_`. One erase, one pop — O(1). The evicted page is gone.

{{zoom: 1x}} {{focus: load-block}} {{zoom: 1.2x}} {{annotate: load-block "Propagate IO failure"}} `pm_.read(id)` fetches the page from disk. If it fails, the error propagates immediately. The cache state is unchanged — no partial insertion.

{{zoom: 1x}} {{focus: push-front}} {{zoom: 1.2x}} {{annotate: push-front "Insert at head"}} On success, `push_front` places the new page at the most-recently-used position. Then `index_` is updated to point at `lru_.begin()`. Both structures stay in sync.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

{{split}}

{{show: page-cache-get none 0s}} {{show: lru-diagram none 0s}} Walk through a cache miss on a full cache. The tail is `page_9`.

{{focus: evict-block}} {{annotate: evict-block "Evict page_9"}} `index_.erase(lru_.back().id)` removes `page_9` from the map. `lru_.pop_back()` drops the tail node. The cache now has one free slot.

{{focus: load-block}} {{focus: push-front}} {{annotate: push-front "New page at front"}} `pm_.read` fetches the new page from disk. `push_front` puts it at the head. `index_[id] = lru_.begin()` registers it. The new page is now the most recently used.

{{focus: none}} {{unsplit}} {{clear: slide}}

```code:page-cache-get lang=cpp
#include "page_cache.h"

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
---
hit-check: 5-8
splice-call: 7
evict-block: 11-14
load-block: 16-19
push-front: 20-22
```

# Why LRU?

{{clear: slide}}

{{show: access-pattern typewriter 2s linear}} Database access is not random. Queries have structure, and structure implies locality.

{{focus: root-row}} {{annotate: root-row "Root touched every query"}} The root page of the B-tree is accessed on every single lookup. A cache that doesn't evict the root is correct by construction. LRU guarantees it — the root is always "recently used."

{{focus: leaf-row}} {{annotate: leaf-row "Leaf accessed, then gone"}} A leaf page is accessed once per query, then ignored for thousands of queries. LRU evicts it naturally — it migrates to the tail as newer pages push in at the head.

{{focus: scan-row}} {{annotate: scan-row "Sequential: pages stay warm"}} A full-table scan touches pages in order. Each page stays in cache while adjacent pages are being read. LRU keeps the working set warm for the duration of the scan.

{{focus: none}} {{clear: slide}}

{{show: lru-vs-alternatives typewriter 2s linear}} LRU is not the only eviction policy. Two alternatives are worth understanding.

{{focus: fifo-row}} {{annotate: fifo-row "Ignores recency"}} FIFO evicts the oldest-inserted page, regardless of how recently it was used. A page accessed 1000 times will be evicted the same as a page accessed once. It ignores recency entirely.

{{focus: lfu-row}} {{annotate: lfu-row "Frequency, not recency"}} LFU evicts the least-frequently-used page. It handles repeated access well but fails on scan patterns — a sequential scan inflates counts for pages that will never be used again, crowding out hot B-tree nodes.

{{focus: lru-row}} {{annotate: lru-row "Best for tree traversal"}} LRU hits the right tradeoff for a B-tree engine: root pages stay warm, cold pages fall off, scans don't pollute the cache permanently. That's why SQLite, InnoDB, and PostgreSQL all use LRU or LRU variants.

{{focus: none}} The cache is the last layer between the B-tree and the disk. Get the eviction policy right, and the engine is fast by default. Get it wrong, and every tree traversal hammers the disk.

{{focus: none}} {{clear: slide}}

```data:access-pattern type=array
["root page    │ accessed every query          │ always stays in cache under LRU", "leaf page    │ accessed once, then cold      │ migrates to tail, evicted later", "scan pages   │ sequential, accessed in order │ stay warm across adjacent reads"]
---
root-row: 0
leaf-row: 1
scan-row: 2
```

```data:lru-vs-alternatives type=array
["FIFO  │ evict oldest inserted   │ ignores recency — evicts hot pages equally", "LFU   │ evict least accessed    │ scan inflation pollutes frequency counts", "LRU   │ evict least recent      │ root stays warm, cold leaves fall off"]
---
fifo-row: 0
lfu-row: 1
lru-row: 2
```
