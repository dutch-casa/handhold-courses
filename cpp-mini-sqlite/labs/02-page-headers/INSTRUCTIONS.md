# Lab 02: Page Headers

## Context

In Lab 01 you built `read_file` — a function that reads all bytes from a file and
returns them in a `std::vector<uint8_t>`. That helper is already provided in
`src/helpers.cpp`. You will call it, not rewrite it.

Every page in a database file starts with an 8-byte header that describes that page.
Your job in this lab is to read those 8 bytes, validate them, and decode the
big-endian `cell_count` field into native endian for the rest of the engine to use.

## What to implement

Open `src/solution.cpp` and implement:

```cpp
DbResult<PageHeader> read_page_header(std::string_view path);
```

The `PageHeader` struct and all types are declared in `src/solution.h`.

## Spec

1. Call `read_file(std::string(path).c_str())` to get the file bytes.
2. If `read_file` returns an error, propagate it unchanged with
   `std::unexpected(bytes.error())`.
3. If the file is shorter than 8 bytes, return
   `std::unexpected(DbError::kCorrupted)`.
4. Decode `cell_count` from bytes [4]–[7] as big-endian. `bytes` is a
   `DbResult<std::vector<uint8_t>>` — dereference it with `*bytes` to get
   the vector, then index into it:

   ```cpp
   const auto& data = *bytes;
   uint32_t cell_count =
     (static_cast<uint32_t>(data[4]) << 24) |
     (static_cast<uint32_t>(data[5]) << 16) |
     (static_cast<uint32_t>(data[6]) <<  8) |
      static_cast<uint32_t>(data[7]);
   ```

5. Return `PageHeader{data[0], {}, cell_count}`.

## Big-endian decoding

On disk, multi-byte integers are stored most-significant-byte first (big-endian).
Your CPU likely uses little-endian. The shift pattern above reconstructs the
4-byte integer correctly regardless of host byte order:

- byte[4] carries the top 8 bits → shift left 24
- byte[5] carries the next 8 bits → shift left 16
- byte[6] carries the next 8 bits → shift left 8
- byte[7] carries the bottom 8 bits → no shift

## Files you touch

- `src/solution.cpp` — implement `read_page_header` here only.

Do not modify `src/solution.h` or `src/helpers.cpp`.

## Running the tests

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
bash tap_runner.sh build
```

All four tests must pass. The stub returns `kIo` for everything, so three tests
will fail until you implement the function.
