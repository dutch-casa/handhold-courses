---
title: B-trees
---

# Why binary search trees fail on disk

{{clear: slide}}

{{show: struct-compare typewriter 2s linear}} Three data structures all give O(log n) search. Only one is fast on disk. The difference is not algorithmic — it is physical.

{{focus: sorted-row}} {{annotate: sorted-row "Insert shifts O(n) elements"}} A sorted array finds any key in O(log n) comparisons via binary search. But inserting a key into the middle requires shifting every element to its right. For a million rows, that is a million writes — before any disk I/O is even considered.

{{focus: bst-row}} {{annotate: bst-row "Pointer chase: one read per level"}} A binary search tree fixes insertion: O(log n) for both search and insert. The problem is pointer chasing. Each node is an independent heap allocation. Following a pointer to a child node means fetching that node from disk. For 1 billion rows, the tree is log₂(1B) ≈ 30 levels deep.

{{focus: bst-row}} A random disk read on a spinning drive takes about 5 milliseconds. 30 levels × 5 ms = 150 milliseconds to find a single key. That is not a database — it is a liability.

{{focus: btree-row}} {{annotate: btree-row "Wide nodes, shallow depth"}} A B-tree fixes the disk problem by packing many keys into one node. The branching factor — how many children each node has — determines the depth. More children per node means fewer levels means fewer disk reads.

{{focus: none}} {{clear: slide}}

```data:struct-compare type=array
["sorted array │ O(log n) search, O(n) insert │ fast lookup, catastrophic insert on disk", "BST           │ O(log n) both                │ fast in RAM — 30 disk reads per lookup at 1B rows", "B-tree        │ O(log n) both                │ wide nodes → 3-4 levels on disk → 4 reads per query"]
---
sorted-row: 0
bst-row: 1
btree-row: 2
```

# Fanout — the only number that matters

{{clear: slide}}

{{show: fanout-compare slide 0.5s ease-out}} This bar chart is the entire argument for B-trees. Each bar is the tree depth — the number of disk reads — to find any key in a billion-row table.

{{focus: fanout-2-row}} {{annotate: fanout-2-row "Binary tree: 30 reads"}} Fanout 2 is a binary search tree. At 1 billion rows, depth is log₂(1B) ≈ 30. Thirty disk reads at 5ms each is 150 milliseconds per query. That is not a database. That is a disaster.

{{focus: fanout-16-row}} {{annotate: fanout-16-row "Fanout 16: 8 reads"}} Fanout 16 shrinks the depth to log₁₆(1B) ≈ 8. Meaningfully better — but still 40ms. An interactive product cannot absorb that latency on every query.

{{focus: fanout-510-row}} {{annotate: fanout-510-row "Four reads — always"}} Fanout 510 pushes depth to log₅₁₀(1B) ≈ 4. Four disk reads covers a billion rows. And at 4096 bytes per node, those reads are cache-friendly. This is why every production database uses a B-tree.

{{focus: none}} {{clear: slide}}

```chart:fanout-compare type=bar
Fanout 2 (binary tree): 30
Fanout 16: 8
Fanout 510 (B-tree): 4
---
fanout-2-row: Fanout 2 (binary tree)
fanout-16-row: Fanout 16
fanout-510-row: Fanout 510 (B-tree)
```

{{show: btree-shape grow 0.5s spring}} This is what three levels of a B-tree look like. One root. A tier of internal nodes. Then the leaves, where all data lives.

{{focus: root-node}} {{annotate: root-node "One root, always in cache"}} The root is accessed on every query. In practice it stays in the page cache permanently. Its keys divide the entire keyspace into ranges — one range per child.

{{focus: i1-node}} {{focus: i2-node}} {{focus: i3-node}} {{annotate: i1-node "Route, don't store"}} Internal nodes hold keys and child page IDs — enough to direct a search down. They do not hold row data. At fanout 510, there are roughly 511 internal nodes at level 1.

