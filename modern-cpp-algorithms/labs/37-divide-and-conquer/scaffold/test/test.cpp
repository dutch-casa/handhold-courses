#include <gtest/gtest.h>
#include "../src/solution.h"

// --- merge_sort ---

TEST(MergeSort, AlreadySorted) {
    EXPECT_EQ(merge_sort({1,2,3,4,5}), (std::vector<int>{1,2,3,4,5}));
}

TEST(MergeSort, ReverseSorted) {
    EXPECT_EQ(merge_sort({5,4,3,2,1}), (std::vector<int>{1,2,3,4,5}));
}

TEST(MergeSort, Mixed) {
    EXPECT_EQ(merge_sort({3,1,4,1,5,9,2,6}), (std::vector<int>{1,1,2,3,4,5,6,9}));
}

TEST(MergeSort, SingleElement) {
    EXPECT_EQ(merge_sort({42}), (std::vector<int>{42}));
}

// --- count_inversions ---

TEST(CountInversions, NoInversions) {
    EXPECT_EQ(count_inversions({1,2,3,4,5}), 0LL);
}

TEST(CountInversions, AllInversions) {
    EXPECT_EQ(count_inversions({5,4,3,2,1}), 10LL);
}

TEST(CountInversions, Mixed) {
    EXPECT_EQ(count_inversions({2,4,1,3,5}), 3LL);
}

// --- majority_element ---

TEST(MajorityElement, Simple) {
    EXPECT_EQ(majority_element({3,2,3}), 3);
}

TEST(MajorityElement, All_same) {
    EXPECT_EQ(majority_element({1,1,1,1}), 1);
}

TEST(MajorityElement, Larger) {
    EXPECT_EQ(majority_element({2,2,1,1,1,2,2}), 2);
}
