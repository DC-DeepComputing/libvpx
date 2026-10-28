---
name: libvpx/guard-wide-simd-stack-alignment-behind-arch-checks
description: When defining DECLARE_ALIGNED buffers or lookup tables for SIMD, use 16-byte alignment for ARM NEON and guard 32/64-byte alignment behind x86 checks
rubric: correctness
severity: should_fix
---

**Rule.** When declaring constant lookup tables, local stack buffers, or
alignment macros for SIMD implementations using DECLARE_ALIGNED, match the
target vector register width (16 bytes for ARM NEON) rather than over-aligning
to 32 or 64 bytes. For shared headers or multi-architecture code defining
alignment constants, array bounds, or pointer offsets for wider SIMD (such as 32
bytes for AVX2 or 64 bytes for AVX-512), guard those macros behind architecture
checks (e.g., #if VPX_ARCH_X86 || VPX_ARCH_X86_64) while keeping alignment at 16
bytes and sizing/headroom offsets minimal for non-x86 platforms.

**Why.** ARM NEON registers are 128 bits (16 bytes) wide, whereas 256-bit AVX2
and 512-bit AVX-512 require 32- and 64-byte alignment respectively.
Unconditionally using 32-byte or 64-byte alignment constants or hardcoding
32-byte alignment in ported NEON code (such as DECLARE_ALIGNED(32, ...) or
hardcoded above_data[64 + 32]) causes unnecessary data and stack frame bloat
across non-x86 targets without providing performance benefits.

## Examples

Bad:

```cpp
// Variant 1: Unconditionally defining 64-byte alignment and hardcoded padding
#define MAX_ALIGNMENT_HBD 64

void vp9_predict_intra(uint8_t *dst) {
  DECLARE_ALIGNED(MAX_ALIGNMENT_HBD, uint8_t, above_data[64 + 32]);
  uint8_t *above_row = above_data + 32;
  /* ... */
}

// Variant 2: Over-aligning ARM NEON tables/buffers to 32 bytes copied from AVX2
DECLARE_ALIGNED(32, static const uint8_t, dot_prod_permute_tbl[48]) = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, /* ... */
};
DECLARE_ALIGNED(32, uint8_t, temp_buf[64 * 64]);
```

Good:

```cpp
// Variant 1: Architecture-guarded alignment and padding prevent stack bloat
#if VPX_ARCH_X86 || VPX_ARCH_X86_64
#define MAX_ALIGNMENT_HBD 64
#define EXTRA_ABOVE_DATA 32
#else
#define MAX_ALIGNMENT_HBD 16
#define EXTRA_ABOVE_DATA 16
#endif

void vp9_predict_intra(uint8_t *dst) {
  DECLARE_ALIGNED(MAX_ALIGNMENT_HBD, uint8_t,
                  above_data[64 + EXTRA_ABOVE_DATA]);
  uint8_t *above_row = above_data + EXTRA_ABOVE_DATA;
}

// Variant 2: Match 128-bit (16-byte) ARM NEON vector register width
DECLARE_ALIGNED(16, static const uint8_t, dot_prod_permute_tbl[48]) = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, /* ... */
};
DECLARE_ALIGNED(16, uint8_t, temp_buf[64 * 64]);
```
