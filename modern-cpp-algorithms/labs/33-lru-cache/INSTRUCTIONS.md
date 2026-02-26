# Lab 33 — LRU Cache

Implement a Least Recently Used cache with O(1) `get` and `put`. The trick is combining a doubly-linked list (for O(1) move-to-front and eviction) with a hash map (for O(1) lookup by key).

## Tasks

- [ ] Implement `LRUCache(capacity)` — construct a cache with the given maximum number of entries
- [ ] Implement `get(key)` — return the cached value or `-1` if absent; move the accessed entry to most-recently-used position
- [ ] Implement `put(key, value)` — insert or update an entry; if inserting would exceed capacity, evict the least recently used entry first

## Hints

- Store entries in a `std::list<std::pair<int,int>>` (front = most recent, back = least recent)
- The map stores `std::list<std::pair<int,int>>::iterator` so you can splice in O(1)
- `get`: look up iterator in map; if found, use `list.splice(list.begin(), list, it)` to move it to front; return value
- `put`: if key exists, update value and splice to front; if key is new and at capacity, erase the back element from both list and map before inserting

Use `Run Tests` to check your progress.
