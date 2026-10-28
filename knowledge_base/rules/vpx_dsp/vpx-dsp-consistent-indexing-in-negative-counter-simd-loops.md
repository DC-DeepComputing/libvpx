---
name: libvpx/vpx_dsp/vpx-dsp-consistent-indexing-in-negative-counter-simd-loops
description: When porting or refactoring SIMD loops between negative-counter and forward-index idioms, ensure pointer bases, offset variables, and loop termination conditions are consistently converted
rubric: correctness
severity: should_fix
---

**Rule.** When adapting, refactoring, or porting SIMD loops that use
negative-index counter idioms (where pointers are advanced to the end and offset
with a negative counter incremented towards zero), do not mix forward-index
variables with negative loop termination conditions. Ensure that pointer base
addresses, offset indexing expressions, counter increment steps, and loop exit
conditions (such as while (count < 0) vs. while (index < count)) are converted
consistently.

**Why.** Mixing negative-counter loop termination conditions (such as n_coeffs <
0) with forward-indexed pointer offsets (such as ptr + index) causes loops to
terminate prematurely without processing elements or access invalid memory when
pointer bases and offset variables are not aligned to the same indexing scheme.

## Examples

Bad:

```cpp
// Negative loop termination mixed with forward pointer indexing
if (skip_block) {
  do {
    store_tran_low(zero, dqcoeff_ptr + index);
    store_tran_low(zero, dqcoeff_ptr + index + 8);
    store_tran_low(zero, qcoeff_ptr + index);
    store_tran_low(zero, qcoeff_ptr + index + 8);
    n_coeffs += 8 * 2;
  } while (n_coeffs < 0);
  *eob_ptr = 0;
  return;
}
```

Good:

```cpp
// Consistently using negative-offset indexing and negative loop termination
if (skip_block) {
  do {
    store_tran_low(zero, dqcoeff_ptr + n_coeffs);
    store_tran_low(zero, dqcoeff_ptr + n_coeffs + 8);
    store_tran_low(zero, qcoeff_ptr + n_coeffs);
    store_tran_low(zero, qcoeff_ptr + n_coeffs + 8);
    n_coeffs += 8 * 2;
  } while (n_coeffs < 0);
  *eob_ptr = 0;
  return;
}
```
