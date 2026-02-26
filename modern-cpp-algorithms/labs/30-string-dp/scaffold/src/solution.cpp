#include "solution.h"
#include <string>

// TODO: build (m+1)x(n+1) DP table then trace back to reconstruct the subsequence
std::string lcs(const std::string& a, const std::string& b) {
    // TODO: dp[i][j] = LCS length of a[0..i) and b[0..j)
    return "";
}

// TODO: dp[i][j] = edit distance between a[0..i) and b[0..j)
int edit_distance(const std::string& a, const std::string& b) {
    // TODO: base cases are empty prefix conversions; fill row by row
    return 0;
}

// TODO: expand around each center (both odd and even length)
std::string longest_palindrome(const std::string& s) {
    // TODO: track best start and length; return substr at end
    return "";
}
