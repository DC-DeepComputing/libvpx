---
name: libvpx/vpx_dsp/avoid-trivial-macros-wrapping-simd-intrinsics
description: When implementing SIMD operations or memory loads, write standard intrinsics or static inline functions instead of trivial macros that implicitly bind local variables
rubric: readability
severity: should_fix
---

**Rule.** Do not define trivial helper macros for short SIMD operations, memory
loads, or unpacks that implicitly read or write local variables. Write standard
SIMD intrinsics directly in the function body, or define explicit static INLINE
helper functions with typed parameters and return values instead.

**Why.** Macros that wrap 1-2 SIMD intrinsics or implicitly bind local variable
names obscure data flow, make code harder to read and debug, and often require
redundant explanatory comments compared to self-documenting intrinsic calls or
typed inline functions.

## Examples

Bad:

```cpp
#define LOAD_SRC_DST                                    \
  /* load source and destination */                     \
  src_reg = _mm256_loadu_si256((__m256i const *)(src)); \
  dst_reg = _mm256_loadu_si256((__m256i const *)(dst));

#define MERGE_WITH_SRC(src_reg, reg)               \
  exp_src_lo = _mm256_unpacklo_epi8(src_reg, reg); \
  exp_src_hi = _mm256_unpackhi_epi8(src_reg, reg);
```

Good:

```cpp
const __m256i dst_reg = _mm256_loadu_si256((const __m256i *)(dst));
const __m256i src_reg = _mm256_loadu_si256((const __m256i *)(src));
const __m256i exp_src_lo = _mm256_unpacklo_epi8(src_reg, zero_reg);
const __m256i exp_src_hi = _mm256_unpackhi_epi8(src_reg, zero_reg);
```