{{focus: l1-node}} {{focus: l2-node}} {{focus: l3-node}} {{focus: l4-node}} {{focus: l5-node}} {{focus: l6-node}} {{annotate: l1-node "All data lives here"}} Leaves hold the actual values. Every key-value pair in the database lives in a leaf. At fanout 510, three levels hold about 133 million rows. Four levels — one more disk read — covers 68 billion.

{{focus: none}} {{clear: slide}}

```data:btree-shape type=b-tree order=3
(root: 100, 300)
  (i1: 20, 60)
    (l1: 5, 10)
    (l2: 30, 40)
  (i2: 150, 220)
    (l3: 110, 130)
    (l4: 160, 200)
  (i3: 380, 450)
    (l5: 320, 360)
    (l6: 400, 420)
---
root-node: root
i1-node: i1
i2-node: i2
i3-node: i3
l1-node: l1
l2-node: l2
l3-node: l3
l4-node: l4
l5-node: l5
l6-node: l6
```

# Node layout — one node, one page

{{clear: slide}}

{{show: btree-node typewriter 2s linear}} {{zoom: 1.2x}} Every B-tree node lives in exactly one page. The `BTreeNode` struct is designed to fill a 4096-byte page — the same page size `PageManager` already manages.

{{focus: korder}} {{annotate: korder "510 keys per node"}} `kOrder = 510` is the maximum number of keys in a single node. The math: 510 keys × 4 bytes + 511 children × 4 bytes + 8 bytes for the header fields = 4092 bytes. That fits inside a 4096-byte page with 4 bytes to spare.

{{focus: num-keys-field}} {{annotate: num-keys-field "Current occupancy"}} `num_keys` tracks how many keys are currently stored. A new node starts at zero and grows toward `kOrder`. During insertion, this count drives the decision to split.

{{focus: is-leaf-field}} {{annotate: is-leaf-field "One struct, two modes"}} `is_leaf` is a `uint32_t` flag, not a virtual dispatch or separate type. `uint32_t` — not `bool` — keeps every field 4-byte aligned and eliminates padding between `is_leaf` and `keys[]`. Internal nodes route searches; leaf nodes store values.

{{focus: keys-array}} {{annotate: keys-array "Always sorted ascending"}} `keys[]` is always sorted. Maintaining sort order on insert enables binary search within a node. With 510 keys, binary search takes at most ⌈log₂(510)⌉ = 9 comparisons to find the right position.

{{focus: children-array}} {{annotate: children-array "Context-dependent meaning"}} `children[]` holds one more entry than `keys[]`. In an internal node, `children[i]` is the page ID of the subtree containing all keys less than `keys[i]`. In a leaf, `children[i]` is the value stored at `keys[i]`.

{{focus: size-assert}} {{annotate: size-assert "Compile-time page check"}} `static_assert` fires at compile time if the struct overflows a page. Changing `kOrder` without rechecking the math produces a compile error, not a silent runtime corruption.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:btree-node lang=cpp
#pragma once
#include "page.h"

constexpr uint32_t kOrder = 510;  // max keys per node; fits in one 4096-byte page

struct BTreeNode {
  uint32_t num_keys;              // 0..kOrder (current key count)
  uint32_t is_leaf;               // uint32_t keeps 4-byte alignment; no padding before keys[]
  uint32_t keys[kOrder];          // sorted key array
  uint32_t children[kOrder + 1];  // page IDs (internal) or values (leaf)
};
static_assert(sizeof(BTreeNode) <= kPageSize, "BTreeNode exceeds page size");
---
korder: 4
num-keys-field: 7
is-leaf-field: 8
keys-array: 9
children-array: 10
size-assert: 12
```

# Search — root to leaf

{{clear: slide}}

{{show: search-tree grow 0.5s spring}} Search descends from root to leaf. This tree has one root and three leaf nodes. Search for key 30.

{{focus: search-root}} {{annotate: search-root "Start here"}} Every search starts at the root. The root holds keys 20 and 40. The key 30 falls between them, so the search follows the middle child pointer.

{{flow: search-path}} {{focus: search-mid}} {{annotate: search-mid "One disk read"}} Following the pointer fetches the middle leaf from disk — one page read, all its keys at once. Key 30 is here. Search complete.

{{focus: search-left}} {{annotate: search-left "Never visited"}} The left and right subtrees are never loaded. A narrow key range eliminates entire subtrees at each level. This is how a 4-level tree at fanout 510 searches 68 billion rows with 4 disk reads.

{{flow: none}} {{focus: none}} {{clear: slide}}

```data:search-tree type=b-tree order=4
(search-root: 20, 40)
  (search-left: 5, 10, 15)
  (search-mid: 25, 30, 35)
  (search-right: 45, 50, 55)
