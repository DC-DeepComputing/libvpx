---
name: libvpx/avoid-redundant-checks-on-compile-time-constants
description: Omit redundant runtime if conditions and assert() checks on compile-time constants, macros, or sizeof expressions that evaluate statically to true
rubric: readability
severity: nit
---

**Rule.** Do not include redundant runtime condition checks (in if or while
statements) or runtime assert() checks against compile-time #define constants,
preprocessor macros, literal constants, or sizeof expressions when the condition
is statically determined. When a constant value is fixed at compile time, rely
on its defined value directly in runtime logic rather than guarding it with
tautological comparisons. If a constant constraint or invariant must be verified
across configurations, check it at compile time (such as with VPX_STATIC_ASSERT)
or omit the redundant check.

**Why.** Checking compile-time constant definitions in runtime if conditions
adds visual clutter, creates misleading dead branches, and obscures active
runtime logic. Similarly, runtime assertions on statically fixed constants and
macros are tautological, execute redundant checks at runtime, and create false
expectations that the verified constant could dynamically change value.

## Examples

Bad:

```cpp
// Bad: Redundant runtime condition check on compile-time constant MAX_EXT_ARFS (defined as 2)
if (arf_pending && MAX_EXT_ARFS > 0) {
  return interval >= MIN_EXT_ARF_INTERVAL * (MAX_EXT_ARFS + 1);
}

// Bad: Runtime assertion on statically defined macro (MAXQ is defined as 255)
assert(MAXQ <= 255);
memset(lc->last_coded_q_map, MAXQ, map_size);
```

Good:

```cpp
// Good: Rely directly on the compile-time constant in runtime logic
if (arf_pending) {
  return interval >= MIN_EXT_ARF_INTERVAL * (MAX_EXT_ARFS + 1);
}

// Good: Omit tautological runtime assertion on statically defined macro
memset(lc->last_coded_q_map, MAXQ, map_size);
```
