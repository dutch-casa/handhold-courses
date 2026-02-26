#pragma once
#include <vector>

// Min-heap backed by a std::vector<int>.
// Invariant: data[0] is always the smallest element.
class MinHeap {
public:
    void push(int val);
    int  pop();          // Remove and return the minimum; UB if empty
    int  top() const;    // Peek at minimum without removing; UB if empty
    int  size() const;
    bool empty() const;

private:
    std::vector<int> data_;

    void sift_up(int idx);
    void sift_down(int idx);
};

// Kth largest element in nums (1-indexed: k=1 means the largest).
int kth_largest(std::vector<int> nums, int k);

// Merge k sorted vectors into a single sorted vector.
std::vector<int> merge_k_sorted(const std::vector<std::vector<int>>& lists);
