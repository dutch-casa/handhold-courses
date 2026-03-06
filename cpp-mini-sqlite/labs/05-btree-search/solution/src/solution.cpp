#include "solution.h"

BTree::BTree(PageCache& cache, PageId root_id)
    : cache_(cache), root_id_(root_id) {}

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
