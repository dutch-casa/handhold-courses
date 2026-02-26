#include <gtest/gtest.h>
#include "../src/solution.h"

// --- SegTree ---

TEST(SegTree, QueryFullRange) {
    SegTree st({1, 3, 5, 7, 9});
    EXPECT_EQ(st.query(0, 4), 25LL);
}

TEST(SegTree, QuerySubrange) {
    SegTree st({1, 3, 5, 7, 9});
    EXPECT_EQ(st.query(1, 3), 15LL); // 3+5+7
}

TEST(SegTree, QuerySingleElement) {
    SegTree st({4, 8, 2});
    EXPECT_EQ(st.query(1, 1), 8LL);
}

TEST(SegTree, UpdateAndQuery) {
    SegTree st({1, 2, 3, 4, 5});
    st.update(2, 10); // replace 3 with 10
    EXPECT_EQ(st.query(0, 4), 22LL);
    EXPECT_EQ(st.query(1, 3), 16LL); // 2+10+4
}

TEST(SegTree, MultipleUpdates) {
    SegTree st({0, 0, 0, 0});
    st.update(0, 5);
    st.update(3, 3);
    EXPECT_EQ(st.query(0, 3), 8LL);
    EXPECT_EQ(st.query(1, 2), 0LL);
}

// --- Fenwick ---

TEST(Fenwick, PrefixSumBasic) {
    Fenwick fw({1, 2, 3, 4, 5});
    EXPECT_EQ(fw.prefix_sum(0), 1LL);
    EXPECT_EQ(fw.prefix_sum(2), 6LL);  // 1+2+3
    EXPECT_EQ(fw.prefix_sum(4), 15LL); // all
}

TEST(Fenwick, RangeSumBasic) {
    Fenwick fw({1, 2, 3, 4, 5});
    EXPECT_EQ(fw.range_sum(1, 3), 9LL);  // 2+3+4
    EXPECT_EQ(fw.range_sum(0, 4), 15LL);
}

TEST(Fenwick, UpdateAndPrefix) {
    Fenwick fw({1, 2, 3});
    fw.update(1, 5); // index 1 becomes 2+5=7
    EXPECT_EQ(fw.prefix_sum(1), 8LL); // 1+7
    EXPECT_EQ(fw.prefix_sum(2), 11LL); // 1+7+3
}

TEST(Fenwick, UpdateAndRangeSum) {
    Fenwick fw({0, 0, 0, 0, 0});
    fw.update(2, 10);
    fw.update(4, 3);
    EXPECT_EQ(fw.range_sum(2, 4), 13LL);
    EXPECT_EQ(fw.range_sum(0, 1), 0LL);
}

TEST(Fenwick, SingleElement) {
    Fenwick fw({42});
    EXPECT_EQ(fw.prefix_sum(0), 42LL);
    EXPECT_EQ(fw.range_sum(0, 0), 42LL);
    fw.update(0, -10);
    EXPECT_EQ(fw.prefix_sum(0), 32LL);
}
