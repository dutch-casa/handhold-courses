#include "solution.h"
#include <sstream>

ParseResult parse_config(std::string_view text) {
    Config cfg;
    std::istringstream stream{std::string(text)};
    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty()) {
            continue;
        }
        auto pos = line.find('=');
        if (pos == std::string::npos) {
            return ParseError{"malformed line: " + line};
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        cfg.entries[std::move(key)] = std::move(value);
    }
    return cfg;
}

std::optional<std::string> get_value(const Config& cfg, std::string_view key) {
    auto it = cfg.entries.find(std::string(key));
    if (it == cfg.entries.end()) {
        return std::nullopt;
    }
    return it->second;
}
