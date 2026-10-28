---
name: libvpx/delineate-group-comments-with-section-dividers
description: When structuring or refactoring function families, use and preserve horizontal divider comments (`// ----------------...`) and blank lines to delineate logical sections
rubric: readability
severity: nit
---

**Rule.** Use distinct horizontal section divider banners (e.g., `//
-----------------------------------------------------------------------------`)
with blank line separation to distinguish group- or section-level comments
spanning multiple functions from individual function doc comments, or split
unrelated function groups into separate files. Do not place bare section headers
directly above a function declaration without visual separation. Additionally,
when refactoring, editing, or applying automated formatting to files structured
into logical sections (such as DSP algorithms or predictor modes), preserve
existing horizontal divider comments between function families and
implementation blocks rather than stripping them.

**Why.** In files organized into logical sections, horizontal divider comments
provide clear visual boundaries between distinct function families and algorithm
modes. Placing bare section comments directly above a function makes them appear
as individual function documentation, while removing existing dividers during
refactoring creates unnecessary cosmetic diff churn and impairs navigation
across large source files.

## Examples

Bad:

```cpp
// Variant 1: Bare section comment directly above a function
// Copy and average
static INLINE void convolve_copy_row(...) {
  // ...
}

// Variant 2: Removing divider banners between predictor modes during refactoring
void vpx_dc_128_predictor_32x32_neon(uint8_t *dst, ptrdiff_t stride,
                                     const uint8_t *above,
                                     const uint8_t *left) {
  dc_store_32x32(dst, stride, vdup_n_u8(128));
}

void vpx_d45_predictor_4x4_neon(uint8_t *dst, ptrdiff_t stride,
                                const uint8_t *above,
                                const uint8_t *left) {
  // ...
}
```

Good:

```cpp
// Variant 1: Section banner with blank line separation
// -----------------------------------------------------------------------------
// Copy and average

static INLINE void convolve_copy_row(...) {
  // ...
}

// Variant 2: Preserved divider banners separating function families
void vpx_dc_128_predictor_32x32_neon(uint8_t *dst, ptrdiff_t stride,
                                     const uint8_t *above,
                                     const uint8_t *left) {
  dc_store_32x32(dst, stride, vdup_n_u8(128));
}

// -----------------------------------------------------------------------------

void vpx_d45_predictor_4x4_neon(uint8_t *dst, ptrdiff_t stride,
                                const uint8_t *above,
                                const uint8_t *left) {
  // ...
}
```
