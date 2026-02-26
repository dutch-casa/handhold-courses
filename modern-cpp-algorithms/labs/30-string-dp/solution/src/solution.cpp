#include "solution.h"
#include <string>
#include <vector>
#include <algorithm>

std::string lcs(const std::string& a, const std::string& b) {
    int m = static_cast<int>(a.size());
    int n = static_cast<int>(b.size());
    // dp[i][j] = LCS length for a[0..i) and b[0..j)
    std::vector<std::vector<int>> dp(
        static_cast<std::size_t>(m + 1),
        std::vector<int>(static_cast<std::size_t>(n + 1), 0)
    );
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            auto si = static_cast<std::size_t>(i);
            auto sj = static_cast<std::size_t>(j);
            if (a[static_cast<std::size_t>(i - 1)] == b[static_cast<std::size_t>(j - 1)]) {
                dp[si][sj] = dp[si - 1][sj - 1] + 1;
            } else {
                dp[si][sj] = std::max(dp[si - 1][sj], dp[si][sj - 1]);
            }
        }
    }
    // Reconstruct by traceback
    std::string result;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        auto si = static_cast<std::size_t>(i);
        auto sj = static_cast<std::size_t>(j);
        if (a[si - 1] == b[sj - 1]) {
            result += a[si - 1];
            --i;
            --j;
        } else if (dp[si - 1][sj] > dp[si][sj - 1]) {
            --i;
        } else {
            --j;
        }
    }
    std::ranges::reverse(result);
    return result;
}

int edit_distance(const std::string& a, const std::string& b) {
    int m = static_cast<int>(a.size());
    int n = static_cast<int>(b.size());
    // dp[i][j] = edit distance between a[0..i) and b[0..j)
    std::vector<std::vector<int>> dp(
        static_cast<std::size_t>(m + 1),
        std::vector<int>(static_cast<std::size_t>(n + 1), 0)
    );
    for (int i = 0; i <= m; ++i) dp[static_cast<std::size_t>(i)][0] = i;
    for (int j = 0; j <= n; ++j) dp[0][static_cast<std::size_t>(j)] = j;
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            auto si = static_cast<std::size_t>(i);
            auto sj = static_cast<std::size_t>(j);
            int cost = (a[si - 1] == b[sj - 1]) ? 0 : 1;
            dp[si][sj] = std::min({
                dp[si - 1][sj] + 1,     // delete from a
                dp[si][sj - 1] + 1,     // insert into a
                dp[si - 1][sj - 1] + cost  // replace
            });
        }
    }
    return dp[static_cast<std::size_t>(m)][static_cast<std::size_t>(n)];
}

std::string longest_palindrome(const std::string& s) {
    if (s.empty()) return "";
    int n = static_cast<int>(s.size());
    int best_start = 0, best_len = 1;

    auto expand = [&](int left, int right) {
        while (left >= 0 && right < n && s[static_cast<std::size_t>(left)] == s[static_cast<std::size_t>(right)]) {
            --left;
            ++right;
        }
        // Window is s[left+1 .. right-1]
        int len = right - left - 1;
        if (len > best_len) {
            best_len = len;
            best_start = left + 1;
        }
    };

    for (int i = 0; i < n; ++i) {
        expand(i, i);       // odd length
        expand(i, i + 1);   // even length
    }
    return s.substr(static_cast<std::size_t>(best_start), static_cast<std::size_t>(best_len));
}
