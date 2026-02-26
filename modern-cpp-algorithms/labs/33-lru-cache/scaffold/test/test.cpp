#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(LRUCache, BasicGetMiss) {
    LRUCache cache(2);
    EXPECT_EQ(cache.get(1), -1);
}

TEST(LRUCache, PutAndGet) {
    LRUCache cache(2);
    cache.put(1, 10);
    EXPECT_EQ(cache.get(1), 10);
}

TEST(LRUCache, EvictLRU) {
    // LeetCode 146 example
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    EXPECT_EQ(cache.get(1), 1);   // access 1 → LRU is now 2
    cache.put(3, 3);              // capacity exceeded → evict 2
    EXPECT_EQ(cache.get(2), -1); // evicted
    EXPECT_EQ(cache.get(3), 3);
}

TEST(LRUCache, UpdateExistingKey) {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(1, 42);
    EXPECT_EQ(cache.get(1), 42);
}

TEST(LRUCache, UpdateMovesToFront) {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(1, 10); // update key 1 → it becomes MRU; LRU is 2
    cache.put(3, 3);  // evict LRU (2)
    EXPECT_EQ(cache.get(2), -1);
    EXPECT_EQ(cache.get(1), 10);
    EXPECT_EQ(cache.get(3), 3);
}

TEST(LRUCache, CapacityOne) {
    LRUCache cache(1);
    cache.put(1, 1);
    cache.put(2, 2);  // evicts 1
    EXPECT_EQ(cache.get(1), -1);
    EXPECT_EQ(cache.get(2), 2);
}

TEST(LRUCache, GetRefreshesOrder) {
    // Sequence from LeetCode problem
    LRUCache cache(2);
    cache.put(2, 1);
    cache.put(1, 1);
    cache.put(2, 3); // update 2 → MRU; LRU is 1
    cache.put(4, 1); // evict 1
    EXPECT_EQ(cache.get(1), -1);
    EXPECT_EQ(cache.get(2), 3);
}

TEST(LRUCache, LargerCapacity) {
    LRUCache cache(3);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);
    cache.get(1);     // 1 is MRU; LRU order: 2, 3, 1
    cache.put(4, 4);  // evict 2 (LRU)
    EXPECT_EQ(cache.get(2), -1);
    EXPECT_EQ(cache.get(1), 1);
    EXPECT_EQ(cache.get(3), 3);
    EXPECT_EQ(cache.get(4), 4);
}
