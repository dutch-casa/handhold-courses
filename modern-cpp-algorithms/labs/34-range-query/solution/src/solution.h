#pragma once
#include <vector>

// Segment tree for range sum queries and point updates.
// Indices are 0-based in the public interface.
class SegTree {
public:
    explicit SegTree(const std::vector<int>& data);

    // Set the value at index i to val (not a delta).
    void update(int index, int val);

    // Return the sum of elements in [left, right] inclusive.
    long long query(int left, int right) const;

private:
    int n_;
    std::vector<long long> tree_;
};

// Fenwick tree (Binary Indexed Tree) for prefix sums and point delta updates.
// Indices are 0-based in the public interface.
class Fenwick {
public:
    explicit Fenwick(const std::vector<int>& data);

    // Add delta to element at index i.
    void update(int index, long long delta);

    // Return the sum of elements in [0, index] inclusive.
    long long prefix_sum(int index) const;

    // Return the sum of elements in [left, right] inclusive.
    long long range_sum(int left, int right) const;

private:
    int n_;
    std::vector<long long> bit_;
};
