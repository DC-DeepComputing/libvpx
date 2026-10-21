---
name: libvpx/vpx_dsp/use-128bit-neon-for-4x4-dotprod-blocks
rubric: safety
severity: blocking
---

**Rule.** When processing 4x4 blocks in Arm NEON (particularly DotProd paths
like variance and SAD), use load_unaligned_u8q() to load all four rows (16
bytes) into a single 128-bit uint8x16_t vector. Execute 128-bit vector
intrinsics (such as vabdq_u8 and vdotq_u32) across the full block rather than
splitting the 4x4 block into multiple 64-bit vector loads (uint8x8_t via
load_u8) and 64-bit operations (vdot_u32, vabd_u8).

**Why.** A 4x4 byte block contains 16 bytes, which fits entirely within a single
128-bit NEON register. Operating on the complete 128-bit vector cuts the number
of load, difference, and dot-product instructions in half, maximizes pipeline
throughput, and reduces register pressure compared to 64-bit vector processing.

## Examples

Bad:

```cpp
uint8x8_t s[2], r[2], abs_diff[2];
uint32x2_t sse = vdup_n_u32(0);

s[0] = load_u8(src_ptr, src_stride);
r[0] = load_u8(ref_ptr, ref_stride);
src_ptr += 2 * src_stride;
ref_ptr += 2 * ref_stride;
s[1] = load_u8(src_ptr, src_stride);
r[1] = load_u8(ref_ptr, ref_stride);

abs_diff[0] = vabd_u8(s[0], r[0]);
abs_diff[1] = vabd_u8(s[1], r[1]);

sse = vdot_u32(sse, abs_diff[0], abs_diff[0]);
sse = vdot_u32(sse, abs_diff[1], abs_diff[1]);

return horizontal_add_uint32x2(sse);
```

Good:

```cpp
uint8x16_t s = load_unaligned_u8q(src_ptr, src_stride);
uint8x16_t r = load_unaligned_u8q(ref_ptr, ref_stride);

uint8x16_t abs_diff = vabdq_u8(s, r);

uint32x4_t sse = vdotq_u32(vdupq_n_u32(0), abs_diff, abs_diff);

return horizontal_add_uint32x4(sse);
```
