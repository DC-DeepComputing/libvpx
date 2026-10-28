---
name: libvpx/remove-unused-static-helpers-after-refactoring
description: When refactoring code or eliminating call sites, remove unused file-static helper functions and orphaned code to prevent dead code accumulation
rubric: readability
severity: nit
---

**Rule.** When refactoring code or removing call sites, delete unused
file-static helper functions, anonymous-namespace helpers, and dead code whose
last caller was eliminated. Do not leave behind orphaned helper functions that
are no longer referenced anywhere in the translation unit.

**Why.** Unused static helper functions and dead code clutter source files,
confuse maintainers about active logic paths, and increase ongoing maintenance
overhead. Deleting orphaned helpers keeps translation units concise and prevents
dead-code bitrot.

## Examples

Bad:

```cpp
// Refactoring removed the only caller of invert_vpx_rational()
static INLINE vpx_rational_t invert_vpx_rational(vpx_rational_t v) {
  vpx_rational_t inverse_v;
  inverse_v.num = v.den;
  inverse_v.den = v.num;
  return inverse_v;
}

void process_frame_rate(vpx_rational_t fps) {
  // New implementation computes frame duration directly
  ...
}
```

Good:

```cpp
// Removed unused invert_vpx_rational() static helper after refactoring

void process_frame_rate(vpx_rational_t fps) {
  // New implementation computes frame duration directly
  ...
}
```
