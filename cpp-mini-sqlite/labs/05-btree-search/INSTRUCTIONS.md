# Lab 05: B-tree search

## Context

In Lesson 5 you saw how a B-tree descends from root to leaf to find a key.
Your job is to implement `BTree::search`.

## What to implement

Open `src/solution.cpp` and implement:

- `DbResult<uint32_t> BTree::search(uint32_t key) const`

The class is declared in `src/solution.h`. Do not modify it.

## Spec

### search

1. Set `node_id = root_id_`.
2. Load the page: call `cache_.get(node_id)`. If it fails, propagate the error with
   `return std::unexpected(result.error())`.
3. Reinterpret the page bytes as a `BTreeNode`:
   ```cpp
   auto& node = *reinterpret_cast<const BTreeNode*>((*result)->data.data());
   ```
   This is safe because `Page::data` is declared `alignas(4)` and `BTreeNode`
   requires 4-byte alignment.
4. Find index `i`: scan left to right while `i < node.num_keys && key > node.keys[i]`.
5. If leaf (`node.is_leaf != 0`):
   - If `i < node.num_keys && node.keys[i] == key`, return `node.children[i]`.
   - Otherwise return `std::unexpected(DbError::kNotFound)`.
6. If internal: set `node_id = node.children[i]` and go to step 2.

The outer loop is iterative — no recursion.

## Running the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All five tests must pass. The stub always returns `kNotFound`, so four tests will
fail until you implement search (the `EmptyTreeReturnsNotFound` test passes with the
stub since it also expects `kNotFound`).
