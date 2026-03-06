#include <cstring>
#include <gtest/gtest.h>
#include "solution.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static Row make_row(uint32_t id, const char* name) {
  Row r{};
  r.id = id;
  r.null_mask = 0;
  if (name == nullptr) {
    r.null_mask = 1;
  } else {
    std::strncpy(r.name, name, kNameMax);
  }
  return r;
}

static SelectStmt make_stmt_no_where() { return SelectStmt{}; }

static SelectStmt make_stmt_where(Predicate pred) {
  SelectStmt s{};
  s.where = std::move(pred);
  return s;
}

static Predicate id_pred(TokenKind op, uint32_t val) {
  Predicate p;
  p.col = "id";
  p.op  = op;
  p.rhs.integer = val;
  return p;
}

static Predicate name_pred(TokenKind op, const char* val) {
  Predicate p;
  p.col = "name";
  p.op  = op;
  p.rhs.text = val;
  return p;
}

static Predicate name_is_null_pred(bool negate) {
  Predicate p;
  p.col    = "name";
  p.op     = TokenKind::kIs;
  p.negate = negate;
  return p;
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

TEST(WhereFilter, IdEqFindsMatch) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, "bob")};
  auto result = execute_select_where(rows, make_stmt_where(id_pred(TokenKind::kEq, 1)));
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_EQ((*result)[0].id, 1u);
}

TEST(WhereFilter, IdEqNoMatch) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, "bob")};
  auto result = execute_select_where(rows, make_stmt_where(id_pred(TokenKind::kEq, 99)));
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(result->empty());
}

TEST(WhereFilter, IdGtFilters) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, "bob"), make_row(3, "carol")};
  auto result = execute_select_where(rows, make_stmt_where(id_pred(TokenKind::kGt, 1)));
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_EQ((*result)[0].id, 2u);
  EXPECT_EQ((*result)[1].id, 3u);
}

TEST(WhereFilter, IdLtFilters) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, "bob"), make_row(3, "carol")};
  auto result = execute_select_where(rows, make_stmt_where(id_pred(TokenKind::kLt, 3)));
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_EQ((*result)[0].id, 1u);
  EXPECT_EQ((*result)[1].id, 2u);
}

TEST(WhereFilter, IdGeIncludesBoundary) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, "bob"), make_row(3, "carol")};
  auto result = execute_select_where(rows, make_stmt_where(id_pred(TokenKind::kGe, 2)));
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_EQ((*result)[0].id, 2u);
  EXPECT_EQ((*result)[1].id, 3u);
}

TEST(WhereFilter, IdLeIncludesBoundary) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, "bob"), make_row(3, "carol")};
  auto result = execute_select_where(rows, make_stmt_where(id_pred(TokenKind::kLe, 2)));
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_EQ((*result)[0].id, 1u);
  EXPECT_EQ((*result)[1].id, 2u);
}

TEST(WhereFilter, IdNeExcludesMatch) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, "bob"), make_row(3, "carol")};
  auto result = execute_select_where(rows, make_stmt_where(id_pred(TokenKind::kNe, 2)));
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_EQ((*result)[0].id, 1u);
  EXPECT_EQ((*result)[1].id, 3u);
}

TEST(WhereFilter, NameEqStringMatch) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, "bob")};
  auto result = execute_select_where(rows, make_stmt_where(name_pred(TokenKind::kEq, "alice")));
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_EQ((*result)[0].name, "alice");
}

TEST(WhereFilter, NameEqStringNoMatch) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, "bob")};
  auto result = execute_select_where(rows, make_stmt_where(name_pred(TokenKind::kEq, "dave")));
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(result->empty());
}

TEST(WhereFilter, NameIsNullReturnsNullRows) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, nullptr), make_row(3, nullptr)};
  auto result = execute_select_where(rows, make_stmt_where(name_is_null_pred(false)));
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_TRUE((*result)[0].name_null);
  EXPECT_TRUE((*result)[1].name_null);
}

TEST(WhereFilter, NameIsNotNullReturnsNonNullRows) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, nullptr), make_row(3, "carol")};
  auto result = execute_select_where(rows, make_stmt_where(name_is_null_pred(true)));
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_FALSE((*result)[0].name_null);
  EXPECT_FALSE((*result)[1].name_null);
}

TEST(WhereFilter, NoWhereReturnsAll) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, "bob"), make_row(3, nullptr)};
  auto result = execute_select_where(rows, make_stmt_no_where());
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->size(), 3u);
}
