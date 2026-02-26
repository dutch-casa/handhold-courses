#include <gtest/gtest.h>
#include "../src/solution.h"
#include <vector>
#include <queue>
#include <optional>

// Helper: build tree from level-order vector (nullopt = null node)
TreeNode* build_tree(const std::vector<std::optional<int>>& vals) {
    if (vals.empty() || !vals[0].has_value()) return nullptr;
    TreeNode* root = new TreeNode(vals[0].value());
    std::queue<TreeNode*> q;
    q.push(root);
    size_t i = 1;
    while (!q.empty() && i < vals.size()) {
        TreeNode* node = q.front();
        q.pop();
        if (i < vals.size() && vals[i].has_value()) {
            node->left = new TreeNode(vals[i].value());
            q.push(node->left);
        }
        i++;
        if (i < vals.size() && vals[i].has_value()) {
            node->right = new TreeNode(vals[i].value());
            q.push(node->right);
        }
        i++;
    }
    return root;
}

// Helper: collect values in level order
std::vector<std::optional<int>> to_level_order(TreeNode* root) {
    std::vector<std::optional<int>> result;
    if (!root) return result;
    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        if (node) {
            result.push_back(node->val);
            q.push(node->left);
            q.push(node->right);
        } else {
            result.push_back(std::nullopt);
        }
    }
    while (!result.empty() && !result.back().has_value()) result.pop_back();
    return result;
}

TEST(MaxDepth, BalancedTree) {
    auto* root = build_tree({3, 9, 20, std::nullopt, std::nullopt, 15, 7});
    EXPECT_EQ(max_depth(root), 3);
}

TEST(MaxDepth, EmptyTree) {
    EXPECT_EQ(max_depth(nullptr), 0);
}

TEST(MaxDepth, SingleNode) {
    auto* root = build_tree({1});
    EXPECT_EQ(max_depth(root), 1);
}

TEST(MaxDepth, LeftSkewed) {
    auto* root = build_tree({1, 2, std::nullopt, 3});
    EXPECT_EQ(max_depth(root), 3);
}

TEST(InvertTree, BasicTree) {
    auto* root = build_tree({4, 2, 7, 1, 3, 6, 9});
    auto* inverted = invert_tree(root);
    auto result = to_level_order(inverted);
    std::vector<std::optional<int>> expected = {4, 7, 2, 9, 6, 3, 1};
    EXPECT_EQ(result, expected);
}

TEST(InvertTree, NullTree) {
    EXPECT_EQ(invert_tree(nullptr), nullptr);
}

TEST(InvertTree, SingleNode) {
    auto* root = build_tree({1});
    auto* inverted = invert_tree(root);
    EXPECT_EQ(inverted->val, 1);
    EXPECT_EQ(inverted->left, nullptr);
    EXPECT_EQ(inverted->right, nullptr);
}

TEST(HasPathSum, PathExists) {
    auto* root = build_tree({5, 4, 8, 11, std::nullopt, 13, 4,
                             7, 2, std::nullopt, std::nullopt, std::nullopt, 1});
    EXPECT_TRUE(has_path_sum(root, 22));
}

TEST(HasPathSum, NoPath) {
    auto* root = build_tree({1, 2, 3});
    EXPECT_FALSE(has_path_sum(root, 5));
}

TEST(HasPathSum, EmptyTree) {
    EXPECT_FALSE(has_path_sum(nullptr, 0));
}

TEST(HasPathSum, SingleNodeMatch) {
    auto* root = build_tree({5});
    EXPECT_TRUE(has_path_sum(root, 5));
}

TEST(HasPathSum, SingleNodeNoMatch) {
    auto* root = build_tree({5});
    EXPECT_FALSE(has_path_sum(root, 1));
}
