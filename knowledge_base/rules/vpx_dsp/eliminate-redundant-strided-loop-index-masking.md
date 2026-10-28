---
name: libvpx/vpx_dsp/eliminate-redundant-strided-loop-index-masking
description: When iterating loops with a stride of N, omit redundant bitwise masking or modulo operations on loop indices that always evaluate to zero
rubric: readability
severity: nit
---

**Rule.** Do not keep redundant bitwise masking or modulo expressions (such as
col & (N - 1) or col % N) on loop indices that advance in strides of N where the
expression is guaranteed to evaluate to zero. When vectorizing or unrolling
loops, simplify residual scalar index calculations by removing zero-valued
offsets.

**Why.** When a loop index advances in multiples of N, bitwise masks and modulo
operations for powers of N always evaluate to zero. Leaving these residual
expressions from scalar code clutters loop arithmetic and obscures active buffer
indexing.

## Examples

Bad:

```cpp
for (col = 0; col < cols; col += 8) {
  // Redundant (col & 7) always evaluates to 0 since col advances by 8
  output = filter_pixels_rv(sum, s,
                            vld1q_s16(vpx_rv + (row & 127) + (col & 7)));
}
```

Good:

```cpp
for (col = 0; col < cols; col += 8) {
  // Omit redundant zero offset on strided loop index
  output = filter_pixels_rv(sum, s, vld1q_s16(vpx_rv + (row & 127)));
}
```
