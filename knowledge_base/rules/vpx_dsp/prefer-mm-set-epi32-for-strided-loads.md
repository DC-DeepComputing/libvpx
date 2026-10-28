---
name: libvpx/vpx_dsp/prefer-mm-set-epi32-for-strided-loads
description: When packing strided 32-bit values into a 128-bit vector in x86 SSE2, use _mm_set_epi32 with loadu_int32 instead of memcpy staging or shift-and-OR chains
rubric: readability
severity: should_fix
---

**Rule.** When packing non-contiguous or strided 32-bit (4-byte) values into a
128-bit __m128i vector in x86 SSE2 code, use _mm_set_epi32() (or
_mm_setr_epi32()) with unaligned 32-bit scalar load helpers like loadu_int32()
or load_unaligned_u32(). Avoid staging bytes through temporary stack buffers
with memcpy() and _mm_loadu_si128(), and avoid assembling vectors via manual
_mm_cvtsi32_si128 scalar loads, _mm_slli_si128 byte shifts, and _mm_or_si128
chains. Note that _mm_set_epi32() accepts arguments in reverse lane order
(highest lane index first), whereas _mm_setr_epi32() accepts them in natural
lane order.

**Why.** Staging strided values through memcpy() into temporary stack buffers
introduces unnecessary stack copies and store-forwarding penalties. Similarly,
manually shifting and OR-ing scalar registers (_mm_cvtsi32_si128,
_mm_slli_si128, _mm_or_si128) emits redundant scalar-to-vector transfers (movd),
byte shifts (pslldq), and bitwise OR (por) instructions. Using _mm_set_epi32()
with loadu_int32() produces clean, concise code and enables compilers to
generate optimal register packing and memory load sequences.

## Examples

Bad:

```cpp
// Bad Variant 1: Staging strided rows through stack buffer via memcpy
uint8_t r_concat[16];
memcpy(r_concat, ref, 4);
memcpy(r_concat + 4, ref + ref_stride, 4);
memcpy(r_concat + 8, ref + 2 * ref_stride, 4);
memcpy(r_concat + 12, ref + 3 * ref_stride, 4);
r = _mm_loadu_si128((const __m128i *)r_concat);

// Bad Variant 2: Manual scalar load, shift, and bitwise OR chains
__m128i r_0 = _mm_cvtsi32_si128(*(const uint32_t *)ref);
__m128i r_1 = _mm_slli_si128(_mm_cvtsi32_si128(*(const uint32_t *)(ref + ref_stride)), 4);
__m128i r_2 = _mm_slli_si128(_mm_cvtsi32_si128(*(const uint32_t *)(ref + 2 * ref_stride)), 8);
__m128i r_3 = _mm_slli_si128(_mm_cvtsi32_si128(*(const uint32_t *)(ref + 3 * ref_stride)), 12);
r = _mm_or_si128(_mm_or_si128(r_0, r_1), _mm_or_si128(r_2, r_3));
```

Good:

```cpp
// Good: Pack strided 32-bit values directly using _mm_set_epi32 and loadu_int32
// (Passed in reverse lane order: lane 3, lane 2, lane 1, lane 0)
const __m128i r = _mm_set_epi32(loadu_int32(ref + 3 * ref_stride),
                                loadu_int32(ref + 2 * ref_stride),
                                loadu_int32(ref + ref_stride),
                                loadu_int32(ref));
```
