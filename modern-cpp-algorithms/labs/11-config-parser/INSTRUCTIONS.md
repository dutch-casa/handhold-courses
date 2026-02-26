# Safe Config Parser

Parse a simple `key=value` config format using `std::variant` for type-safe error handling.

## Tasks

- [ ] Define `Config` struct with `std::map<std::string, std::string>` for key-value pairs
- [ ] Define `ParseError` struct with a `std::string message` field
- [ ] Define `ParseResult` as `std::variant<Config, ParseError>`
- [ ] Implement `parse_config(string_view text)` — parse `"key=value\n"` lines into a `Config`, returning `ParseError` for malformed lines
- [ ] Implement `get_value(const Config& cfg, string_view key)` — return `optional<string>` for a given key

## Hints

Split the input by newlines. For each non-empty line, find the `=` separator. If a line has no `=`, return a `ParseError`.

Use `std::holds_alternative<Config>(result)` and `std::get<Config>(result)` to inspect the variant.

Empty lines should be skipped, not treated as errors.

Use `Run Tests` in the testing panel to check your progress.
