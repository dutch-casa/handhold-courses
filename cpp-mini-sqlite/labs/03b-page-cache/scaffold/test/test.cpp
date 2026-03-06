#include <gtest/gtest.h>

#include <array>
#include <cstdio>
#include <vector>

#include "../src/solution.h"

static const char* kTempPath = "/tmp/handhold_lab03b_page_cache.bin";

// Create a binary file with `n` pages. Page i is filled with 0xAA + i * 0x11.
static void create_pages(int n) {
  FILE* f = std::fopen(kTempPath, "wb");
  ASSERT_NE(f, nullptr) << "Failed to create temp file";
  for (int i = 0; i < n; ++i) {
    std::array<uint8_t, kPageSize> page;
    page.fill(static_cast<uint8_t>(0xAA + i * 0x11));
    size_t written = std::fwrite(page.data(), 1, kPageSize, f);
    ASSERT_EQ(written, kPageSize) << "Failed to write temp page " << i;
  }
  std::fclose(f);
}

struct TempFileCleanup {
  ~TempFileCleanup() { std::remove(kTempPath); }
};

// Cache miss on empty cache: loading page 0 must return valid data.
// Page 0 is filled with 0xAA by create_pages.
TEST(PageCacheTest, Get_Miss_LoadsPage) {
  TempFileCleanup cleanup;
  create_pages(1);

  PageManager pm(kTempPath);
  PageCache cache(pm, 4);

  auto result = cache.get(0);
  ASSERT_TRUE(result.has_value()) << "get(0) failed";
  EXPECT_EQ((*result)->id, 0u);
  EXPECT_EQ((*result)->data[0], 0xAAu);
}

// Cache hit: two gets for the same page must return the same pointer.
// The page must not be re-read from disk (same allocation, same address).
TEST(PageCacheTest, Get_Hit_ReturnsSamePointer) {
  TempFileCleanup cleanup;
  create_pages(1);

  PageManager pm(kTempPath);
  PageCache cache(pm, 4);

  auto r1 = cache.get(0);
  ASSERT_TRUE(r1.has_value()) << "first get(0) failed";

  auto r2 = cache.get(0);
  ASSERT_TRUE(r2.has_value()) << "second get(0) failed";

  EXPECT_EQ(*r1, *r2) << "cache hit must return the same pointer";
}

// LRU eviction: capacity 2, load p0 then p1 (fills cache), promote p0,
// load p2 (must evict p1, not p0). All three gets must succeed and return
// correct fill bytes.
TEST(PageCacheTest, Get_LRU_EvictsLeastRecent) {
  TempFileCleanup cleanup;
  create_pages(3);  // p0=0xAA, p1=0xBB, p2=0xCC

  PageManager pm(kTempPath);
  PageCache cache(pm, 2);

  // Fill cache: p0 then p1.
  auto r0 = cache.get(0);
  ASSERT_TRUE(r0.has_value()) << "get(0) failed";
  EXPECT_EQ((*r0)->data[0], 0xAAu);

  auto r1 = cache.get(1);
  ASSERT_TRUE(r1.has_value()) << "get(1) failed";
  EXPECT_EQ((*r1)->data[0], 0xBBu);

  // Promote p0 to MRU — p1 becomes LRU.
  auto r0b = cache.get(0);
  ASSERT_TRUE(r0b.has_value()) << "second get(0) failed";

  // Load p2: must evict p1 (LRU), not p0 (MRU).
  auto r2 = cache.get(2);
  ASSERT_TRUE(r2.has_value()) << "get(2) failed";
  EXPECT_EQ((*r2)->data[0], 0xCCu);
}

// PageManager opened on a nonexistent path has a null file handle.
// get() must propagate the kIo error from PageManager::read.
// This test PASSES with the stub (stub always returns kIo).
TEST(PageCacheTest, Get_IoError_BadManager) {
  PageManager pm("/tmp/no-such-file-lab03b.bin");
  PageCache cache(pm, 4);

  auto result = cache.get(0);
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), DbError::kIo);
}
