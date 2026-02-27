---
title: Matrix problems
---

# Matrix traversal

{{show: traverse-code typewriter 2s linear}} A matrix is a 2D array. How you traverse it determines both correctness and performance.

{{focus: row-major}} {{zoom: 1.2x}} {{annotate: row-major "Cache-friendly"}} Row-major order: iterate rows first, columns second. In C++, 2D vectors store rows contiguously. Row-major traversal follows memory layout — every access hits the cache.

{{zoom: 1x}} {{focus: col-major}} {{zoom: 1.2x}} {{annotate: col-major "Cache-hostile"}} Column-major: iterate columns first. Each access jumps across rows — a different cache line every time. Same algorithm, 10x slower on large matrices.

{{zoom: 1x}} {{focus: diagonal}} {{zoom: 1.2x}} {{annotate: diagonal "i + j = constant"}} Diagonal traversal: cells where i plus j equals a constant lie on the same anti-diagonal. Useful for DP dependency patterns.

{{zoom: 1x}} {{focus: none}} Always prefer row-major in C++. If the algorithm demands column access, consider transposing first.

```code:traverse-code lang=cpp
// Row-major (fast — follows memory layout)
for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
        process(matrix[i][j]);

// Column-major (slow — cache misses)
for (int j = 0; j < cols; j++)
    for (int i = 0; i < rows; i++)
        process(matrix[i][j]);

// Anti-diagonal (i + j = d)
for (int d = 0; d < rows + cols - 1; d++)
    for (int i = std::max(0, d - cols + 1); i < std::min(rows, d + 1); i++)
        process(matrix[i][d - i]);
---
row-major: 2-4
col-major: 7-9
diagonal: 12-14
```

# Matrix rotation

{{clear: slide}}

{{show: rotate-code typewriter 1.5s linear}} Rotate a square matrix 90 degrees clockwise. In-place, no extra matrix.

{{focus: transpose}} {{zoom: 1.2x}} {{annotate: transpose "Flip diagonal"}} Step one: transpose. Swap matrix at i j with matrix at j i. The diagonal stays fixed. Rows become columns.

{{zoom: 1x}} {{focus: reverse-rows}} {{zoom: 1.2x}} {{annotate: reverse-rows "Mirror each row"}} Step two: reverse each row. After transposing, reversing rows completes the 90-degree rotation. Two simple passes.

{{zoom: 1x}} {{focus: none}} Transpose plus reverse-rows equals 90 clockwise. Transpose plus reverse-columns equals 90 counter-clockwise. These compose cleanly.

```code:rotate-code lang=cpp
void rotate_image(std::vector<std::vector<int>>& m) {
    int n = static_cast<int>(m.size());
    // Transpose
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            std::swap(m[i][j], m[j][i]);
    // Reverse each row
    for (auto& row : m)
        std::reverse(row.begin(), row.end());
}
---
transpose: 4-6
reverse-rows: 8-9
```

# Spiral order

{{clear: slide}}

{{show: spiral-viz grow 0.5s spring}} Peel layers from outside in. Four boundaries shrink inward with each complete loop.

{{focus: outer}} {{annotate: outer "First layer"}} Start at top-left. Go right along the top row. Down the right column. Left along the bottom row. Up the left column. One layer done.

{{focus: inner}} {{annotate: inner "Shrink boundaries"}} Shrink all four boundaries inward. Repeat. Each layer is one ring closer to the center.

{{focus: none}}

{{show: spiral-code typewriter 1.5s linear}} Four boundary variables: top, bottom, left, right. After traversing each edge, move that boundary inward.

{{focus: bounds}} {{zoom: 1.2x}} {{annotate: bounds "Four walls"}} Initialize: top at zero, bottom at rows minus one, left at zero, right at cols minus one.

{{zoom: 1x}} {{focus: traverse}} {{zoom: 1.2x}} {{annotate: traverse "Peel one layer"}} Each iteration processes four edges and shrinks the box. Check boundary crossings to avoid duplicates in non-square matrices.

{{zoom: 1x}} {{focus: none}}

