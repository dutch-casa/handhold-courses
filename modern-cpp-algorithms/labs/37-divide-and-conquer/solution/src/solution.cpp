#include "solution.h"
#include <algorithm>

static std::vector<int> merge_halves(const std::vector<int>& left, const std::vector<int>& right) {
    std::vector<int> result;
    result.reserve(left.size() + right.size());
    std::size_t i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            result.push_back(left[i++]);
        } else {
            result.push_back(right[j++]);
        }
    }
    while (i < left.size())  result.push_back(left[i++]);
    while (j < right.size()) result.push_back(right[j++]);
    return result;
}

std::vector<int> merge_sort(std::vector<int> nums) {
    if (nums.size() <= 1) return nums;
    std::size_t mid = nums.size() / 2;
    auto left  = merge_sort({nums.begin(), nums.begin() + static_cast<long>(mid)});
    auto right = merge_sort({nums.begin() + static_cast<long>(mid), nums.end()});
    return merge_halves(left, right);
}

// Returns sorted array and accumulates inversions into `count`.
static std::vector<int> merge_count(std::vector<int> nums, long long& count) {
    if (nums.size() <= 1) return nums;
    std::size_t mid = nums.size() / 2;
    auto left  = merge_count({nums.begin(), nums.begin() + static_cast<long>(mid)}, count);
    auto right = merge_count({nums.begin() + static_cast<long>(mid), nums.end()}, count);

    std::vector<int> result;
    result.reserve(nums.size());
    std::size_t i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            result.push_back(left[i++]);
        } else {
            // All remaining elements in left are greater than right[j]
            count += static_cast<long long>(left.size() - i);
            result.push_back(right[j++]);
        }
    }
    while (i < left.size())  result.push_back(left[i++]);
    while (j < right.size()) result.push_back(right[j++]);
    return result;
}

long long count_inversions(std::vector<int> nums) {
    long long count = 0;
    merge_count(std::move(nums), count);
    return count;
}

int majority_element(const std::vector<int>& nums) {
    int candidate = nums[0];
    int votes = 1;
    for (std::size_t i = 1; i < nums.size(); ++i) {
        if (votes == 0) {
            candidate = nums[i];
            votes = 1;
        } else if (nums[i] == candidate) {
            ++votes;
        } else {
            --votes;
        }
    }
    return candidate;
}
