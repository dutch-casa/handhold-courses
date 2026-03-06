#include <gtest/gtest.h>

#include <cstdio>
#include <cstring>
#include <vector>

#include "../src/solution.h"

static const char* kTempPath = "/tmp/handhold_test_file_reader.bin";
static const char* kMissingPath = "/tmp/handhold_test_definitely_does_not_exist_xyz987.bin";

static void write_temp(const std::vector<uint8_t>& data) {
  FILE* f = std::fopen(kTempPath, "wb");
  if (f) {
    std::fwrite(data.data(), 1, data.size(), f);
    std::fclose(f);
  }
}

struct TempFileCleanup {
  ~TempFileCleanup() { std::remove(kTempPath); }
};

TEST(FileReaderTest, ReadsExistingFile) {
  TempFileCleanup cleanup;
  const std::vector<uint8_t> expected = {0x01, 0x02, 0x03, 0xDE, 0xAD, 0xBE, 0xEF};
  write_temp(expected);

  DbResult<std::vector<uint8_t>> result = read_file(kTempPath);

  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), expected);
}

TEST(FileReaderTest, ReturnsErrorForMissing) {
  // Ensure the file doesn't exist.
  std::remove(kMissingPath);

  DbResult<std::vector<uint8_t>> result = read_file(kMissingPath);

  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DbError::kIo);
}

TEST(FileReaderTest, EmptyFile) {
  TempFileCleanup cleanup;
  write_temp({});

  DbResult<std::vector<uint8_t>> result = read_file(kTempPath);

  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(result.value().empty());
}

TEST(FileReaderTest, LargeFile) {
  TempFileCleanup cleanup;
  constexpr size_t kSize = 64 * 1024;  // 64 KB
  std::vector<uint8_t> data(kSize);
  for (size_t i = 0; i < kSize; ++i) {
    data[i] = static_cast<uint8_t>(i & 0xFF);
  }
  write_temp(data);

  DbResult<std::vector<uint8_t>> result = read_file(kTempPath);

  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value().size(), kSize);
  EXPECT_EQ(result.value(), data);
}
