#include "solution.h"
#include <sstream>
#include <format>

std::map<std::string, int> count_words(std::string_view text) {
    std::map<std::string, int> counts;
    std::istringstream stream(std::string(text));
    std::string word;
    while (stream >> word) {
        ++counts[word];
    }
    return counts;
}

std::string most_frequent(const std::map<std::string, int>& counts) {
    std::string best;
    int best_count = 0;
    for (const auto& [word, count] : counts) {
        if (count > best_count) {
            best = word;
            best_count = count;
        }
    }
    return best;
}

std::string format_counts(const std::map<std::string, int>& counts) {
    std::string result;
    bool first = true;
    for (const auto& [word, count] : counts) {
        if (!first) result += ", ";
        result += std::format("{}: {}", word, count);
        first = false;
    }
    return result;
}
