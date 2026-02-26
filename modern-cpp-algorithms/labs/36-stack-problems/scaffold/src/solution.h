#pragma once
#include <vector>

// For each element, the next strictly greater element to the right; -1 if none.
std::vector<int> next_greater(const std::vector<int>& nums);

// For each day, number of days to wait for a warmer temperature; 0 if never.
std::vector<int> daily_temperatures(const std::vector<int>& temps);

// Largest rectangle area in a histogram.
int largest_rectangle(const std::vector<int>& heights);
