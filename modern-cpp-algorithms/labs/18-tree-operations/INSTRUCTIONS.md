# Tree Operations — Recursive Tree Traversal

Binary trees are the gateway to recursive thinking. Each node delegates to its children; the base case is a null pointer.

## Tasks

- [ ] Implement `max_depth(root)` — return the maximum depth of a binary tree. An empty tree has depth 0.
- [ ] Implement `invert_tree(root)` — invert (mirror) a binary tree in place and return the root.
- [ ] Implement `has_path_sum(root, target)` — return `true` if there exists a root-to-leaf path where the node values sum to `target`.

## Tree Structure

```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};
```

A helper `build_tree` is provided in the test file to construct trees from vectors.

## Hints

- `max_depth`: `1 + max(depth(left), depth(right))`. Base case: null returns 0.
- `invert_tree`: swap left and right children, then recurse on both.
- `has_path_sum`: subtract `val` from `target` as you descend. At a leaf, check if `target == val`.

Use `Run Tests` to check your progress.
