#include "solution.h"

int first_true(const std::vector<int>& arr, std::function<bool(int)> pred) {
    int lo = 0, hi = static_cast<int>(arr.size()) - 1;
    int result = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (pred(arr[mid])) {
            result = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return result;
}

int first_bad_version(int n, std::function<bool(int)> is_bad) {
    int lo = 1, hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (is_bad(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

int find_peak(const std::vector<int>& arr) {
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
