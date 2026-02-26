#include <gtest/gtest.h>
#include "../src/solution.h"

// --- merge_intervals ---

TEST(MergeIntervals, NoOverlap) {
    auto result = merge_intervals({{1,2},{3,4},{5,6}});
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], (std::pair<int,int>{1,2}));
    EXPECT_EQ(result[1], (std::pair<int,int>{3,4}));
    EXPECT_EQ(result[2], (std::pair<int,int>{5,6}));
}

TEST(MergeIntervals, AllOverlap) {
    auto result = merge_intervals({{1,4},{2,5},{3,6}});
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0], (std::pair<int,int>{1,6}));
}

TEST(MergeIntervals, MixedOverlap) {
    auto result = merge_intervals({{1,3},{2,6},{8,10},{15,18}});
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], (std::pair<int,int>{1,6}));
    EXPECT_EQ(result[1], (std::pair<int,int>{8,10}));
    EXPECT_EQ(result[2], (std::pair<int,int>{15,18}));
}

TEST(MergeIntervals, UnsortedInput) {
    auto result = merge_intervals({{15,18},{1,3},{8,10},{2,6}});
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], (std::pair<int,int>{1,6}));
}

// --- insert_interval ---

TEST(InsertInterval, InsertInMiddle) {
    auto result = insert_interval({{1,3},{6,9}}, {2,5});
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], (std::pair<int,int>{1,5}));
    EXPECT_EQ(result[1], (std::pair<int,int>{6,9}));
}

TEST(InsertInterval, MergesMultiple) {
    auto result = insert_interval({{1,2},{3,5},{6,7},{8,10},{12,16}}, {4,8});
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], (std::pair<int,int>{1,2}));
    EXPECT_EQ(result[1], (std::pair<int,int>{3,10}));
    EXPECT_EQ(result[2], (std::pair<int,int>{12,16}));
}

// --- min_meeting_rooms ---

TEST(MinMeetingRooms, NoOverlap) {
    EXPECT_EQ(min_meeting_rooms({{0,5},{10,15},{20,25}}), 1);
}

TEST(MinMeetingRooms, AllOverlap) {
    EXPECT_EQ(min_meeting_rooms({{0,10},{0,10},{0,10}}), 3);
}

TEST(MinMeetingRooms, Typical) {
    EXPECT_EQ(min_meeting_rooms({{0,30},{5,10},{15,20}}), 2);
}
