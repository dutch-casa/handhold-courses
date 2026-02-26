#include "solution.h"
#include <vector>
#include <queue>

// TODO: push to lower, rebalance so lower.size() >= upper.size(),
// then if lower.size() > upper.size() + 1, move excess to upper
void MedianFinder::add_num(int num) {
    // TODO
}

// TODO: if sizes equal return average of tops; else return lower.top()
double MedianFinder::find_median() const {
    // TODO
    return 0.0;
}

// TODO: build frequency map; use min-heap of size k to track top-k
std::vector<int> top_k_frequent(const std::vector<int>& nums, int k) {
    // TODO
    return {};
}

// TODO: compute frequency of each task type; use the formula
// max(n, (max_freq - 1) * (cooldown + 1) + count_of_max_freq)
int task_scheduler(const std::vector<char>& tasks, int cooldown) {
    // TODO
    return 0;
}
