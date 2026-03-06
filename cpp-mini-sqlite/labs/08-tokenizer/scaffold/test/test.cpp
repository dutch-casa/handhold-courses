#include <gtest/gtest.h>
#include "solution.h"

namespace {

TEST(TokenizerTest, Empty) {
  auto result = tokenize("");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_EQ((*result)[0].kind, TokenKind::kEof);
  EXPECT_TRUE((*result)[0].value.empty());
}

TEST(TokenizerTest, WhitespaceOnly) {
  auto result = tokenize("   \n\t  ");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_EQ((*result)[0].kind, TokenKind::kEof);
}

TEST(TokenizerTest, Keywords) {
  auto result = tokenize("SELECT * FROM users");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 5u);
  EXPECT_EQ((*result)[0].kind, TokenKind::kSelect);
  EXPECT_EQ((*result)[0].value, "SELECT");
  EXPECT_EQ((*result)[1].kind, TokenKind::kStar);
  EXPECT_EQ((*result)[1].value, "*");
  EXPECT_EQ((*result)[2].kind, TokenKind::kFrom);
  EXPECT_EQ((*result)[2].value, "FROM");
  EXPECT_EQ((*result)[3].kind, TokenKind::kIdent);
  EXPECT_EQ((*result)[3].value, "users");
  EXPECT_EQ((*result)[4].kind, TokenKind::kEof);
}

TEST(TokenizerTest, CaseInsensitive) {
  auto result = tokenize("select * from users");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 5u);
  EXPECT_EQ((*result)[0].kind, TokenKind::kSelect);
  EXPECT_EQ((*result)[1].kind, TokenKind::kStar);
  EXPECT_EQ((*result)[2].kind, TokenKind::kFrom);
  EXPECT_EQ((*result)[3].kind, TokenKind::kIdent);
  EXPECT_EQ((*result)[4].kind, TokenKind::kEof);
}

TEST(TokenizerTest, IntegerLiteral) {
  auto result = tokenize("42");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_EQ((*result)[0].kind, TokenKind::kInteger);
  EXPECT_EQ((*result)[0].value, "42");
  EXPECT_EQ((*result)[1].kind, TokenKind::kEof);
}

TEST(TokenizerTest, StringLiteral) {
  auto result = tokenize("'hello'");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_EQ((*result)[0].kind, TokenKind::kString);
  EXPECT_EQ((*result)[0].value, "'hello'");
  EXPECT_EQ((*result)[1].kind, TokenKind::kEof);
}

TEST(TokenizerTest, Operators) {
  auto result = tokenize("<= >= !=");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 4u);
  EXPECT_EQ((*result)[0].kind, TokenKind::kLe);
  EXPECT_EQ((*result)[1].kind, TokenKind::kGe);
  EXPECT_EQ((*result)[2].kind, TokenKind::kNe);
  EXPECT_EQ((*result)[3].kind, TokenKind::kEof);
}

TEST(TokenizerTest, UnterminatedString) {
  auto result = tokenize("'oops");
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DbError::kSyntax);
}

TEST(TokenizerTest, FullStatement) {
  auto result = tokenize("SELECT * FROM t WHERE id = 42");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 9u);
  EXPECT_EQ((*result)[0].kind,  TokenKind::kSelect);
  EXPECT_EQ((*result)[1].kind,  TokenKind::kStar);
  EXPECT_EQ((*result)[2].kind,  TokenKind::kFrom);
  EXPECT_EQ((*result)[3].kind,  TokenKind::kIdent);
  EXPECT_EQ((*result)[3].value, "t");
  EXPECT_EQ((*result)[4].kind,  TokenKind::kWhere);
  EXPECT_EQ((*result)[5].kind,  TokenKind::kIdent);
  EXPECT_EQ((*result)[5].value, "id");
  EXPECT_EQ((*result)[6].kind,  TokenKind::kEq);
  EXPECT_EQ((*result)[7].kind,  TokenKind::kInteger);
  EXPECT_EQ((*result)[7].value, "42");
  EXPECT_EQ((*result)[8].kind,  TokenKind::kEof);
}

}  // namespace
