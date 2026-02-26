#pragma once
#include <vector>
#include <queue>

// Streaming median using two heaps.
// lower = max-heap holding the smaller half of seen numbers.
// upper = min-heap holding the larger half.
// Invariant: lower.size() == upper.size()  OR  lower.size() == upper.size() + 1.
class MedianFinder {
public:
    void   add_num(int num);
    double find_median() const;

private:
    std::priority_queue<int>                            lower_; // max-heap
    std::priority_queue<int, std::vector<int>,
                        std::greater<int>>              upper_; // min-heap
};

// Top k most frequent elements in nums (order of results does not matter).
std::vector<int> top_k_frequent(const std::vector<int>& nums, int k);

// Minimum CPU intervals to finish all tasks with at least `cooldown` intervals
// between any two executions of the same task type.
int task_scheduler(const std::vector<char>& tasks, int cooldown);
