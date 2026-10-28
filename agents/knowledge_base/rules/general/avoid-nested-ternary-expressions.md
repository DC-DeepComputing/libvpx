---
name: libvpx/avoid-nested-ternary-expressions
description: When writing multi-condition logic, avoid nested or chained ternary operators (? :) in favor of explicit if/else branches or early returns
rubric: readability
severity: should_fix
---

**Rule.** Avoid nested or chained ternary conditional expressions (`? ... : ...
?`). Use explicit `if` / `else if` branching statements or early `return`s
instead to keep control flow clear and readable.

**Why.** Nested and chained ternary operators obscure control flow, operator
precedence, and branch conditions, making multi-way logic difficult to read,
modify, and debug compared to structured `if`/`else` blocks.

## Examples

Bad:

```cpp
static INLINE int get_extra_arfs(int interval, int arf_pending) {
  if (arf_pending) {
    return interval >= MIN_INTERVAL * (MAX_ARFS + 1)
               ? MAX_ARFS
               : interval >= MIN_INTERVAL * MAX_ARFS ? MAX_ARFS - 1 : 0;
  }
  return 0;
}
```

Good:

```cpp
static INLINE int get_extra_arfs(int interval, int arf_pending) {
  if (arf_pending) {
    if (interval >= MIN_INTERVAL * (MAX_ARFS + 1)) return MAX_ARFS;
    if (interval >= MIN_INTERVAL * MAX_ARFS) return MAX_ARFS - 1;
  }
  return 0;
}
```
