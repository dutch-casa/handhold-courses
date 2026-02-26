#include "solution.h"
#include <sstream>

ParseResult parse_config(std::string_view text) {
    // TODO: Parse "key=value\n" lines into Config, return ParseError for malformed lines
    return ParseError{"not implemented"};
}

std::optional<std::string> get_value(const Config& cfg, std::string_view key) {
    // TODO: Look up key in cfg.entries, return nullopt if missing
    return std::nullopt;
}
