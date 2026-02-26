# Lab 34 — Range Query Engine

Two classical data structures for answering range sum queries with point updates in O(log n). The segment tree stores intervals explicitly; the Fenwick tree (BIT) is more compact and uses index arithmetic.

## Tasks

- [ ] Implement `SegTree` — construct from a vector; support O(log n) point update and O(log n) range sum query
- [ ] Implement `Fenwick` — construct from a vector; support O(log n) point delta update, O(log n) prefix sum, and O(log n) range sum

## Hints

- `SegTree`: internal array of size `2*n`; leaves at indices `[n, 2n)`; parent of node `i` is `i/2`; children of `i` are `2i` and `2i+1`; update propagates up; query accumulates from leaves inward
- `SegTree::update(i, val)`: sets the leaf at logical index `i` to `val` (not a delta); update tree[i+n] = val, then walk up: tree[parent] = tree[left] + tree[right]
- `SegTree::query(l, r)`: expand [l, r] outward — if left boundary is odd (right child), accumulate and move in; if right boundary is even (left child), accumulate and move in
- `Fenwick`: 1-indexed internally; `update(i, delta)` adds delta to index i and propagates to ancestors via `i += i & (-i)`; `prefix_sum(i)` walks down via `i -= i & (-i)`
- `Fenwick::range_sum(l, r)`: `prefix_sum(r) - prefix_sum(l-1)`

Use `Run Tests` to check your progress.
