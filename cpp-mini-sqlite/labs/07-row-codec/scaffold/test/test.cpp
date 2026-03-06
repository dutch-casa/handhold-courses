#include <gtest/gtest.h>
#include <array>
#include <cassert>
#include <cstring>
#include "solution.h"

// Helper: build a Row with all fields explicitly set.
static Row make_row(uint32_t id, uint8_t null_mask, const char* name) {
  assert(std::strlen(name) <= kNameMax);
  Row r{};
  r.id        = id;
  r.null_mask = null_mask;
  std::memcpy(r.name, name, std::min(std::strlen(name), static_cast<size_t>(kNameMax)));
  return r;
}

TEST(RowCodecTest, RoundTrip) {
  Row original = make_row(1, 0, "alice");
  std::array<uint8_t, kRowSize> buf{};
  encode_row(original, buf);
  auto result = decode_row(buf);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->id,        original.id);
  EXPECT_EQ(result->null_mask, original.null_mask);
  EXPECT_EQ(std::memcmp(result->name, original.name, kNameMax), 0);
}

TEST(RowCodecTest, IntegerField) {
  Row r = make_row(42, 0, "");
  std::array<uint8_t, kRowSize> buf{};
  encode_row(r, buf);
  auto result = decode_row(buf);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->id, 42u);
}

TEST(RowCodecTest, TextField) {
  Row r = make_row(7, 0, "hello");
  std::array<uint8_t, kRowSize> buf{};
  encode_row(r, buf);
  auto result = decode_row(buf);
  ASSERT_TRUE(result.has_value());
  // Compare only the first 5 chars; rest should be zero from make_row init.
  EXPECT_EQ(std::memcmp(result->name, "hello", 5), 0);
}

TEST(RowCodecTest, NullableField) {
  Row r = make_row(99, 1, "");
  std::array<uint8_t, kRowSize> buf{};
  encode_row(r, buf);
  auto result = decode_row(buf);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->null_mask & 1, 1);
}

TEST(RowCodecTest, BufferTooSmall) {
  std::array<uint8_t, kRowSize - 1> small{};
  auto result = decode_row(small);
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DbError::kCorrupted);
}

TEST(RowCodecTest, MultipleRowsInPage) {
  constexpr uint32_t kCount = 3;
  std::array<uint8_t, kCount * kRowSize> page{};

  Row rows[kCount] = {
    make_row(10, 0, "alpha"),
    make_row(20, 0, "beta"),
    make_row(30, 1, ""),
  };

  for (uint32_t i = 0; i < kCount; ++i) {
    std::span<uint8_t> slot(page.data() + i * kRowSize, kRowSize);
    encode_row(rows[i], slot);
  }

  for (uint32_t i = 0; i < kCount; ++i) {
    std::span<const uint8_t> slot(page.data() + i * kRowSize, kRowSize);
    auto result = decode_row(slot);
    ASSERT_TRUE(result.has_value()) << "row " << i << " failed to decode";
    EXPECT_EQ(result->id,        rows[i].id)        << "row " << i << " id mismatch";
    EXPECT_EQ(result->null_mask, rows[i].null_mask) << "row " << i << " null_mask mismatch";
    EXPECT_EQ(std::memcmp(result->name, rows[i].name, kNameMax), 0) << "row " << i << " name mismatch";
  }
}
