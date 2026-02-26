#include "solution.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

void MedianFinder::add_num(int num) {
    // Route: push to lower if num <= lower's max (or lower is empty)
    if (lower_.empty() || num <= lower_.top()) {
        lower_.push(num);
    } else {
        upper_.push(num);
    }

    // Rebalance: lower may have at most 1 more element than upper
    if (lower_.size() > upper_.size() + 1) {
        upper_.push(lower_.top());
        lower_.pop();
    } else if (upper_.size() > lower_.size()) {
        lower_.push(upper_.top());
        upper_.pop();
    }
}

double MedianFinder::find_median() const {
    if (lower_.size() == upper_.size()) {
        return (static_cast<double>(lower_.top()) + upper_.top()) / 2.0;
    }
    // lower always holds the extra element when odd count
    return static_cast<double>(lower_.top());
}

std::vector<int> top_k_frequent(const std::vector<int>& nums, int k) {
    std::unordered_map<int, int> freq;
    for (int n : nums) ++freq[n];

    // Min-heap of {count, value}; keeps only the k highest-frequency entries
    using Entry = std::pair<int,int>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> minHeap;

    for (auto [val, count] : freq) {
        minHeap.emplace(count, val);
        if (static_cast<int>(minHeap.size()) > k) minHeap.pop();
    }

    std::vector<int> result;
    result.reserve(k);
    while (!minHeap.empty()) {
        result.push_back(minHeap.top().second);
        minHeap.pop();
    }
    return result;
}

int task_scheduler(const std::vector<char>& tasks, int cooldown) {
    std::unordered_map<char, int> freq;
    for (char t : tasks) ++freq[t];

    int max_freq = 0;
    int count_max = 0;
    for (auto [t, f] : freq) {
        if (f > max_freq) {
            max_freq = f;
            count_max = 1;
        } else if (f == max_freq) {
            ++count_max;
        }
    }

    // Formula: each "frame" of size (cooldown + 1) holds one of each max-freq task
    // plus fill tasks; if enough variety, just run all tasks without idle
    int slots = (max_freq - 1) * (cooldown + 1) + count_max;
    return std::max(static_cast<int>(tasks.size()), slots);
}