---
search-root: search-root
search-left: search-left
search-mid: search-mid
search-right: search-right
search-path: search-root, search-mid
```

{{show: btree-search typewriter 2s linear}} {{zoom: 1.2x}} Search is an iterative descent from root to leaf. No recursion, no stack growth — just a loop that loads one page per iteration.

{{focus: outer-loop}} {{zoom: 1.2x}} {{annotate: outer-loop "One iteration per level"}} The outer `while (true)` loop runs once per tree level. At fanout 510 and 1 billion rows, it runs four times. Each iteration loads one page and advances to the next.

{{zoom: 1x}} {{focus: load-node}} {{zoom: 1.2x}} {{annotate: load-node "One page read per level"}} `cache_.get(node_id)` fetches the page from the `PageCache`. If it is already in cache, no disk I/O occurs. If not, one disk read brings in 4096 bytes — the entire node, all 510 keys at once.

{{zoom: 1x}} {{focus: reinterpret}} {{zoom: 1.2x}} {{annotate: reinterpret "Page bytes are the node"}} `reinterpret_cast<const BTreeNode*>` treats the raw page bytes as a `BTreeNode` directly. No deserialization, no copy, no allocation. This is safe only because `Page::data` is declared `alignas(4)` — `BTreeNode` requires 4-byte alignment, and without it the cast is undefined behavior on strict-alignment architectures.

{{zoom: 1x}} {{focus: inner-scan}} {{zoom: 1.2x}} {{annotate: inner-scan "Find child index"}} The inner `while` loop scans forward until it finds the first index `i` where `keys[i] >= key`. This is a linear scan — acceptable here because `kOrder` is small enough that a simple loop is faster than a binary search setup in practice.

{{zoom: 1x}} {{focus: leaf-check}} {{zoom: 1.2x}} {{annotate: leaf-check "Exact match or absent"}} At a leaf node, the key is either at position `i` or absent. If `keys[i] == key`, return the stored value. Otherwise, return `kNotFound`. There is no third outcome.

{{zoom: 1x}} {{focus: child-descent}} {{zoom: 1.2x}} {{annotate: child-descent "Follow the child pointer"}} At an internal node, `children[i]` is the page ID of the subtree that could contain `key`. Assign it to `node_id` and let the loop continue. The next iteration fetches that page.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:btree-search lang=cpp
DbResult<uint32_t> BTree::search(uint32_t key) const {
  PageId node_id = root_id_;
  while (true) {
    auto result = cache_.get(node_id);
    if (!result) return std::unexpected(result.error());
    auto& node = *reinterpret_cast<const BTreeNode*>((*result)->data.data());

    // Find first i where keys[i] >= key.
    uint32_t i = 0;
    while (i < node.num_keys && key > node.keys[i]) ++i;

    if (node.is_leaf) {
      if (i < node.num_keys && node.keys[i] == key) return node.children[i];
      return std::unexpected(DbError::kNotFound);
    }
    node_id = node.children[i];
  }
}
---
outer-loop: 3
load-node: 4-5
reinterpret: 6
inner-scan: 9-10
leaf-check: 12-14
child-descent: 16
```

# Insertion — find, split, propagate

{{clear: slide}}

{{show: pre-split grow 0.5s spring}} Before inserting into a full leaf, the leaf must be split. This is the moment where one node becomes two and the parent gains a new key.

{{focus: full-leaf}} {{annotate: full-leaf "At capacity — no room"}} The leaf holds four keys: 10, 20, 30, 40. It is full. Inserting key 25 here would overflow the node.

