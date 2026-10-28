---
name: libvpx/avoid-pointer-indirection-in-swap-macros
description: When implementing value-swapping or exchange macros, swap values directly using a single local temporary variable instead of creating pointer aliases or taking addresses.
rubric: design
severity: should_fix
---

**Rule.** When defining value-swapping or exchange macros (such as `VPX_SWAP`),
swap values directly using a single local temporary variable instead of taking
addresses of macro arguments or creating intermediate pointer aliases.

**Why.** Taking addresses of macro arguments and creating intermediate pointer
variables adds needless indirection, dereferencing overhead, and code
complexity. Furthermore, it prevents swapping expressions that cannot have their
address taken (such as register variables). Direct assignment through a single
temporary variable of the given type is cleaner, more efficient, and works
safely on all assignable lvalues.

## Examples

Bad:

```cpp
#define VPX_SWAP(type, a, b) \
  do {                       \
    type *a__ = &(a);        \
    type *b__ = &(b);        \
    type temp__ = *b__;      \
    *b__ = *a__;             \
    *a__ = temp__;           \
  } while (0)
```

Good:

```cpp
#define VPX_SWAP(type, a, b) \
  do {                       \
    type temp__ = (b);       \
    (b) = (a);               \
    (a) = temp__;            \
  } while (0)
```
