#pragma once
#include <vector>

// Return all elements of matrix in clockwise spiral order.
std::vector<int> spiral_order(const std::vector<std::vector<int>>& matrix);

// Rotate an n×n matrix 90° clockwise in-place.
void rotate_image(std::vector<std::vector<int>>& matrix);

// Set entire row and column to 0 for any cell that is 0.
void set_zeroes(std::vector<std::vector<int>>& matrix);

// Binary search in a row-sorted matrix (each row sorted, rows ascending).
bool search_matrix(const std::vector<std::vector<int>>& matrix, int target);
