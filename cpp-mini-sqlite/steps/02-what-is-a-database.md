---
title: What Is a Database File?
---

# Open a real .db file in hex

{{show: magic-bytes typewriter 2s linear}} Every SQLite file starts with the same 16 bytes. Open any `.db` file in a hex editor and the first thing you see is `53 51 4C 69 74 65 20 66 6F 72 6D 61 74 20 33 00`.

{{focus: magic}} {{annotate: magic "ASCII: SQLite format 3"}} Those bytes are ASCII for `SQLite format 3`, followed by a null terminator. The file announces what it is before anything else.

{{focus: page-size-bytes}} {{annotate: page-size-bytes "Page size field"}} Bytes 16 and 17 store the page size as a big-endian uint16. Default is `0x10 0x00` — that's 4096 in decimal. Every page in the file is exactly that many bytes.

{{focus: none}} {{clear: slide}}

{{show: header-read typewriter 2s linear}} Here's what reading that header looks like in C++. The function returns `DbResult` — the same contract from Lesson 01.

{{focus: open-call}} {{zoom: 1.2x}} {{annotate: open-call "IO at the edge"}} `read_file` does the syscall. If the disk fails, it returns `DbError::kIo`. The rest of this function never runs.

{{zoom: 1x}} {{focus: size-check}} {{zoom: 1.2x}} {{annotate: size-check "Validate before use"}} If the file is shorter than 18 bytes, it can't be a valid SQLite file. `kCorrupted` — not `kIo`. The syscall succeeded; the data is wrong.

{{zoom: 1x}} {{focus: parse-magic}} {{zoom: 1.2x}} {{annotate: parse-magic "Compare known prefix"}} `std::memcmp` against the known magic string. A mismatch means the file isn't SQLite — or it's been overwritten. Return `kCorrupted`.

{{zoom: 1x}} {{focus: parse-pagesize}} {{zoom: 1.2x}} {{annotate: parse-pagesize "Big-endian decode"}} Page size is big-endian: shift the high byte left 8 bits, OR in the low byte. No library needed.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```data:magic-bytes type=array
[0x53, 0x51, 0x4C, 0x69, 0x74, 0x65, 0x20, 0x66, 0x6F, 0x72, 0x6D, 0x61, 0x74, 0x20, 0x33, 0x00, 0x10, 0x00]
---
magic: 0-15
page-size-bytes: 16-17
```

```code:header-read lang=cpp
#include <cstring>
#include <cstdint>
#include <vector>
#include "db_result.h"

// Reads all bytes from a file — defined in the file-reader lab.
DbResult<std::vector<uint8_t>> read_file(const char* path);

static constexpr uint8_t kSqliteMagic[16] = {
  0x53,0x51,0x4C,0x69,0x74,0x65,0x20,0x66,
  0x6F,0x72,0x6D,0x61,0x74,0x20,0x33,0x00
};

struct FileHeader {
  uint16_t page_size;
};

DbResult<FileHeader> read_file_header(const char* path) {
  auto bytes = read_file(path);
  if (!bytes) {
    return std::unexpected(bytes.error());
  }
  if (bytes->size() < 18) {
    return std::unexpected(DbError::kCorrupted);
  }
  if (std::memcmp(bytes->data(), kSqliteMagic, 16) != 0) {
    return std::unexpected(DbError::kCorrupted);
  }
  uint16_t page_size = static_cast<uint16_t>(
    (static_cast<uint32_t>((*bytes)[16]) << 8) |
     static_cast<uint32_t>((*bytes)[17]));
  return FileHeader{page_size};
}
---
open-call: 18-22
size-check: 23-25
parse-magic: 26-28
parse-pagesize: 29-33
```

# Pages — fixed-size chunks

{{clear: slide}}

{{show: page-layout typewriter 2s linear}} A database file is not one big blob. It's divided into pages — fixed-size chunks of exactly `page_size` bytes each.

{{focus: page-0}} {{annotate: page-0 "First page, offset 0"}} Page 0 starts at byte 0. It holds the file header and the root of the first B-tree. It's the entry point to everything.

{{focus: page-1}} {{annotate: page-1 "Second page, offset 4096"}} Page 1 starts at byte 4096. It holds whatever the storage engine put there — another tree node, an overflow page, a free page.

