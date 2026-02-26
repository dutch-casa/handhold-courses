#include <gtest/gtest.h>
#include "../src/solution.h"
#include <algorithm>
#include <set>

static bool is_valid_sudoku(const std::vector<std::vector<int>>& board) {
    for (int i = 0; i < 9; i++) {
        std::set<int> row_set, col_set;
        for (int j = 0; j < 9; j++) {
            if (board[i][j] < 1 || board[i][j] > 9) return false;
            if (!row_set.insert(board[i][j]).second) return false;
            if (!col_set.insert(board[j][i]).second) return false;
        }
    }
    for (int br = 0; br < 3; br++) {
        for (int bc = 0; bc < 3; bc++) {
            std::set<int> box_set;
            for (int r = br * 3; r < br * 3 + 3; r++) {
                for (int c = bc * 3; c < bc * 3 + 3; c++) {
                    if (!box_set.insert(board[r][c]).second) return false;
                }
            }
        }
    }
    return true;
}

TEST(SolveSudoku, BasicPuzzle) {
    std::vector<std::vector<int>> board = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    EXPECT_TRUE(solve_sudoku(board));
    EXPECT_TRUE(is_valid_sudoku(board));
}

TEST(SolveSudoku, AlreadySolved) {
    std::vector<std::vector<int>> board = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };
    EXPECT_TRUE(solve_sudoku(board));
    EXPECT_TRUE(is_valid_sudoku(board));
}

TEST(GenerateParentheses, N1) {
    auto result = generate_parentheses(1);
    std::sort(result.begin(), result.end());
    EXPECT_EQ(result, std::vector<std::string>{"()"});
}

TEST(GenerateParentheses, N2) {
    auto result = generate_parentheses(2);
    std::sort(result.begin(), result.end());
    std::vector<std::string> expected = {"(())", "()()"};
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST(GenerateParentheses, N3) {
    auto result = generate_parentheses(3);
    EXPECT_EQ(result.size(), 5u);
    std::set<std::string> unique(result.begin(), result.end());
    EXPECT_EQ(unique.size(), 5u);
    EXPECT_TRUE(unique.count("((()))"));
    EXPECT_TRUE(unique.count("(()())"));
    EXPECT_TRUE(unique.count("(())()"));
    EXPECT_TRUE(unique.count("()(())"));
    EXPECT_TRUE(unique.count("()()()"));
}

TEST(GenerateParentheses, N0) {
    auto result = generate_parentheses(0);
    EXPECT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0], "");
}
