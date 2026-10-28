---
name: libvpx/vpx_dsp/separate-dual-simd-helper-suffix-with-underscore
description: When naming dual-operation SIMD helper functions or kernel routines, separate the _dual suffix with an underscore (e.g., _4_dual) rather than concatenating tokens (e.g., 4dual)
rubric: readability
severity: should_fix
---

**Rule.** When naming dual-operation helper functions or SIMD routines, use
`_dual` separated by an underscore (such as `_4_dual`, `_8_dual`, or
`<name>_dual`) rather than concatenating `dual` directly to preceding numeric or
descriptive tokens (e.g., `4dual`). Maintain standard snake_case token
separation across all DSP helper and kernel definitions.

**Why.** In libvpx, functions follow lowercase snake_case with underscores
separating distinct tokens, dimensions, and qualifiers. Appending `dual`
directly to numbers (like `4dual`) creates non-standard identifiers, obscures
vector dimensions or lane counts, and breaks naming consistency across DSP and
codec kernels.

## Examples

Bad:

```cpp
// Concatenating 'dual' directly to numeric token without underscore
static INLINE void dct_const_round_shift_high_4dual(const int64x2x2_t *const in,
                                                    int32x4_t *const d0,
                                                    int32x4_t *const d1);

static INLINE int32x4x2_t highbd_idct_adddual(const int32x4x2_t s0,
                                              const int32x4x2_t s1);
```

Good:

```cpp
// Separating '_dual' with an underscore
static INLINE void dct_const_round_shift_high_4_dual(const int64x2x2_t *const in,
                                                     int32x4_t *const d0,
                                                     int32x4_t *const d1);

static INLINE int32x4x2_t highbd_idct_add_dual(const int32x4x2_t s0,
                                               const int32x4x2_t s1);
```