{{focus: page-2}} {{annotate: page-2 "Third page, offset 8192"}} Page 2 at byte 8192, and so on. The pattern is mechanical. Every page is the same size. No exceptions.

{{focus: none}} The file doesn't "know" about this division. There's no directory. No table of contents at the OS level. The page structure is something we impose on the byte stream. The bytes are just bytes.

{{focus: none}} {{clear: slide}}

{{show: page-count-code typewriter 2s linear}} Given the file size and the page size, the page count is trivial to compute.

{{focus: count-fn}} {{annotate: count-fn "No rounding needed"}} Integer division. If the file is a valid database, the size is always a multiple of `page_size`. An uneven size means `kCorrupted`.

{{focus: mod-check}} {{annotate: mod-check "Invariant check"}} The `%` guard enforces that invariant. A partial page at the end of the file is a structural violation.

{{focus: none}} That's it. No pointers. No linked lists. Fixed-size pages mean the entire page directory is implicit in the arithmetic.

```data:page-layout type=array
["Page 0  │ bytes    0 – 4095  │ file header + root B-tree node", "Page 1  │ bytes 4096 – 8191  │ tree node / overflow / free", "Page 2  │ bytes 8192 – 12287 │ tree node / overflow / free", "Page 3  │ bytes 12288 – ...  │ ..."]
---
page-0: 0
page-1: 1
page-2: 2
```

```code:page-count-code lang=cpp
#include <cstdint>
#include <cstddef>
#include "db_result.h"

using PageId = uint32_t;
constexpr uint32_t kPageSize = 4096;

// Returns the number of pages in a file of `file_size` bytes.
// Precondition: file_size / page_size <= UINT32_MAX.
DbResult<uint32_t> page_count(size_t file_size, uint32_t page_size) {
  if (page_size == 0 || file_size % page_size != 0) {
    return std::unexpected(DbError::kCorrupted);
  }
  return static_cast<uint32_t>(file_size / page_size);
}
---
count-fn: 10-14
mod-check: 11-13
```

# The page ID — a byte-offset formula

{{clear: slide}}

{{show: offset-code typewriter 2s linear}} Every page has an ID — a plain `uint32_t`. To read page N, multiply by the page size. That's the byte offset into the file.

{{focus: offset-fn}} {{zoom: 1.4x}} {{annotate: offset-fn "offset = id × page_size"}} One multiplication. No seek table. No indirection. The mapping from page ID to file offset is pure arithmetic.

{{zoom: 1x}} {{focus: none}} This is why fixed-size pages matter. If pages were variable-length, you'd need an index that maps IDs to offsets. With fixed pages, the index is the formula itself.

{{focus: none}} {{clear: slide}}

{{show: offset-code slide 0.3s}} {{split}} {{show: page-layout none 0s}} Here's `page_offset` — the function students will use in Lab 02.

{{focus: pageid-alias}} {{annotate: pageid-alias "Strong alias for PageId"}} `using PageId = uint32_t` — not a typedef, not a raw integer in function signatures. The alias makes the type self-documenting: this argument is a page ID, not a byte count.

{{focus: offset-fn}} {{zoom: 1.3x}} {{annotate: offset-fn "One multiplication"}} `static_cast<size_t>(id) * kPageSize` — the cast widens to `size_t` before multiplying to avoid 32-bit overflow on large files.

{{zoom: 1x}} {{focus: none}} {{unsplit}} The function is three lines. It does meaningful work — it defines the fundamental addressing scheme of the storage engine. Small doesn't mean shallow.

```code:offset-code lang=cpp
#include <cstdint>
#include <cstddef>

using PageId = uint32_t;
constexpr uint32_t kPageSize = 4096;

// Maps a PageId to its byte offset in the database file.
// Precondition: id < total page count.
size_t page_offset(PageId id) {
  return static_cast<size_t>(id) * kPageSize;
}
---
pageid-alias: 4
offset-fn: 9-11
```

# The page header — 8 bytes of metadata

{{clear: slide}}

{{show: header-bytes typewriter 2s linear}} Every page begins with an 8-byte header. Those 8 bytes tell the storage engine what kind of page this is and how many cells it holds.

