#pragma once
#include <functional>
#include <vector>

std::vector<double> filter_positive(const std::vector<double>& data);
std::vector<double> transform_abs(const std::vector<double>& data);
double sum_if(const std::vector<double>& data, std::function<bool(double)> predicate);
std::vector<double> pipeline(const std::vector<double>& data,
                             std::function<bool(double)> filter,
                             std::function<double(double)> transform);
