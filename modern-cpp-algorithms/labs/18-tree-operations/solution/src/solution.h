#pragma once

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
    TreeNode(int v, TreeNode* l, TreeNode* r) : val(v), left(l), right(r) {}
};

int max_depth(TreeNode* root);
TreeNode* invert_tree(TreeNode* root);
bool has_path_sum(TreeNode* root, int target);
