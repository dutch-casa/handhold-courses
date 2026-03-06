#pragma once
#include <array>
#include <cstdint>
#include <expected>
#include <span>

// Error codes for all database operations. Not every code applies to each lab.
enum class DbError { kNotFound, kCorrupted, kOutOfMemory, kIo, kSyntax, kFull };

template <typename T>
using DbResult = std::expected<T, DbError>;

constexpr uint32_t kNameMax = 32;  // max name bytes in encoded row
constexpr uint32_t kRowSize = 40;  // encoded row size in bytes

// Codec-owned byte offsets within an encoded row.
// The struct member offsets differ — the codec inserts 3 padding bytes at
// kOffNullMask+1..kOffName-1 so that name lands at a word-aligned offset.
constexpr uint32_t kOffId       = 0;   // 4 bytes
constexpr uint32_t kOffNullMask = 4;   // 1 byte
constexpr uint32_t kOffName     = 8;   // 32 bytes (offsets 5..7 are padding, always zero)

// Row layout (encoded, 40 bytes total):
// offset  0..3:  id        (uint32_t, host byte order)
// offset  4:     null_mask (uint8_t; bit 0 = name is SQL NULL; bits 1-7 reserved)
// offset  5..7:  padding   (always zero)
// offset  8..39: name      (char[32], not null-terminated)
struct Row {
  uint32_t id;
  uint8_t  null_mask;  // bit 0: name is null; bits 1-7 reserved for future nullable fields
  char     name[kNameMax];
};

// Encodes row into out[0..kRowSize-1].
// Precondition: out.size() >= kRowSize (asserted in debug builds).
void encode_row(const Row& row, std::span<uint8_t> out);

// Decodes a row from in[0..kRowSize-1].
// Returns kCorrupted if in.size() < kRowSize.
DbResult<Row> decode_row(std::span<const uint8_t> in);
