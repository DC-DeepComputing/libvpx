---
name: libvpx/avoid-vpxmin-vpxmax-on-mixed-signedness
description: When using VPXMIN or VPXMAX comparison macros, avoid operands with differing signedness; use explicit conditional clamping or matching types instead
rubric: correctness
severity: should_fix
---

**Rule.** Do not pass operands of differing signedness (such as mixing signed
int and unsigned int) into ternary comparison macros like VPXMIN or VPXMAX.
Ensure operand types match or use explicit conditional if clamping statements
instead to prevent unintended unsigned integer promotions.

**Why.** VPXMIN and VPXMAX expand to ternary comparison expressions (((x) < (y))
? (x) : (y)). Evaluating relational operators on operands of differing
signedness triggers standard C implicit conversion of signed integers to
unsigned, corrupting comparisons when signed values are negative and causing
compiler sign-compare warnings.

## Examples

Bad:

```cpp
// width is unsigned int, cpi->initial_width is int
cm->width = VPXMIN(width, cpi->initial_width);
```

Good:

```cpp
// Use explicit conditional clamping or align operand types
cm->width = width;
if (cm->width > cpi->initial_width) {
  cm->width = cpi->initial_width;
}
```
