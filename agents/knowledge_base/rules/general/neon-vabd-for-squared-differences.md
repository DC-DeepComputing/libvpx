---
name: libvpx/neon-vabd-for-squared-differences
rubric: safety
severity: should_fix
---

**Rule.** In ARM NEON SIMD routines computing element-wise absolute differences
or squared differences ((a - b)^2), use absolute difference intrinsics (vabd_* /
vabdl_*) directly on narrow vectors rather than widening operands with vmovl_*
prior to subtracting with vsub*. For 8-bit vector inputs, use vabdl_u8 /
vabdl_s8 (or vabd_u8 / vabd_s8) directly. For 16-bit elements, use full-vector
non-widening vabdq_u16 followed by widening multiplications (vmull_u16 or
vmlal_u16) on the halves, rather than widening upfront with vabdl_u16 and
multiplying with 32-bit vmulq_u32.

**Why.** Because (a - b)^2 == |a - b|^2, computing absolute differences directly
eliminates redundant vmovl widening instructions and reduces register pressure.
For 8-bit operands, vabdl_u8 computes |a - b| and widens results to 16-bit in a
single instruction, replacing two vmovl_u8 widens and one vsubq_u16. For 16-bit
operands, 16-bit absolute differences fit within uint16_t without overflow;
using full-vector 128-bit vabdq_u16 avoids extracting input halves upfront, and
widening 16-bit vmull_u16 provides higher throughput and lower latency than
32-bit vmulq_u32.

## Examples

Bad:

```cpp
// Bad: Widening 8-bit operands with vmovl before subtraction increases register pressure.
const uint16x8_t a_u16 = vmovl_u8(vget_low_u8(a_u8));
const uint16x8_t b_u16 = vmovl_u8(vget_low_u8(b_u8));
uint16x8_t dist_8 = vmulq_u16(vsubq_u16(a_u16, b_u16), vsubq_u16(a_u16, b_u16));

// Bad: Widening 16-bit differences upfront requires 32-bit vector multiplies.
uint32x4_t dist_lo = vabdl_u16(vget_low_u16(a_reg), vget_low_u16(b_reg));
uint32x4_t dist_hi = vabdl_u16(vget_high_u16(a_reg), vget_high_u16(b_reg));
dist_lo = vmulq_u32(dist_lo, dist_lo);
dist_hi = vmulq_u32(dist_hi, dist_hi);
```

Good:

```cpp
// Good: Widening absolute difference directly on 8-bit inputs.
uint16x8_t dist_8 = vabdl_u8(vget_low_u8(a_u8), vget_low_u8(b_u8));
dist_8 = vmulq_u16(dist_8, dist_8);

// Good: Full-vector 16-bit vabdq_u16 followed by widening vmull_u16.
uint16x8_t dist_16 = vabdq_u16(a_reg, b_reg);
uint32x4_t dist_lo = vmull_u16(vget_low_u16(dist_16), vget_low_u16(dist_16));
uint32x4_t dist_hi = vmull_u16(vget_high_u16(dist_16), vget_high_u16(dist_16));
```
