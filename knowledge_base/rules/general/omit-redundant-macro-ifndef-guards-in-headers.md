---
name: libvpx/omit-redundant-macro-ifndef-guards-in-headers
description: 'When defining macros in header files with standard include guards, omit redundant #ifndef guards around macro definitions unless external overriding is explicitly intended'
rubric: readability
severity: should_fix
---

**Rule.** Do not wrap macro definitions in redundant #ifndef guards inside
header files that already have standard file-level #include guards, unless
external overriding is explicitly intended.

**Why.** Standard file-level include guards already prevent multiple macro
definitions within a translation unit. Wrapping internal macros in individual
#ifndef blocks adds unnecessary boilerplate and falsely suggests that the macro
is intended to be overridden externally.

## Examples

Bad:

```cpp
#ifndef VPX_SWAP
#define VPX_SWAP(type, a, b) \
  do {                       \
    type temp__ = (a);       \
    (a) = (b);               \
    (b) = temp__;            \
  } while (0)
#endif
```

Good:

```cpp
#define VPX_SWAP(type, a, b) \
  do {                       \
    type temp__ = (a);       \
    (a) = (b);               \
    (b) = temp__;            \
  } while (0)
```
