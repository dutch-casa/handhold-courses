---
title: SIMD Row Scanning
---

# Scalar versus SIMD

{{clear: slide}}

{{show: simd-comparison typewriter 2s linear}} The scalar scan loop processes one row per loop iteration.

{{focus: scalar-row}} {{annotate: scalar-row "One comparison per cycle"}} Each iteration loads `row.id`, compares it against the target, then moves to the next row. On modern hardware this is roughly one comparison per 2 to 4 clock cycles. {{focus: simd-row}} {{annotate: simd-row "Four comparisons at once"}} SIMD — Single Instruction Multiple Data — loads four 32-bit integers into one 128-bit register and compares all four against the target with a single instruction. {{annotate: simd-row "Four times the throughput"}} At the same clock speed, the SIMD loop processes four rows per iteration instead of one.

{{focus: register-diagram}} {{annotate: register-diagram "128-bit register"}} A 128-bit SSE2 register holds four 32-bit values packed contiguously. A single `_mm_cmpeq_epi32` instruction compares all four against a broadcast scalar — the same as four separate comparisons but in one cycle. {{focus: avx-row}} {{annotate: avx-row "AVX: 256-bit width"}} With 256-bit AVX2 registers, eight `uint32_t` values fit in one register. The principle is identical: one instruction, eight comparisons.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```data:simd-comparison type=array
["scalar loop   │  1 row/iteration  │  load, compare, branch, advance", "SSE2 loop     │  4 rows/iteration │  _mm_loadu_si128, _mm_cmpeq_epi32, _mm_movemask_epi8", "AVX2 loop     │  8 rows/iteration │  _mm256_loadu_si256, _mm256_cmpeq_epi32, _mm256_movemask_epi8"]
---
scalar-row: 0
simd-row: 1
avx-row: 2
register-diagram: 1
```

# SSE2 intrinsics for id scanning

{{clear: slide}}

{{show: sse2-snippet typewriter 2s linear}} SSE2 provides three intrinsics for this scan.

{{focus: loadu-call}} {{annotate: loadu-call "16-byte load"}} `_mm_loadu_si128` loads 16 unaligned bytes — four `uint32_t` values — from a pointer into a 128-bit `__m128i` register. The `u` suffix means unaligned: it works on any address, not just 16-byte-aligned addresses. {{annotate: loadu-call "Row pointer cast"}} The pointer is cast from `const Row*` to `const __m128i*`. This is safe when the rows are packed contiguously in a vector.

{{focus: cmpeq-call}} {{annotate: cmpeq-call "4-integer comparison"}} `_mm_cmpeq_epi32` compares two 128-bit registers element-wise as four 32-bit integers. Each lane produces `0xFFFFFFFF` if the values are equal, or `0x00000000` if they differ. {{annotate: cmpeq-call "Target broadcast"}} `_mm_set1_epi32(target_id)` fills all four lanes of a register with the same value — the id you are searching for.

{{focus: movemask-call}} {{annotate: movemask-call "High-bit extraction"}} `_mm_movemask_epi8` extracts the high bit of each of the 16 bytes in the register and packs them into a 16-bit integer. {{annotate: movemask-call "Non-zero means match"}} If the result is non-zero, at least one of the four rows matched. {{annotate: movemask-call "Bit position = lane"}} The specific bit positions tell you which lane matched.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:sse2-snippet lang=cpp
#include <immintrin.h>  // SSE2 intrinsics

// Broadcast the target id into all four lanes of a 128-bit register.
__m128i target = _mm_set1_epi32(static_cast<int>(search_id));

// Load four Row::id fields from four consecutive rows.
// Row is 40 bytes; the id field is at offset 0, but rows are not packed
// id-to-id. Gather is needed for non-contiguous id fields — see full loop.
__m128i ids = _mm_loadu_si128(reinterpret_cast<const __m128i*>(id_ptr));

// Compare: each lane is 0xFFFFFFFF on match, 0x00000000 on mismatch.
__m128i cmp = _mm_cmpeq_epi32(ids, target);

