#pragma once
#include <vector>
#include <string>
#include <utility>

int open_lock(const std::vector<std::string>& deadends, std::string target);
std::vector<int> topo_sort(int n, const std::vector<std::pair<int, int>>& edges);
