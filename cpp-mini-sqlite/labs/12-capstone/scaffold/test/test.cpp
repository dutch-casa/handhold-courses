#include <cstdio>
#include <string>
#include <gtest/gtest.h>
#include "solution.h"

// Each test gets a fresh file and cleans up after itself.
class DatabaseTest : public testing::Test {
 protected:
  const char* kPath = "test_capstone.db";

  void SetUp()    override { std::remove(kPath); }
  void TearDown() override { std::remove(kPath); }
};

TEST_F(DatabaseTest, OpenCreatesFile) {
  auto db = Database::open(kPath);
  ASSERT_TRUE(db.has_value());
  FILE* f = std::fopen(kPath, "rb");
  ASSERT_NE(f, nullptr);
  std::fclose(f);
}

TEST_F(DatabaseTest, InsertAndSelectAll) {
  auto db = Database::open(kPath);
  ASSERT_TRUE(db.has_value());
  ASSERT_TRUE(db->query("INSERT INTO users VALUES (1, 'alice')").has_value());
  ASSERT_TRUE(db->query("INSERT INTO users VALUES (2, 'bob')").has_value());

  auto result = db->query("SELECT * FROM users");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_EQ((*result)[0].id,   1u);
  EXPECT_EQ((*result)[0].name, "alice");
  EXPECT_EQ((*result)[1].id,   2u);
  EXPECT_EQ((*result)[1].name, "bob");
}

TEST_F(DatabaseTest, InsertAndSelectWhereIdEq) {
  auto db = Database::open(kPath);
  ASSERT_TRUE(db.has_value());
  ASSERT_TRUE(db->query("INSERT INTO users VALUES (1, 'alice')").has_value());
  ASSERT_TRUE(db->query("INSERT INTO users VALUES (2, 'bob')").has_value());

  auto result = db->query("SELECT * FROM users WHERE id = 2");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_EQ((*result)[0].id,   2u);
  EXPECT_EQ((*result)[0].name, "bob");
}

TEST_F(DatabaseTest, InsertNullName) {
  auto db = Database::open(kPath);
  ASSERT_TRUE(db.has_value());
  ASSERT_TRUE(db->query("INSERT INTO users VALUES (7, NULL)").has_value());

  auto result = db->query("SELECT * FROM users WHERE name IS NULL");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_TRUE((*result)[0].name_null);
  EXPECT_EQ((*result)[0].id, 7u);
}

TEST_F(DatabaseTest, SelectWhereIdGt) {
  auto db = Database::open(kPath);
  ASSERT_TRUE(db.has_value());
  ASSERT_TRUE(db->query("INSERT INTO users VALUES (1, 'a')").has_value());
  ASSERT_TRUE(db->query("INSERT INTO users VALUES (5, 'b')").has_value());
  ASSERT_TRUE(db->query("INSERT INTO users VALUES (9, 'c')").has_value());

  auto result = db->query("SELECT * FROM users WHERE id > 3");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_EQ((*result)[0].id, 5u);
  EXPECT_EQ((*result)[1].id, 9u);
}

TEST_F(DatabaseTest, SelectWhereNameIsNotNull) {
  auto db = Database::open(kPath);
  ASSERT_TRUE(db.has_value());
  ASSERT_TRUE(db->query("INSERT INTO users VALUES (1, 'alice')").has_value());
  ASSERT_TRUE(db->query("INSERT INTO users VALUES (2, NULL)").has_value());

  auto result = db->query("SELECT * FROM users WHERE name IS NOT NULL");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 1u);
  EXPECT_EQ((*result)[0].name, "alice");
}

TEST_F(DatabaseTest, InsertMultiple) {
  auto db = Database::open(kPath);
  ASSERT_TRUE(db.has_value());
  for (int i = 1; i <= 10; ++i) {
    std::string sql = "INSERT INTO users VALUES (" + std::to_string(i) + ", 'x')";
    ASSERT_TRUE(db->query(sql).has_value());
  }

  auto result = db->query("SELECT * FROM users");
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->size(), 10u);
}

TEST_F(DatabaseTest, SyntaxError) {
  auto db = Database::open(kPath);
  ASSERT_TRUE(db.has_value());

  auto result = db->query("NOT VALID SQL !!!");
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DbError::kSyntax);
}

// The payoff: close the database, reopen from the same file, data persists.
TEST_F(DatabaseTest, PersistenceAcrossReopen) {
  {
    auto db = Database::open(kPath);
    ASSERT_TRUE(db.has_value());
    ASSERT_TRUE(db->query("INSERT INTO users VALUES (1, 'alice')").has_value());
    ASSERT_TRUE(db->query("INSERT INTO users VALUES (2, 'bob')").has_value());
  }  // ~Database → ~RowFile → fclose: data must be on disk now

  auto db2 = Database::open(kPath);
  ASSERT_TRUE(db2.has_value());

  auto result = db2->query("SELECT * FROM users");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_EQ((*result)[0].id,   1u);
  EXPECT_EQ((*result)[0].name, "alice");
  EXPECT_EQ((*result)[1].id,   2u);
  EXPECT_EQ((*result)[1].name, "bob");
}
