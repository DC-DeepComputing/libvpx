---
name: libvpx/parenthesize-logical-or-when-combined-with-and
description: When combining logical OR (`||`) and logical AND (`&&`) in conditions or assertions, parenthesize OR subexpressions to prevent operator precedence bugs
rubric: correctness
severity: should_fix
---

**Rule.** When combining logical OR (`||`) and logical AND (`&&`) operators in
expressions, conditions, or assertions, explicitly parenthesize the logical OR
subexpressions (e.g., `(A || B) && C`). Do not rely on default operator
precedence when mixing disjunctions and conjunctions.

**Why.** In C and C++, logical AND (`&&`) has higher operator precedence than
logical OR (`||`). An unparenthesized expression `A || B && C` binds as `A || (B
&& C)`, bypassing condition `C` whenever `A` is true.

## Examples

Bad:

```cpp
// Evaluates as (size == 8) || ((size == 16) && (cols % 8 == 0))
assert(size == 8 || size == 16 && cols % 8 == 0);
```

Good:

```cpp
// Explicit parentheses ensure cols % 8 == 0 is checked for both sizes
assert((size == 8 || size == 16) && cols % 8 == 0);
```
