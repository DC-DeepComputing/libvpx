---
name: libvpx/vpx_dsp/avoid-64-bit-simd-loads-on-4-byte-rows
description: When reading 4-byte wide image rows into SIMD vectors, avoid 64-bit loads like _mm_loadl_epi64; use 32-bit load helpers to prevent buffer over-reads.
rubric: safety
severity: should_fix
---

**Rule.** When loading 4-byte wide image or block rows into SIMD vectors (where
row stride may exceed width), do not use 64-bit load intrinsics like
_mm_loadl_epi64. Use 32-bit load helpers (such as loadu_int32,
load_unaligned_u32, or _mm_cvtsi32_si128) or verify contiguous memory layout
before issuing wider vector loads.

**Why.** A 64-bit load (_mm_loadl_epi64) reads 8 bytes from memory. On 4-byte
wide rows with non-contiguous strides or near allocation boundaries, loading 8
bytes causes out-of-bounds memory reads (buffer over-reads), leading to
undefined behavior or AddressSanitizer failures.

## Examples

Bad:

```cpp
// When width == 4 and ref_stride > 4, loading 64 bits over-reads 4 bytes:
const __m128i r_0 = _mm_loadl_epi64((const __m128i *)ref);
const __m128i r_1 = _mm_loadl_epi64((const __m128i *)(ref + ref_stride));
```

Good:

```cpp
// Use 32-bit load helpers for 4-byte rows to prevent over-reading:
const __m128i r = _mm_set_epi32(loadu_int32(ref + 3 * ref_stride),
                                loadu_int32(ref + 2 * ref_stride),
                                loadu_int32(ref + ref_stride),
                                loadu_int32(ref));
// Or load 32 bits into a vector register directly:
const __m128i r_0 = load_unaligned_u32(ref);
```
