---
name: libvpx/general-use-void-cast-for-unused-parameters
description: When suppressing unused parameter or variable warnings, use portable (void)param; casts inside the function body, but omit casts for used parameters
rubric: readability
severity: should_fix
---

**Rule.** To suppress unused parameter or variable compiler warnings across all
toolchains, explicitly cast the unused identifier to (void) (e.g., (void)param;)
inside the function body rather than using compiler-specific attributes like
__attribute__((unused)) or custom macros. Do not include (void)param; casts for
parameters or variables that are actually referenced in the function body.

**Why.** Compiler attributes like __attribute__((unused)) are non-portable
across toolchains such as MSVC. Standard (void)param; casts portably silence
unused parameter warnings across all compilers, but leaving (void) casts on
parameters that are actually referenced in the function creates dead and
misleading code.

## Examples

Bad:

```cpp
// Bad: Non-portable compiler attribute for unused parameter.
void vpx_highbd_quantize_b_neon(
    const tran_low_t *coeff, intptr_t n_coeffs,
    const int16_t __attribute__((unused)) *scan) {
  (void)n_coeffs;
}

// Bad: (void) cast on parameter 'stride' that is actually used in function body.
void vpx_d45_predictor_4x4_neon(uint8_t *dst, ptrdiff_t stride,
                                const uint8_t *above, const uint8_t *left) {
  (void)stride;
  (void)left;
  store_u8_4x1(dst + 0 * stride, d0);
}
```

Good:

```cpp
// Good: Standard portable (void) casts for genuinely unused parameters.
void vpx_highbd_quantize_b_neon(
    const tran_low_t *coeff, intptr_t n_coeffs,
    const int16_t *scan) {
  (void)n_coeffs;
  (void)scan;
}

// Good: Only cast genuinely unused parameter 'left'; omit cast on used 'stride'.
void vpx_d45_predictor_4x4_neon(uint8_t *dst, ptrdiff_t stride,
                                const uint8_t *above, const uint8_t *left) {
  (void)left;
  store_u8_4x1(dst + 0 * stride, d0);
}
```
