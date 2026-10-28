---
name: libvpx/vpx_dsp/prefer-vbsl-for-neon-vector-selection
description: When selecting between vector values using a mask in ARM NEON SIMD code, use vbsl_* or vbslq_* instead of combining vand, vmvn, and vorr
rubric: safety
severity: should_fix
---

**Rule.** In ARM NEON SIMD implementations, use dedicated vector bitwise select
intrinsics (vbsl_* / vbslq_*) to choose between two vector values based on a
selection mask. Do not manually construct bitwise multiplexers using
combinations of vand, vmvn, and vorr (or vbic).

**Why.** ARM NEON provides single-cycle hardware bitwise select instructions
(VBSL / BSL). Synthesizing vector selection through separate logical operations
(vand, vmvn, vorr) requires multiple instructions, increases register pressure,
and degrades SIMD kernel throughput.

## Examples

Bad:

```cpp
// Anti-pattern: manual bitwise selection using multiple operations
const uint8x8_t k_masked = vand_u8(k_out, mask);
const uint8x8_t v_masked = vand_u8(v0, vmvn_u8(mask));
return vorr_u8(v_masked, k_masked);
```

Good:

```cpp
// Recommended: single-instruction bitwise select
return vbsl_u8(mask, k_out, v0);
```
