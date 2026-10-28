---
name: libvpx/document-reason-for-specialized-function-variants
description: When implementing a specialized function variant instead of sharing common helper code, document why unification is not possible in an explanatory comment
rubric: readability
severity: nit
---

**Rule.** When implementing a specialized variant of a function instead of
sharing code with an existing generic or smaller-variant implementation,
document why code sharing is not possible (such as intermediate overflow risks,
different dynamic ranges, scaling factors, or instruction set constraints) in an
accompanying comment above the definition.

**Why.** Without explanatory comments, specialized implementations appear as
unnecessary code duplication, leading future maintainers or reviewers to
question the design or attempt misguided code consolidation that risks
re-introducing overflow, precision, or architectural bugs.

## Examples

Bad:

```cpp
static INLINE int16x8_t dequantize_coeff_32(int16x8_t qcoeff,
                                            int16x8_t dequant) {
  int16x8_t dqcoeff;
  int32x4_t dqcoeffe = vec_mule(qcoeff, dequant);
  int32x4_t dqcoeffo = vec_mulo(qcoeff, dequant);
  // ...
  return dqcoeff;
}
```

Good:

```cpp
// DeQuantization function used for 32x32 blocks. Quantized coeff of 32x32
// blocks are twice as big as for other block sizes. As such, using
// vec_mladd results in overflow.
static INLINE int16x8_t dequantize_coeff_32(int16x8_t qcoeff,
                                            int16x8_t dequant) {
  int16x8_t dqcoeff;
  int32x4_t dqcoeffe = vec_mule(qcoeff, dequant);
  int32x4_t dqcoeffo = vec_mulo(qcoeff, dequant);
  // ...
  return dqcoeff;
}
```
