#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include "../src/solution.h"

static const char* kTempPath = "/tmp/btree_lab06_test.db";

// Create a file with a single empty leaf page at page 0.
static void create_empty_tree(const char* path) {
  FILE* f = std::fopen(path, "w+b");
  ASSERT_NE(f, nullptr) << "Failed to create test file";
  Page page{0, {}};
  page.data.fill(0);
  auto* root = reinterpret_cast<BTreeNode*>(page.data.data());
  root->is_leaf  = 1;
  root->num_keys = 0;
  size_t written = std::fwrite(page.data.data(), 1, kPageSize, f);
  std::fclose(f);
  ASSERT_EQ(written, kPageSize) << "Failed to write initial root page";
}

class BTreeInsertTest : public ::testing::Test {
 protected:
  void SetUp() override { create_empty_tree(kTempPath); }
  void TearDown() override { std::remove(kTempPath); }
};

// Insert one key; search must find it.
TEST_F(BTreeInsertTest, InsertAndFindSingleKey) {
  PageManager pm(kTempPath);
  PageCache   cache(pm);
  BTree       tree(pm, cache, 0, 1);

  ASSERT_TRUE(tree.insert(42, 420).has_value());
  EXPECT_EQ(tree.search(42).value(), 420u);
  EXPECT_EQ(tree.search(1).error(), DbError::kNotFound);
}

// Insert multiple keys out of order; all must be findable.
TEST_F(BTreeInsertTest, InsertMultipleKeysOutOfOrder) {
  PageManager pm(kTempPath);
  PageCache   cache(pm);
  BTree       tree(pm, cache, 0, 1);

  std::vector<uint32_t> keys = {50, 10, 80, 30, 70, 20, 60, 40, 90};
  for (uint32_t k : keys) {
    ASSERT_TRUE(tree.insert(k, k * 10).has_value()) << "insert failed for key " << k;
  }
  for (uint32_t k : keys) {
    EXPECT_EQ(tree.search(k).value(), k * 10u) << "search failed for key " << k;
  }
}

// Insert kOrder+1 keys to force a leaf split; all must survive.
TEST_F(BTreeInsertTest, LeafSplitPreservesAllKeys) {
  PageManager pm(kTempPath);
  PageCache   cache(pm);
  BTree       tree(pm, cache, 0, 1);

  const uint32_t n = kOrder + 1;  // 511 keys triggers a leaf split
  for (uint32_t i = 0; i < n; ++i) {
    ASSERT_TRUE(tree.insert(i, i * 2).has_value()) << "insert failed for key " << i;
  }
  for (uint32_t i = 0; i < n; ++i) {
    EXPECT_EQ(tree.search(i).value(), i * 2u) << "search failed for key " << i;
  }
}

// Keys outside the inserted set must not appear.
TEST_F(BTreeInsertTest, NoSpuriousKeys) {
  PageManager pm(kTempPath);
  PageCache   cache(pm);
  BTree       tree(pm, cache, 0, 1);

  for (uint32_t i = 1; i <= 20; ++i) {
    ASSERT_TRUE(tree.insert(i * 5, i).has_value());
  }
  EXPECT_EQ(tree.search(0).error(),   DbError::kNotFound);
  EXPECT_EQ(tree.search(3).error(),   DbError::kNotFound);
  EXPECT_EQ(tree.search(101).error(), DbError::kNotFound);
  for (uint32_t i = 1; i <= 20; ++i) {
    EXPECT_EQ(tree.search(i * 5).value(), i) << "key " << i * 5 << " not found";
  }
}
