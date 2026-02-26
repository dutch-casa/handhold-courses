#include <gtest/gtest.h>
#include <limits>
#include "../src/solution.h"

TEST(TypeExplorer, SizeOfInt) {
    EXPECT_EQ(size_of_int(), sizeof(int));
}

TEST(TypeExplorer, SizeOfDouble) {
    EXPECT_EQ(size_of_double(), sizeof(double));
}

TEST(TypeExplorer, SizeOfChar) {
    EXPECT_EQ(size_of_char(), sizeof(char));
}

TEST(TypeExplorer, IntMax) {
    EXPECT_EQ(int_max(), std::numeric_limits<int>::max());
}

TEST(TypeExplorer, IntMin) {
    EXPECT_EQ(int_min(), std::numeric_limits<int>::min());
}

TEST(TypeExplorer, WithinRangeTrue) {
    EXPECT_TRUE(is_within_int_range(0));
    EXPECT_TRUE(is_within_int_range(42));
    EXPECT_TRUE(is_within_int_range(-100));
    EXPECT_TRUE(is_within_int_range(std::numeric_limits<int>::max()));
    EXPECT_TRUE(is_within_int_range(std::numeric_limits<int>::min()));
}

TEST(TypeExplorer, WithinRangeFalse) {
    EXPECT_FALSE(is_within_int_range(static_cast<long long>(std::numeric_limits<int>::max()) + 1));
    EXPECT_FALSE(is_within_int_range(static_cast<long long>(std::numeric_limits<int>::min()) - 1));
}

TEST(TypeExplorer, ClassifyPositive) {
    EXPECT_EQ(classify(3.14), "positive");
    EXPECT_EQ(classify(0.001), "positive");
}

TEST(TypeExplorer, ClassifyNegative) {
    EXPECT_EQ(classify(-1.0), "negative");
    EXPECT_EQ(classify(-0.001), "negative");
}

TEST(TypeExplorer, ClassifyZero) {
    EXPECT_EQ(classify(0.0), "zero");
}
