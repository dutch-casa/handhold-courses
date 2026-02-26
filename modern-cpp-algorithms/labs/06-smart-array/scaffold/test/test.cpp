#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(SmartArrayTest, InitialState) {
    SmartArray arr(4);
    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.capacity(), 4);
}

TEST(SmartArrayTest, PushAndAt) {
    SmartArray arr(4);
    arr.push(10);
    arr.push(20);
    arr.push(30);
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.at(0), 10);
    EXPECT_EQ(arr.at(1), 20);
    EXPECT_EQ(arr.at(2), 30);
}

TEST(SmartArrayTest, AutoResize) {
    SmartArray arr(2);
    arr.push(1);
    arr.push(2);
    arr.push(3);
    EXPECT_EQ(arr.size(), 3);
    EXPECT_GE(arr.capacity(), 3);
    EXPECT_EQ(arr.at(0), 1);
    EXPECT_EQ(arr.at(1), 2);
    EXPECT_EQ(arr.at(2), 3);
}

TEST(SmartArrayTest, CapacityDoubles) {
    SmartArray arr(2);
    arr.push(1);
    arr.push(2);
    EXPECT_EQ(arr.capacity(), 2);
    arr.push(3);
    EXPECT_EQ(arr.capacity(), 4);
}

TEST(SmartArrayTest, BoundsCheck) {
    SmartArray arr(4);
    arr.push(10);
    EXPECT_THROW(arr.at(1), std::out_of_range);
    EXPECT_THROW(arr.at(-1), std::out_of_range);
}

TEST(SmartArrayTest, ManyElements) {
    SmartArray arr(1);
    for (int i = 0; i < 100; ++i) {
        arr.push(i * i);
    }
    EXPECT_EQ(arr.size(), 100);
    EXPECT_EQ(arr.at(0), 0);
    EXPECT_EQ(arr.at(99), 9801);
}
