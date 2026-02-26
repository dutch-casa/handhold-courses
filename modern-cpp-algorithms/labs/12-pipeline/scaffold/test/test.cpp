#include <gtest/gtest.h>
#include "../src/solution.h"
#include <cmath>

TEST(FilterPositive, MixedValues) {
    std::vector<double> data = {-3.0, 1.0, -2.0, 4.0, 0.0, 5.0};
    auto result = filter_positive(data);
    ASSERT_EQ(result.size(), 3u);
    EXPECT_DOUBLE_EQ(result[0], 1.0);
    EXPECT_DOUBLE_EQ(result[1], 4.0);
    EXPECT_DOUBLE_EQ(result[2], 5.0);
}

TEST(FilterPositive, AllNegative) {
    std::vector<double> data = {-1.0, -2.0, -3.0};
    auto result = filter_positive(data);
    EXPECT_TRUE(result.empty());
}

TEST(FilterPositive, AllPositive) {
    std::vector<double> data = {1.0, 2.0, 3.0};
    auto result = filter_positive(data);
    EXPECT_EQ(result.size(), 3u);
}

TEST(FilterPositive, Empty) {
    std::vector<double> data;
    auto result = filter_positive(data);
    EXPECT_TRUE(result.empty());
}

TEST(TransformAbs, MixedValues) {
    std::vector<double> data = {-3.0, 1.0, -2.0, 0.0};
    auto result = transform_abs(data);
    ASSERT_EQ(result.size(), 4u);
    EXPECT_DOUBLE_EQ(result[0], 3.0);
    EXPECT_DOUBLE_EQ(result[1], 1.0);
    EXPECT_DOUBLE_EQ(result[2], 2.0);
    EXPECT_DOUBLE_EQ(result[3], 0.0);
}

TEST(TransformAbs, Empty) {
    std::vector<double> data;
    auto result = transform_abs(data);
    EXPECT_TRUE(result.empty());
}

TEST(SumIf, PositivesOnly) {
    std::vector<double> data = {-1.0, 2.0, -3.0, 4.0};
    double result = sum_if(data, [](double x) { return x > 0; });
    EXPECT_DOUBLE_EQ(result, 6.0);
}

TEST(SumIf, AllMatch) {
    std::vector<double> data = {1.0, 2.0, 3.0};
    double result = sum_if(data, [](double) { return true; });
    EXPECT_DOUBLE_EQ(result, 6.0);
}

TEST(SumIf, NoneMatch) {
    std::vector<double> data = {1.0, 2.0, 3.0};
    double result = sum_if(data, [](double) { return false; });
    EXPECT_DOUBLE_EQ(result, 0.0);
}

TEST(SumIf, Empty) {
    std::vector<double> data;
    double result = sum_if(data, [](double) { return true; });
    EXPECT_DOUBLE_EQ(result, 0.0);
}

TEST(Pipeline, FilterAndSquare) {
    std::vector<double> data = {-2.0, 3.0, -1.0, 4.0};
    auto result = pipeline(
        data,
        [](double x) { return x > 0; },
        [](double x) { return x * x; }
    );
    ASSERT_EQ(result.size(), 2u);
    EXPECT_DOUBLE_EQ(result[0], 9.0);
    EXPECT_DOUBLE_EQ(result[1], 16.0);
}

TEST(Pipeline, FilterNoneTransformAbs) {
    std::vector<double> data = {-1.0, -2.0};
    auto result = pipeline(
        data,
        [](double x) { return x > 0; },
        [](double x) { return std::abs(x); }
    );
    EXPECT_TRUE(result.empty());
}

TEST(Pipeline, FilterAllTransformDouble) {
    std::vector<double> data = {1.0, 2.0, 3.0};
    auto result = pipeline(
        data,
        [](double) { return true; },
        [](double x) { return x * 2; }
    );
    ASSERT_EQ(result.size(), 3u);
    EXPECT_DOUBLE_EQ(result[0], 2.0);
    EXPECT_DOUBLE_EQ(result[1], 4.0);
    EXPECT_DOUBLE_EQ(result[2], 6.0);
}

TEST(Pipeline, Empty) {
    std::vector<double> data;
    auto result = pipeline(
        data,
        [](double) { return true; },
        [](double x) { return x; }
    );
    EXPECT_TRUE(result.empty());
}