{{focus: type-byte}} {{annotate: type-byte "Leaf or interior"}} Byte 0 is the page type. `0x0D` means leaf — this page holds actual data rows. `0x05` means interior — this page holds pointers to child pages.

{{focus: reserved-bytes}} {{annotate: reserved-bytes "Reserved, always zero"}} Bytes 1–3 are reserved. Zero them on write, ignore on read. They're placeholder space for future fields.

{{focus: count-bytes}} {{annotate: count-bytes "Big-endian uint32"}} Bytes 4–7 are `cell_count` as a big-endian uint32. A cell is one key-value pair. This count tells you how many rows live on this page.

{{focus: none}} {{clear: slide}}

{{show: header-struct typewriter 2s linear}} {{split}} {{show: header-bytes none 0s}} Here's the C++ struct that mirrors those 8 bytes exactly.

{{focus: page-type-field}} {{annotate: page-type-field "One byte, two values"}} `uint8_t page_type` — exactly one byte. The two valid values are `0x0D` and `0x05`. Any other value means a corrupted file.

{{focus: reserved-field}} {{annotate: reserved-field "Layout padding"}} `uint8_t reserved[3]` — three bytes of padding. Struct layout matches the on-disk format byte for byte.

{{focus: cell-count-field}} {{annotate: cell-count-field "uint32, big-endian"}} `uint32_t cell_count` — stored big-endian on disk, but `PageHeader` holds it native after decoding. The decoder does the conversion once.

{{focus: parse-fn}} {{zoom: 1.2x}} {{annotate: parse-fn "Decode once, use native"}} `parse_page_header` reads the raw bytes and flips `cell_count` to native endian. Callers use plain arithmetic — no byte-swap at every access.

{{focus: none}} {{unsplit}} The struct owns nothing. No heap, no pointers, no destructor. It's four fields that fit in a register pair. Zero-cost to copy, trivially destructible.

{{focus: none}} {{clear: slide}}

```data:header-bytes type=array
[0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03]
---
type-byte: 0
reserved-bytes: 1-3
count-bytes: 4-7
```

```code:header-struct lang=cpp
#include <cstdint>
#include <span>
#include "db_result.h"

using PageId = uint32_t;
constexpr uint32_t kPageSize = 4096;

// On-disk page header: 8 bytes at the start of every page.
struct PageHeader {
  uint8_t  page_type;    // 0x0D = leaf, 0x05 = interior
  uint8_t  reserved[3];  // zero on write; ignored on read
  uint32_t cell_count;   // number of cells on this page (native endian)
};

// Parses the first 8 bytes of a page's raw data.
DbResult<PageHeader> parse_page_header(std::span<const uint8_t> page) {
  if (page.size() < 8) {
    return std::unexpected(DbError::kCorrupted);
  }
  if (page[0] != 0x0D && page[0] != 0x05) {
    return std::unexpected(DbError::kCorrupted);
  }
  uint32_t cell_count =
    (static_cast<uint32_t>(page[4]) << 24) |
    (static_cast<uint32_t>(page[5]) << 16) |
    (static_cast<uint32_t>(page[6]) <<  8) |
     static_cast<uint32_t>(page[7]);
  return PageHeader{page[0], {0, 0, 0}, cell_count};
}
---
page-type-field: 10
reserved-field: 11
cell-count-field: 12
parse-fn: 16-28
```

# The grand plan — the full system

{{clear: slide}}

{{show: system-diagram grow 0.5s spring}} Here's the full system we'll build over this course. Three layers: storage, parser, executor.

{{focus: storage-node}} {{annotate: storage-node "Pages on disk"}} The storage engine reads and writes pages. It knows nothing about SQL. It only knows page IDs, byte offsets, and B-tree structure.

{{focus: parser-node}} {{annotate: parser-node "SQL to plan"}} The parser takes SQL text and turns it into a query plan — a structured description of what the database should do.

{{focus: executor-node}} {{annotate: executor-node "Plan to rows"}} The executor walks the query plan and drives the storage engine. It's the bridge between SQL semantics and raw page operations.

{{focus: sql-node}} {{annotate: sql-node "Client entry point"}} SQL arrives as a string. Everything else is a consequence of parsing and executing that string.

{{focus: none}} {{clear: slide}}

