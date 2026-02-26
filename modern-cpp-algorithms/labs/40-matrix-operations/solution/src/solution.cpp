#include "solution.h"
#include <algorithm>
#include <unordered_set>

std::vector<int> spiral_order(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return {};

    std::vector<int> result;
    int top    = 0;
    int bottom = static_cast<int>(matrix.size()) - 1;
    int left   = 0;
    int right  = static_cast<int>(matrix[0].size()) - 1;

    while (top <= bottom && left <= right) {
        for (int c = left; c <= right; ++c)
            result.push_back(matrix[static_cast<std::size_t>(top)][static_cast<std::size_t>(c)]);
        ++top;

        for (int r = top; r <= bottom; ++r)
            result.push_back(matrix[static_cast<std::size_t>(r)][static_cast<std::size_t>(right)]);
        --right;

        if (top <= bottom) {
            for (int c = right; c >= left; --c)
                result.push_back(matrix[static_cast<std::size_t>(bottom)][static_cast<std::size_t>(c)]);
            --bottom;
        }

        if (left <= right) {
            for (int r = bottom; r >= top; --r)
                result.push_back(matrix[static_cast<std::size_t>(r)][static_cast<std::size_t>(left)]);
            ++left;
        }
    }

    return result;
}

void rotate_image(std::vector<std::vector<int>>& matrix) {
    const int n = static_cast<int>(matrix.size());

    // Transpose
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            std::swap(matrix[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)],
                      matrix[static_cast<std::size_t>(j)][static_cast<std::size_t>(i)]);
        }
    }

    // Reverse each row
    for (auto& row : matrix) {
        std::reverse(row.begin(), row.end());
    }
}

void set_zeroes(std::vector<std::vector<int>>& matrix) {
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());

    std::vector<int> zero_rows, zero_cols;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (matrix[static_cast<std::size_t>(r)][static_cast<std::size_t>(c)] == 0) {
                zero_rows.push_back(r);
                zero_cols.push_back(c);
            }
        }
    }

    for (int r : zero_rows) {
        for (int c = 0; c < cols; ++c) {
            matrix[static_cast<std::size_t>(r)][static_cast<std::size_t>(c)] = 0;
        }
    }

    for (int c : zero_cols) {
        for (int r = 0; r < rows; ++r) {
            matrix[static_cast<std::size_t>(r)][static_cast<std::size_t>(c)] = 0;
        }
    }
}

bool search_matrix(const std::vector<std::vector<int>>& matrix, int target) {
    if (matrix.empty() || matrix[0].empty()) return false;

    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());
    int lo = 0;
    int hi = rows * cols - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int val = matrix[static_cast<std::size_t>(mid / cols)][static_cast<std::size_t>(mid % cols)];
        if (val == target) return true;
        if (val < target)  lo = mid + 1;
        else               hi = mid - 1;
    }

    return false;
}
