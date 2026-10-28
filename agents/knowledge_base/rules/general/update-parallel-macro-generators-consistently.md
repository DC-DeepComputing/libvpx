---
name: libvpx/update-parallel-macro-generators-consistently
description: When updating macro-based function generators with parallel variants, ensure parameter and calculation updates are applied consistently across all corresponding macro definitions and invocations
rubric: correctness
severity: should_fix
---

**Rule.** When updating macro-based function generators that have parallel
variants (such as standard vs. average implementations or architecture-specific
generators), verify that parameter lists and calculations are updated
consistently across all corresponding macro definitions and invocations. Ensure
all newly added macro parameters are actually utilized in the expansion body
rather than left unused.

**Why.** Incomplete updates to parallel generator macros leave parameters unused
and calculations unadjusted in counterpart functions, leading to logic bugs,
incorrect calculations, or missed fixes across parallel macro variants.

## Examples

Bad:

```cpp
// Bad: 'cast_prod' parameter added to FN signature but omitted in calculation
#define FN(w, h, wf, wlog2, hlog2, opt, cast_prod, cast) \
  unsigned int vpx_variance##w##x##h##_##opt(            \
      const uint8_t *src_ptr, int src_stride,            \
      const uint8_t *ref_ptr, int ref_stride,            \
      unsigned int *sse) {                               \
    ...                                                  \
    return *sse - ((cast se * se) >> (wlog2 + hlog2));   \
  }
```

Good:

```cpp
// Good: 'cast_prod' is consistently applied in the calculation body
#define FN(w, h, wf, wlog2, hlog2, opt, cast_prod, cast) \
  unsigned int vpx_variance##w##x##h##_##opt(            \
      const uint8_t *src_ptr, int src_stride,            \
      const uint8_t *ref_ptr, int ref_stride,            \
      unsigned int *sse) {                               \
    ...                                                  \
    return *sse - (cast_prod(cast se * se) >> (wlog2 + hlog2)); \
  }
```
