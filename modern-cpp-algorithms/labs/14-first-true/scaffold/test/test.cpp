#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(FirstTrue, BasicThreshold) {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6};
    EXPECT_EQ(first_true(arr, [](int x) { return x >= 4; }), 3);
}

TEST(FirstTrue, AllTrue) {
    std::vector<int> arr = {10, 20, 30};
    EXPECT_EQ(first_true(arr, [](int x) { return x > 0; }), 0);
}

TEST(FirstTrue, NoneTrue) {
    std::vector<int> arr = {1, 2, 3};
    EXPECT_EQ(first_true(arr, [](int x) { return x > 100; }), -1);
}

TEST(FirstTrue, SingleElement) {
    std::vector<int> arr = {5};
    EXPECT_EQ(first_true(arr, [](int x) { return x == 5; }), 0);
}

TEST(FirstTrue, EmptyArray) {
    std::vector<int> arr = {};
    EXPECT_EQ(first_true(arr, [](int x) { return x > 0; }), -1);
}

TEST(FirstBadVersion, MiddleBad) {
    EXPECT_EQ(first_bad_version(10, [](int v) { return v >= 5; }), 5);
}

TEST(FirstBadVersion, FirstIsBad) {
    EXPECT_EQ(first_bad_version(5, [](int v) { return v >= 1; }), 1);
}

TEST(FirstBadVersion, LastIsBad) {
    EXPECT_EQ(first_bad_version(100, [](int v) { return v >= 100; }), 100);
}

TEST(FindPeak, SmallMountain) {
    std::vector<int> arr = {1, 3, 5, 4, 2};
    EXPECT_EQ(find_peak(arr), 2);
}

TEST(FindPeak, PeakAtStart) {
    std::vector<int> arr = {5, 4, 3, 2, 1};
    EXPECT_EQ(find_peak(arr), 0);
}

TEST(FindPeak, PeakAtEnd) {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    EXPECT_EQ(find_peak(arr), 4);
}

TEST(FindPeak, LargeMountain) {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 7};
    EXPECT_EQ(find_peak(arr), 9);
}
