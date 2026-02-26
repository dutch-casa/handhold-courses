#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(CanShip, FitsInOne) {
    EXPECT_TRUE(can_ship({1, 2, 3}, 1, 6));
}

TEST(CanShip, NeedsTwoTrucks) {
    EXPECT_TRUE(can_ship({1, 2, 3, 4, 5}, 2, 9));
}

TEST(CanShip, CapacityTooSmall) {
    EXPECT_FALSE(can_ship({1, 2, 3, 4, 5}, 2, 4));
}

TEST(CanShip, ExactFit) {
    EXPECT_TRUE(can_ship({3, 3, 3}, 3, 3));
}

TEST(MinCapacity, BasicCase) {
    EXPECT_EQ(min_capacity({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 5), 15);
}

TEST(MinCapacity, SingleTruck) {
    EXPECT_EQ(min_capacity({1, 2, 3}, 1), 6);
}

TEST(MinCapacity, OneTruckPerPackage) {
    EXPECT_EQ(min_capacity({3, 5, 2}, 3), 5);
}

TEST(MinCapacity, TwoTrucks) {
    EXPECT_EQ(min_capacity({1, 2, 3, 1, 1}, 2), 5);
}

TEST(PeakOfMountain, BasicPeak) {
    EXPECT_EQ(peak_of_mountain({1, 3, 5, 4, 2}), 2);
}

TEST(PeakOfMountain, PeakAtStart) {
    EXPECT_EQ(peak_of_mountain({10, 5, 2}), 0);
}

TEST(PeakOfMountain, PeakAtEnd) {
    EXPECT_EQ(peak_of_mountain({1, 2, 3}), 2);
}
