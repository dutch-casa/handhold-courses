#include "solution.h"

static bool is_valid(const std::vector<std::vector<int>>& board, int row, int col, int num) {
    for (int j = 0; j < 9; j++) {
        if (board[row][j] == num) return false;
    }
    for (int i = 0; i < 9; i++) {
        if (board[i][col] == num) return false;
    }
    int box_r = (row / 3) * 3, box_c = (col / 3) * 3;
    for (int i = box_r; i < box_r + 3; i++) {
        for (int j = box_c; j < box_c + 3; j++) {
            if (board[i][j] == num) return false;
        }
    }
    return true;
}

bool solve_sudoku(std::vector<std::vector<int>>& board) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != 0) continue;
            for (int num = 1; num <= 9; num++) {
                if (!is_valid(board, i, j, num)) continue;
                board[i][j] = num;
                if (solve_sudoku(board)) return true;
                board[i][j] = 0;
            }
            return false;
        }
    }
    return true;
}

static void gen_parens(int open, int close, int n,
                       std::string& current, std::vector<std::string>& result) {
    if (static_cast<int>(current.size()) == 2 * n) {
        result.push_back(current);
        return;
    }
    if (open < n) {
        current.push_back('(');
        gen_parens(open + 1, close, n, current, result);
        current.pop_back();
    }
    if (close < open) {
        current.push_back(')');
        gen_parens(open, close + 1, n, current, result);
        current.pop_back();
    }
}

std::vector<std::string> generate_parentheses(int n) {
    if (n == 0) return {""};
    std::vector<std::string> result;
    std::string current;
    gen_parens(0, 0, n, current, result);
    return result;
}
