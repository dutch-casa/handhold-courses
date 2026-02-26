---
title: Tree DFS
---

# Binary trees in C++

{{show: node-code typewriter 1.5s linear}} A binary tree node owns its children. unique ptr makes the ownership explicit — each node owns exactly the subtree below it.

{{focus: struct}} {{zoom: 1.2x}} {{annotate: struct "Node shape"}} Three fields: value, left child, right child. The unique ptrs mean no manual memory management. When a node is destroyed, its entire subtree is freed recursively.

{{zoom: 1x}} {{focus: factory}} {{zoom: 1.3x}} {{annotate: factory "Build nodes"}} A factory function wraps make unique. Cleaner than constructing nodes manually. Each call creates one node with optional children.

{{zoom: 1x}} {{focus: build}} {{zoom: 1.2x}} {{annotate: build "Compose"}} Build trees bottom-up by nesting calls. The leaves are created first, then passed up as children. Ownership transfers via move semantics.

{{zoom: 1x}} {{focus: none}} This is the standard tree representation. unique ptr children, value-semantic nodes, no raw pointers for ownership.

```code:node-code lang=cpp
#include <memory>

struct TreeNode {
    int val;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;

    TreeNode(int v) : val(v) {}
};

std::unique_ptr<TreeNode> make_node(int val,
    std::unique_ptr<TreeNode> left = nullptr,
    std::unique_ptr<TreeNode> right = nullptr)
{
    auto node = std::make_unique<TreeNode>(val);
    node->left = std::move(left);
    node->right = std::move(right);
    return node;
}

//       1
//      / \
//     2   3
//    / \
//   4   5
auto tree = make_node(1,
    make_node(2, make_node(4), make_node(5)),
    make_node(3));
---
struct: 3-9
factory: 11-19
build: 25-27
```

# Preorder traversal

{{clear: slide}}

{{show: binary-tree grow 0.5s spring}} Preorder: visit the root, then left, then right. You process the node before its children.

{{focus: visit-1}} {{annotate: visit-1 "Visit 1 first"}} Start at 1. Process it. The root is always first in preorder.

{{focus: visit-2}} {{annotate: visit-2 "Then left"}} Go left to 2. Process it. Continue left.

{{focus: visit-4}} {{annotate: visit-4 "Leaf"}} 4 is a leaf. Process it. No children. Backtrack.

{{focus: visit-5}} {{annotate: visit-5 "Right child"}} Back at 2, go right to 5. Process it. Backtrack to 1.

{{focus: visit-3}} {{annotate: visit-3 "Right subtree"}} Go right to 3. Process it. Preorder complete: 1, 2, 4, 5, 3.

{{focus: none}} Preorder visits parents before children. Use it when you need to process top-down — cloning a tree, serializing, prefix expressions.

{{clear: slide}}

{{show: call-stack grow 0.5s spring}} {{show: preorder-code typewriter 1.5s linear}} The call stack mirrors the traversal. Each recursive call pushes a frame.

{{focus: base}} {{zoom: 1.2x}} {{annotate: base "Guard"}} Base case: null node, return. Every leaf's children hit this.

{{zoom: 1x}} {{focus: process}} {{zoom: 1.3x}} {{annotate: process "Root first"}} Process the current node. In preorder, this happens before the recursive calls.

{{zoom: 1x}} {{focus: recurse}} {{zoom: 1.2x}} {{annotate: recurse "Left then right"}} Recurse left, then right. The call stack unwinds naturally.

{{zoom: 1x}} {{focus: none}} Three lines of real work. Base case, process, recurse. That's DFS on a tree.

```data:binary-tree type=tree
     1
    / \
   2   3
  / \
 4   5
---
visit-1: 1
visit-2: 2
visit-4: 4
visit-5: 5
visit-3: 3
```

```data:call-stack type=stack
["preorder(1)", "preorder(2)", "preorder(4)", "preorder(5)", "preorder(3)"]
---
base: []
process: ["preorder(1)"]
recurse: ["preorder(1)", "preorder(2)"]
```

```code:preorder-code lang=cpp
#include <vector>
#include <memory>

void preorder(const TreeNode* node, std::vector<int>& result) {
    if (node == nullptr) return;

    result.push_back(node->val);
    preorder(node->left.get(), result);
    preorder(node->right.get(), result);
}

// Output: [1, 2, 4, 5, 3]
---
base: 5
process: 7
recurse: 8-9
```

# Inorder traversal

{{clear: slide}}

{{show: inorder-tree grow 0.5s spring}} Inorder: left, root, right. For a binary search tree, this produces sorted output.

{{focus: go-left}} {{annotate: go-left "Deepest left"}} Go left as far as possible. Reach 4. It has no left child — process it.

