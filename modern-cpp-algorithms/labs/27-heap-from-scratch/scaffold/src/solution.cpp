#include "solution.h"
#include <vector>

// TODO: append val, then restore heap property upward
void MinHeap::push(int val) {
    // TODO: data_.push_back(val); sift_up(data_.size() - 1);
}

// TODO: swap root with last, pop last, sift_down from root
int MinHeap::pop() {
    // TODO
    return 0;
}

int MinHeap::top() const {
    // TODO: return data_[0];
    return 0;
}

int MinHeap::size() const {
    return static_cast<int>(data_.size());
}

bool MinHeap::empty() const {
    return data_.empty();
}

// TODO: while idx > 0 and data_[idx] < data_[parent], swap and move up
void MinHeap::sift_up(int idx) {
    // TODO
}

// TODO: while a child is smaller than current, swap with smaller child
void MinHeap::sift_down(int idx) {
    // TODO
}

// TODO: use std::nth_element with std::greater<> to find kth largest
int kth_largest(std::vector<int> nums, int k) {
    // TODO
    return 0;
}

// TODO: use a min-heap of {value, list_idx, elem_idx} tuples
std::vector<int> merge_k_sorted(const std::vector<std::vector<int>>& lists) {
    // TODO
    return {};
}
