#include "solution.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>

void MinHeap::push(int val) {
    data_.push_back(val);
    sift_up(static_cast<int>(data_.size()) - 1);
}

int MinHeap::pop() {
    int result = data_[0];
    data_[0] = data_.back();
    data_.pop_back();
    if (!data_.empty()) sift_down(0);
    return result;
}

int MinHeap::top() const {
    return data_[0];
}

int MinHeap::size() const {
    return static_cast<int>(data_.size());
}

bool MinHeap::empty() const {
    return data_.empty();
}

void MinHeap::sift_up(int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (data_[idx] < data_[parent]) {
            std::swap(data_[idx], data_[parent]);
            idx = parent;
        } else {
            break;
        }
    }
}

void MinHeap::sift_down(int idx) {
    int n = static_cast<int>(data_.size());
    while (true) {
        int left  = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;

        if (left < n && data_[left] < data_[smallest])   smallest = left;
        if (right < n && data_[right] < data_[smallest]) smallest = right;

        if (smallest == idx) break;

        std::swap(data_[idx], data_[smallest]);
        idx = smallest;
    }
}

int kth_largest(std::vector<int> nums, int k) {
    // nth_element rearranges so that nums[k-1] holds the kth largest;
    // elements before it are >= it, elements after are <= it.
    std::nth_element(nums.begin(), nums.begin() + (k - 1), nums.end(),
                     std::greater<int>{});
    return nums[k - 1];
}

std::vector<int> merge_k_sorted(const std::vector<std::vector<int>>& lists) {
    // Min-heap entry: {value, list_index, element_index}
    using Entry = std::tuple<int, int, int>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;

    for (int i = 0; i < static_cast<int>(lists.size()); ++i) {
        if (!lists[i].empty()) {
            pq.emplace(lists[i][0], i, 0);
        }
    }

    std::vector<int> result;
    while (!pq.empty()) {
        auto [val, li, ei] = pq.top();
        pq.pop();
        result.push_back(val);
        int next = ei + 1;
        if (next < static_cast<int>(lists[li].size())) {
            pq.emplace(lists[li][next], li, next);
        }
    }
    return result;
}
