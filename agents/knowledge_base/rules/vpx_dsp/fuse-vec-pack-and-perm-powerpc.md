---
name: libvpx/vpx_dsp/fuse-vec-pack-and-perm-powerpc
rubric: safety
severity: should_fix
---

**Rule.** In PowerPC VSX and AltiVec SIMD routines, combine consecutive vector
packing (vec_pack, vec_packs, vec_packsu) and lane reordering (vec_perm,
vec_mergeh, vec_mergel) operations into a single vec_perm call using a dedicated
byte-permutation mask (such as vec_perm_odd_even_pack) when intermediate values
fit within target bit widths.

**Why.** Calling vec_pack followed by a separate vec_perm emits multiple vector
instructions. Fusing truncation and lane reorganization into a single vec_perm
with a customized permutation mask eliminates redundant packing instructions and
improves SIMD throughput in DSP kernels.

## Examples

Bad:

```cpp
// Redundant two-step packing and permuting
const int16x8_t pack_sum = vec_pack(ssum_e, ssum_o);
*add = vec_perm(pack_sum, pack_sum, vec_perm_merge);
```

Good:

```cpp
// Fused into a single permute operation with a dedicated mask
*add = (int16x8_t)vec_perm(ssum_e, ssum_o, vec_perm_odd_even_pack);
```
