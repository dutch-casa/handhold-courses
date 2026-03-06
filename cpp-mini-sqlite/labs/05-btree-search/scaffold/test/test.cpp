#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include "../src/solution.h"

static const char* kTempPath = "/tmp/btree_lab05_test.db";

// Write a BTreeNode to page `page_id` in a file.
// Creates the file if it does not exist; extends it if needed.
static void write_node(const char* path, PageId page_id, const BTreeNode& node) {
  FILE* f = std::fopen(path, "r+b");
  if (!f) f = std::fopen(path, "w+b");
  ASSERT_NE(f, nullptr) << "Failed to open test file";

  // Extend file to cover page_id if needed.
  std::fseek(f, 0, SEEK_END);
  long cur_size = std::ftell(f);
  long required  = static_cast<long>((page_id + 1) * kPageSize);
  if (cur_size < required) {
    std::vector<uint8_t> zeros(static_cast<size_t>(required - cur_size), 0u);
    size_t written = std::fwrite(zeros.data(), 1, zeros.size(), f);
    ASSERT_EQ(written, zeros.size()) << "Failed to extend file";
  }

  std::fseek(f, static_cast<long>(page_id * kPageSize), SEEK_SET);
  Page page{page_id, {}};
  std::memcpy(page.data.data(), &node, sizeof(node));
  size_t written = std::fwrite(page.data.data(), 1, kPageSize, f);
  std::fclose(f);
  ASSERT_EQ(written, kPageSize) << "Failed to write node page";
}

class BTreeSearchTest : public ::testing::Test {
 protected:
  void SetUp() override { std::remove(kTempPath); }
  void TearDown() override { std::remove(kTempPath); }
};

// Single leaf: keys [10, 20, 30], values [100, 200, 300].
TEST_F(BTreeSearchTest, FindsKeyInLeaf) {
  BTreeNode leaf{};
  leaf.is_leaf  = 1;
  leaf.num_keys = 3;
  leaf.keys[0] = 10; leaf.children[0] = 100;
  leaf.keys[1] = 20; leaf.children[1] = 200;
  leaf.keys[2] = 30; leaf.children[2] = 300;
  write_node(kTempPath, 0, leaf);

  PageManager pm(kTempPath);
  PageCache   cache(pm);
  BTree       tree(cache, 0);

  EXPECT_EQ(tree.search(10).value(), 100u);
  EXPECT_EQ(tree.search(20).value(), 200u);
  EXPECT_EQ(tree.search(30).value(), 300u);
}

// Single leaf: keys not present.
TEST_F(BTreeSearchTest, ReturnsNotFoundForMissingKey) {
  BTreeNode leaf{};
  leaf.is_leaf  = 1;
  leaf.num_keys = 2;
  leaf.keys[0] = 10; leaf.children[0] = 100;
  leaf.keys[1] = 30; leaf.children[1] = 300;
  write_node(kTempPath, 0, leaf);

  PageManager pm(kTempPath);
  PageCache   cache(pm);
  BTree       tree(cache, 0);

  EXPECT_EQ(tree.search(20).error(), DbError::kNotFound);
  EXPECT_EQ(tree.search(0).error(),  DbError::kNotFound);
  EXPECT_EQ(tree.search(99).error(), DbError::kNotFound);
}

// Two-level tree:
//   page 0: internal, key=20, children=[1, 2]
//   page 1: leaf, keys=[10], values=[100]
//   page 2: leaf, keys=[20, 30], values=[200, 300]
TEST_F(BTreeSearchTest, DescendsThroughInternalNode) {
  BTreeNode root{};
  root.is_leaf  = 0;
  root.num_keys = 1;
  root.keys[0]     = 20;
  root.children[0] = 1;  // subtree for keys < 20
  root.children[1] = 2;  // subtree for keys >= 20
  write_node(kTempPath, 0, root);

  BTreeNode left{};
  left.is_leaf  = 1;
  left.num_keys = 1;
  left.keys[0] = 10; left.children[0] = 100;
  write_node(kTempPath, 1, left);

  BTreeNode right{};
  right.is_leaf  = 1;
  right.num_keys = 2;
  right.keys[0] = 20; right.children[0] = 200;
  right.keys[1] = 30; right.children[1] = 300;
  write_node(kTempPath, 2, right);

  PageManager pm(kTempPath);
  PageCache   cache(pm);
  BTree       tree(cache, 0);

  EXPECT_EQ(tree.search(10).value(), 100u);
  EXPECT_EQ(tree.search(20).value(), 200u);
  EXPECT_EQ(tree.search(30).value(), 300u);
  EXPECT_EQ(tree.search(25).error(), DbError::kNotFound);
}

// Empty root leaf: any search must return kNotFound.
TEST_F(BTreeSearchTest, EmptyTreeReturnsNotFound) {
  BTreeNode leaf{};
  leaf.is_leaf  = 1;
  leaf.num_keys = 0;
  write_node(kTempPath, 0, leaf);

  PageManager pm(kTempPath);
  PageCache   cache(pm);
  BTree       tree(cache, 0);

  EXPECT_EQ(tree.search(0).error(),   DbError::kNotFound);
  EXPECT_EQ(tree.search(100).error(), DbError::kNotFound);
}

// Boundary keys at first and last positions.
TEST_F(BTreeSearchTest, BoundaryKeysFound) {
  BTreeNode leaf{};
  leaf.is_leaf  = 1;
  leaf.num_keys = 4;
  for (uint32_t i = 0; i < 4; ++i) {
    leaf.keys[i]     = (i + 1) * 100;
    leaf.children[i] = leaf.keys[i] * 10;
  }
  write_node(kTempPath, 0, leaf);

  PageManager pm(kTempPath);
  PageCache   cache(pm);
  BTree       tree(cache, 0);

  EXPECT_EQ(tree.search(100).value(), 1000u);
  EXPECT_EQ(tree.search(400).value(), 4000u);
  EXPECT_EQ(tree.search(99).error(),  DbError::kNotFound);
  EXPECT_EQ(tree.search(401).error(), DbError::kNotFound);
}
