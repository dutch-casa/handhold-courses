#pragma once
#include <vector>
#include <utility>

// Merge all overlapping intervals in the list.
// Input need not be sorted.
std::vector<std::pair<int,int>> merge_intervals(std::vector<std::pair<int,int>> intervals);

// Insert new_interval into a sorted, non-overlapping list and merge as needed.
std::vector<std::pair<int,int>> insert_interval(
    std::vector<std::pair<int,int>> intervals,
    std::pair<int,int> new_interval);

// Minimum number of simultaneous rooms needed for all meetings.
int min_meeting_rooms(std::vector<std::pair<int,int>> intervals);
