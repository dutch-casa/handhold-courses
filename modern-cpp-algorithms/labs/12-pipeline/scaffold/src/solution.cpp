#include "solution.h"
#include <cmath>
#include <algorithm>
#include <numeric>

std::vector<double> filter_positive(const std::vector<double>& data) {
    // TODO: Return a vector containing only the positive values from data
    return {};
}

std::vector<double> transform_abs(const std::vector<double>& data) {
    // TODO: Return a vector with the absolute value of each element
    return {};
}

double sum_if(const std::vector<double>& data, std::function<bool(double)> predicate) {
    // TODO: Sum elements where predicate(element) returns true
    return 0.0;
}

std::vector<double> pipeline(const std::vector<double>& data,
                             std::function<bool(double)> filter,
                             std::function<double(double)> transform) {
    // TODO: Filter elements, then transform the survivors
    return {};
}
