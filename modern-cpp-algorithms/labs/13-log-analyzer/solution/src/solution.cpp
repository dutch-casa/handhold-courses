#include "solution.h"
#include <sstream>
#include <algorithm>

std::vector<std::string> split_lines(std::string_view text) {
    std::vector<std::string> result;
    std::istringstream stream{std::string(text)};
    std::string line;
    while (std::getline(stream, line)) {
        if (!line.empty()) {
            result.push_back(std::move(line));
        }
    }
    return result;
}

std::vector<std::string> filter_by_level(const std::vector<std::string>& lines, std::string_view level) {
    std::vector<std::string> result;
    std::string needle(level);
    for (const auto& line : lines) {
        if (line.find(needle) != std::string::npos) {
            result.push_back(line);
        }
    }
    return result;
}

std::vector<std::string> take_recent(const std::vector<std::string>& lines, int n) {
    if (n <= 0) {
        return {};
    }
    auto count = static_cast<size_t>(n);
    if (count >= lines.size()) {
        return lines;
    }
    return {lines.end() - static_cast<ptrdiff_t>(count), lines.end()};
}

int count_level(const std::vector<std::string>& lines, std::string_view level) {
    std::string needle(level);
    int count = 0;
    for (const auto& line : lines) {
        if (line.find(needle) != std::string::npos) {
            ++count;
        }
    }
    return count;
}
