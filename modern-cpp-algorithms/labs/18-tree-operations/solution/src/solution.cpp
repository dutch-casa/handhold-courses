#include "solution.h"
#include <algorithm>

int max_depth(TreeNode* root) {
    if (!root) return 0;
    return 1 + std::max(max_depth(root->left), max_depth(root->right));
}

TreeNode* invert_tree(TreeNode* root) {
    if (!root) return nullptr;
    std::swap(root->left, root->right);
    invert_tree(root->left);
    invert_tree(root->right);
    return root;
}

bool has_path_sum(TreeNode* root, int target) {
    if (!root) return false;
    if (!root->left && !root->right) return root->val == target;
    int remaining = target - root->val;
    return has_path_sum(root->left, remaining) || has_path_sum(root->right, remaining);
}
