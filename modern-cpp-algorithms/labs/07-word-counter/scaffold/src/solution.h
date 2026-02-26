#pragma once
#include <map>
#include <string>
#include <string_view>

std::map<std::string, int> count_words(std::string_view text);
std::string most_frequent(const std::map<std::string, int>& counts);
std::string format_counts(const std::map<std::string, int>& counts);
