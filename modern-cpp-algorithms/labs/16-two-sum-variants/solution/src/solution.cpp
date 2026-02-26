#include "solution.h"
#include <algorithm>

std::pair<int, int> two_sum_sorted(const std::vector<int>& nums, int target) {
    int left = 0, right = static_cast<int>(nums.size()) - 1;
    while (left < right) {
        int sum = nums[left] + nums[right];
        if (sum == target) return {left + 1, right + 1};
        if (sum < target) left++;
        else right--;
    }
    return {-1, -1};
}

std::vector<std::vector<int>> three_sum(std::vector<int> nums) {
    std::sort(nums.begin(), nums.end());
    std::vector<std::vector<int>> result;
    int n = static_cast<int>(nums.size());
    for (int i = 0; i < n - 2; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        int left = i + 1, right = n - 1;
        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];
            if (sum == 0) {
                result.push_back({nums[i], nums[left], nums[right]});
                while (left < right && nums[left] == nums[left + 1]) left++;
                while (left < right && nums[right] == nums[right - 1]) right--;
                left++;
                right--;
            } else if (sum < 0) {
                left++;
            } else {
                right--;
            }
        }
    }
    return result;
}

int max_water(const std::vector<int>& heights) {
    int left = 0, right = static_cast<int>(heights.size()) - 1;
    int best = 0;
    while (left < right) {
        int area = std::min(heights[left], heights[right]) * (right - left);
        best = std::max(best, area);
        if (heights[left] < heights[right]) left++;
        else right--;
    }
    return best;
}
