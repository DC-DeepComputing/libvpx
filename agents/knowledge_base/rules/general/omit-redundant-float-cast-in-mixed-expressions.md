---
name: libvpx/omit-redundant-float-cast-in-mixed-expressions
description: When combining integer and floating-point operands in arithmetic or min/max macros, omit redundant integer or floating-point casts because implicit conversions promote integers automatically.
rubric: readability
severity: nit
---

**Rule.** Do not add redundant explicit casts—such as `(double)`, `(float)`,
`(int64_t)`, or `static_cast<int64_t>`—to integer variables, constants, or
literals when performing mixed arithmetic or using standard macros/functions
like `VPXMIN`, `VPXMAX`, `fmin`, or `fmax` with floating-point operands. Rely on
standard C/C++ implicit arithmetic conversions, which automatically promote
integer operands to the floating-point type.

**Why.** Under standard C and C++ usual arithmetic conversion rules, binary
operators and comparison/min-max expressions combining integer and
floating-point operands automatically promote the integer to the floating-point
type. Adding explicit casts (such as casting an integer operand to `(int64_t)`
prior to floating-point division, or casting integer constants/literals like
`INT_MAX` or `100` to `(double)` or `(float)`) is completely redundant, alters
no semantics or compiler diagnostics, and adds unnecessary visual clutter.

## Examples

Bad:

```cpp
// Redundant (int64_t) cast before division with double framerate:
lrc->avg_frame_bandwidth =
    (int)VPXMIN((int64_t)lc->target_bandwidth / lc->framerate, INT_MAX);

// Redundant (double) cast on INT_MAX in mixed float/int VPXMIN expression:
lrc->bits_off_target += (int)VPXMIN(
    round(lc->target_bandwidth / lc->framerate), (double)INT_MAX);

// Redundant (float) cast on integer literal in mixed arithmetic:
const float scale = raw_weight * (float)100 / total_weight;
```

Good:

```cpp
// Implicit promotion converts lc->target_bandwidth (int) to double:
lrc->avg_frame_bandwidth =
    (int)VPXMIN(lc->target_bandwidth / lc->framerate, INT_MAX);

// Implicit promotion converts INT_MAX to double automatically:
lrc->bits_off_target +=
    (int)VPXMIN(round(lc->target_bandwidth / lc->framerate), INT_MAX);

// Integer literal 100 promotes to float automatically when multiplied by float:
const float scale = raw_weight * 100 / total_weight;
```