{{focus: split-parent}} {{annotate: split-parent "Parent absorbs the median"}} The median key — 20 — is promoted to the parent. The parent gains one key and one new child pointer. The leaf splits into two halves: keys below the median, and keys above.

{{focus: none}} {{clear: slide}}

```data:pre-split type=b-tree order=4
(split-parent: 50)
  (full-leaf: 10, 20, 30, 40)
  (sib: 60, 70)
---
split-parent: split-parent
full-leaf: full-leaf
sib: sib
```

{{show: post-split grow 0.5s spring}} After the split the parent has grown by one key — the promoted median. The original leaf is now two smaller leaves, each with room to accept new insertions.

{{focus: post-parent}} {{annotate: post-parent "Median promoted"}} The parent now holds key 20 alongside the existing keys. It gained one key and one child pointer — one node became two children.

{{focus: post-left}} {{annotate: post-left "Left half"}} Keys below the median — just 10 here — stay in the left child. This leaf now has room for up to three more keys before the next split.

{{focus: post-right}} {{annotate: post-right "Right half, ready for 25"}} Keys above the median — 30 and 40 — move to the right child. The new key 25 will be inserted here.

{{focus: none}} {{clear: slide}}

```data:post-split type=b-tree order=4
(post-parent: 20, 50)
  (post-left: 10)
  (post-right: 30, 40)
  (post-sib: 60, 70)
---
post-parent: post-parent
post-left: post-left
post-right: post-right
post-sib: post-sib
```

{{show: split-child typewriter 2s linear}} {{zoom: 1.2x}} `split_child` is the operation that performs the split. It receives the full child and its parent, and produces two children with the median promoted.

{{focus: mid-compute}} {{zoom: 1.2x}} {{annotate: mid-compute "Median divides the node"}} `mid = kOrder / 2` is the median index. The key at `mid` is the pivot. Keys below `mid` stay in the left child; keys above `mid` move to the right child; the key at `mid` is promoted to the parent.

{{zoom: 1x}} {{focus: copy-right}} {{zoom: 1.2x}} {{annotate: copy-right "Right half to new page"}} The right half of `child` — keys from `mid+1` onward, plus the corresponding children if `child` is an internal node — is copied into `right`, which lives on a freshly allocated page.

{{zoom: 1x}} {{focus: shrink-left}} {{zoom: 1.2x}} {{annotate: shrink-left "Left child truncated"}} `child.num_keys = mid` shrinks the left child to only the keys below the median. The median key itself is not in either child after the split — it belongs to the parent.

{{zoom: 1x}} {{focus: promote-median}} {{zoom: 1.2x}} {{annotate: promote-median "Median ascends to parent"}} The loop shifts the parent's existing keys and child pointers right by one position to open a slot at `idx`. `child.keys[mid]` goes into `parent.keys[idx]`; the new right page's ID goes into `parent.children[idx+1]`. The parent gains one key and one child pointer.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

{{show: btree-insert typewriter 2s linear}} {{zoom: 1.2x}} With `split_child` defined, the top-level insert descends from root to leaf exactly once — splitting every full node along the way.

{{focus: node-of-helper}} {{zoom: 1.2x}} {{annotate: node-of-helper "Reinterpret cast helper"}} `node_of` is a local lambda that reinterprets a `const Page*` as a mutable `BTreeNode&`. The `const_cast` is safe because pages owned by `PageCache::lru_` are non-const — the cache returns `const Page*` only to discourage external mutation.

{{zoom: 1x}} {{focus: root-full}} {{zoom: 1.2x}} {{annotate: root-full "Tree grows upward"}} If the root is full, a new root is created first. The old root becomes the new root's first child and is split immediately. The tree grows one level taller at the top — not at the leaves. All leaves remain at the same depth.

{{zoom: 1x}} {{focus: descend-loop}} {{zoom: 1.2x}} {{annotate: descend-loop "Single pass, root to leaf"}} The descent loop loads each node, finds the correct child index `i`, and advances. It never revisits a node. The path from root to leaf is traversed once.

