---
name: libvpx/vpx_dsp/prefer-vst1-over-vst2-for-contiguous-stores
rubric: safety
severity: should_fix
---

**Rule.** Do not use interleaved NEON store intrinsics (such as vst2q_* or
vst2_*) with duplicated register lanes to write contiguous memory blocks. Prefer
sequential single-vector stores (such as vst1q_* or vst1_*) with pointer offsets
to write contiguous chunks.

**Why.** Interleaved stores (vst2/vst2q) interleave elements across vectors and
often induce compiler code generation inefficiencies, register spilling, and
aggressive unrolling with high instruction latency. Sequential vst1/vst1q stores
translate directly to linear vector stores, yielding better compiler
optimization and memory throughput.

## Examples

Bad:

```cpp
uint8x16x2_t d;
d.val[0] = d.val[1] = vdupq_lane_u8(left_u8d, 0);
vst2q_u8(dst, d);
```

Good:

```cpp
const uint8x16_t d = vdupq_lane_u8(left_u8d, 0);
vst1q_u8(dst, d);
vst1q_u8(dst + 16, d);
```
