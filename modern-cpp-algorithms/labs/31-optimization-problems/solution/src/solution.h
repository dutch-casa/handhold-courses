#pragma once
#include <vector>

// 0/1 Knapsack: maximum total value with items having given weights and values,
// subject to total weight <= capacity.
int knapsack(const std::vector<int>& weights, const std::vector<int>& values, int capacity);

struct TreeNode {
    int val;
    TreeNode* left  = nullptr;
    TreeNode* right = nullptr;
};

// House robber on binary tree: maximum sum with no two adjacent nodes robbed.
int rob_tree(TreeNode* root);

// TSP bitmask DP: minimum-cost Hamiltonian cycle through all cities.
// dist is an n×n matrix; start and end at city 0.
int tsp(const std::vector<std::vector<int>>& dist);
