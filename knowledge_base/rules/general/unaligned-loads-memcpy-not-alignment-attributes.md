---
name: libvpx/unaligned-loads-memcpy-not-alignment-attributes
description: When implementing unaligned memory loads or stores, use portable memcpy or central load_unaligned helpers instead of compiler-specific alignment attributes like __attribute__((aligned(1)))
rubric: correctness
severity: should_fix
---

**Rule.** Do not use compiler-specific alignment attributes (such as
__attribute__((aligned(1))) or __attribute__((packed))) or pointer casts for
unaligned memory accesses. Use standard fixed-size memcpy calls or central
unaligned load/store helper functions (such as load_unaligned_u8,
load_unaligned_u32, or mem_get_*) instead.

**Why.** Alignment attributes on typedefs or pointer types are
GCC/Clang-specific extensions that fail to compile or optimize properly on MSVC
and other toolchains. Modern compilers optimize small constant-size memcpy calls
into efficient single-instruction unaligned loads or stores safely across all
supported platforms without undefined behavior.

## Examples

Bad:

```cpp
// Using GCC-specific attribute to force unaligned load:
typedef uint32_t __attribute__((aligned(1))) uint32_unaligned;

static INLINE uint8x16_t load_32x4_from_u8(const uint8_t *a) {
  uint32x4_t a_u32 = vdupq_n_u32(0);
  a_u32 = vld1q_lane_u32((const uint32_unaligned *)a, a_u32, 0);
  return vreinterpretq_u8_u32(a_u32);
}
```

Good:

```cpp
// Portable unaligned load using memcpy or central helper:
static INLINE uint8x8_t load_unaligned_u8_4x1(const uint8_t *buf) {
  uint32_t a;
  uint32x2_t a_u32;
  memcpy(&a, buf, sizeof(a));
  a_u32 = vdup_n_u32(0);
  a_u32 = vset_lane_u32(a, a_u32, 0);
  return vreinterpret_u8_u32(a_u32);
}
```
