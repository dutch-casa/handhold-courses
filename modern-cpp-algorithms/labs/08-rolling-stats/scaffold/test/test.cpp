#include <gtest/gtest.h>
#include "../src/solution.h"
#include <cmath>
#include <vector>

TEST(RollingMean, BasicWindow) {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
    EXPECT_DOUBLE_EQ(rolling_mean(data, 3), 4.0); // mean of {3,4,5}
}

TEST(RollingMean, FullSpan) {
    std::vector<double> data = {2.0, 4.0, 6.0};
    EXPECT_DOUBLE_EQ(rolling_mean(data, 3), 4.0);
}

TEST(RollingMean, WindowLargerThanData) {
    std::vector<double> data = {10.0, 20.0};
    EXPECT_DOUBLE_EQ(rolling_mean(data, 5), 15.0);
}

TEST(RollingMean, SingleElement) {
    std::vector<double> data = {7.0, 3.0, 9.0};
    EXPECT_DOUBLE_EQ(rolling_mean(data, 1), 9.0);
}

TEST(RollingMedian, OddWindow) {
    std::vector<double> data = {1.0, 5.0, 2.0, 8.0, 3.0};
    EXPECT_DOUBLE_EQ(rolling_median(data, 3), 3.0); // median of {2,8,3} sorted={2,3,8}
}

TEST(RollingMedian, EvenWindow) {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0};
    EXPECT_DOUBLE_EQ(rolling_median(data, 4), 2.5); // median of {1,2,3,4}
}

TEST(RollingMedian, SingleElement) {
    std::vector<double> data = {5.0, 10.0, 15.0};
    EXPECT_DOUBLE_EQ(rolling_median(data, 1), 15.0);
}

TEST(RollingStddev, Uniform) {
    std::vector<double> data = {5.0, 5.0, 5.0};
    EXPECT_DOUBLE_EQ(rolling_stddev(data, 3), 0.0);
}

TEST(RollingStddev, BasicWindow) {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
    // stddev of {3,4,5}: mean=4, deviations={-1,0,1}, variance=2/3
    double expected = std::sqrt(2.0 / 3.0);
    EXPECT_NEAR(rolling_stddev(data, 3), expected, 1e-9);
}

TEST(RollingStddev, SingleElement) {
    std::vector<double> data = {42.0};
    EXPECT_DOUBLE_EQ(rolling_stddev(data, 1), 0.0);
}
