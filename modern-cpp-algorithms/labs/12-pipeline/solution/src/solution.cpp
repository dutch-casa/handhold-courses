#include "solution.h"
#include <cmath>
#include <algorithm>
#include <numeric>

std::vector<double> filter_positive(const std::vector<double>& data) {
    std::vector<double> result;
    std::copy_if(data.begin(), data.end(), std::back_inserter(result),
                 [](double x) { return x > 0; });
    return result;
}

std::vector<double> transform_abs(const std::vector<double>& data) {
    std::vector<double> result(data.size());
    std::transform(data.begin(), data.end(), result.begin(),
                   [](double x) { return std::abs(x); });
    return result;
}

double sum_if(const std::vector<double>& data, std::function<bool(double)> predicate) {
    double total = 0.0;
    for (double x : data) {
        if (predicate(x)) {
            total += x;
        }
    }
    return total;
}

std::vector<double> pipeline(const std::vector<double>& data,
                             std::function<bool(double)> filter,
                             std::function<double(double)> transform) {
    std::vector<double> filtered;
    std::copy_if(data.begin(), data.end(), std::back_inserter(filtered), filter);

    std::vector<double> result(filtered.size());
    std::transform(filtered.begin(), filtered.end(), result.begin(), transform);
    return result;
}
