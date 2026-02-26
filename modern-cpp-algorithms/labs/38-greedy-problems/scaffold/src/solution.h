#pragma once
#include <string>
#include <vector>

// Jump Game I: can you reach the last index?
bool can_jump(const std::vector<int>& nums);

// Jump Game II: minimum jumps to reach the last index.
int min_jumps(const std::vector<int>& nums);

// Gas Station: return starting index for full circular trip, -1 if impossible.
int gas_station(const std::vector<int>& gas, const std::vector<int>& cost);

// Partition Labels: each letter in at most one part; return part sizes.
std::vector<int> partition_labels(const std::string& s);
