#include "solution.h"

SegTree::SegTree(const std::vector<int>& data) : n_(static_cast<int>(data.size())) {
    tree_.assign(static_cast<std::size_t>(2 * n_), 0LL);
    // Place leaves
    for (int i = 0; i < n_; ++i) {
        tree_[static_cast<std::size_t>(n_ + i)] = static_cast<long long>(data[static_cast<std::size_t>(i)]);
    }
    // Build internal nodes bottom-up
    for (int i = n_ - 1; i >= 1; --i) {
        auto si = static_cast<std::size_t>(i);
        tree_[si] = tree_[2 * si] + tree_[2 * si + 1];
    }
}

void SegTree::update(int index, int val) {
    // Set leaf, propagate up
    std::size_t i = static_cast<std::size_t>(n_ + index);
    tree_[i] = static_cast<long long>(val);
    while (i > 1) {
        i /= 2;
        tree_[i] = tree_[2 * i] + tree_[2 * i + 1];
    }
}

long long SegTree::query(int left, int right) const {
    long long result = 0LL;
    // Convert to leaf indices, then expand inward
    std::size_t l = static_cast<std::size_t>(n_ + left);
    std::size_t r = static_cast<std::size_t>(n_ + right + 1); // exclusive
    while (l < r) {
        if (l & 1) { result += tree_[l]; ++l; }
        if (r & 1) { --r; result += tree_[r]; }
        l /= 2;
        r /= 2;
    }
    return result;
}

Fenwick::Fenwick(const std::vector<int>& data) : n_(static_cast<int>(data.size())) {
    bit_.assign(static_cast<std::size_t>(n_ + 1), 0LL);
    for (int i = 0; i < n_; ++i) {
        update(i, static_cast<long long>(data[static_cast<std::size_t>(i)]));
    }
}

void Fenwick::update(int index, long long delta) {
    // Convert to 1-indexed
    for (int i = index + 1; i <= n_; i += i & (-i)) {
        bit_[static_cast<std::size_t>(i)] += delta;
    }
}

long long Fenwick::prefix_sum(int index) const {
    long long sum = 0LL;
    // Convert to 1-indexed
    for (int i = index + 1; i > 0; i -= i & (-i)) {
        sum += bit_[static_cast<std::size_t>(i)];
    }
    return sum;
}

long long Fenwick::range_sum(int left, int right) const {
    if (left == 0) return prefix_sum(right);
    return prefix_sum(right) - prefix_sum(left - 1);
}
