---
name: libvpx/omit-redundant-extern-on-function-declarations
description: When declaring function prototypes in header files, omit redundant extern qualifiers since function declarations have external linkage by default.
rubric: readability
severity: nit
---

**Rule.** Do not use 'extern' on function declarations or prototypes in header
files. Function declarations at file or namespace scope have external linkage by
default in C and C++, making 'extern' redundant; reserve 'extern' for global
variable declarations (such as 'extern const') or 'extern "C"' linkage blocks.

**Why.** In C and C++, all function prototypes declared at file scope implicitly
have external linkage. Adding 'extern' adds redundant visual clutter without
altering linkage or compiler behavior.

## Examples

Bad:

```cpp
// Redundant 'extern' on function prototypes:
extern void idct16x16_256_add_half1d(const void *input, int16_t *output,
                                     void *dest, int stride, int highbd_flag);
extern int vp9_get_reference_mode(const VP9_COMMON *cm);
```

Good:

```cpp
// Omit redundant 'extern' on function prototypes:
void idct16x16_256_add_half1d(const void *input, int16_t *output,
                              void *dest, int stride, int highbd_flag);
int vp9_get_reference_mode(const VP9_COMMON *cm);
```
