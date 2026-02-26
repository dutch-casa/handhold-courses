#include "solution.h"

std::vector<int> next_greater(const std::vector<int>& nums) {
    // TODO: use a monotonic stack to find next greater element for each position
    return std::vector<int>(nums.size(), -1);
}

std::vector<int> daily_temperatures(const std::vector<int>& temps) {
    // TODO: use a monotonic stack of indices; resolve when a warmer day is found
    return std::vector<int>(temps.size(), 0);
}

int largest_rectangle(const std::vector<int>& heights) {
    // TODO: use a monotonic increasing stack; compute area when a shorter bar appears
    return 0;
}
