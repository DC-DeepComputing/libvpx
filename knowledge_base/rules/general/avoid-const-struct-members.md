---
name: libvpx/avoid-const-struct-members
description: When defining struct fields or function pointers, avoid const-qualifying members directly and enforce immutability via const struct instances instead
rubric: readability
severity: nit
---

**Rule.** Do not declare struct fields, member variables, or function pointer
members as top-level const in struct definitions. Enforce immutability at the
struct instance level (such as const StructType * or const StructType &)
instead, reserving const on pointer members for the pointed-to data (e.g., const
char *name).

**Why.** Declaring top-level const fields or function pointers inside a struct
definition prevents struct assignment, copying, and reuse, and is redundant when
instances are passed and accessed by const pointer or reference.

## Examples

Bad:

```cpp
typedef struct VpxInterface {
  const char *const name;
  const uint32_t fourcc;
  vpx_codec_iface_t *(*const codec_interface)(void);
} VpxInterface;
```

Good:

```cpp
typedef struct VpxInterface {
  const char *name;
  uint32_t fourcc;
  vpx_codec_iface_t *(*codec_interface)(void);
} VpxInterface;
```