```data:spiral-viz type=matrix
[[1,2,3],[8,9,4],[7,6,5]]
---
outer: [0][0], [0][1], [0][2], [1][2], [2][2], [2][1], [2][0], [1][0]
inner: [1][1]
```

```code:spiral-code lang=cpp
std::vector<int> spiral_order(const std::vector<std::vector<int>>& m) {
    if (m.empty()) return {};
    int top = 0, bot = static_cast<int>(m.size()) - 1;
    int left = 0, right = static_cast<int>(m[0].size()) - 1;
    std::vector<int> result;
    while (top <= bot && left <= right) {
        for (int j = left; j <= right; j++) result.push_back(m[top][j]);
        top++;
        for (int i = top; i <= bot; i++) result.push_back(m[i][right]);
        right--;
        if (top <= bot) {
            for (int j = right; j >= left; j--) result.push_back(m[bot][j]);
            bot--;
        }
        if (left <= right) {
            for (int i = bot; i >= top; i--) result.push_back(m[i][left]);
            left++;
        }
    }
    return result;
}
---
bounds: 3-4
traverse: 6-18
```

# Set matrix zeroes

{{clear: slide}}

{{show: zeroes-code typewriter 2s linear}} If an element is zero, set its entire row and column to zero. Do it in-place with O(1) extra space.

{{focus: markers}} {{zoom: 1.2x}} {{annotate: markers "Use first row/col"}} The trick: use the first row and first column as markers. If cell i j is zero, set m at i zero and m at zero j to zero. Two booleans track whether the first row and first column themselves need zeroing.

{{zoom: 1x}} {{focus: apply}} {{zoom: 1.2x}} {{annotate: apply "Second pass"}} Second pass: for each cell not in the first row or column, if its row marker or column marker is zero, set it to zero.

{{zoom: 1x}} {{focus: first-rc}} {{zoom: 1.2x}} {{annotate: first-rc "Handle edges"}} Finally, zero out the first row and first column if their booleans were set. Order matters — do this last.

{{zoom: 1x}} {{focus: none}} Two passes, O(1) space, clean separation of marking and applying.

```code:zeroes-code lang=cpp
void set_zeroes(std::vector<std::vector<int>>& m) {
    int rows = static_cast<int>(m.size());
    int cols = static_cast<int>(m[0].size());
    bool first_row = false, first_col = false;
    // Mark
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (m[i][j] == 0) {
                if (i == 0) first_row = true;
                if (j == 0) first_col = true;
                m[i][0] = 0;
                m[0][j] = 0;
            }
    // Apply (skip first row/col)
    for (int i = 1; i < rows; i++)
        for (int j = 1; j < cols; j++)
            if (m[i][0] == 0 || m[0][j] == 0) m[i][j] = 0;
    // Handle first row/col
    if (first_row) for (int j = 0; j < cols; j++) m[0][j] = 0;
    if (first_col) for (int i = 0; i < rows; i++) m[i][0] = 0;
}
---
markers: 6-13
apply: 15-17
first-rc: 19-20
```

# Search 2D matrix

{{clear: slide}}

{{show: search-code typewriter 1.5s linear}} A matrix where each row is sorted and the first element of each row is greater than the last element of the previous row. Search for a target in O(log(m*n)).

{{focus: flatten}} {{zoom: 1.2x}} {{annotate: flatten "Virtual 1D array"}} Treat the matrix as a flattened sorted array. Index k maps to row k divided by cols, column k mod cols. Now it's a standard binary search.

{{zoom: 1x}} {{focus: binary}} {{zoom: 1.2x}} {{annotate: binary "Standard search"}} Binary search on indices 0 through m times n minus one. Convert each mid to row and column. Compare and narrow.

{{zoom: 1x}} {{focus: none}} One binary search, O(log(m*n)). No need to search rows separately.

```code:search-code lang=cpp
bool search_matrix(const std::vector<std::vector<int>>& m, int target) {
    if (m.empty()) return false;
    int rows = static_cast<int>(m.size());
    int cols = static_cast<int>(m[0].size());
    int lo = 0, hi = rows * cols - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int val = m[mid / cols][mid % cols];
        if (val == target) return true;
        if (val < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return false;
}
---
flatten: 5-8
binary: 6-12
```
