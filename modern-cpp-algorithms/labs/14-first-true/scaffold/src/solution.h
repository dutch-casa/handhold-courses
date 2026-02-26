#pragma once
#include <vector>
#include <functional>

int first_true(const std::vector<int>& arr, std::function<bool(int)> pred);
int first_bad_version(int n, std::function<bool(int)> is_bad);
int find_peak(const std::vector<int>& arr);
