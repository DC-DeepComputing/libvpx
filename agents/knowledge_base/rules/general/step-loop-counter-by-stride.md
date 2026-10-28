---
name: libvpx/step-loop-counter-by-stride
description: When a loop induction variable is exclusively used scaled by a constant stride, step the counter directly by that stride instead of multiplying.
rubric: readability
severity: nit
---

**Rule.** When a loop induction variable or counter is exclusively used scaled
by a constant factor or stride (such as in block-strided pointer arithmetic or
buffer offset calculations), increment the loop induction variable directly by
that step size (e.g., `for (i = 0; i < total; i += STEP)`) rather than
unit-stepping and scaling the index on each access in the loop body.

**Why.** Stepping the loop counter directly by the stride or block step size
simplifies pointer arithmetic and array indexing expressions (e.g., `ptr + i`
instead of `ptr + i * 8`), eliminates redundant constant multiplications or
shifts inside the loop body, and clearly aligns the loop bounds with the
processed element count or byte span.

## Examples

Bad:

```cpp
// Bad: unit-stepping loop counter requires scaling offset or pointer inside loop body
for (i = 0; i < 2; i++) {
  transpose_16bit_8x8(l + i * 8, out);
  transpose_16bit_8x8(r + i * 8, out + 8);
  idct16_8col(out);
}
```

Good:

```cpp
// Good: stepping by stride simplifies offset expressions and clarifies buffer span
for (i = 0; i < 16; i += 8) {
  transpose_16bit_8x8(l + i, out);
  transpose_16bit_8x8(r + i, out + 8);
  idct16_8col(out);
}
```
