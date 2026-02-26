#pragma once
#include <vector>
#include <utility>

int num_islands(std::vector<std::vector<char>>& grid);
int count_components(int n, const std::vector<std::pair<int, int>>& edges);
bool can_finish(int num_courses, const std::vector<std::pair<int, int>>& prereqs);
