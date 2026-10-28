---
name: libvpx/avoid-complex-expressions-in-macro-arguments
description: When passing arguments to function-like macros that evaluate parameters multiple times (such as VPXMIN or VPXMAX), extract complex expressions into local variables first
rubric: safety
severity: nit
---

**Rule.** Do not pass complex, expensive, or multi-step arithmetic expressions
directly as arguments to function-like macros (such as VPXMIN, VPXMAX, or MAXF)
that evaluate their parameters multiple times. Extract the computed
subexpression into a named local const variable before invoking the macro.

**Why.** Function-like macros that expand arguments across ternary operators
(e.g., (((x) < (y)) ? (x) : (y))) evaluate non-trivial expressions multiple
times unless factored out. Passing complex expressions directly duplicates
computation across macro expansions, relies unnecessarily on compiler
subexpression elimination, and obscures readability.

## Examples

Bad:

```cpp
// Complex arithmetic expression passed directly to multi-evaluating macro:
rc->sb64_target_rate = (int)VPXMIN(
    (((int64_t)rc->this_frame_target * 64 * 64) / (cm->width * cm->height)),
    INT_MAX);
```

Good:

```cpp
// Extract complex arithmetic into a local variable before the macro call:
const int64_t sb64_target_rate =
    ((int64_t)rc->this_frame_target * 64 * 64) / (cm->width * cm->height);
rc->sb64_target_rate = (int)VPXMIN(sb64_target_rate, INT_MAX);
```
