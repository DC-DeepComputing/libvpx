---
name: libvpx/vpx_dsp/neon-wrapping-arithmetic-for-wraplow
description: In ARM NEON inverse transforms, use wrapping arithmetic (vaddq_s16, vsubq_s16) and non-saturating narrowing (vrshrn_n_s32) instead of saturating intrinsics for intermediate WRAPLOW calculations
rubric: correctness
severity: should_fix
---

**Rule.** In ARM NEON inverse transform implementations (such as IDCT, IADST,
and IHT routines), use wrapping arithmetic (vaddq_s16, vsubq_s16, vadd_s16,
vsub_s16) and non-saturating rounding shift and narrow (vrshrn_n_s32) instead of
saturating intrinsics (vqaddq_s16, vqsubq_s16, vqrshrn_n_s32) for intermediate
butterfly calculations governed by WRAPLOW(). Reserve saturating arithmetic and
narrowing for final-stage pixel clamping or operations explicitly requiring
saturation.

**Why.** The WRAPLOW() macro in transform specifications and reference code
defines 16-bit modular integer overflow wrapping rather than saturation. Using
saturating NEON intrinsics (like vqaddq_s16, vqsubq_s16, or vqrshrn_n_s32)
clamps intermediate values upon overflow, diverging from reference C behavior
and introducing mathematical errors and visual artifacts.

## Examples

Bad:

```cpp
// Intermediate butterfly arithmetic using saturating intrinsics:
s2_22 = vqaddq_s16(s1_21, s1_22);
s2_20 = vqsubq_s16(s1_23, s1_20);

// Intermediate narrowing using saturating rounding shift and narrow:
return vcombine_s16(vqrshrn_n_s32(temp_low, DCT_CONST_BITS),
                    vqrshrn_n_s32(temp_high, DCT_CONST_BITS));
```

Good:

```cpp
// Intermediate butterfly arithmetic using wrapping arithmetic (WRAPLOW compliant):
s2_22 = vaddq_s16(s1_21, s1_22);
s2_20 = vsubq_s16(s1_23, s1_20);

// Intermediate narrowing using non-saturating rounding shift and narrow:
return vcombine_s16(vrshrn_n_s32(temp_low, DCT_CONST_BITS),
                    vrshrn_n_s32(temp_high, DCT_CONST_BITS));
```
