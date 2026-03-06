#include "solution.h"
#include <cstring>

BTree::BTree(PageManager& pm, PageCache& cache, PageId root_id, uint32_t page_count)
    : pm_(pm), cache_(cache), root_id_(root_id), next_page_id_(page_count) {}

void split_child(BTreeNode& parent, uint32_t idx,
                 BTreeNode& child, PageId right_id, BTreeNode& right) {
  const uint32_t mid = kOrder / 2;

  right.is_leaf  = child.is_leaf;
  right.num_keys = kOrder - mid - 1;
  for (uint32_t j = 0; j < right.num_keys; ++j)
    right.keys[j] = child.keys[mid + 1 + j];
  // Copy children unconditionally: internal nodes store page IDs, leaf nodes
  // store associated values — both live in the children array at the same indices.
  for (uint32_t j = 0; j <= right.num_keys; ++j)
    right.children[j] = child.children[mid + 1 + j];
  child.num_keys = mid;

  for (uint32_t j = parent.num_keys; j > idx; --j) {
    parent.keys[j]         = parent.keys[j - 1];
    parent.children[j + 1] = parent.children[j];
  }
  parent.keys[idx]         = child.keys[mid];
  parent.children[idx + 1] = right_id;
  ++parent.num_keys;
}

DbResult<void> BTree::insert(uint32_t key, uint32_t value) {
  // Helper: write a node back to disk at page `id`.
  auto flush = [&](PageId id, const BTreeNode& node) -> DbResult<void> {
    Page page{id, {}};
    std::memcpy(page.data.data(), &node, sizeof(node));
    return pm_.write(page);
  };

  // If root is full, grow the tree upward.
  // Aliasing note: as_node() returns a reference into the cache's std::list.
  // That reference stays valid as long as the page is not evicted. Each
  // cache_.get() call below risks eviction only when the cache is full (64
  // entries). The maximum tree height for kOrder=510 is ~4, well under 64,
  // so no eviction occurs during a single insert.
  {
    auto rr = cache_.get(root_id_);
    if (!rr) return std::unexpected(rr.error());
    BTreeNode& root = as_node(*rr);

    if (root.num_keys == kOrder) {
      auto right_id_r = alloc_page();
      if (!right_id_r) return std::unexpected(right_id_r.error());
      PageId right_id = *right_id_r;

      auto new_root_id_r = alloc_page();
      if (!new_root_id_r) return std::unexpected(new_root_id_r.error());
      PageId new_root_id = *new_root_id_r;

      auto right_r = cache_.get(right_id);
      if (!right_r) return std::unexpected(right_r.error());
      BTreeNode& right = as_node(*right_r);

      BTreeNode new_root{};
      new_root.is_leaf     = 0;
      new_root.num_keys    = 0;
      new_root.children[0] = root_id_;
      split_child(new_root, 0, root, right_id, right);

      if (auto r = flush(root_id_, root); !r) return r;
      if (auto r = flush(right_id, right); !r) return r;
      if (auto r = flush(new_root_id, new_root); !r) return r;
      root_id_ = new_root_id;
    }
  }

  // Descend, splitting full children before entering them.
  PageId cur = root_id_;
  while (true) {
    auto result = cache_.get(cur);
    if (!result) return std::unexpected(result.error());
    BTreeNode& node = as_node(*result);

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
      return flush(cur, node);
    }

    PageId child_page_id = node.children[i];
    auto child_result = cache_.get(child_page_id);
    if (!child_result) return std::unexpected(child_result.error());
    BTreeNode& child = as_node(*child_result);

    if (child.num_keys == kOrder) {
      auto right_id_r = alloc_page();
      if (!right_id_r) return std::unexpected(right_id_r.error());
      PageId right_id = *right_id_r;

      auto right_r = cache_.get(right_id);
      if (!right_r) return std::unexpected(right_r.error());
      BTreeNode& right = as_node(*right_r);

      split_child(node, i, child, right_id, right);
      if (key > node.keys[i]) ++i;

      if (auto r = flush(cur, node); !r) return r;
      if (auto r = flush(child_page_id, child); !r) return r;
      if (auto r = flush(right_id, right); !r) return r;
    }

    cur = node.children[i];
  }
}
