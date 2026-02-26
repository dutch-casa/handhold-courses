#include "solution.h"

// TODO: store leaves at [n_, 2n_); build bottom-up by summing children
SegTree::SegTree(const std::vector<int>& data) : n_(static_cast<int>(data.size())) {
    tree_.assign(static_cast<std::size_t>(2 * n_), 0LL);
    // TODO: initialize leaves and compute internal nodes
}

// TODO: set tree_[index+n_] = val; walk up recalculating parents
void SegTree::update(int index, int val) {
    // TODO
}

// TODO: accumulate from both ends moving inward until l > r
long long SegTree::query(int left, int right) const {
    // TODO
    return 0LL;
}

// TODO: build BIT from data using n_ point updates
Fenwick::Fenwick(const std::vector<int>& data) : n_(static_cast<int>(data.size())) {
    bit_.assign(static_cast<std::size_t>(n_ + 1), 0LL);
    // TODO: add each element via update
}

// TODO: propagate delta up via i += i & (-i)
void Fenwick::update(int index, long long delta) {
    // TODO: convert to 1-indexed
}

// TODO: accumulate walking down via i -= i & (-i)
long long Fenwick::prefix_sum(int index) const {
    // TODO: convert to 1-indexed
    return 0LL;
}

long long Fenwick::range_sum(int left, int right) const {
    // TODO: prefix_sum(right) - prefix_sum(left-1)
    return 0LL;
}
