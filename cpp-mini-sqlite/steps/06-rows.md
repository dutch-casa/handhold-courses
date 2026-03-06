---
title: Storing Rows
---

# Fixed-width rows

{{clear: slide}}

{{show: row-offsets typewriter 2s linear}} A database must find any row by its index — without scanning from the start. The answer is not a linked list and not an index tree. It is arithmetic.

{{focus: row0}} {{annotate: row0 "Row 0 at byte 0"}} If every row is exactly 40 bytes, row zero starts at byte zero. No searching required — the position is a multiplication.

{{focus: row1}} {{annotate: row1 "Row 1 at byte 40"}} Row one starts at byte 40. Row two at byte 80. Row N at byte `N * 40`. The database jumps directly to any row with a single offset calculation.

{{focus: row2}} {{annotate: row2 "O(1) random access"}} This is random access in O(1). The only requirement is that the row size never changes. Variably-sized rows break the arithmetic — fixed-width rows make it exact.

{{focus: none}} {{clear: slide}}

```data:row-offsets type=array
["offset    0 │ row 0 │ 40 bytes", "offset   40 │ row 1 │ 40 bytes", "offset   80 │ row 2 │ 40 bytes", "offset 4040 │ row 101 │ 40 bytes  (last on the page)"]
---
row0: 0
row1: 1
row2: 2
```

# The Row struct

{{clear: slide}}

{{show: row-struct typewriter 2s linear}} {{zoom: 1.2x}} The `Row` struct holds the same fields as the on-disk layout. The codec — not the struct — controls the exact byte offsets; the struct just provides typed access to each field.

{{focus: id-field}} {{annotate: id-field "4-byte primary key"}} `id` is a `uint32_t` at offset zero. Four bytes, little-endian. Every row in the database has a unique id; the B-tree is keyed on it.

{{focus: null-field}} {{annotate: null-field "NULL is one bit"}} `null_mask` is a single byte at offset 4. Bit 0 controls whether `name` holds SQL NULL. A zero bit means `name` is present; a one bit means the value is absent regardless of what bytes occupy the name region.

{{focus: padding-comment}} {{annotate: padding-comment "Struct offset 5, wire offset 8"}} In the struct, `name` sits at memory offset 5 — right after the one-byte `null_mask`. The codec writes it at wire offset 8 instead, inserting three explicit zero bytes so that `name` lands on a 4-byte-aligned boundary. The `memset` call zeros those three bytes automatically.

{{focus: name-field}} {{annotate: name-field "Fixed 32-byte char buffer"}} `name` is a `char[32]` at offset 8. It is never null-terminated. The length is always `kNameMax` bytes — shorter names are padded with zeros by the encoder. The null_mask bit is the only way to distinguish an empty name from SQL NULL.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:row-struct lang=cpp
constexpr uint32_t kNameMax = 32;  // max name bytes in encoded row
constexpr uint32_t kRowSize = 40;  // encoded row size in bytes

// Row layout (encoded):
// offset  0..3:  id        (uint32_t, host byte order)
// offset  4:     null_mask (uint8_t; bit 0 = name is SQL NULL; bits 1-7 reserved)
// offset  5..7:  padding   (always zero)
// offset  8..39: name      (char[32], not null-terminated)
struct Row {
  uint32_t id;
  uint8_t  null_mask;  // bit 0: name is null; bits 1-7 reserved
  char     name[kNameMax];
};
---
id-field: 10
null-field: 11
padding-comment: 7
name-field: 12
```

# Encoding a row

{{clear: slide}}

{{show: encode-fn typewriter 2s linear}} {{zoom: 1.2x}} Encoding a row means writing its fields into a flat byte buffer at exact offsets. The caller owns the buffer; `encode_row` only writes to it.

{{focus: assert-line}} {{annotate: assert-line "Debug precondition"}} The assert fires in debug builds if the caller passes a buffer smaller than 40 bytes. It documents the contract — the function does not resize or allocate; the caller must guarantee the span is large enough.

{{focus: memset-line}} {{annotate: memset-line "Zero first, write after"}} `std::memset` zeros all 40 bytes before writing any field. This guarantees the padding bytes at offsets 5–7 are always zero, even if the Row struct has garbage in those positions in memory.

{{focus: id-copy}} {{annotate: id-copy "kOffId = 0"}} `std::memcpy` copies the 4 bytes of `row.id` into the output at `kOffId`. Using `memcpy` instead of a cast avoids strict aliasing undefined behavior on any platform.

{{focus: null-copy}} {{annotate: null-copy "kOffNullMask = 4"}} `std::memcpy` writes the null_mask byte at `kOffNullMask`. The one-byte copy is deliberate — `sizeof(row.null_mask)` is always 1, and the explicit sizeof makes that invariant visible.

{{focus: name-copy}} {{annotate: name-copy "kOffName = 8"}} `std::memcpy` writes all `kNameMax` bytes of `row.name` at `kOffName`. Short names must already be zero-padded in the caller's Row before encoding — the encoder makes no assumptions about unused bytes.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:encode-fn lang=cpp
// Encodes row into out[0..kRowSize-1].
// Precondition: out.size() >= kRowSize (asserted in debug builds).
void encode_row(const Row& row, std::span<uint8_t> out) {
  assert(out.size() >= kRowSize);
  std::memset(out.data(), 0, kRowSize);
  std::memcpy(out.data() + kOffId,       &row.id,        sizeof(row.id));
  std::memcpy(out.data() + kOffNullMask, &row.null_mask, sizeof(row.null_mask));
  std::memcpy(out.data() + kOffName,      row.name,       kNameMax);
}
---
assert-line: 4
memset-line: 5
id-copy: 6
null-copy: 7
name-copy: 8
```

