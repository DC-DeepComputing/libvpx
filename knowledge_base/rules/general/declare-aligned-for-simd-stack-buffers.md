---
name: libvpx/declare-aligned-for-simd-stack-buffers
description: When declaring static constant lookup tables or temporary stack buffers used in SIMD operations, wrap definitions with DECLARE_ALIGNED to ensure vector alignment
rubric: safety
severity: should_fix
---

**Rule.** When declaring static constant lookup, shuffle, or filter tables or
temporary stack buffers used for block, pixel, or transform processing in SIMD
routines, wrap their declarations with DECLARE_ALIGNED(16, type, var) (or 32/64
bytes for AVX-2/AVX-512) rather than unaligned array declarations. Use aligned
vector load and store intrinsics (such as _mm_load_si128 or _mm256_load_si256)
when reading from or writing to these aligned tables and buffers. Do not rely on
compiler default stack alignment or natural scalar type alignment for SIMD
vector operations.

**Why.** By default, local stack buffers and static constant arrays receive only
natural scalar type alignment rather than the 16-, 32-, or 64-byte alignment
required for aligned SIMD vector loads and stores (such as SSE, AVX, and NEON).
Using DECLARE_ALIGNED portably guarantees necessary byte alignment across GCC,
Clang, and MSVC. This enables high-performance aligned vector operations,
prevents general protection faults and alignment crashes on strict-alignment
architectures, and eliminates unaligned memory access penalties.

## Examples

Bad:

```cpp
// Variant 1: Unaligned static constant table loaded with unaligned SIMD load
static const uint8_t rotate_right_epu16[16] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1
};
const __m128i rotrw = _mm_loadu_si128((const __m128i *)rotate_right_epu16);

// Variant 2: Unaligned stack arrays passed to SIMD / SSE routines
int y_diff_sse[256] = { 0 };
int u_diff_sse[256] = { 0 };
```

Good:

```cpp
// Variant 1: Properly aligned static constant table loaded with aligned SIMD load
DECLARE_ALIGNED(16, static const uint8_t,
                rotate_right_epu16[16]) = {
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1
};
const __m128i rotrw = _mm_load_si128((const __m128i *)rotate_right_epu16);

// Variant 2: Properly aligned stack buffers for 16-byte vector operations
DECLARE_ALIGNED(16, int, y_diff_sse[256]);
DECLARE_ALIGNED(16, int, u_diff_sse[256]);
```
