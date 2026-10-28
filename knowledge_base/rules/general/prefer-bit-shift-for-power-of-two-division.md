---
name: libvpx/prefer-bit-shift-for-power-of-two-division
description: When dividing non-negative integers, block dimensions, strides, or constants by powers of two, prefer bitwise right-shifts over division; never right-shift negative expressions
rubric: correctness
severity: should_fix
---

**Rule.** When halving or dividing known non-negative integer expressions, block
dimensions, strides, loop trip counts, or constants by powers of two (e.g., 2,
4, 8, 16), prefer bitwise right-shift operations (e.g., >> 1, >> 3) over
division operators (/ 2, / 8). However, never apply right-shifts to signed
negative expressions or variables (e.g., -LIMIT >> 1); use explicit division (/
2) or check magnitudes with abs(val) <= (LIMIT >> 1) for symmetric range bounds.

**Why.** In libvpx codec and DSP arithmetic, bit-shifting is the standard
convention for fixed-point math and power-of-two division on non-negative
values, avoiding division fixup overhead emitted by compilers for signed
integers. For negative signed integers, arithmetic right shift rounds towards
negative infinity rather than truncating towards zero like integer division, and
unary negation before shift (-LIMIT >> 1) evaluates the shift on a negative
operand, causing asymmetric bounds or off-by-one errors.

## Examples

Bad:

```cpp
// Integer division on non-negative block dimensions and constants
for (j = 0; j < (size / 8); ++j) {
  const int half = size / 2;
  process_block(src + j * 8, half);
}

// Right-shifting negative expressions for symmetric bounds checks
if (candidate->mv[0].as_mv.row < INT16_MAX >> 1 &&
    candidate->mv[0].as_mv.row > -INT16_MAX >> 1) {
  base_mv->as_mv.row = candidate->mv[0].as_mv.row * 2;
}
```

Good:

```cpp
// Bitwise right-shift on non-negative block dimensions and constants
for (j = 0; j < (size >> 3); ++j) {
  const int half = size >> 1;
  process_block(src + (j << 3), half);
}

// Positive upper bound with abs() for symmetric magnitude validation
if (abs(candidate->mv[0].as_mv.row) <= INT16_MAX >> 1) {
  base_mv->as_mv.row = candidate->mv[0].as_mv.row * 2;
}
```