# Decoding a row

{{clear: slide}}

{{show: decode-fn typewriter 2s linear}} {{zoom: 1.2x}} Decoding is the mirror of encoding. Read bytes from fixed offsets into a `Row` struct. The function returns `DbResult<Row>` — failure is a real possibility when reading from disk.

{{focus: size-guard}} {{annotate: size-guard "Guard: reject short spans"}} The first thing `decode_row` does is check the span length. If the caller passes a truncated buffer — a partial read, a corrupted page — the function returns `kCorrupted` immediately. A short span is malformed data, not a hardware failure, so `kCorrupted` is the right code.

{{focus: row-init}} {{annotate: row-init "Value-initialize to zero"}} `Row row{}` value-initializes all fields to zero. If any `memcpy` is accidentally skipped, the field holds zero rather than stack garbage. This is a cheap, correct default.

{{focus: id-read}} {{annotate: id-read "Read id bytes"}} `std::memcpy` reads the id bytes from the input span. The same aliasing reasoning applies as in encoding — `memcpy` is the correct primitive for reinterpreting raw bytes as a typed value.

{{focus: null-read}} {{annotate: null-read "kOffNullMask = 4"}} The null_mask byte at `kOffNullMask` is read back into `row.null_mask`. The caller can then test `row.null_mask & 1` to determine whether the name field holds SQL NULL.

{{focus: name-read}} {{annotate: name-read "kOffName = 8"}} `kNameMax` bytes are copied from `kOffName` into `row.name`. The buffer is not null-terminated; the decoder does not add a terminator. Consumers must treat it as a fixed-length char array, not a C string.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:decode-fn lang=cpp
// Decodes one row from in[0..kRowSize-1].
// Returns kCorrupted if in.size() < kRowSize.
DbResult<Row> decode_row(std::span<const uint8_t> in) {
  if (in.size() < kRowSize) return std::unexpected(DbError::kCorrupted);
  Row row{};
  std::memcpy(&row.id,        in.data() + kOffId,       sizeof(row.id));
  std::memcpy(&row.null_mask, in.data() + kOffNullMask, sizeof(row.null_mask));
  std::memcpy(row.name,       in.data() + kOffName,     kNameMax);
  return row;
}
---
size-guard: 4
row-init: 5
id-read: 6
null-read: 7
name-read: 8
```

# Rows per page

{{clear: slide}}

{{show: capacity-compare typewriter 2s linear}} The page size is 4096 bytes. The row size is 40 bytes. Integer division gives the exact capacity: 102 rows fit on one leaf page.

{{focus: page-size-row}} {{annotate: page-size-row "One disk transfer unit"}} `kPageSize = 4096` is the fundamental I/O unit. Every read and write to disk transfers exactly 4096 bytes — one page. The B-tree never reads a partial page or allocates more than one page at a time for a node.

{{focus: row-size-row}} {{annotate: row-size-row "40 bytes: 4+1+3+32"}} `kRowSize = 40` breaks down as: 4 bytes for id, 1 byte for null_mask, 3 bytes of explicit padding, 32 bytes for name. No hidden compiler padding — the layout is fully specified.

{{focus: capacity-row}} {{annotate: capacity-row "102 rows, no heap"}} `4096 / 40 = 102` rows per page, with 16 bytes left over. The slot-array header introduced in the next section occupies exactly those 16 bytes. This capacity is achieved with zero heap allocation — the B-tree packs rows directly into the pre-allocated 4096-byte page buffer.

{{focus: none}} {{clear: slide}}

```data:capacity-compare type=array
["kPageSize = 4096  │ bytes per page  │ one disk transfer unit", "kRowSize  =   40  │ bytes per row   │ 4 (id) + 1 (null_mask) + 3 (pad) + 32 (name)", "capacity  =  102  │ rows per page   │ 4096 / 40 = 102 rows, 16 bytes remaining"]
---
page-size-row: 0
row-size-row: 1
capacity-row: 2
```

# The slot array (preview)

{{clear: slide}}

{{show: slot-layout typewriter 2s linear}} A real leaf page is not just row bytes. It has a small header — a slot array — that records which of the 102 slots are currently occupied.

{{focus: slot-bits-row}} {{annotate: slot-bits-row "16 bytes of presence bits"}} The slot array is a bitfield at the front of the page. With 102 slots, you need 102 bits — 13 bytes rounded up to 16 for alignment. Bit `i` set means slot `i` holds a live row. Bit `i` clear means the slot is free, regardless of what bytes sit there.

{{focus: row-slots-row}} {{annotate: row-slots-row "Slots packed after header"}} Row slots occupy the remainder of the page after the header. Each slot is exactly `kRowSize` bytes at offset `header_size + i * kRowSize`. The slot array and row array are parallel structures — the bitfield indexes into the row array.

{{focus: delete-row}} {{annotate: delete-row "Delete clears one bit"}} Deleting a row clears its bit in the slot array and zeroes its slot. The page is not compacted. Future inserts scan the slot array for the first zero bit and write into that slot. Compaction happens separately during vacuum.

{{focus: none}} {{clear: slide}}

```data:slot-layout type=array
["header │ slot bits  │ 16 bytes at offset 0  │ bit i = slot i is live", "slots  │ row slots  │ 16..4095              │ 102 × 40 bytes, packed immediately after header", "delete │ clear bit  │ set slot_bits[i] = 0  │ no compaction; vacuumed separately"]
---
slot-bits-row: 0
row-slots-row: 1
delete-row: 2
```
