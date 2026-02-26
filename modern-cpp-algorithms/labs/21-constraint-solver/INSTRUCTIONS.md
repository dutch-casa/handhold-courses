# Constraint Solver — Advanced Backtracking

Backtracking with constraint propagation solves complex combinatorial problems by pruning invalid branches early.

## Tasks

- [ ] Implement `solve_sudoku(board)` — solve a 9x9 sudoku puzzle in place. Empty cells contain `0`. Return `true` if solved, `false` if unsolvable.
- [ ] Implement `generate_parentheses(n)` — return all valid combinations of `n` pairs of parentheses.

## Hints

- `solve_sudoku`: find the next empty cell, try digits 1-9, check row/column/3x3 box constraints, recurse. Backtrack on failure.
- `generate_parentheses`: track open and close counts. Add `(` if open < n, add `)` if close < open.

Use `Run Tests` to check your progress.