{{show: system-diagram grow 0.5s spring}} Trace a `SELECT` query through the diagram. SQL comes in at the top.

{{focus: sql-to-parser}} {{annotate: sql-to-parser "Text to structure"}} The query string flows into the parser. Out comes a plan: "scan the `users` table, return all rows where `id = 42`."

{{focus: executor-to-storage}} {{annotate: executor-to-storage "Plan to page reads"}} The executor drives storage: "give me page 3, page 7, page 12." The storage engine translates those IDs to byte offsets and reads from disk.

{{focus: none}} Each layer has one job. {{focus: storage-node}} The storage engine doesn't know SQL. {{focus: parser-node}} The parser doesn't know about pages. {{focus: executor-node}} The executor coordinates — it knows both interfaces, but implements neither.

{{focus: none}} This lesson covers the storage engine. The next three lessons build it from the ground up: reading pages, building B-trees, inserting and querying rows.

```diagram:system-diagram
sql [client: SQL string]
parser [service: Tokenizer + Parser]
executor [service: Executor]
storage [database: B-tree + PageManager]
sql --"string_view"--> parser
parser --"Stmt variant"--> executor
executor --"page reads/writes"--> storage
{SQL Layer: sql, parser}
{Execution: executor}
{Storage Engine: storage}
---
sql-node: sql
parser-node: parser
executor-node: executor
storage-node: storage
sql-to-parser: sql
executor-to-storage: executor
```

# Why fixed-size pages?

{{clear: slide}}

{{show: alignment-diagram typewriter 2s linear}} Fixed-size pages exist for three reasons: alignment, predictability, and no fragmentation.

{{focus: align-row}} {{annotate: align-row "OS page aligned"}} Alignment first. A 4096-byte database page aligns with the OS virtual memory page size and the default filesystem block on most platforms. The OS page cache holds it as a single unit — no straddling, no partial reads.

{{focus: pred-row}} {{annotate: pred-row "O(1) random access"}} Predictability second. Given a page ID, `page_offset` gives the exact byte position in O(1). No traversal. No seek index. The address is computable from the integer alone.

{{focus: frag-row}} {{annotate: frag-row "No variable gaps"}} No fragmentation third. Variable-size records leave gaps when deleted. Fixed-size pages don't. A free page is the same size as an occupied page. Reuse is trivial: hand the page ID to the next writer.

{{focus: none}} {{clear: slide}}

{{show: tradeoff-code typewriter 2s linear}} There's one tradeoff: internal fragmentation. A page holds as many cells as fit. The last cell might leave wasted space at the end of the page.

{{focus: waste-region}} {{zoom: 1.2x}} {{annotate: waste-region "Slack space at end"}} If cells are 200 bytes and the page is 4096 bytes, each page wastes up to 199 bytes — fewer than 5%. Acceptable.

{{zoom: 1x}} {{focus: alternative-cost}} {{annotate: alternative-cost "Variable cost: an index"}} The alternative — variable-size pages — eliminates internal fragmentation but requires a seek index. That index is complexity, and complexity has bugs. Fixed pages trade a few bytes for a far simpler addressing model.

{{focus: none}} The databases that matter — SQLite, PostgreSQL, InnoDB — all use fixed-size pages. The tradeoff is settled. The page model is the foundation of every storage engine in production.

```data:alignment-diagram type=array
["alignment    │ 4096-byte page = 1 OS page   │ cache unit fits exactly, no splits", "predictable  │ offset = id × page_size     │ O(1) address from integer alone", "no frag      │ free page = same size        │ reuse is hand-the-id-back"]
---
align-row: 0
pred-row: 1
frag-row: 2
```

```code:tradeoff-code lang=cpp
#include <cstdint>

constexpr uint32_t kPageSize   = 4096;
constexpr uint32_t kCellSize   = 200;   // example: fixed-size cells

// How many cells fit on one page?
constexpr uint32_t kCellsPerPage = kPageSize / kCellSize;   // 20

// Slack at the end of each page:
constexpr uint32_t kSlackBytes = kPageSize % kCellSize;     // 96 bytes wasted

// That's 96 / 4096 ≈ 2.3% internal fragmentation per page.
// Variable-length records would eliminate this — at the cost of a seek index.
---
waste-region: 9-10
alternative-cost: 13
```
