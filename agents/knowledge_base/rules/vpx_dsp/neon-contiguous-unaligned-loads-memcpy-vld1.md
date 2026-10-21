---
name: libvpx/vpx_dsp/neon-contiguous-unaligned-loads-memcpy-vld1
rubric: safety
severity: should_fix
---

**Rule.** When implementing contiguous unaligned vector load helpers in ARM Neon
(such as 8-byte or 16-byte unaligned loads without stride), copy unaligned bytes
to a local buffer with memcpy() and load the vector using vld1_u8() or
vld1q_u8() rather than chaining scalar memcpy calls with vdup and vset_lane
intrinsics.

**Why.** Compilers (Clang, GCC, and MSVC) reliably recognize a memcpy into a
local buffer followed by vld1/vld1q and lower it directly to single unaligned
vector load instructions. In contrast, piecemeal scalar memcpy calls combined
with vdup and vset_lane intrinsics generate inefficient scalar loads and
scalar-to-vector lane inserts, increasing instruction count and hurting SIMD
throughput.

## Examples

Bad:

```cpp
static INLINE uint8x16_t load_unaligned_u8_16x1(const uint8_t *buf) {
  uint64_t a[2];
  uint64x2_t a_u64;
  memcpy(&a[0], buf + 0, 8);
  memcpy(&a[1], buf + 8, 8);
  a_u64 = vdupq_n_u64(0);
  a_u64 = vsetq_lane_u64(a[0], a_u64, 0);
  a_u64 = vsetq_lane_u64(a[1], a_u64, 1);
  return vreinterpretq_u8_u64(a_u64);
}
```

Good:

```cpp
static INLINE uint8x16_t load_unaligned_u8_16x1(const uint8_t *buf) {
  uint8_t a[16];
  memcpy(a, buf, 16);
  return vld1q_u8(a);
}
```
