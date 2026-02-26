#include "solution.h"
#include <stack>
#include <algorithm>

std::vector<int> next_greater(const std::vector<int>& nums) {
    const auto n = static_cast<int>(nums.size());
    std::vector<int> result(n, -1);
    std::stack<int> st; // indices waiting for their next greater

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && nums[i] > nums[st.top()]) {
            result[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }

    return result;
}

std::vector<int> daily_temperatures(const std::vector<int>& temps) {
    const auto n = static_cast<int>(temps.size());
    std::vector<int> result(n, 0);
    std::stack<int> st; // indices of days waiting for a warmer day

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && temps[i] > temps[st.top()]) {
            result[st.top()] = i - st.top();
            st.pop();
        }
        st.push(i);
    }

    return result;
}

int largest_rectangle(const std::vector<int>& heights) {
    // Append a sentinel 0 to flush all remaining bars at the end.
    std::vector<int> h = heights;
    h.push_back(0);

    std::stack<int> st; // monotonic increasing stack of indices
    int max_area = 0;

    for (int i = 0; i < static_cast<int>(h.size()); ++i) {
        while (!st.empty() && h[i] < h[st.top()]) {
            int height = h[st.top()];
            st.pop();
            int width = st.empty() ? i : i - st.top() - 1;
            max_area = std::max(max_area, height * width);
        }
        st.push(i);
    }

    return max_area;
}
