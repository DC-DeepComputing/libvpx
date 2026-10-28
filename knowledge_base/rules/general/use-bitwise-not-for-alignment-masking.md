---
name: libvpx/use-bitwise-not-for-alignment-masking
description: When masking low bits or aligning integer dimensions, use bitwise NOT on the alignment mask (e.g., & ~15) instead of fixed-width hex literals like 0xfffffff0
rubric: readability
severity: nit
---

**Rule.** When masking low bits or aligning dimensions to a power-of-two
boundary, use bitwise NOT on the alignment offset (e.g., `& ~15` or `&
~(alignment - 1)`). Do not use hardcoded fixed-width hexadecimal literals (such
as `& 0xfffffff0` or `& 0xfffffff8`).

**Why.** Fixed-width hex literals with the high bit set evaluate to unsigned
types in C, causing signedness mismatch warnings and redundant casts when used
with signed integer variables. Bitwise NOT preserves operand signedness and
sign-extends portably across 32-bit and 64-bit integer widths.

## Examples

Bad:

```cpp
if ((int)((cm->Width + 15) & 0xfffffff0) != cm->yv12_fb[cm->lst_fb_idx].y_width) {
  return -1;
}
```

Good:

```cpp
if (((cm->Width + 15) & ~15) != cm->yv12_fb[cm->lst_fb_idx].y_width) {
  return -1;
}
```
