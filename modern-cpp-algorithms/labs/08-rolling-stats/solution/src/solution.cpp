#include "solution.h"
#include <algorithm>
#include <cmath>
#include <numeric>

double rolling_mean(std::span<const double> data, int window_size) {
    auto n = static_cast<int>(data.size());
    auto w = std::min(window_size, n);
    auto window = data.last(static_cast<size_t>(w));
    double sum = std::accumulate(window.begin(), window.end(), 0.0);
    return sum / w;
}

double rolling_median(std::span<const double> data, int window_size) {
    auto n = static_cast<int>(data.size());
    auto w = std::min(window_size, n);
    auto window = data.last(static_cast<size_t>(w));

    std::vector<double> sorted(window.begin(), window.end());
    std::sort(sorted.begin(), sorted.end());

    if (w % 2 == 1) {
        return sorted[static_cast<size_t>(w / 2)];
    }
    return (sorted[static_cast<size_t>(w / 2 - 1)] + sorted[static_cast<size_t>(w / 2)]) / 2.0;
}

double rolling_stddev(std::span<const double> data, int window_size) {
    auto n = static_cast<int>(data.size());
    auto w = std::min(window_size, n);
    auto window = data.last(static_cast<size_t>(w));

    double mean = std::accumulate(window.begin(), window.end(), 0.0) / w;
    double sq_sum = 0.0;
    for (double v : window) {
        double diff = v - mean;
        sq_sum += diff * diff;
    }
    return std::sqrt(sq_sum / w);
}
