---
name: libvpx/assert-fixed-step-simd-preconditions
description: When implementing or refactoring fixed-step SIMD or DSP loops without tail handling, assert both minimum size and modulo preconditions on block dimensions
rubric: correctness
severity: should_fix
---

**Rule.** When implementing or refactoring SIMD or DSP functions that process
data in fixed-size blocks (e.g., 8, 16, or 32 elements per iteration) without
tail handling, assert both minimum size (assert(size >= N);) and
modulo/alignment preconditions (assert((size % N) == 0);) before entering the
loop.

**Why.** Vectorized loops that decrement a counter by fixed chunk sizes and
check for zero termination will miss zero if the input length is smaller than
the step size or not an exact multiple. This causes integer underflow, infinite
loops, or buffer overruns in unverified builds.

## Examples

Bad:

```cpp
int64_t vp9_block_error_fp_neon(const tran_low_t *coeff,
                                const tran_low_t *dqcoeff, int block_size) {
  int64_t error = 0;
  do {
    // Process 16 elements per iteration...
    coeff += 16;
    dqcoeff += 16;
    block_size -= 16;
  } while (block_size != 0);
  return error;
}
```

Good:

```cpp
int64_t vp9_block_error_fp_neon(const tran_low_t *coeff,
                                const tran_low_t *dqcoeff, int block_size) {
  int64_t error = 0;
  assert(block_size >= 16);
  assert((block_size % 16) == 0);

  do {
    // Process 16 elements per iteration...
    coeff += 16;
    dqcoeff += 16;
    block_size -= 16;
  } while (block_size != 0);
  return error;
}
```