{{focus: up-2}} {{annotate: up-2 "Visit parent"}} Back to 2. Process it. Now go right.

{{focus: visit-5}} {{annotate: visit-5 "Right child"}} Process 5. Back to 1.

{{focus: visit-1}} {{annotate: visit-1 "Root in middle"}} Process 1. The root appears in the middle — between left and right subtrees.

{{focus: visit-3}} {{annotate: visit-3 "Right subtree"}} Process 3. Inorder complete: 4, 2, 5, 1, 3.

{{focus: none}} Inorder on a BST gives sorted order. That's its main use. For general trees, it processes left before root before right.

{{clear: slide}}

{{show: inorder-code typewriter 1.5s linear}} Same structure, different order of operations.

{{focus: left-first}} {{zoom: 1.2x}} {{annotate: left-first "Left subtree"}} Recurse left first. The push back happens after left returns.

{{zoom: 1x}} {{focus: then-root}} {{zoom: 1.3x}} {{annotate: then-root "Process"}} Process the current node between the two recursive calls. This is what makes it "in order."

{{zoom: 1x}} {{focus: then-right}} {{zoom: 1.2x}} {{annotate: then-right "Right subtree"}} Recurse right last.

{{zoom: 1x}} {{focus: none}} Move the push back line — before both calls gives preorder, between gives inorder, after both gives postorder. The structure is identical.

```data:inorder-tree type=tree
     1
    / \
   2   3
  / \
 4   5
---
go-left: 4
up-2: 2
visit-5: 5
visit-1: 1
visit-3: 3
```

```code:inorder-code lang=cpp
void inorder(const TreeNode* node, std::vector<int>& result) {
    if (node == nullptr) return;

    inorder(node->left.get(), result);
    result.push_back(node->val);
    inorder(node->right.get(), result);
}

// Output: [4, 2, 5, 1, 3]
---
left-first: 4
then-root: 5
then-right: 6
```

# Postorder traversal

{{clear: slide}}

{{show: postorder-tree grow 0.5s spring}} Postorder: left, right, root. Children before parent. Process the node after its entire subtree.

{{focus: leaves}} {{annotate: leaves "Leaves first"}} Process 4, then 5. Both leaves are handled before their parent.

{{focus: parent-2}} {{annotate: parent-2 "After children"}} Now process 2. Both children are done. The parent processes last in its subtree.

{{focus: leaf-3}} {{annotate: leaf-3 "Right leaf"}} Process 3. It's a leaf on the right.

{{focus: root-last}} {{annotate: root-last "Root last"}} Process 1 last. Postorder complete: 4, 5, 2, 3, 1.

{{focus: none}} Postorder visits children before parents. Use it for bottom-up operations — computing subtree sizes, deleting trees, evaluating expression trees.

{{clear: slide}}

{{show: postorder-code typewriter 1.5s linear}} Push back moves after both recursive calls.

{{focus: children}} {{zoom: 1.2x}} {{annotate: children "Both subtrees"}} Recurse into left, then right. Both complete before the current node processes.

{{zoom: 1x}} {{focus: then-self}} {{zoom: 1.3x}} {{annotate: then-self "Process last"}} The push back is the last line. The node processes after its entire subtree has been traversed.

{{zoom: 1x}} {{focus: none}} Three traversals, one template. The only difference is where you put the push back.

```data:postorder-tree type=tree
     1
    / \
   2   3
  / \
 4   5
---
leaves: 4, 5
parent-2: 2
leaf-3: 3
root-last: 1
```

```code:postorder-code lang=cpp
void postorder(const TreeNode* node, std::vector<int>& result) {
    if (node == nullptr) return;

    postorder(node->left.get(), result);
    postorder(node->right.get(), result);
    result.push_back(node->val);
}

// Output: [4, 5, 2, 3, 1]
---
children: 4-5
then-self: 6
```

# Iterative DFS

{{clear: slide}}

{{show: iter-code typewriter 2s linear}} Recursive DFS uses the call stack. Iterative DFS uses an explicit stack. Same traversal, no recursion. Useful when recursion depth could overflow.

{{focus: init}} {{zoom: 1.2x}} {{annotate: init "Seed the stack"}} Push the root onto the stack. This replaces the first function call.

{{zoom: 1x}} {{focus: loop}} {{zoom: 1.3x}} {{annotate: loop "Pop and process"}} Each iteration pops the top node, processes it, and pushes its children. This is preorder — process before pushing children.

{{zoom: 1x}} {{focus: push-order}} {{zoom: 1.2x}} {{annotate: push-order "Right first"}} Push right before left. The stack is LIFO, so left will be popped first. This preserves left-to-right order.

{{zoom: 1x}} {{focus: null-check}} {{annotate: null-check "Skip nulls"}} Check for null before pushing. Don't push null children onto the stack.

