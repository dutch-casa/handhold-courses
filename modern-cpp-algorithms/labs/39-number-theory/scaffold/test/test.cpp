#include <gtest/gtest.h>
#include "../src/solution.h"

// --- sieve ---

TEST(Sieve, SmallPrimes) {
    auto s = sieve(20);
    EXPECT_FALSE(s[0]);
    EXPECT_FALSE(s[1]);
    EXPECT_TRUE(s[2]);
    EXPECT_TRUE(s[3]);
    EXPECT_FALSE(s[4]);
    EXPECT_TRUE(s[5]);
    EXPECT_TRUE(s[7]);
    EXPECT_TRUE(s[11]);
    EXPECT_TRUE(s[13]);
    EXPECT_FALSE(s[9]);
}

TEST(Sieve, NoPrimesBelow2) {
    auto s = sieve(1);
    EXPECT_FALSE(s[0]);
    EXPECT_FALSE(s[1]);
}

// --- single_number ---

TEST(SingleNumber, Basic) {
    EXPECT_EQ(single_number({2,2,1}), 1);
}

TEST(SingleNumber, Larger) {
    EXPECT_EQ(single_number({4,1,2,1,2}), 4);
}

// --- count_bits ---

TEST(CountBits, Zero) {
    EXPECT_EQ(count_bits(0), 0);
}

TEST(CountBits, AllOnes) {
    EXPECT_EQ(count_bits(7), 3);
}

TEST(CountBits, PowerOfTwo) {
    EXPECT_EQ(count_bits(16), 1);
}

// --- is_power_of_two ---

TEST(IsPowerOfTwo, PowersOfTwo) {
    EXPECT_TRUE(is_power_of_two(1));
    EXPECT_TRUE(is_power_of_two(2));
    EXPECT_TRUE(is_power_of_two(1024));
}

TEST(IsPowerOfTwo, NotPowers) {
    EXPECT_FALSE(is_power_of_two(0));
    EXPECT_FALSE(is_power_of_two(3));
    EXPECT_FALSE(is_power_of_two(6));
}

// --- mod_pow ---

TEST(ModPow, Basic) {
    EXPECT_EQ(mod_pow(2, 10, 1000), 24);
}

TEST(ModPow, LargeExp) {
    EXPECT_EQ(mod_pow(3, 100, 1000000007LL), 981453966LL);
}
