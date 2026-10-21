---
name: libvpx/vpx_dsp/vpx-dsp-prefer-storel-epi64-over-extract-epi32
rubric: safety
severity: should_fix
---

**Rule.** When storing 64 bits (8 bytes) of contiguous data from a 128-bit
vector register to memory, use _mm_storel_epi64() instead of multiple 32-bit
lane extractions (_mm_extract_epi32) or scalar stores. This generates a single
64-bit store (movq) and maintains compatibility with baseline SSE2 without
requiring SSE4.1 instructions.

**Why.** Extracting individual 32-bit lanes with _mm_extract_epi32 requires
SSE4.1 support and generates multiple extraction and scalar store instructions.
Using _mm_storel_epi64 compiles to a single 64-bit movq store instruction and
broadens hardware compatibility by requiring only baseline SSE2.

## Examples

Bad:

```cpp
// Storing 64 bits via SSE4.1 32-bit extractions
int32_t *store_diff = (int32_t *)diff;
store_diff[0] = _mm_extract_epi32(x0, 0);
store_diff[1] = _mm_extract_epi32(x0, 1);
```

Good:

```cpp
// Storing 64 bits via single SSE2 64-bit store
_mm_storel_epi64((__m128i *)diff, x0);
```
