#include "solution.h"
#include <unordered_map>
#include <algorithm>

int longest_unique_substr(std::string_view s) {
    std::unordered_map<char, int> last_seen;
    int best = 0, left = 0;
    for (int right = 0; right < static_cast<int>(s.size()); right++) {
        auto it = last_seen.find(s[right]);
        if (it != last_seen.end() && it->second >= left) {
            left = it->second + 1;
        }
        last_seen[s[right]] = right;
        best = std::max(best, right - left + 1);
    }
    return best;
}

std::string min_window(std::string_view s, std::string_view t) {
    if (t.empty() || s.size() < t.size()) return "";

    std::unordered_map<char, int> need, have;
    for (char c : t) need[c]++;

    int required = static_cast<int>(need.size());
    int formed = 0;
    int best_len = static_cast<int>(s.size()) + 1;
    int best_start = 0;
    int left = 0;

    for (int right = 0; right < static_cast<int>(s.size()); right++) {
        char c = s[right];
        have[c]++;
        auto it = need.find(c);
        if (it != need.end() && have[c] == it->second) {
            formed++;
        }

        while (formed == required) {
            int window_len = right - left + 1;
            if (window_len < best_len) {
                best_len = window_len;
                best_start = left;
            }
            char lc = s[left];
            have[lc]--;
            auto lit = need.find(lc);
            if (lit != need.end() && have[lc] < lit->second) {
                formed--;
            }
            left++;
        }
    }

    if (best_len > static_cast<int>(s.size())) return "";
    return std::string(s.substr(best_start, best_len));
}
