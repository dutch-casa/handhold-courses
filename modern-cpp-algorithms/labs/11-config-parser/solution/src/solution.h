#pragma once
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

struct Config {
    std::map<std::string, std::string> entries;
};

struct ParseError {
    std::string message;
};

using ParseResult = std::variant<Config, ParseError>;

ParseResult parse_config(std::string_view text);
std::optional<std::string> get_value(const Config& cfg, std::string_view key);
