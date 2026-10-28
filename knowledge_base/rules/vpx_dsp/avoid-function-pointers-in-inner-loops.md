---
name: libvpx/vpx_dsp/avoid-function-pointers-in-inner-loops
description: When writing performance-critical SIMD or DSP inner loops and kernels, invoke static INLINE helpers directly instead of passing function pointers or callbacks
rubric: safety
severity: should_fix
---

**Rule.** In performance-critical DSP, SIMD, and codec inner loops or kernels,
avoid passing function pointer arguments or callbacks (such as store, write, or
blend functions) to share loop boilerplate across variants. Instead, invoke
static INLINE helper functions directly or provide dedicated specialized kernel
routines to guarantee full compiler inlining and eliminate indirect call
overhead across all compilers.

**Why.** Passing function pointers into hot inner loops relies on compiler
devirtualization and constant propagation that often fail across different
compilers (GCC, Clang, MSVC) and optimization levels. Indirect calls introduce
runtime call overhead, inhibit auto-vectorization and instruction scheduling,
and force vector register spills to the stack. Direct calls to static INLINE
functions or specialized kernel variants allow compilers to fully inline and
optimize loop bodies.

## Examples

Bad:

```cpp
// Passing function pointer callback into hot inner loop / kernel
typedef void (*WritePixels)(const __m256i *r0, const __m256i *r1,
                            uint16_t *dst, ptrdiff_t pitch);

static void filter_block_horiz_avx2(
    const uint16_t *src, ptrdiff_t src_pitch, const WritePixels write_fn,
    uint16_t *dst, ptrdiff_t dst_pitch, uint32_t height) {
  for (uint32_t i = 0; i < height; ++i) {
    // Indirect call inside inner loop prevents compiler inlining
    write_fn(&res0, &res1, dst, dst_pitch);
  }
}
```

Good:

```cpp
// Use direct static INLINE helper calls or specialized routines
static INLINE void write_pixels_8x2_avx2(const __m256i *r0, const __m256i *r1,
                                         uint16_t *dst, ptrdiff_t pitch) {
  _mm256_storeu_si256((__m256i *)dst, *r0);
  _mm256_storeu_si256((__m256i *)(dst + pitch), *r1);
}

static void filter_block_horiz_avx2(
    const uint16_t *src, ptrdiff_t src_pitch,
    uint16_t *dst, ptrdiff_t dst_pitch, uint32_t height) {
  for (uint32_t i = 0; i < height; ++i) {
    write_pixels_8x2_avx2(&res0, &res1, dst, dst_pitch);
  }
}
```