{{zoom: 1x}} {{focus: none}} Iterative DFS avoids stack overflow for deep trees. The explicit stack replaces the call stack one-for-one.

```code:iter-code lang=cpp
#include <stack>
#include <vector>

std::vector<int> preorder_iterative(const TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) return result;

    std::stack<const TreeNode*> stk;
    stk.push(root);

    while (!stk.empty()) {
        const auto* node = stk.top();
        stk.pop();
        result.push_back(node->val);

        if (node->right) stk.push(node->right.get());
        if (node->left) stk.push(node->left.get());
    }
    return result;
}
---
init: 8-9
loop: 11-14
push-order: 16-17
null-check: 16-17
```

# The DFS template

{{clear: slide}}

{{show: dfs-template typewriter 1.5s linear}} Every tree DFS follows the same skeleton. The template handles any problem where you process nodes top-down or bottom-up.

{{focus: signature}} {{zoom: 1.2x}} {{annotate: signature "Params vary"}} The signature changes per problem. Extra parameters carry accumulated state — current depth, running sum, path so far.

{{zoom: 1x}} {{focus: base}} {{zoom: 1.3x}} {{annotate: base "Termination"}} Base case: null node returns a default value. For counting problems, zero. For boolean, false. For paths, empty.

{{zoom: 1x}} {{focus: recurse}} {{zoom: 1.2x}} {{annotate: recurse "Divide"}} Recurse on both children. Each call returns its subtree's answer.

{{zoom: 1x}} {{focus: combine}} {{zoom: 1.3x}} {{annotate: combine "Conquer"}} Combine the left and right answers with the current node's value. Max depth? One plus the max of left and right. Path sum? Check if sum equals target.

{{zoom: 1x}} {{focus: none}} Base case, recurse, combine. Every tree DFS problem maps onto this skeleton.

```code:dfs-template lang=cpp
// Tree DFS template (pseudocode)
ReturnType dfs(const TreeNode* node /*, extra params */) {
    if (node == nullptr) {
        return /* base case value */;
    }

    auto left_result = dfs(node->left.get() /*, updated params */);
    auto right_result = dfs(node->right.get() /*, updated params */);

    return /* combine(node->val, left_result, right_result) */;
}
---
signature: 2
base: 3-5
recurse: 7-8
combine: 10
```

# Common tree problems

{{clear: slide}}

{{show: depth-code typewriter 1.5s linear}} Three classics that drill the template.

{{focus: max-depth}} {{zoom: 1.2x}} {{annotate: max-depth "Height"}} Max depth: null returns 0. Otherwise 1 plus the max of left depth and right depth. Pure postorder — children report their depth, parent adds one.

{{zoom: 1x}} {{focus: none}} {{zoom: 1x}}

{{clear: slide}}

{{show: pathsum-code typewriter 1.5s linear}} Path sum: does a root-to-leaf path exist with a given sum?

{{focus: leaf-check}} {{zoom: 1.2x}} {{annotate: leaf-check "Leaf test"}} At a leaf, check if the remaining sum equals the node's value. If yes, the path works.

{{zoom: 1x}} {{focus: propagate}} {{zoom: 1.3x}} {{annotate: propagate "Subtract"}} Pass the reduced sum to children. target sum minus the current value. The sum accumulates as you descend.

{{zoom: 1x}} {{focus: none}} {{zoom: 1x}}

{{clear: slide}}

{{show: invert-code typewriter 1.5s linear}} Invert a tree: swap every node's left and right children.

{{focus: swap}} {{zoom: 1.2x}} {{annotate: swap "Mirror"}} Swap left and right, then recurse into both. Postorder or preorder both work — the swap is independent of child processing order.

{{zoom: 1x}} {{focus: none}} Max depth, path sum, invert. Three problems, one template. The DFS skeleton is your workhorse.

```code:depth-code lang=cpp
int max_depth(const TreeNode* node) {
    if (node == nullptr) return 0;
    return 1 + std::max(
        max_depth(node->left.get()),
        max_depth(node->right.get()));
}
---
max-depth: 1-5
```

```code:pathsum-code lang=cpp
bool has_path_sum(const TreeNode* node, int target) {
    if (node == nullptr) return false;

    if (!node->left && !node->right) {
        return node->val == target;
    }

    int remaining = target - node->val;
    return has_path_sum(node->left.get(), remaining)
        || has_path_sum(node->right.get(), remaining);
}
---
leaf-check: 4-6
propagate: 8-10
```

```code:invert-code lang=cpp
TreeNode* invert(TreeNode* node) {
    if (node == nullptr) return nullptr;

    std::swap(node->left, node->right);
    invert(node->left.get());
    invert(node->right.get());
    return node;
}
---
swap: 4-6
```
