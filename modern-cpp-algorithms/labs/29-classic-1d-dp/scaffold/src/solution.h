#pragma once
#include <vector>

// Count distinct ways to reach step n taking 1 or 2 steps at a time.
int climb_stairs(int n);

// Maximum sum of non-adjacent elements.
int house_robber(const std::vector<int>& nums);

// Minimum number of coins summing to amount; -1 if impossible.
int coin_change(const std::vector<int>& coins, int amount);

// Length of the longest strictly increasing subsequence.
int lis_length(const std::vector<int>& nums);
