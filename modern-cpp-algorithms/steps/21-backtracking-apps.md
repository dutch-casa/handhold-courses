---
title: Backtracking applications
---

# Word search

{{show: wordsearch-code typewriter 2s linear}} Find a word in a 2D grid by moving through adjacent cells. Each cell used at most once per path. This is DFS with backtracking on a grid.

{{focus: start-scan}} {{zoom: 1.2x}} {{annotate: start-scan "Try every cell"}} Scan every cell. If it matches the first character, start a DFS from there.

{{zoom: 1x}} {{focus: base}} {{zoom: 1.3x}} {{annotate: base "Word complete"}} Base case: the index equals the word length. We matched every character. Return true.

{{zoom: 1x}} {{focus: bounds}} {{zoom: 1.2x}} {{annotate: bounds "Guard"}} Guard: out of bounds, already visited (marked '#'), or character mismatch. Any of these — return false immediately.

{{zoom: 1x}} {{focus: mark}} {{zoom: 1.3x}} {{annotate: mark "Choose"}} Choose: mark the cell as visited by replacing it with '#'. This prevents revisiting during this path.

{{zoom: 1x}} {{focus: explore}} {{zoom: 1.2x}} {{annotate: explore "Four directions"}} Explore: recurse in all four directions. If any path finds the word, return true.

{{zoom: 1x}} {{focus: restore}} {{zoom: 1.3x}} {{annotate: restore "Unchoose"}} Unchoose: restore the original character. The cell is available for other paths. This is the backtrack.

{{zoom: 1x}} {{focus: none}} Grid backtracking: mark, explore, restore. The grid itself serves as the visited set.

```code:wordsearch-code lang=cpp
#include <vector>
#include <string>
#include <array>

bool dfs(std::vector<std::vector<char>>& board, const std::string& word,
         int r, int c, int idx) {
    if (idx == static_cast<int>(word.size())) return true;

    int rows = static_cast<int>(board.size());
    int cols = static_cast<int>(board[0].size());
    if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
    if (board[r][c] != word[idx]) return false;

    char saved = board[r][c];
    board[r][c] = '#';

    constexpr std::array<std::pair<int,int>, 4> dirs = {{{-1,0},{1,0},{0,-1},{0,1}}};
    for (auto [dr, dc] : dirs) {
        if (dfs(board, word, r + dr, c + dc, idx + 1)) return true;
    }

    board[r][c] = saved;
    return false;
}

bool word_search(std::vector<std::vector<char>>& board, const std::string& word) {
    for (int r = 0; r < static_cast<int>(board.size()); ++r) {
        for (int c = 0; c < static_cast<int>(board[0].size()); ++c) {
            if (dfs(board, word, r, c, 0)) return true;
        }
    }
    return false;
}
---
start-scan: 26-31
base: 7
bounds: 10-12
mark: 14-15
explore: 17-20
restore: 22
```

# Sudoku solver

{{clear: slide}}

{{show: sudoku-code typewriter 2s linear}} Fill a 9x9 grid so that every row, column, and 3x3 box contains digits 1 through 9. Pure constraint backtracking.

{{focus: find-empty}} {{zoom: 1.2x}} {{annotate: find-empty "Next blank"}} Scan for the first empty cell (marked '.'). If none found, the board is complete — return true.

{{zoom: 1x}} {{focus: try-digits}} {{zoom: 1.3x}} {{annotate: try-digits "1 through 9"}} Try placing digits 1 through 9. Each digit is a candidate.

{{zoom: 1x}} {{focus: valid}} {{zoom: 1.2x}} {{annotate: valid "Check constraints"}} Validate: check the row, column, and 3x3 box. If the digit violates any constraint, skip it. This is the pruning.

{{zoom: 1x}} {{focus: place}} {{zoom: 1.3x}} {{annotate: place "Choose"}} Place the digit and recurse. If recursion succeeds, the puzzle is solved.

{{zoom: 1x}} {{focus: undo}} {{zoom: 1.2x}} {{annotate: undo "Unchoose"}} If recursion fails, remove the digit. Try the next candidate. If no digit works, return false — the caller backtracks further.

