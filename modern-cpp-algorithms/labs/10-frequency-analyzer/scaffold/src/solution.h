#pragma once
#include <map>
#include <string>
#include <string_view>

std::map<char, int> char_freq(std::string_view text);
std::map<std::string, int> word_freq(std::string_view text);
std::map<std::string, int> bigram_freq(std::string_view text);
std::string most_common_bigram(std::string_view text);
