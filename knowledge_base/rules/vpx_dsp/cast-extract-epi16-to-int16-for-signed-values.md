---
name: libvpx/vpx_dsp/cast-extract-epi16-to-int16-for-signed-values
description: When extracting signed 16-bit values from SIMD vectors using _mm_extract_epi16, explicitly cast to (int16_t) instead of manual sign-extension masking or branching
rubric: readability
severity: should_fix
---

**Rule.** When extracting signed 16-bit integer values from SIMD registers using
_mm_extract_epi16() or _mm256_extract_epi16(), explicitly cast the result to
(int16_t) (e.g., (int16_t)_mm_extract_epi16(vec, lane)). Do not leave the
zero-extended int uncast when assigned to wider signed types, and do not use
manual bitwise masking and conditional branches to perform sign extension.

**Why.** _mm_extract_epi16() zero-extends the extracted 16-bit lane into an int,
producing positive values (0x0000–0xFFFF) for negative 16-bit quantities.
Casting explicitly to (int16_t) cleanly conveys the signed reinterpretation,
ensures correct sign-extension when promoted, and avoids verbose manual bitwise
masking or conditional branching.

## Examples

Bad:

```cpp
int mean;
...
mean = _mm_extract_epi16(sum, 0);
if ((mean & 0x00008000) == 0x00008000) {
  mean = 0xFFFF0000 | mean;
}
```

Good:

```cpp
int16_t mean;
...
mean = (int16_t)_mm_extract_epi16(sum, 0);
```
