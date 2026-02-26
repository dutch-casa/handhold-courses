#include "solution.h"
#include <sstream>
#include <vector>

std::map<char, int> char_freq(std::string_view text) {
    std::map<char, int> freq;
    for (char c : text) {
        ++freq[c];
    }
    return freq;
}

std::map<std::string, int> word_freq(std::string_view text) {
    std::map<std::string, int> freq;
    std::istringstream stream{std::string(text)};
    std::string word;
    while (stream >> word) {
        ++freq[word];
    }
    return freq;
}

std::map<std::string, int> bigram_freq(std::string_view text) {
    std::map<std::string, int> freq;
    std::istringstream stream{std::string(text)};
    std::vector<std::string> words;
    std::string word;
    while (stream >> word) {
        words.push_back(std::move(word));
    }
    for (size_t i = 0; i + 1 < words.size(); ++i) {
        std::string key = words[i] + " " + words[i + 1];
        ++freq[key];
    }
    return freq;
}

std::string most_common_bigram(std::string_view text) {
    auto freq = bigram_freq(text);
    if (freq.empty()) {
        return "";
    }
    std::string best;
    int best_count = 0;
    for (const auto& [bigram, count] : freq) {
        if (count > best_count) {
            best = bigram;
            best_count = count;
        }
    }
    return best;
}
