---
name: libvpx/vpx_dsp/use-store-prefix-for-simd-destination-helpers
description: When naming DSP or SIMD helper functions that output vector registers to memory buffers, use the store_ prefix instead of write_
rubric: readability
severity: nit
---

**Rule.** When defining DSP and SIMD helper functions that pack or write vector
registers into destination memory buffers, name them with the `store_` prefix
(e.g., `store_8x1_pixels`, `store_16x2`, `store_tran_low`) rather than `write_`
(such as `write_8x1_pixels`). Aligning helper names with canonical load/store
SIMD terminology maintains consistency across architecture ports.

**Why.** In SIMD implementations across libvpx, memory output helpers
consistently use `store_` to match SIMD load/store terminology and hardware
intrinsics (`_mm_store*`, `vst1_*`). Using `write_` introduces naming
inconsistency across sibling DSP kernels and architecture backends.

## Examples

Bad:

```cpp
// Non-idiomatic: using 'write_' prefix for vector destination helper
static INLINE void write_8x1_pixels(const __m256i *y, const __m256i *mask,
                                    uint16_t *dst, ptrdiff_t pitch) {
  const __m128i res = _mm256_castsi256_si128(*y);
  _mm_storeu_si128((__m128i *)dst, res);
}
```

Good:

```cpp
// Idiomatic: using standard 'store_' prefix for SIMD memory output
static INLINE void store_8x1_pixels(const __m256i *y, const __m256i *mask,
                                    uint16_t *dst, ptrdiff_t pitch) {
  const __m128i res = _mm256_castsi256_si128(*y);
  _mm_storeu_si128((__m128i *)dst, res);
}
```
