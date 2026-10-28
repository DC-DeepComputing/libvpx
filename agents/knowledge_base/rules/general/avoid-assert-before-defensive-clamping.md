---
name: libvpx/avoid-assert-before-defensive-clamping
description: When defensively clamping or handling edge cases at runtime with VPXMAX, VPXMIN, or clamp, do not assert the same condition; choose either assertion or runtime fallback.
rubric: correctness
severity: should_fix
---

**Rule.** Do not pair an `assert()` check with defensive runtime clamping (such
as `VPXMAX`, `VPXMIN`, or `clamp`) or fallback logic for the exact same
condition. Decide whether the condition represents an internal invariant
violation (assert and remove redundant fallback logic) or a permissible edge
case to handle safely at runtime (clamp or fallback without asserting).

**Why.** Asserting a condition that is immediately guarded or clamped at runtime
creates a contradiction. If the boundary value can occur during execution, debug
builds will crash on the assertion despite the runtime fallback; if the value is
an impossible state, the defensive runtime code is dead and redundant.

## Examples

Bad:

```cpp
assert(target_bits > 0);
target_bits = VPXMAX(target_bits, 1);
diff = abs(target_bits - actual_bits) * 1. / target_bits;
```

Good:

```cpp
/* When handling permissible runtime edge cases, clamp without asserting: */
target_bits = VPXMAX(target_bits, 1);
diff = abs(target_bits - actual_bits) * 1. / target_bits;

/* Or when enforcing a strict invariant, assert without redundant clamping: */
assert(target_bits > 0);
diff = abs(target_bits - actual_bits) * 1. / target_bits;
```
