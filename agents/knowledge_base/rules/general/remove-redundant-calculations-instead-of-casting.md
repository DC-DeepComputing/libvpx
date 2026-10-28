---
name: libvpx/remove-redundant-calculations-instead-of-casting
description: When resolving compiler or sanitizer integer overflow and truncation warnings, remove redundant calculations and dead expressions rather than masking them with explicit casts
rubric: readability
severity: should_fix
---

**Rule.** Do not add explicit type casts to silence compiler or sanitizer
integer truncation or overflow warnings without analyzing the underlying
arithmetic. If a calculation is redundant with a prior assignment, relies on
wrap-around to produce an already-known constant, or is dead code, eliminate the
calculation entirely instead of masking it with a cast.

**Why.** Explicitly casting an overflowing or truncated calculation masks dead
or copy-pasted logic and can introduce implementation-defined behavior or
unexpected wrap-around. Removing redundant calculations eliminates sanitizer
diagnostics cleanly while preventing dead code from confusing future
maintainers.

## Examples

Bad:

```cpp
*shift = 0;
/* use multiplication and constant shift by 16 */
*shift = (short)(1 << (16 - *shift));
```

Good:

```cpp
*shift = 0;
```