{{zoom: 1x}} {{focus: split-before-enter}} {{zoom: 1.2x}} {{annotate: split-before-enter "Split before entering"}} Before descending into child `i`, the loop checks whether that child is full. If it is, `split_child` fires while still in the parent — the child is split before being entered. Every node on the descent path has room, guaranteed.

{{zoom: 1x}} {{focus: leaf-insert}} {{zoom: 1.2x}} {{annotate: leaf-insert "Shift right, write, done"}} At the leaf, existing keys above position `i` shift right by one. The new key and value are written at `i`. `num_keys` increments. The insertion is complete with no backtracking.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:split-child lang=cpp
// Split the full child at children[idx] in parent.
// new_page receives the right half; parent gains the promoted median key.
// Precondition: child.num_keys == kOrder.
void split_child(BTreeNode& parent, uint32_t idx,
                 BTreeNode& child, PageId new_page_id, BTreeNode& right) {
  const uint32_t mid = kOrder / 2;

  // Copy right half into right node.
  right.is_leaf   = child.is_leaf;
  right.num_keys  = kOrder - mid - 1;
  for (uint32_t j = 0; j < right.num_keys; ++j)
    right.keys[j] = child.keys[mid + 1 + j];
  if (!child.is_leaf) {
    for (uint32_t j = 0; j <= right.num_keys; ++j)
      right.children[j] = child.children[mid + 1 + j];
  }
  child.num_keys = mid;

  // Promote median into parent.
  for (uint32_t j = parent.num_keys; j > idx; --j) {
    parent.keys[j]         = parent.keys[j - 1];
    parent.children[j + 1] = parent.children[j];
  }
  parent.keys[idx]         = child.keys[mid];
  parent.children[idx + 1] = new_page_id;
  ++parent.num_keys;
}
---
mid-compute: 6
copy-right: 9-16
shrink-left: 17
promote-median: 20-26
```

```code:btree-insert lang=cpp
DbResult<void> BTree::insert(uint32_t key, uint32_t value) {
  // Reinterpret a cache page as a mutable BTreeNode.
  auto node_of = [](const Page* p) -> BTreeNode& {
    return *reinterpret_cast<BTreeNode*>(const_cast<uint8_t*>(p->data.data()));
  };

  // Root full: create new root, split old root as its first child.
  {
    auto root_result = cache_.get(root_id_);
    if (!root_result) return std::unexpected(root_result.error());
    BTreeNode& root = node_of(*root_result);
    if (root.num_keys == kOrder) {
      PageId new_root_id = allocate_page();
      BTreeNode& new_root = node_of(cache_.get(new_root_id).value());
      new_root.is_leaf     = 0;
      new_root.num_keys    = 0;
      new_root.children[0] = root_id_;
      PageId right_id = allocate_page();
      split_child(new_root, 0, root, right_id, node_of(cache_.get(right_id).value()));
      root_id_ = new_root_id;
    }
  }

  // Descend, splitting any full child before entering it.
  PageId cur = root_id_;
  while (true) {
    auto result = cache_.get(cur);
    if (!result) return std::unexpected(result.error());
    BTreeNode& node = node_of(*result);

    uint32_t i = node.num_keys;
    while (i > 0 && key < node.keys[i - 1]) --i;

    if (node.is_leaf) {
      for (uint32_t j = node.num_keys; j > i; --j) {
        node.keys[j]     = node.keys[j - 1];
        node.children[j] = node.children[j - 1];
      }
      node.keys[i]     = key;
      node.children[i] = value;
      ++node.num_keys;
      return {};
    }

    auto child_result = cache_.get(node.children[i]);
    if (!child_result) return std::unexpected(child_result.error());
    BTreeNode& child = node_of(*child_result);
    if (child.num_keys == kOrder) {
      PageId right_id = allocate_page();
      split_child(node, i, child, right_id, node_of(cache_.get(right_id).value()));
      if (key > node.keys[i]) ++i;
    }
    cur = node.children[i];
  }
}
---
node-of-helper: 3-5
root-full: 8-22
descend-loop: 24-54
leaf-insert: 34-43
split-before-enter: 45-53
```
