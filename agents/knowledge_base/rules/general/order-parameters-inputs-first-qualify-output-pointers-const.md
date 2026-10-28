---
name: libvpx/order-parameters-inputs-first-qualify-output-pointers-const
description: When declaring or defining functions with output parameters, order inputs before outputs and qualify non-reassigned output pointer parameters with '*const'
rubric: readability
severity: should_fix
---

**Rule.** Order function parameters with all input parameters first followed by
output parameters. When passing output parameters as pointers, qualify
non-reassigned pointers as `Type *const param` (or `const Type *const param` for
immutable pointers to read-only buffers) so the pointer itself cannot be
accidentally reassigned within the function body.

**Why.** Placing inputs before outputs maintains a consistent and predictable
calling convention across the codebase. Declaring pointer parameters as `*const`
enforces const-correctness on the pointer itself, preventing unintended pointer
arithmetic or reassignment bugs while still allowing modification of the
pointed-to output buffer.

## Examples

Bad:

```cpp
// Anti-pattern: Output pointers placed before inputs and missing *const
static void accumulate_sum_sumsq(int32x4_t *sum, int32x4_t *sumsq,
                                 const int16x4_t x, const int32x4_t xy) {
  *sum = vaddq_s32(*sum, vmovl_s16(x));
  *sumsq = vaddq_s32(*sumsq, xy);
}
```

Good:

```cpp
// Inputs first, outputs last, and output pointers qualified with *const
static void accumulate_sum_sumsq(const int16x4_t x, const int32x4_t xy,
                                 int32x4_t *const sum,
                                 int32x4_t *const sumsq) {
  *sum = vaddq_s32(*sum, vmovl_s16(x));
  *sumsq = vaddq_s32(*sumsq, xy);
}
```
