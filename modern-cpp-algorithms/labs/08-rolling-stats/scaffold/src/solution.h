#pragma once
#include <span>
#include <vector>

double rolling_mean(std::span<const double> data, int window_size);
double rolling_median(std::span<const double> data, int window_size);
double rolling_stddev(std::span<const double> data, int window_size);
