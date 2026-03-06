# Lab 06: B-tree insertion

## Context

In Lesson 5 you saw how B-tree insertion splits full nodes on the way down.
Your job is to implement `split_child` and `BTree::insert`.

`BTree::search`, `BTree::alloc_page()`, and `BTree::as_node()` are
pre-implemented in `src/helpers.cpp`. You only touch `src/solution.cpp`.

## What to implement

Open `src/solution.cpp` and implement:

- `void split_child(BTreeNode& parent, uint32_t idx, BTreeNode& child, PageId right_id, BTreeNode& right)`
- `DbResult<void> BTree::insert(uint32_t key, uint32_t value)`

The class is declared in `src/solution.h`. Do not modify it.

## Spec

### split_child

Precondition: `child.num_keys == kOrder`.

1. `mid = kOrder / 2`
2. Set `right.is_leaf = child.is_leaf` and `right.num_keys = kOrder - mid - 1`.
3. Copy `child.keys[mid+1 .. kOrder-1]` to `right.keys[0 ..]`.
4. Copy `child.children[mid+1 .. kOrder]` to `right.children[0 ..]`.
   (Both leaf nodes — where `children[i]` is the value for `keys[i]` — and internal
   nodes — where `children[i]` is a page ID — must copy their children here.)
5. Set `child.num_keys = mid` (shrinks the left child; median key is now logically
   removed from child but still readable at `child.keys[mid]`).
6. Shift `parent.keys[idx..num_keys-1]` right by one.
7. Shift `parent.children[idx+1..num_keys]` right by one.
8. `parent.keys[idx] = child.keys[mid]` (promote median).
9. `parent.children[idx+1] = right_id`.
10. `++parent.num_keys`.

### insert

Use the split-on-the-way-down strategy:

1. Load root via `cache_.get(root_id_)`. Use `as_node()` to get a mutable reference.
2. If root is full (`num_keys == kOrder`):
   - `right_id = alloc_page()`, `new_root_id = alloc_page()`.
   - Load `right_id` via `cache_.get()` to get the right-child node.
   - Declare `new_root` as a local `BTreeNode{}` on the stack (not cache-loaded — it
     is written to `new_root_id` via flush at the end of this block).
   - Initialize `new_root`: `is_leaf=0, num_keys=0, children[0]=root_id_`.
   - Call `split_child(new_root, 0, root, right_id, right_node)`.
   - Flush all three pages to disk: `flush(root_id_, root)`, `flush(right_id, right)`,
     `flush(new_root_id, new_root)`.
   - Set `root_id_ = new_root_id`.
3. Descend from `root_id_`:
   - Load current node. Find index `i` (scan backwards: `i = num_keys`, decrement while
     `i > 0 && key < keys[i-1]`).
   - **If leaf**: shift right, write key+value at `i`, `++num_keys`, `flush(cur, node)`, return.
   - **If internal**: load `children[i]`. If full, split it (allocate right page,
     call `split_child`, flush all three modified pages, adjust `i` if `key > keys[i]`).
     Descend into `children[i]`.

### Helper: flush

To write a modified node back to disk, build a `Page` and write it:
```cpp
Page page{id, {}};
std::memcpy(page.data.data(), &node, sizeof(node));
pm_.write(page);
```

### Pre-implemented helpers

- `BTree::as_node(const Page* p)` — reinterprets page bytes as mutable `BTreeNode&`.
  Safe because `Page::data` is `alignas(4)`.
- `BTree::alloc_page()` — allocates a new zeroed page, returns its `PageId`.
- `BTree::search()` — same iterative descent as Lab 5.

## Running the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All four tests must pass. The stub's `split_child` does nothing and `insert` returns
`kNotFound`, so all four tests fail until you implement both functions.
