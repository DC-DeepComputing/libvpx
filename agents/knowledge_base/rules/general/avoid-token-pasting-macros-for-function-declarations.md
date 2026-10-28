---
name: libvpx/avoid-token-pasting-macros-for-function-declarations
description: When declaring external or fallback function prototypes, write explicit function declarations instead of using token-pasting macros or typedef aliases
rubric: readability
severity: should_fix
---

**Rule.** Do not use token-pasting macros (with ##) or typedef aliases to
generate function declarations or prototypes. Write explicit function prototype
declarations with their full names and parameter signatures directly in the
source or header file.

**Why.** Generating function prototypes via token-pasting macros and typedef
wrappers adds unnecessary preprocessor abstraction, obscures symbol names and
signatures, and prevents developer tools and grep from finding function
declarations and definitions.

## Examples

Bad:

```cpp
typedef void HbdFilter1dFunc(const uint16_t *, ptrdiff_t, uint16_t *, ptrdiff_t,
                             uint32_t, const int16_t *, int);

#define HIGHBD_FUNC(width, dir, avg, opt) \
  vpx_highbd_filter_block1d##width##_##dir##_##avg##opt

HbdFilter1dFunc HIGHBD_FUNC(4, h8, , sse2);
HbdFilter1dFunc HIGHBD_FUNC(4, h2, , sse2);
```

Good:

```cpp
void vpx_highbd_filter_block1d4_h8_sse2(const uint16_t *, ptrdiff_t, uint16_t *,
                                        ptrdiff_t, uint32_t, const int16_t *,
                                        int);
void vpx_highbd_filter_block1d4_h2_sse2(const uint16_t *, ptrdiff_t, uint16_t *,
                                        ptrdiff_t, uint32_t, const int16_t *,
                                        int);
```
