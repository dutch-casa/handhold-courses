#pragma once
#include <string>

// Longest common subsequence of a and b (the subsequence string, not its length).
std::string lcs(const std::string& a, const std::string& b);

// Minimum edit distance (insert, delete, substitute) from a to b.
int edit_distance(const std::string& a, const std::string& b);

// Longest palindromic substring; returns the first one if there are ties.
std::string longest_palindrome(const std::string& s);
