# Lab 03b: PageCache — LRU cache

## Context

In Lab 3 you built `PageManager`, which reads and writes pages from disk.
Every call hits the file. A B-tree query may access the same page dozens of times.
A cache avoids those redundant reads.

A provided `PageManager` implementation lives in `src/helpers.cpp`. You call it,
not rewrite it.

## What to implement

Open `src/solution.cpp` and implement:

```cpp
DbResult<const Page*> PageCache::get(PageId id);
```

The `PageCache` struct, all types, and `PageManager` are declared in `src/solution.h`.

## Spec

`get` has three cases, evaluated in order:

### 1. Cache hit

1. Search `index_` for `id` using `index_.find(id)`.
2. If found (iterator `it != index_.end()`):
   a. Promote: `lru_.splice(lru_.begin(), lru_, it->second)`.
      This moves the node to the front in O(1). The iterator in `index_` still
      points to the same node — no update needed.
   b. Return `&lru_.front()`.

### 2. Cache miss — evict if at capacity

3. If `lru_.size() == capacity_`:
   a. `index_.erase(lru_.back().id)` — remove the LRU entry from the map.
   b. `lru_.pop_back()` — drop the LRU node from the list.

### 3. Load from disk

4. Call `pm_.read(id)`.
5. If it returns an error, propagate: `return std::unexpected(result.error())`.
6. `lru_.push_front(std::move(*result))` — insert the new page at the front.
7. `index_[id] = lru_.begin()` — register it in the map.
8. Return `&lru_.front()`.

## Data structure invariant

After every call to `get`:
- Every node in `lru_` has exactly one entry in `index_`.
- Every entry in `index_` points to a live node in `lru_`.

The `splice` in step 2a preserves this invariant because it moves the node
without invalidating the iterator stored in `index_`.

## Why std::list?

`std::list::splice` moves a node in O(1) without invalidating any iterators.
This is the only standard container that enables O(1) LRU promotion.
`std::unordered_map<PageId, std::list<Page>::iterator>` gives O(1) hit detection.

## Running the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All four tests must pass. The stub returns kIo for every get call, so three tests
will fail until you implement the function.
