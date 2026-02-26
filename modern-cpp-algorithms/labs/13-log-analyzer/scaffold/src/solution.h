#pragma once
#include <string>
#include <string_view>
#include <vector>

std::vector<std::string> split_lines(std::string_view text);
std::vector<std::string> filter_by_level(const std::vector<std::string>& lines, std::string_view level);
std::vector<std::string> take_recent(const std::vector<std::string>& lines, int n);
int count_level(const std::vector<std::string>& lines, std::string_view level);