{{zoom: 1x}} {{focus: none}} Sudoku is the poster child for constraint backtracking. The validity check prunes the decision tree massively — from 9 to the 81st possibilities down to a manageable search.

```code:sudoku-code lang=cpp
#include <vector>

bool is_valid(const std::vector<std::vector<char>>& board, int r, int c, char ch) {
    for (int i = 0; i < 9; ++i) {
        if (board[r][i] == ch) return false;          // row
        if (board[i][c] == ch) return false;          // column
        int br = 3 * (r / 3) + i / 3;
        int bc = 3 * (c / 3) + i % 3;
        if (board[br][bc] == ch) return false;        // box
    }
    return true;
}

bool solve_sudoku(std::vector<std::vector<char>>& board) {
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board[r][c] != '.') continue;

            for (char ch = '1'; ch <= '9'; ++ch) {
                if (!is_valid(board, r, c, ch)) continue;

                board[r][c] = ch;
                if (solve_sudoku(board)) return true;
                board[r][c] = '.';
            }
            return false;  // no digit works → backtrack
        }
    }
    return true;  // all cells filled
}
---
find-empty: 16-18
try-digits: 20
valid: 21
place: 23-24
undo: 25-26
```

# Generate parentheses

{{clear: slide}}

{{show: parens-code typewriter 2s linear}} Generate all valid combinations of n pairs of parentheses. The constraint: at every point, the number of open parens must be at least the number of close parens.

{{focus: base}} {{zoom: 1.2x}} {{annotate: base "Complete"}} Base case: the string has 2n characters. It's a valid combination. Save it.

{{zoom: 1x}} {{focus: open}} {{zoom: 1.3x}} {{annotate: open "Add open"}} If open count is less than n, we can add an open paren. This branch always works — you can always open a new pair.

{{zoom: 1x}} {{focus: close}} {{zoom: 1.2x}} {{annotate: close "Add close"}} If close count is less than open count, we can add a close paren. This is the constraint — you can only close what's been opened.

{{zoom: 1x}} {{focus: none}} Two choices at each step: open or close. The constraint prunes invalid sequences. No explicit unchoose needed because string concatenation creates new copies.

```code:parens-code lang=cpp
#include <vector>
#include <string>

void generate(int n, int open, int close,
              std::string& current, std::vector<std::string>& results) {
    if (static_cast<int>(current.size()) == 2 * n) {
        results.push_back(current);
        return;
    }

    if (open < n) {
        current.push_back('(');
        generate(n, open + 1, close, current, results);
        current.pop_back();
    }

    if (close < open) {
        current.push_back(')');
        generate(n, open, close + 1, current, results);
        current.pop_back();
    }
}

// generate(3, ...) produces:
// ((())), (()()),  (())(), ()(()),  ()()()
---
base: 6-9
open: 11-14
close: 16-19
```

# Partition problems

{{clear: slide}}

{{show: partition-code typewriter 2s linear}} Partition a string into all possible sets of palindromic substrings. Backtracking where the "candidate" is a substring, not a single element.

{{focus: check-pal}} {{zoom: 1.2x}} {{annotate: check-pal "Valid partition"}} At each position, try every possible prefix. If the prefix is a palindrome, it's a valid partition segment. Recurse on the remainder.

{{zoom: 1x}} {{focus: try-prefix}} {{zoom: 1.3x}} {{annotate: try-prefix "Grow prefix"}} The inner loop extends the prefix from length 1 to the remaining string length. Each valid palindrome prefix becomes a choice.

{{zoom: 1x}} {{focus: base-part}} {{zoom: 1.2x}} {{annotate: base-part "Consumed all"}} Base case: start index equals string length. The entire string has been partitioned into palindromes. Save the result.

{{zoom: 1x}} {{focus: backtrack}} {{annotate: backtrack "Pop segment"}} Standard backtrack: push the palindrome substring, recurse, pop.

