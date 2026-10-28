---
name: libvpx/use-arm-neon-vreinterpret-for-vector-casts
description: When reinterpreting NEON vector types or loading differently-typed buffers, use vreinterpret intrinsics rather than C-style vector or pointer casts
rubric: readability
severity: nit
---

**Rule.** Use ARM NEON reinterpret intrinsics (`vreinterpret_<dst>_<src>` or
`vreinterpretq_<dst>_<src>`) rather than C-style vector casts or buffer pointer
casts when converting between NEON vector types of the same bit width. When
loading data from memory into a differently-typed vector, load using the
intrinsic matching the buffer's declared pointer type (e.g., `vld1_u16` or
`vld1q_u16` for `uint16_t*`) and convert with `vreinterpret*`/`vreinterpretq_*`
instead of casting the pointer argument (such as `(const int16_t *)`).

**Why.** C-style vector casts (such as `(int16x8_t)vec`) rely on non-standard
compiler extensions that are not portable across all C compilers and toolchains.
Similarly, casting buffer pointers at load time bypasses pointer type checking
and obscures signedness or element width conversions. The standard ACLE
`vreinterpret` and `vreinterpretq` intrinsics provide explicit, portable vector
type conversions that compile to zero-cost register renames while preserving
type safety.

## Examples

Bad:

```cpp
// Bad: C-style cast between vector types is non-portable
uint16x8_t cmp = vceqq_s16(in[0], nonzero_bias_a);
int16x8_t mask = (int16x8_t)cmp;

// Bad: Casting buffer pointers directly in load intrinsics
const int16x4_t d0 = vld1_s16((const int16_t *)dst_u16);
const int16x8_t q0 = vld1q_s16((const int16_t *)src_u16);
```

Good:

```cpp
// Good: Use ACLE vreinterpret intrinsic for vector type conversions
uint16x8_t cmp = vceqq_s16(in[0], nonzero_bias_a);
int16x8_t mask = vreinterpretq_s16_u16(cmp);

// Good: Load with matching pointer type and reinterpret vector register
const int16x4_t d0 = vreinterpret_s16_u16(vld1_u16(dst_u16));
const int16x8_t q0 = vreinterpretq_s16_u16(vld1q_u16(src_u16));
```
