#include "solution.h"

BTree::BTree(PageManager& pm, PageCache& cache, PageId root_id, uint32_t page_count)
    : pm_(pm), cache_(cache), root_id_(root_id), next_page_id_(page_count) {}

// TODO: implement per INSTRUCTIONS.md spec.
void split_child(BTreeNode& parent, uint32_t idx,
                 BTreeNode& child, PageId right_id, BTreeNode& right) {
  (void)parent; (void)idx; (void)child; (void)right_id; (void)right;
}

// TODO: implement per INSTRUCTIONS.md spec.
DbResult<void> BTree::insert(uint32_t key, uint32_t value) {
  (void)key; (void)value;
  return std::unexpected(DbError::kNotFound);
}
