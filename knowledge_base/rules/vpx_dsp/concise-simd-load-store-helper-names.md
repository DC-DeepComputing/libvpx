---
name: libvpx/vpx_dsp/concise-simd-load-store-helper-names
description: When naming SIMD memory load and store helpers, use concise element type and vector width specifiers without redundantly spelling both source and destination types
rubric: readability
severity: should_fix
---

**Rule.** Name SIMD memory load and store helper functions concisely based on
the element type, vector width, or block dimensions being transferred (e.g.,
`load_unaligned_u8`, `store_unaligned_u8q`, `store_u8_8x4`), rather than
redundantly specifying both source and destination types (e.g.,
`store_unaligned_u8x16_to_u8q`). Reserve `_to_<type>` suffixes strictly for
functions that perform explicit element type conversions (such as
`store_s16q_to_tran_low`).

**Why.** Specifying both the source register type and destination memory type in
helper names when transferring same-type data introduces unnecessary verbosity
and diverges from libvpx's established SIMD naming conventions. Concise names
indicating element type and width keep vector code readable while explicit
conversion suffixes remain distinct.

## Examples

Bad:

```cpp
// Redundantly specifying both source vector type and destination buffer type
static INLINE void store_unaligned_u8x16_to_u8q(uint8_t *dst,
                                                ptrdiff_t stride,
                                                const uint8x16_t v) {
  store_unaligned_u8(dst, stride, vget_low_u8(v));
  store_unaligned_u8(dst + stride, stride, vget_high_u8(v));
}

store_unaligned_u8x16_to_u8q(dst_ptr, dst_pitch, vcombine_u8(f0, f1));
```

Good:

```cpp
// Concisely named by element type and vector width
static INLINE void store_unaligned_u8q(uint8_t *dst, ptrdiff_t stride,
                                       const uint8x16_t v) {
  store_unaligned_u8(dst, stride, vget_low_u8(v));
  store_unaligned_u8(dst + stride, stride, vget_high_u8(v));
}

store_unaligned_u8q(dst_ptr, dst_pitch, vcombine_u8(f0, f1));
```
