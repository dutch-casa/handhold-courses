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

static SelectStmt make_stmt() { return SelectStmt{}; }

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

TEST(ExecuteSelect, EmptyRowSetReturnsEmpty) {
  std::vector<Row> rows;
  auto result = execute_select(rows, make_stmt());
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(result->empty());
}

TEST(ExecuteSelect, SingleRowNamePresent) {
  std::vector<Row> rows = {make_row(1, "alice")};
  auto result = execute_select(rows, make_stmt());
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_EQ((*result)[0].name, "alice");
  EXPECT_FALSE((*result)[0].name_null);
}

TEST(ExecuteSelect, SingleRowNameNull) {
  std::vector<Row> rows = {make_row(7, nullptr)};
  auto result = execute_select(rows, make_stmt());
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_TRUE((*result)[0].name_null);
}

TEST(ExecuteSelect, MultipleRowsAllReturned) {
  std::vector<Row> rows = {make_row(1, "alice"), make_row(2, "bob"), make_row(3, "carol")};
  auto result = execute_select(rows, make_stmt());
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 3u);
  EXPECT_EQ((*result)[0].id, 1u);
  EXPECT_EQ((*result)[1].id, 2u);
  EXPECT_EQ((*result)[2].id, 3u);
}

TEST(ExecuteSelect, ResultRowIdMatchesInput) {
  std::vector<Row> rows = {make_row(42, "dave")};
  auto result = execute_select(rows, make_stmt());
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_EQ((*result)[0].id, 42u);
}

TEST(ExecuteSelect, ResultRowNameMatchesInput) {
  // Fill name buffer with 'A', then write "eve\0" at the start.
  // The remaining bytes are non-null — strnlen must stop at the explicit '\0'.
  Row r{};
  r.id = 5;
  r.null_mask = 0;
  std::memset(r.name, 0x41, kNameMax);
  r.name[0] = 'e'; r.name[1] = 'v'; r.name[2] = 'e'; r.name[3] = '\0';
  std::vector<Row> rows = {r};
  auto result = execute_select(rows, make_stmt());
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_EQ((*result)[0].name, "eve");
}

TEST(ExecuteSelect, NullFlagSetFromNullMaskBit0) {
  // null_mask = 0x03: bit 0 and bit 1 both set.
  // Only bit 0 controls name_null — the others must be ignored.
  Row r{};
  r.id = 3;
  r.null_mask = 0x03;
  std::vector<Row> rows = {r};
  auto result = execute_select(rows, make_stmt());
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_TRUE((*result)[0].name_null);
}

TEST(ExecuteSelect, NullFlagNotSetWhenNullMaskZero) {
  Row r = make_row(4, "frank");  // null_mask = 0
  std::vector<Row> rows = {r};
  auto result = execute_select(rows, make_stmt());
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_FALSE((*result)[0].name_null);
}
