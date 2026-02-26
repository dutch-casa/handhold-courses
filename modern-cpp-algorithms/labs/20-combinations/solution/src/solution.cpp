#include "solution.h"
#include <algorithm>

static void permute_helper(std::vector<int>& nums, int start,
                           std::vector<std::vector<int>>& result) {
    if (start == static_cast<int>(nums.size())) {
        result.push_back(nums);
        return;
    }
    for (int i = start; i < static_cast<int>(nums.size()); i++) {
        std::swap(nums[start], nums[i]);
        permute_helper(nums, start + 1, result);
        std::swap(nums[start], nums[i]);
    }
}

std::vector<std::vector<int>> permutations(std::vector<int> nums) {
    std::vector<std::vector<int>> result;
    permute_helper(nums, 0, result);
    return result;
}

static void combine_helper(int start, int n, int k,
                            std::vector<int>& current,
                            std::vector<std::vector<int>>& result) {
    if (static_cast<int>(current.size()) == k) {
        result.push_back(current);
        return;
    }
    int remaining = k - static_cast<int>(current.size());
    for (int i = start; i <= n - remaining + 1; i++) {
        current.push_back(i);
        combine_helper(i + 1, n, k, current, result);
        current.pop_back();
    }
}

std::vector<std::vector<int>> combinations(int n, int k) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    combine_helper(1, n, k, current, result);
    return result;
}

static void subset_helper(const std::vector<int>& nums, int index,
                           std::vector<int>& current,
                           std::vector<std::vector<int>>& result) {
    result.push_back(current);
    for (int i = index; i < static_cast<int>(nums.size()); i++) {
        current.push_back(nums[i]);
        subset_helper(nums, i + 1, current, result);
        current.pop_back();
    }
}

std::vector<std::vector<int>> subsets(std::vector<int> nums) {
    std::sort(nums.begin(), nums.end());
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    subset_helper(nums, 0, current, result);
    return result;
}
