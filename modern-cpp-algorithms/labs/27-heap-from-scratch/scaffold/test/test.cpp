#include <gtest/gtest.h>
#include "../src/solution.h"

// --- MinHeap ---

TEST(MinHeap, PushAndTop) {
    MinHeap h;
    h.push(5);
    h.push(3);
    h.push(8);
    h.push(1);
    EXPECT_EQ(h.top(), 1);
}

TEST(MinHeap, PopOrder) {
    MinHeap h;
    for (int v : {4, 2, 7, 1, 9, 3}) h.push(v);
    EXPECT_EQ(h.pop(), 1);
    EXPECT_EQ(h.pop(), 2);
    EXPECT_EQ(h.pop(), 3);
    EXPECT_EQ(h.pop(), 4);
}

TEST(MinHeap, SizeAndEmpty) {
    MinHeap h;
    EXPECT_TRUE(h.empty());
    h.push(10);
    EXPECT_EQ(h.size(), 1);
    h.push(20);
    EXPECT_EQ(h.size(), 2);
    h.pop();
    EXPECT_EQ(h.size(), 1);
}

TEST(MinHeap, SingleElement) {
    MinHeap h;
    h.push(42);
    EXPECT_EQ(h.top(), 42);
    EXPECT_EQ(h.pop(), 42);
    EXPECT_TRUE(h.empty());
}

TEST(MinHeap, Duplicates) {
    MinHeap h;
    h.push(3);
    h.push(3);
    h.push(1);
    EXPECT_EQ(h.pop(), 1);
    EXPECT_EQ(h.pop(), 3);
    EXPECT_EQ(h.pop(), 3);
}

// --- kth_largest ---

TEST(KthLargest, BasicCase) {
    // [3,2,1,5,6,4], k=2  => 5
    EXPECT_EQ(kth_largest({3,2,1,5,6,4}, 2), 5);
}

TEST(KthLargest, KEqualsOne) {
    EXPECT_EQ(kth_largest({7,3,9,1}, 1), 9);
}

TEST(KthLargest, KEqualsSize) {
    EXPECT_EQ(kth_largest({5,3,8}, 3), 3);
}

// --- merge_k_sorted ---

TEST(MergeKSorted, BasicMerge) {
    std::vector<std::vector<int>> lists = {{1,4,7},{2,5,8},{3,6,9}};
    auto result = merge_k_sorted(lists);
    EXPECT_EQ(result, (std::vector<int>{1,2,3,4,5,6,7,8,9}));
}

TEST(MergeKSorted, EmptyLists) {
    EXPECT_TRUE(merge_k_sorted({}).empty());
    EXPECT_TRUE(merge_k_sorted({{}}).empty());
}

TEST(MergeKSorted, SingleList) {
    auto result = merge_k_sorted({{1,2,3}});
    EXPECT_EQ(result, (std::vector<int>{1,2,3}));
}
