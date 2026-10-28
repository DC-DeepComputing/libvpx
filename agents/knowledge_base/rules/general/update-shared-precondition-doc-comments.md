---
name: libvpx/update-shared-precondition-doc-comments
description: When adding sibling helper functions sharing preconditions or invariants described in an existing shared comment block, update the comment to reference all applicable functions
rubric: readability
severity: nit
---

**Rule.** When adding or modifying helper functions that share invariants,
preconditions, or undefined-behavior caveats described in an existing shared
comment block, update the comment to explicitly reference all applicable
functions. Do not leave the comment mentioning only a subset of the functions
governed by that constraint.

**Why.** Shared doc comments and precondition notices that name only one
function mislead callers into assuming adjacent sibling functions do not have
the same constraint or undefined-behavior caveats, leading to subtle bugs or
misuse.

## Examples

Bad:

```cpp
// These versions of get_msb() are only valid when n != 0 because all
// of the optimized versions are undefined when n == 0:
static INLINE int get_lsb(unsigned int n) {
  assert(n != 0);
  return __builtin_ctz(n);
}

static INLINE int get_msb(unsigned int n) {
  assert(n != 0);
  return 31 ^ __builtin_clz(n);
}
```

Good:

```cpp
// These versions of get_lsb() and get_msb() are only valid when n != 0
// because all of the optimized versions are undefined when n == 0:
static INLINE int get_lsb(unsigned int n) {
  assert(n != 0);
  return __builtin_ctz(n);
}

static INLINE int get_msb(unsigned int n) {
  assert(n != 0);
  return 31 ^ __builtin_clz(n);
}
```
