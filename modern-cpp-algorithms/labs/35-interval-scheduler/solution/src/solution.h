#pragma once
#include <vector>
#include <utility>

std::vector<std::pair<int,int>> merge_intervals(std::vector<std::pair<int,int>> intervals);

std::vector<std::pair<int,int>> insert_interval(
    std::vector<std::pair<int,int>> intervals,
    std::pair<int,int> new_interval);

int min_meeting_rooms(std::vector<std::pair<int,int>> intervals);