{{zoom: 1x}} {{focus: none}} Partition problems: instead of choosing individual elements, choose how far to extend the next segment. The palindrome check is the pruning.

```code:partition-code lang=cpp
#include <vector>
#include <string>

bool is_palindrome(const std::string& s, int lo, int hi) {
    while (lo < hi) {
        if (s[lo] != s[hi]) return false;
        ++lo; --hi;
    }
    return true;
}

void partition(const std::string& s, int start,
               std::vector<std::string>& current,
               std::vector<std::vector<std::string>>& results) {
    if (start == static_cast<int>(s.size())) {
        results.push_back(current);
        return;
    }

    for (int end = start; end < static_cast<int>(s.size()); ++end) {
        if (!is_palindrome(s, start, end)) continue;

        current.push_back(s.substr(start, end - start + 1));
        partition(s, end + 1, current, results);
        current.pop_back();
    }
}

// partition("aab") produces:
// {{"a","a","b"}, {"aa","b"}}
---
check-pal: 4-9
try-prefix: 20-21
base-part: 15-18
backtrack: 23-25
```

# String backtracking

{{clear: slide}}

{{show: lettercombo-code typewriter 2s linear}} Phone number letter combinations. Each digit maps to letters. Generate all possible strings — one letter per digit.

{{focus: mapping}} {{zoom: 1.2x}} {{annotate: mapping "Digit → letters"}} Each digit maps to a group of letters. The mapping is the candidate set at each decision level.

{{zoom: 1x}} {{focus: base}} {{zoom: 1.3x}} {{annotate: base "All digits used"}} When the current string has as many characters as digits, it's complete.

{{zoom: 1x}} {{focus: iterate}} {{zoom: 1.2x}} {{annotate: iterate "Try each letter"}} For the current digit, iterate its mapped letters. Each letter is a candidate. Push, recurse, pop.

{{zoom: 1x}} {{focus: none}} String backtracking problems: letter combinations, IP address restoration, decode ways. The candidates change at each level, but the template is identical.

```code:lettercombo-code lang=cpp
#include <vector>
#include <string>
#include <array>

std::vector<std::string> letter_combos(const std::string& digits) {
    if (digits.empty()) return {};

    constexpr std::array<const char*, 10> mapping = {
        "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
    };

    std::vector<std::string> results;
    std::string current;

    auto backtrack = [&](auto& self, int idx) -> void {
        if (idx == static_cast<int>(digits.size())) {
            results.push_back(current);
            return;
        }
        for (const char* p = mapping[digits[idx] - '0']; *p != '\0'; ++p) {
            current.push_back(*p);
            self(self, idx + 1);
            current.pop_back();
        }
    };

    backtrack(backtrack, 0);
    return results;
}
---
mapping: 8-10
base: 16-19
iterate: 20-24
```

# Backtracking vs DP

{{clear: slide}}

{{show: compare grow 0.5s spring}} When do you backtrack, and when do you use dynamic programming? The key differentiator: overlapping subproblems.

{{focus: bt-use}} {{annotate: bt-use "All solutions"}} Backtracking: you need all solutions, or the search space has no overlapping subproblems. Permutations, combinations, constraint satisfaction. Each path is unique.

{{focus: dp-use}} {{annotate: dp-use "Optimal value"}} DP: you need the optimal value (count, min, max) and subproblems overlap. Fibonacci, shortest paths, knapsack. Many paths share the same subproblem.

{{focus: overlap}} {{annotate: overlap "Shared work?"}} The test: do different decision paths lead to the same subproblem? If yes, DP memoizes and avoids recomputation. If no, backtracking is the right tool.

{{focus: none}} Backtracking generates. DP optimizes. Sometimes you start with backtracking, notice overlapping subproblems, and add memoization — that's top-down DP. The transition is natural.

```data:compare type=array
["Backtracking: generate all solutions, unique paths", "DP: optimize (count/min/max), overlapping subproblems", "Overlap test: same state reached via different choices?", "Hybrid: backtracking + memoization = top-down DP"]
---
bt-use: 0
dp-use: 1
overlap: 2
```
