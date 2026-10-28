---
name: libvpx/vpx_dsp/prune-sparse-idct-zero-columns
description: When implementing sparse inverse transform SIMD routines, verify the non-zero coefficient footprint against scan order to prune all-zero vector calculations
rubric: safety
severity: should_fix
---

**Rule.** In partial and sparse inverse transform (IDCT) SIMD routines,
accurately verify the non-zero coefficient footprint in the 2D matrix against
the codec's scan order (such as vp9_default_iscan_*). Calculate transform stages
only for active columns and rows, and omit loads and butterfly operations for
vectors guaranteed to be all zeros.

**Why.** In sparse IDCT variants (e.g., idct32x32_34, idct16x16_10), non-zero
coefficients only populate a small corner of the block. Overestimating the
active column or row bounds wastes registers and SIMD instructions computing
redundant arithmetic on all-zero vectors.

## Examples

Bad:

```cpp
// Assuming 34 non-zero coefficients in idct32x32 occupy 7 columns (0-6):
static void idct32_7_neon(const int16_t *input, int16_t *output) {
  // Unnecessarily computes transform butterfly stages for column 6:
  multiply_shift_and_narrow(in6, -cospi_26_64, &s2_11);
  multiply_shift_and_narrow(in6, cospi_6_64, &s2_12);
}
```

Good:

```cpp
// Scan order confirms 34 coefficients occupy only columns 0-5 (columns 6-7 are all 0):
void vpx_idct32_6_neon(const tran_low_t *input, int16_t *output) {
  // Loads and transform calculations for in[6] and in[7] are omitted entirely,
  // computing stages only for active columns in[0] .. in[5].
}
```
