#include <gtest/gtest.h>
#include "solution.h"

namespace {

// Helpers for building token vectors without a tokenizer dependency.
static Token tok(TokenKind k, std::string_view v = "") {
  return Token{k, v};
}

// ── SelectStar ──────────────────────────────────────────────────────────────

TEST(ParserTest, SelectStar) {
  // SELECT * FROM users
  std::vector<Token> tokens = {
    tok(TokenKind::kSelect, "SELECT"),
    tok(TokenKind::kStar,   "*"),
    tok(TokenKind::kFrom,   "FROM"),
    tok(TokenKind::kIdent,  "users"),
    tok(TokenKind::kEof),
  };
  Parser p(tokens);
  auto result = p.parse();
  ASSERT_TRUE(result.has_value());
  auto* s = std::get_if<SelectStmt>(&*result);
  ASSERT_NE(s, nullptr);
  EXPECT_EQ(s->table, "users");
  EXPECT_FALSE(s->where.has_value());
}

// ── SelectWhere ─────────────────────────────────────────────────────────────

TEST(ParserTest, SelectWhere) {
  // SELECT * FROM t WHERE id = 42
  std::vector<Token> tokens = {
    tok(TokenKind::kSelect,  "SELECT"),
    tok(TokenKind::kStar,    "*"),
    tok(TokenKind::kFrom,    "FROM"),
    tok(TokenKind::kIdent,   "t"),
    tok(TokenKind::kWhere,   "WHERE"),
    tok(TokenKind::kIdent,   "id"),
    tok(TokenKind::kEq,      "="),
    tok(TokenKind::kInteger, "42"),
    tok(TokenKind::kEof),
  };
  Parser p(tokens);
  auto result = p.parse();
  ASSERT_TRUE(result.has_value());
  auto* s = std::get_if<SelectStmt>(&*result);
  ASSERT_NE(s, nullptr);
  EXPECT_EQ(s->table, "t");
  ASSERT_TRUE(s->where.has_value());
  EXPECT_EQ(s->where->col, "id");
  EXPECT_EQ(s->where->op, TokenKind::kEq);
  EXPECT_FALSE(s->where->rhs.is_null);
  EXPECT_EQ(s->where->rhs.integer, 42u);
}

// ── SelectWhereNull ──────────────────────────────────────────────────────────

TEST(ParserTest, SelectWhereNull) {
  // SELECT * FROM t WHERE name IS NULL
  std::vector<Token> tokens = {
    tok(TokenKind::kSelect, "SELECT"),
    tok(TokenKind::kStar,   "*"),
    tok(TokenKind::kFrom,   "FROM"),
    tok(TokenKind::kIdent,  "t"),
    tok(TokenKind::kWhere,  "WHERE"),
    tok(TokenKind::kIdent,  "name"),
    tok(TokenKind::kIs,     "IS"),
    tok(TokenKind::kNull,   "NULL"),
    tok(TokenKind::kEof),
  };
  Parser p(tokens);
  auto result = p.parse();
  ASSERT_TRUE(result.has_value());
  auto* s = std::get_if<SelectStmt>(&*result);
  ASSERT_NE(s, nullptr);
  ASSERT_TRUE(s->where.has_value());
  EXPECT_EQ(s->where->col, "name");
  EXPECT_EQ(s->where->op, TokenKind::kIs);
  EXPECT_FALSE(s->where->negate);
  EXPECT_TRUE(s->where->rhs.is_null);
}

// ── SelectWhereNotNull ───────────────────────────────────────────────────────

TEST(ParserTest, SelectWhereNotNull) {
  // SELECT * FROM t WHERE name IS NOT NULL
  std::vector<Token> tokens = {
    tok(TokenKind::kSelect, "SELECT"),
    tok(TokenKind::kStar,   "*"),
    tok(TokenKind::kFrom,   "FROM"),
    tok(TokenKind::kIdent,  "t"),
    tok(TokenKind::kWhere,  "WHERE"),
    tok(TokenKind::kIdent,  "name"),
    tok(TokenKind::kIs,     "IS"),
    tok(TokenKind::kNot,    "NOT"),
    tok(TokenKind::kNull,   "NULL"),
    tok(TokenKind::kEof),
  };
  Parser p(tokens);
  auto result = p.parse();
  ASSERT_TRUE(result.has_value());
  auto* s = std::get_if<SelectStmt>(&*result);
  ASSERT_NE(s, nullptr);
  ASSERT_TRUE(s->where.has_value());
  EXPECT_EQ(s->where->op, TokenKind::kIs);
  EXPECT_TRUE(s->where->negate);
  EXPECT_TRUE(s->where->rhs.is_null);
}

// ── CreateTable ──────────────────────────────────────────────────────────────

TEST(ParserTest, CreateTable) {
  // CREATE TABLE users (id, name)
  std::vector<Token> tokens = {
    tok(TokenKind::kCreate,  "CREATE"),
    tok(TokenKind::kTable,   "TABLE"),
    tok(TokenKind::kIdent,   "users"),
    tok(TokenKind::kLparen,  "("),
    tok(TokenKind::kIdent,   "id"),
    tok(TokenKind::kComma,   ","),
    tok(TokenKind::kIdent,   "name"),
    tok(TokenKind::kRparen,  ")"),
    tok(TokenKind::kEof),
  };
  Parser p(tokens);
  auto result = p.parse();
  ASSERT_TRUE(result.has_value());
  auto* s = std::get_if<CreateStmt>(&*result);
  ASSERT_NE(s, nullptr);
  EXPECT_EQ(s->table, "users");
  ASSERT_EQ(s->cols.size(), 2u);
  EXPECT_EQ(s->cols[0], "id");
  EXPECT_EQ(s->cols[1], "name");
}

// ── InsertValues ─────────────────────────────────────────────────────────────

TEST(ParserTest, InsertValues) {
  // INSERT INTO users VALUES (1, 'alice')
  std::vector<Token> tokens = {
    tok(TokenKind::kInsert,  "INSERT"),
    tok(TokenKind::kInto,    "INTO"),
    tok(TokenKind::kIdent,   "users"),
    tok(TokenKind::kValues,  "VALUES"),
    tok(TokenKind::kLparen,  "("),
    tok(TokenKind::kInteger, "1"),
    tok(TokenKind::kComma,   ","),
    tok(TokenKind::kString,  "'alice'"),
    tok(TokenKind::kRparen,  ")"),
    tok(TokenKind::kEof),
  };
  Parser p(tokens);
  auto result = p.parse();
  ASSERT_TRUE(result.has_value());
  auto* s = std::get_if<InsertStmt>(&*result);
  ASSERT_NE(s, nullptr);
  EXPECT_EQ(s->table, "users");
  ASSERT_EQ(s->values.size(), 2u);
  EXPECT_FALSE(s->values[0].is_null);
  EXPECT_EQ(s->values[0].integer, 1u);
  EXPECT_FALSE(s->values[1].is_null);
  EXPECT_EQ(s->values[1].text, "alice");
}

// ── SelectWhereRelOp ─────────────────────────────────────────────────────────

TEST(ParserTest, SelectWhereRelOp) {
  // SELECT * FROM t WHERE age > 10
  std::vector<Token> tokens = {
    tok(TokenKind::kSelect,  "SELECT"),
    tok(TokenKind::kStar,    "*"),
    tok(TokenKind::kFrom,    "FROM"),
    tok(TokenKind::kIdent,   "t"),
    tok(TokenKind::kWhere,   "WHERE"),
    tok(TokenKind::kIdent,   "age"),
    tok(TokenKind::kGt,      ">"),
    tok(TokenKind::kInteger, "10"),
    tok(TokenKind::kEof),
  };
  Parser p(tokens);
  auto result = p.parse();
  ASSERT_TRUE(result.has_value());
  auto* s = std::get_if<SelectStmt>(&*result);
  ASSERT_NE(s, nullptr);
  ASSERT_TRUE(s->where.has_value());
  EXPECT_EQ(s->where->col, "age");
  EXPECT_EQ(s->where->op, TokenKind::kGt);
  EXPECT_FALSE(s->where->negate);
  EXPECT_EQ(s->where->rhs.integer, 10u);
}

// ── InsertNullValue ───────────────────────────────────────────────────────────

TEST(ParserTest, InsertNullValue) {
  // INSERT INTO t VALUES (NULL)
  std::vector<Token> tokens = {
    tok(TokenKind::kInsert, "INSERT"),
    tok(TokenKind::kInto,   "INTO"),
    tok(TokenKind::kIdent,  "t"),
    tok(TokenKind::kValues, "VALUES"),
    tok(TokenKind::kLparen, "("),
    tok(TokenKind::kNull,   "NULL"),
    tok(TokenKind::kRparen, ")"),
    tok(TokenKind::kEof),
  };
  Parser p(tokens);
  auto result = p.parse();
  ASSERT_TRUE(result.has_value());
  auto* s = std::get_if<InsertStmt>(&*result);
  ASSERT_NE(s, nullptr);
  ASSERT_EQ(s->values.size(), 1u);
  EXPECT_TRUE(s->values[0].is_null);
}

// ── MissingFrom ───────────────────────────────────────────────────────────────

TEST(ParserTest, MissingFrom) {
  // SELECT * users (FROM is absent — syntax error)
  std::vector<Token> tokens = {
    tok(TokenKind::kSelect, "SELECT"),
    tok(TokenKind::kStar,   "*"),
    tok(TokenKind::kIdent,  "users"),
    tok(TokenKind::kEof),
  };
  Parser p(tokens);
  auto result = p.parse();
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DbError::kSyntax);
}

// ── UnknownStatement ─────────────────────────────────────────────────────────

TEST(ParserTest, UnknownStatement) {
  // An identifier as the leading token is not a valid statement start.
  std::vector<Token> tokens = {
    tok(TokenKind::kIdent, "foo"),
    tok(TokenKind::kEof),
  };
  Parser p(tokens);
  auto result = p.parse();
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DbError::kSyntax);
}

}  // namespace
