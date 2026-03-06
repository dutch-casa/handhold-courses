#include <gtest/gtest.h>

#include <cstdio>
#include <cstring>

#include "../src/solution.h"

static const char* kTempPath = "/tmp/handhold_lab02_page_header.bin";

static void write_temp(const uint8_t* data, size_t len) {
  FILE* f = std::fopen(kTempPath, "wb");
  ASSERT_NE(f, nullptr) << "Failed to open temp file for writing";
  std::fwrite(data, 1, len, f);
  std::fclose(f);
}

struct TempFileCleanup {
  ~TempFileCleanup() { std::remove(kTempPath); }
};

TEST(PageHeaderTest, ReadsCorrectType) {
  TempFileCleanup cleanup;
  const uint8_t bytes[8] = {0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  write_temp(bytes, sizeof(bytes));

  auto result = read_page_header(kTempPath);

  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->page_type, 0x0D);
}

TEST(PageHeaderTest, ReadsCorrectCellCount) {
  TempFileCleanup cleanup;
  // cell_count = 42 = 0x0000002A, big-endian: bytes [4-7] = {0x00,0x00,0x00,0x2A}
  const uint8_t bytes[8] = {0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A};
  write_temp(bytes, sizeof(bytes));

  auto result = read_page_header(kTempPath);

  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->cell_count, 42u);
}

TEST(PageHeaderTest, ReturnsErrorForShortFile) {
  TempFileCleanup cleanup;
  const uint8_t bytes[4] = {0x0D, 0x00, 0x00, 0x00};
  write_temp(bytes, sizeof(bytes));

  auto result = read_page_header(kTempPath);

  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DbError::kCorrupted);
}

TEST(PageHeaderTest, ReturnsErrorForMissing) {
  auto result = read_page_header("/tmp/no-such-file-cpp-lab-02.bin");
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DbError::kIo);
}
