#include "solution.h"
#include <algorithm>
#include <numeric>

bool can_ship(const std::vector<int>& weights, int trucks, int capacity) {
    int trips = 1;
    int current_load = 0;
    for (int w : weights) {
        if (w > capacity) return false;
        if (current_load + w > capacity) {
            trips++;
            current_load = w;
        } else {
            current_load += w;
        }
    }
    return trips <= trucks;
}

int min_capacity(const std::vector<int>& weights, int trucks) {
    int lo = *std::max_element(weights.begin(), weights.end());
    int hi = std::accumulate(weights.begin(), weights.end(), 0);
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (can_ship(weights, trucks, mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

int peak_of_mountain(const std::vector<int>& arr) {
    int lo = 0, hi = static_cast<int>(arr.size()) - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < arr[mid + 1]) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}