// Extract high bit of each byte → 16-bit mask. Non-zero means at least one match.
int mask = _mm_movemask_epi8(cmp);
---
loadu-call: 9
cmpeq-call: 12
movemask-call: 15
```

# The SIMD scan loop

{{clear: slide}}

{{show: simd-loop typewriter 2s linear}} The full SIMD scan processes four rows per iteration and falls back to scalar for the tail.

{{focus: stride-comment}} {{zoom: 1.2x}} {{annotate: stride-comment "Non-contiguous id fields"}} `Row` is 40 bytes, so consecutive `row.id` fields are 40 bytes apart — not 4 bytes apart. A simple `_mm_loadu_si128` on the row array would load 16 bytes starting at `rows[0]`, which contains `id`, `null_mask`, and 11 bytes of `name` — not four `id` values. {{annotate: stride-comment "Manual gather required"}} We must gather the four `id` fields manually before loading them into the SIMD register.

{{zoom: 1x}} {{focus: gather-block}} {{zoom: 1.2x}} {{annotate: gather-block "Four sequential loads"}} The gather block loads `rows[i].id` through `rows[i+3].id` into a temporary array of four `int32_t` values, then loads that array into the SIMD register. {{annotate: gather-block "Compiler hint"}} With optimization enabled, the compiler often fuses these four scalar loads into a single SIMD gather instruction.

{{zoom: 1x}} {{focus: simd-compare}} {{zoom: 1.2x}} {{annotate: simd-compare "Single instruction, four comparisons"}} One `_mm_cmpeq_epi32` compares all four gathered ids against the target in a single instruction. {{zoom: 1x}} {{focus: mask-check}} {{zoom: 1.2x}} {{annotate: mask-check "Mask non-zero means hit"}} If `mask` is non-zero, at least one of the four rows matched. The scalar fallback then identifies the exact lane.

{{zoom: 1x}} {{focus: scalar-tail}} {{zoom: 1.2x}} {{annotate: scalar-tail "Remainder rows"}} When the row count is not a multiple of four, the final 0 to 3 rows are handled by a plain scalar loop. No special alignment is required.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```code:simd-loop lang=cpp
#include <immintrin.h>

void scan_ids(const std::vector<Row>& rows,
              uint32_t search_id,
              std::vector<uint32_t>& matches) {
  __m128i target = _mm_set1_epi32(static_cast<int>(search_id));
  size_t i = 0;

  // SIMD loop: four rows per iteration.
  for (; i + 4 <= rows.size(); i += 4) {
    // Gather the four id fields into a contiguous array.
    int32_t ids[4] = {
      static_cast<int32_t>(rows[i + 0].id),
      static_cast<int32_t>(rows[i + 1].id),
      static_cast<int32_t>(rows[i + 2].id),
      static_cast<int32_t>(rows[i + 3].id),
    };
    __m128i loaded = _mm_loadu_si128(reinterpret_cast<const __m128i*>(ids));
    __m128i cmp    = _mm_cmpeq_epi32(loaded, target);
    int mask       = _mm_movemask_epi8(cmp);

    if (mask != 0) {
      // At least one match: check each lane individually.
      for (size_t j = 0; j < 4; ++j) {
        if (rows[i + j].id == search_id) {
          matches.push_back(static_cast<uint32_t>(i + j));
        }
      }
    }
  }

  // Scalar tail: rows.size() % 4 remaining rows.
  for (; i < rows.size(); ++i) {
    if (rows[i].id == search_id) {
      matches.push_back(static_cast<uint32_t>(i));
    }
  }
}
---
stride-comment: 11
gather-block: 12
simd-compare: 19
mask-check: 22
scalar-tail: 32
```

# Alignment and portability

{{clear: slide}}

{{show: portability-table typewriter 2s linear}} SSE2 is available on every x86-64 processor — it has been part of the architecture since 2001.

{{focus: sse2-row}} {{annotate: sse2-row "SSE2 guaranteed"}} When you compile for x86-64 without specifying a target, the compiler can safely emit SSE2 instructions. No runtime check is needed. {{focus: neon-row}} {{annotate: neon-row "ARM NEON"}} On ARM64, NEON is the equivalent: 128-bit registers, with `vld1q_u32` for loading and `vceqq_u32` for comparison. {{annotate: neon-row "No movemask on NEON"}} NEON has no single movemask instruction. Extracting a match mask requires a multi-step reduction: shrink each lane to its high bit with `vshrn_n_u16`, then read the result with `vget_lane_u64`. {{annotate: neon-row "Different headers"}} The header is `<arm_neon.h>` and the type is `uint32x4_t` instead of `__m128i`.

{{focus: autovect-row}} {{annotate: autovect-row "Auto-vectorization flag"}} With `-O3 -march=native`, the compiler will often auto-vectorize a scalar loop. Profiling is the only way to confirm whether it did. {{annotate: autovect-row "Target CPU features"}} `-march=native` enables all instruction set extensions available on the build machine. Do not use it for binaries you distribute — the binary will crash on machines with older instruction sets.

{{focus: std-simd-row}} {{annotate: std-simd-row "Portable SIMD wrapper"}} The C++ standard library defines `std::experimental::simd` in the Parallelism TS. It provides a portable abstraction over SSE2 and NEON. {{annotate: std-simd-row "Not yet standard"}} As of C++23, it is not part of the standard library proper. GCC and Clang support it; MSVC support is incomplete. Use it when portability matters more than raw control.

{{zoom: 1x}} {{focus: none}} {{clear: slide}}

```data:portability-table type=array
["SSE2              │  x86-64  │  guaranteed, since 2001, header: <immintrin.h>", "AVX2              │  x86-64  │  Haswell+, check CPUID or use -mavx2", "NEON              │  ARM64   │  guaranteed on AArch64, header: <arm_neon.h>", "-O3 -march=native │  any     │  let the compiler vectorize; verify with objdump", "std::simd (TS)    │  any     │  portable abstraction, GCC/Clang, C++23 experimental"]
---
sse2-row: 0
neon-row: 2
autovect-row: 3
std-simd-row: 4
```
