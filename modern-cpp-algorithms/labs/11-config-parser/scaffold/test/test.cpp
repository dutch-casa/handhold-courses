#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(ParseConfig, BasicKeyValue) {
    auto result = parse_config("host=localhost\nport=8080\n");
    ASSERT_TRUE(std::holds_alternative<Config>(result));
    auto cfg = std::get<Config>(result);
    EXPECT_EQ(cfg.entries["host"], "localhost");
    EXPECT_EQ(cfg.entries["port"], "8080");
}

TEST(ParseConfig, SingleEntry) {
    auto result = parse_config("name=test\n");
    ASSERT_TRUE(std::holds_alternative<Config>(result));
    auto cfg = std::get<Config>(result);
    EXPECT_EQ(cfg.entries.size(), 1u);
    EXPECT_EQ(cfg.entries["name"], "test");
}

TEST(ParseConfig, EmptyValue) {
    auto result = parse_config("key=\n");
    ASSERT_TRUE(std::holds_alternative<Config>(result));
    auto cfg = std::get<Config>(result);
    EXPECT_EQ(cfg.entries["key"], "");
}

TEST(ParseConfig, SkipsEmptyLines) {
    auto result = parse_config("a=1\n\nb=2\n");
    ASSERT_TRUE(std::holds_alternative<Config>(result));
    auto cfg = std::get<Config>(result);
    EXPECT_EQ(cfg.entries.size(), 2u);
}

TEST(ParseConfig, MalformedLine) {
    auto result = parse_config("host=localhost\nbadline\nport=8080\n");
    ASSERT_TRUE(std::holds_alternative<ParseError>(result));
}

TEST(ParseConfig, EmptyInput) {
    auto result = parse_config("");
    ASSERT_TRUE(std::holds_alternative<Config>(result));
    auto cfg = std::get<Config>(result);
    EXPECT_TRUE(cfg.entries.empty());
}

TEST(ParseConfig, ValueWithEquals) {
    auto result = parse_config("expr=a=b\n");
    ASSERT_TRUE(std::holds_alternative<Config>(result));
    auto cfg = std::get<Config>(result);
    EXPECT_EQ(cfg.entries["expr"], "a=b");
}

TEST(GetValue, ExistingKey) {
    Config cfg;
    cfg.entries["host"] = "localhost";
    cfg.entries["port"] = "8080";
    auto val = get_value(cfg, "host");
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(val.value(), "localhost");
}

TEST(GetValue, MissingKey) {
    Config cfg;
    cfg.entries["host"] = "localhost";
    auto val = get_value(cfg, "missing");
    EXPECT_FALSE(val.has_value());
}

TEST(GetValue, EmptyConfig) {
    Config cfg;
    auto val = get_value(cfg, "anything");
    EXPECT_FALSE(val.has_value());
}
