#include "solution.h"
#include <algorithm>
#include <vector>

std::vector<std::pair<int,int>> merge_intervals(std::vector<std::pair<int,int>> intervals) {
    if (intervals.empty()) return {};

    std::sort(intervals.begin(), intervals.end());

    std::vector<std::pair<int,int>> merged;
    merged.push_back(intervals[0]);

    for (std::size_t i = 1; i < intervals.size(); ++i) {
        auto& [ms, me] = merged.back();
        auto [cs, ce] = intervals[i];
        if (cs <= me) {
            me = std::max(me, ce);
        } else {
            merged.push_back({cs, ce});
        }
    }

    return merged;
}

std::vector<std::pair<int,int>> insert_interval(
    std::vector<std::pair<int,int>> intervals,
    std::pair<int,int> new_interval) {

    std::vector<std::pair<int,int>> result;
    std::size_t i = 0;
    const std::size_t n = intervals.size();

    // Collect intervals that end before new_interval starts
    while (i < n && intervals[i].second < new_interval.first) {
        result.push_back(intervals[i]);
        ++i;
    }

    // Merge all overlapping intervals with new_interval
    while (i < n && intervals[i].first <= new_interval.second) {
        new_interval.first  = std::min(new_interval.first,  intervals[i].first);
        new_interval.second = std::max(new_interval.second, intervals[i].second);
        ++i;
    }
    result.push_back(new_interval);

    // Append remaining non-overlapping intervals
    while (i < n) {
        result.push_back(intervals[i]);
        ++i;
    }

    return result;
}

int min_meeting_rooms(std::vector<std::pair<int,int>> intervals) {
    if (intervals.empty()) return 0;

    std::vector<int> starts, ends;
    starts.reserve(intervals.size());
    ends.reserve(intervals.size());

    for (auto [s, e] : intervals) {
        starts.push_back(s);
        ends.push_back(e);
    }

    std::sort(starts.begin(), starts.end());
    std::sort(ends.begin(), ends.end());

    int rooms = 0;
    int max_rooms = 0;
    std::size_t ei = 0;

    for (std::size_t si = 0; si < starts.size(); ++si) {
        if (starts[si] < ends[ei]) {
            ++rooms;
        } else {
            ++ei;
        }
        max_rooms = std::max(max_rooms, rooms);
    }

    return max_rooms;
}
