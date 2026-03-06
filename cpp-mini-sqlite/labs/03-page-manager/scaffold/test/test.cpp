#include <gtest/gtest.h>

#include <array>
#include <cstdio>
#include <vector>

#include "../src/solution.h"

static const char* kTempPath = "/tmp/handhold_lab03_page_manager.bin";

// Create a file pre-filled with `pages` * kPageSize zero bytes.
static void create_temp(int pages) {
  FILE* f = std::fopen(kTempPath, "wb");
  ASSERT_NE(f, nullptr) << "Failed to create temp file";
  std::vector<uint8_t> zeros(static_cast<size_t>(pages) * kPageSize, 0u);
  size_t written = std::fwrite(zeros.data(), 1, zeros.size(), f);
  std::fclose(f);
  ASSERT_EQ(written, zeros.size()) << "Failed to write temp file";
}

struct TempFileCleanup {
  ~TempFileCleanup() { std::remove(kTempPath); }
};

// Write page 0 filled with 0xAB, read it back, verify all bytes are 0xAB.
TEST(PageManagerTest, WriteAndReadBack) {
  TempFileCleanup cleanup;
  create_temp(1);

  PageManager pm(kTempPath);

  Page page{};
  page.id = 0;
  page.data.fill(0xAB);

  auto write_result = pm.write(page);
  ASSERT_TRUE(write_result.has_value()) << "write failed";

  auto read_result = pm.read(0);
  ASSERT_TRUE(read_result.has_value()) << "read failed";
  EXPECT_EQ(read_result->id, 0u);
  EXPECT_EQ(read_result->data[0], 0xABu);
  EXPECT_EQ(read_result->data[kPageSize - 1], 0xABu);
}

// Write page 1 filled with 0xCD, verify raw file bytes at offset kPageSize
// all equal 0xCD (tests the offset formula id * kPageSize).
TEST(PageManagerTest, OffsetFormula) {
  TempFileCleanup cleanup;
  create_temp(2);

  PageManager pm(kTempPath);

  Page page{};
  page.id = 1;
  page.data.fill(0xCD);

  auto write_result = pm.write(page);
  ASSERT_TRUE(write_result.has_value()) << "write failed";

  // Read raw bytes from file at offset kPageSize to verify placement.
  FILE* f = std::fopen(kTempPath, "rb");
  ASSERT_NE(f, nullptr);
  std::fseek(f, static_cast<long>(kPageSize), SEEK_SET);
  std::array<uint8_t, kPageSize> raw{};
  std::fread(raw.data(), 1, kPageSize, f);
  std::fclose(f);

  EXPECT_EQ(raw[0], 0xCDu);
  EXPECT_EQ(raw[kPageSize - 1], 0xCDu);
}

// Write pages 0 and 2 with distinct fill bytes, read both back, verify data.
TEST(PageManagerTest, MultiplePages) {
  TempFileCleanup cleanup;
  create_temp(3);

  PageManager pm(kTempPath);

  Page p0{};
  p0.id = 0;
  p0.data.fill(0x11);

  Page p2{};
  p2.id = 2;
  p2.data.fill(0x22);

  ASSERT_TRUE(pm.write(p0).has_value()) << "write p0 failed";
  ASSERT_TRUE(pm.write(p2).has_value()) << "write p2 failed";

  auto r0 = pm.read(0);
  ASSERT_TRUE(r0.has_value()) << "read p0 failed";
  EXPECT_EQ(r0->data[0], 0x11u);

  auto r2 = pm.read(2);
  ASSERT_TRUE(r2.has_value()) << "read p2 failed";
  EXPECT_EQ(r2->data[0], 0x22u);
}

// File has only 1 page; reading page 1 (which is beyond EOF) must return kIo.
// This test PASSES with the stub (stub always returns kIo for read).
TEST(PageManagerTest, ReadFromSmallFile) {
  TempFileCleanup cleanup;
  create_temp(1);

  PageManager pm(kTempPath);

  auto result = pm.read(1);
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DbError::kIo);
}
