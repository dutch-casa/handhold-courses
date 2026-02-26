#include "solution.h"
#include <algorithm>
#include <array>
#include <numeric>

bool can_jump(const std::vector<int>& nums) {
    int farthest = 0;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        if (i > farthest) return false;
        farthest = std::max(farthest, i + nums[i]);
    }
    return true;
}

int min_jumps(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n <= 1) return 0;

    int jumps = 0;
    int current_end = 0;
    int farthest = 0;

    for (int i = 0; i < n - 1; ++i) {
        farthest = std::max(farthest, i + nums[i]);
        if (i == current_end) {
            ++jumps;
            current_end = farthest;
        }
    }

    return jumps;
}

int gas_station(const std::vector<int>& gas, const std::vector<int>& cost) {
    int total = 0;
    int tank  = 0;
    int start = 0;

    for (int i = 0; i < static_cast<int>(gas.size()); ++i) {
        int net = gas[i] - cost[i];
        total += net;
        tank  += net;
        if (tank < 0) {
            start = i + 1;
            tank  = 0;
        }
    }

    return total >= 0 ? start : -1;
}

std::vector<int> partition_labels(const std::string& s) {
    // Precompute the last occurrence index for each character.
    std::array<int, 26> last{};
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        last[static_cast<std::size_t>(s[i] - 'a')] = i;
    }

    std::vector<int> sizes;
    int start = 0;
    int end   = 0;

    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        end = std::max(end, last[static_cast<std::size_t>(s[i] - 'a')]);
        if (i == end) {
            sizes.push_back(end - start + 1);
            start = i + 1;
        }
    }

    return sizes;
}
