#pragma once
#include <vector>

// Return a sorted copy of nums using merge sort.
std::vector<int> merge_sort(std::vector<int> nums);

// Count pairs (i,j) where i<j but nums[i]>nums[j].
long long count_inversions(std::vector<int> nums);

// Return the element appearing more than n/2 times (guaranteed to exist).
int majority_element(const std::vector<int>& nums);
