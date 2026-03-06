#include <gtest/gtest.h>
#include <cstddef>
#include "../src/solution.h"

TEST(ArenaTest, AllocReturnsNonNull) {
  Arena arena(1024);
  void* ptr = arena.alloc(64);
  ASSERT_NE(ptr, nullptr);
}

TEST(ArenaTest, AllocAdvancesUsed) {
  Arena arena(1024);
  arena.alloc(100);
  EXPECT_EQ(arena.used(), 100u);
  arena.alloc(200);
  EXPECT_EQ(arena.used(), 300u);
}

TEST(ArenaTest, ResetClearsUsed) {
  Arena arena(1024);
  arena.alloc(500);
  EXPECT_EQ(arena.used(), 500u);
  arena.reset();
  EXPECT_EQ(arena.used(), 0u);
  // After reset, full capacity is available again.
  void* ptr = arena.alloc(1024);
  EXPECT_NE(ptr, nullptr);
}

TEST(ArenaTest, AllocReturnsNullWhenFull) {
  Arena arena(100);
  void* ptr = arena.alloc(101);
  EXPECT_EQ(ptr, nullptr);
}
