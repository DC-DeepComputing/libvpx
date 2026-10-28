---
name: libvpx/avoid-redundant-nested-block-scopes
description: When structuring functions, loop bodies, or conditional branches, avoid redundant nested block braces (`{ ... }`) that add unnecessary indentation without variable-scoping or RAII purpose
rubric: readability
severity: nit
---

**Rule.** Do not introduce redundant inner block braces (`{ ... }`) inside
function bodies, loop bodies, or conditional branches when the enclosing scope
already bounds variable lifetimes. Keep variable declarations and statements at
the existing outer block indentation level rather than creating unnecessary
nested scopes without lifetime, shadowing, or RAII purpose.

**Why.** An extra compound statement enclosing sequential code, function bodies,
or loop iterations adds visual clutter and extra indentation levels without
providing any semantic or lifetime benefit. Declaring variables directly in the
enclosing block keeps the code flat and concise.

## Examples

Bad:

```cpp
// Bad: Redundant inner braces enclosing function statements
void reset_fb_idx_unused_slot(VP9_COMP *const cpi) {
  {
    static const int flag_list[4] = { 0, VP9_LAST_FLAG, VP9_GOLD_FLAG,
                                      VP9_ALT_FLAG };
    for (int i = 0; i < 4; ++i) { /* ... */ }
  }
}

// Bad: Redundant nested block inside a loop body
while (index < n_coeffs) {
  {
    __m128i qcoeff0, qcoeff1;
    // Process coefficients...
  }
  index += 16;
}
```

Good:

```cpp
// Good: Statements declared directly at function scope
void reset_fb_idx_unused_slot(VP9_COMP *const cpi) {
  static const int flag_list[4] = { 0, VP9_LAST_FLAG, VP9_GOLD_FLAG,
                                    VP9_ALT_FLAG };
  for (int i = 0; i < 4; ++i) { /* ... */ }
}

// Good: Variables declared directly within loop body
while (index < n_coeffs) {
  __m128i qcoeff0, qcoeff1;
  // Process coefficients...
  index += 16;
}
```
