# Lab 07: Encode and Decode a Row

## Context

In Lesson 6 you saw how every row is exactly 40 bytes at a fixed offset in a
leaf page. Your job is to implement `encode_row` and `decode_row` in
`src/solution.cpp`.

Both functions are declared in `src/solution.h`. Do not modify that file.

## What to implement

Open `src/solution.cpp` and implement:

- `void encode_row(const Row& row, std::span<uint8_t> out)`
- `DbResult<Row> decode_row(std::span<const uint8_t> in)`

## Spec

### encode_row

Precondition: `out.size() >= kRowSize` (asserted in debug builds — keep it).

1. Call `std::memset(out.data(), 0, kRowSize)` to zero all bytes.
2. `std::memcpy(out.data() + kOffId, &row.id, sizeof(row.id))` — write 4-byte id.
3. `std::memcpy(out.data() + kOffNullMask, &row.null_mask, sizeof(row.null_mask))` — write null_mask.
4. `std::memcpy(out.data() + kOffName, row.name, kNameMax)` — write 32 name bytes.

### decode_row

1. If `in.size() < kRowSize`, return `std::unexpected(DbError::kCorrupted)`.
2. Declare `Row row{}`.
3. `std::memcpy(&row.id, in.data() + kOffId, sizeof(row.id))`.
4. `std::memcpy(&row.null_mask, in.data() + kOffNullMask, sizeof(row.null_mask))`.
5. `std::memcpy(row.name, in.data() + kOffName, kNameMax)`.
6. Return `row`.

## Running the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All six tests must pass. The stub `encode_row` is a no-op and `decode_row`
always returns `kCorrupted`, so five tests fail until you implement both
functions (`BufferTooSmall` passes immediately since it also expects
`kCorrupted`).
