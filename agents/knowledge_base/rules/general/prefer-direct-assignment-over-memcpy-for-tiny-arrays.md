---
name: libvpx/prefer-direct-assignment-over-memcpy-for-tiny-arrays
description: When copying tiny fixed-size arrays with 1–2 elements, use direct element-by-element assignment instead of invoking memcpy with sizeof
rubric: readability
severity: nit
---

**Rule.** When copying tiny fixed-size arrays (such as 1–2 elements) or
individual scalar values, use direct element-by-element assignment instead of
calling memcpy(). Reserve memcpy() for larger buffers or structures where scalar
assignment is impractical.

**Why.** Direct element-by-element assignment is simpler, more readable, avoids
memcpy boilerplate, and prevents subtle bugs where sizeof accidentally evaluates
the size of a decayed pointer.

## Examples

Bad:

```cpp
// Anti-pattern: using memcpy with sizeof on a tiny 2-element array
memcpy(best_flag_preduv_computed, flag_preduv_computed,
       sizeof(best_flag_preduv_computed));
```

Good:

```cpp
// Preferred: direct element assignment
best_flag_preduv_computed[0] = flag_preduv_computed[0];
best_flag_preduv_computed[1] = flag_preduv_computed[1];
```
