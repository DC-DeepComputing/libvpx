---
name: libvpx/vpx_dsp/break-neon-accumulator-dependency-chains
rubric: safety
severity: should_fix
---

**Rule.** In ARM NEON reduction loops and multi-accumulator SIMD routines, use
multiple independent accumulator vectors (such as an array of 2 to 4 partial sum
registers) to break instruction dependency chains across loop iterations. When
reducing multiple accumulator vectors into a final scalar sum, combine them into
a single vector using parallel vector additions (vaddq_* / vadd_*, such as
vaddq_s32, vaddq_u32, or vaddq_s16) rather than performing individual horizontal
reductions on each accumulator. Perform only a single horizontal reduction
(horizontal_add_*, horizontal_add_4d_*, or vaddvq_*) on the final combined
vector.

**Why.** Modern out-of-order ARM processors can issue multiple arithmetic
instructions per cycle with multi-cycle latency; accumulating into a single
register across iterations stalls the pipeline on read-after-write (RAW)
dependencies. Furthermore, horizontal vector reduction instructions (such as
ADDV / vaddvq_*) and pairwise additions have higher latency and lower throughput
than parallel lane additions (vaddq_*). Combining partial accumulators with
vector additions first maximizes throughput and minimizes costly cross-lane
operations.

## Examples

Bad:

```cpp
// Bad: Insufficient accumulators causing dependency stalls, and multiple horizontal adds
int32x4_t left = vdupq_n_s32(0);
int32x4_t right = vdupq_n_s32(0);
for (r = 0; r < 16; ++r) {
  left = vaddw_s16(left, vget_low_s16(a));
  left = vaddw_s16(left, vget_high_s16(a));
  right = vaddw_s16(right, vget_low_s16(b));
  right = vaddw_s16(right, vget_high_s16(b));
}
// Inefficient: individual horizontal reductions per accumulator
int sum = horizontal_add_int32x4(left) + horizontal_add_int32x4(right);
```

Good:

```cpp
// Good: Multiple independent accumulators combined via vector adds before single horizontal add
int32x4_t partial_sum[4] = { vdupq_n_s32(0), vdupq_n_s32(0),
                             vdupq_n_s32(0), vdupq_n_s32(0) };
for (r = 0; r < 16; ++r) {
  partial_sum[0] = vaddw_s16(partial_sum[0], vget_low_s16(a));
  partial_sum[1] = vaddw_s16(partial_sum[1], vget_high_s16(a));
  partial_sum[2] = vaddw_s16(partial_sum[2], vget_low_s16(b));
  partial_sum[3] = vaddw_s16(partial_sum[3], vget_high_s16(b));
}
// Combine accumulator vectors first with standard vector additions
partial_sum[0] = vaddq_s32(partial_sum[0], partial_sum[1]);
partial_sum[2] = vaddq_s32(partial_sum[2], partial_sum[3]);
partial_sum[0] = vaddq_s32(partial_sum[0], partial_sum[2]);
const int sum = horizontal_add_int32x4(partial_sum[0]);
```
