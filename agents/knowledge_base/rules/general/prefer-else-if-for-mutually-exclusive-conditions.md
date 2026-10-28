---
name: libvpx/prefer-else-if-for-mutually-exclusive-conditions
description: When testing mutually exclusive conditions, discrete values, or enum constants of a variable, use else-if chains or switch statements instead of consecutive independent if blocks
rubric: readability
severity: nit
---

**Rule.** Use `else if` or a `switch` statement rather than consecutive
independent `if` statements when checking mutually exclusive conditions,
discrete values, states, or enum constants of the same variable (unless an
earlier branch explicitly returns or terminates control flow). For discrete enum
constants or mode sets, prefer a `switch` statement with explicit cases and a
`default:` branch containing an assertion (such as `assert(0)`) or error
handling for invalid values.

**Why.** Sequential independent `if` statements obscure mutual exclusivity,
imply multiple conditions could match simultaneously, and perform redundant
evaluations after a match has occurred. They can also introduce bugs if an
earlier branch mutates the variable being tested. Chaining with `else if` or
using `switch` makes branching intent explicit, stops redundant checks once a
condition succeeds, and enables compiler checks or assertions (`assert(0)`) for
unhandled cases.

## Examples

Bad:

```cpp
// Bad: Independent if blocks for mutually exclusive values or enums
if (ref_frame == GOLDEN_FRAME) {
  cpi->gld_fb_idx = cpi->lst_fb_idx;
}
if (ref_frame == ALTREF_FRAME) {
  cpi->alt_fb_idx = cpi->lst_fb_idx;
}

if (left_16 == cols) {
  // Process left 16 columns
}
if (right_16 == cols) {
  // Process right 16 columns
}
```

Good:

```cpp
// Good: Chained with else-if or handled via switch with default assertion
if (ref_frame == GOLDEN_FRAME) {
  cpi->gld_fb_idx = cpi->lst_fb_idx;
} else if (ref_frame == ALTREF_FRAME) {
  cpi->alt_fb_idx = cpi->lst_fb_idx;
}

switch (cols) {
  case left_16:
    // Process left 16 columns
    break;
  case right_16:
    // Process right 16 columns
    break;
  default:
    assert(0);
    break;
}
```
