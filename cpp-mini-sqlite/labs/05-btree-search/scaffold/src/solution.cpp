#include "solution.h"

BTree::BTree(PageCache& cache, PageId root_id)
    : cache_(cache), root_id_(root_id) {}

DbResult<uint32_t> BTree::search(uint32_t key) const {
  // TODO: implement iterative descent from root_id_ to leaf.
  // See INSTRUCTIONS.md for the full spec.
  (void)key;
  return std::unexpected(DbError::kNotFound);
}
