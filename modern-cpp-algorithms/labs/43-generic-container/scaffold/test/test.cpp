#include <gtest/gtest.h>
#include "../src/solution.h"
#include <string>

TEST(SortedContainer, InsertMaintainsOrder) {
    SortedContainer<int> sc;
    sc.insert(3);
    sc.insert(1);
    sc.insert(2);
    EXPECT_EQ(sc.to_vector(), (std::vector<int>{1, 2, 3}));
}

TEST(SortedContainer, Contains) {
    SortedContainer<int> sc;
    sc.insert(5);
    sc.insert(3);
    EXPECT_TRUE(sc.contains(5));
    EXPECT_FALSE(sc.contains(4));
}

TEST(SortedContainer, Remove) {
    SortedContainer<int> sc;
    sc.insert(1);
    sc.insert(2);
    sc.insert(3);
    EXPECT_TRUE(sc.remove(2));
    EXPECT_EQ(sc.to_vector(), (std::vector<int>{1, 3}));
    EXPECT_FALSE(sc.remove(99));
}

TEST(SortedContainer, Size) {
    SortedContainer<int> sc;
    EXPECT_EQ(sc.size(), 0u);
    sc.insert(1);
    sc.insert(2);
    EXPECT_EQ(sc.size(), 2u);
}

TEST(SortedContainer, At) {
    SortedContainer<int> sc;
    sc.insert(30);
    sc.insert(10);
    sc.insert(20);
    EXPECT_EQ(sc.at(0), 10);
    EXPECT_EQ(sc.at(1), 20);
    EXPECT_EQ(sc.at(2), 30);
}

TEST(SortedContainer, Duplicates) {
    SortedContainer<int> sc;
    sc.insert(1);
    sc.insert(1);
    sc.insert(1);
    EXPECT_EQ(sc.size(), 3u);
    sc.remove(1);
    EXPECT_EQ(sc.size(), 2u);
}

TEST(SortedContainer, Strings) {
    SortedContainer<std::string> sc;
    sc.insert("banana");
    sc.insert("apple");
    sc.insert("cherry");
    EXPECT_EQ(sc.at(0), "apple");
    EXPECT_EQ(sc.at(1), "banana");
    EXPECT_EQ(sc.at(2), "cherry");
}

TEST(SortedContainer, Empty) {
    SortedContainer<int> sc;
    EXPECT_FALSE(sc.contains(1));
    EXPECT_FALSE(sc.remove(1));
    EXPECT_EQ(sc.to_vector(), std::vector<int>{});
}
