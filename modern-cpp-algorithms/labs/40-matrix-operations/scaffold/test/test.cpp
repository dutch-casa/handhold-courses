#include <gtest/gtest.h>
#include "../src/solution.h"

// --- spiral_order ---

TEST(SpiralOrder, ThreeByThree) {
    std::vector<std::vector<int>> m = {{1,2,3},{4,5,6},{7,8,9}};
    EXPECT_EQ(spiral_order(m), (std::vector<int>{1,2,3,6,9,8,7,4,5}));
}

TEST(SpiralOrder, TwoByFour) {
    std::vector<std::vector<int>> m = {{1,2,3,4},{5,6,7,8}};
    EXPECT_EQ(spiral_order(m), (std::vector<int>{1,2,3,4,8,7,6,5}));
}

TEST(SpiralOrder, SingleRow) {
    std::vector<std::vector<int>> m = {{1,2,3}};
    EXPECT_EQ(spiral_order(m), (std::vector<int>{1,2,3}));
}

// --- rotate_image ---

TEST(RotateImage, ThreeByThree) {
    std::vector<std::vector<int>> m = {{1,2,3},{4,5,6},{7,8,9}};
    rotate_image(m);
    EXPECT_EQ(m[0], (std::vector<int>{7,4,1}));
    EXPECT_EQ(m[1], (std::vector<int>{8,5,2}));
    EXPECT_EQ(m[2], (std::vector<int>{9,6,3}));
}

TEST(RotateImage, TwoByTwo) {
    std::vector<std::vector<int>> m = {{1,2},{3,4}};
    rotate_image(m);
    EXPECT_EQ(m[0], (std::vector<int>{3,1}));
    EXPECT_EQ(m[1], (std::vector<int>{4,2}));
}

// --- set_zeroes ---

TEST(SetZeroes, BasicCase) {
    std::vector<std::vector<int>> m = {{1,1,1},{1,0,1},{1,1,1}};
    set_zeroes(m);
    EXPECT_EQ(m[0], (std::vector<int>{1,0,1}));
    EXPECT_EQ(m[1], (std::vector<int>{0,0,0}));
    EXPECT_EQ(m[2], (std::vector<int>{1,0,1}));
}

TEST(SetZeroes, TwoZeroes) {
    std::vector<std::vector<int>> m = {{0,1,2,0},{3,4,5,2},{1,3,1,5}};
    set_zeroes(m);
    EXPECT_EQ(m[0], (std::vector<int>{0,0,0,0}));
    EXPECT_EQ(m[1], (std::vector<int>{0,4,5,0}));
    EXPECT_EQ(m[2], (std::vector<int>{0,3,1,0}));
}

// --- search_matrix ---

TEST(SearchMatrix, Found) {
    std::vector<std::vector<int>> m = {{1,3,5,7},{10,11,16,20},{23,30,34,60}};
    EXPECT_TRUE(search_matrix(m, 3));
    EXPECT_TRUE(search_matrix(m, 60));
}

TEST(SearchMatrix, NotFound) {
    std::vector<std::vector<int>> m = {{1,3,5,7},{10,11,16,20},{23,30,34,60}};
    EXPECT_FALSE(search_matrix(m, 13));
}
