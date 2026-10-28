---
name: libvpx/vp8/compute-eob-in-simd-registers
description: In SIMD quantization routines, compute the end-of-block (EOB) index directly in vector registers using scan-order lookup table masks and vector maximums instead of scalar memory scanning
rubric: safety
severity: should_fix
---

**Rule.** In SIMD quantization routines, compute the end-of-block (EOB) index
directly within vector registers by comparing quantized coefficients against
zero, masking with scan-order lookup tables (such as iscan or
vp8_default_inv_zig_zag), and reducing across lanes with horizontal vector
maximum operations. Do not spill quantized coefficients to memory to determine
EOB via scalar scanning loops or unrolled SET_EOB checks.

**Why.** Spilling intermediate quantized coefficients to memory to search for
the last non-zero coefficient index introduces store-to-load forwarding
penalties, cache traffic, and scalar pipeline stalls. Keeping the entire
non-zero mask, scan lookup, and maximum reduction inside SIMD registers avoids
memory round-trips and significantly improves quantization throughput.

## Examples

Bad:

```cpp
// Spilling vector coefficients to memory and scanning with scalar checks
gssdlc1 %[ftmp5], 0x07(%[qcoeff])
...
SET_EOB(15, 16);
SET_EOB(14, 15);
...
SET_EOB(0, 1);
*d->eob = eob;
```

Good:

```cpp
// Computing EOB in SIMD registers via comparison, scan mask, and vector max
pcmpeqh %[nz_mask], %[qcoeff], %[zero]
xor     %[nz_mask], %[nz_mask], %[all_ones]
and     %[nz_mask], %[nz_mask], %[inv_zig_zag]
pmaxsh  %[eob_vec], %[nz_mask], %[eob_vec]
...
*d->eob = eob;
```
