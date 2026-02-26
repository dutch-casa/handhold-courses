# Lab 40 — Matrix Operations

2D arrays are a staple of both interviews and real systems (image processing, game boards, linear algebra). These four operations cover traversal order, in-place transformation, constraint propagation, and staircase search.

## Tasks

- [ ] Implement `spiral_order(matrix)` — return all elements in spiral order (top row left-to-right, then right column top-to-bottom, then bottom row right-to-left, then left column bottom-to-top, repeat inward)
- [ ] Implement `rotate_image(matrix)` — rotate an n×n matrix 90° clockwise in-place; do it in two steps: transpose, then reverse each row
- [ ] Implement `set_zeroes(matrix)` — if any cell is 0, set its entire row and column to 0; do it in-place without a copy of the whole matrix
- [ ] Implement `search_matrix(matrix, target)` — search a matrix where each row is sorted and each row's first element is greater than the previous row's last; return true if target exists

## Hints

- `spiral_order`: maintain four boundaries (top, bottom, left, right); shrink each boundary after traversing that edge; stop when boundaries cross
- `rotate_image`: transpose by swapping `matrix[i][j]` with `matrix[j][i]` for i < j; then reverse each row with `std::reverse`
- `set_zeroes`: on first pass, record which rows and columns contain zeros; on second pass, zero out those rows and columns; avoid using O(m*n) extra space
- `search_matrix`: treat it as a 1D sorted array and binary search, converting mid-index to `(mid/cols, mid%cols)`; alternatively, start at top-right and step left or down

Use `Run Tests` to check your progress.
