---
name: libvpx/omit-redundant-enum-tag-in-typedef
description: When defining typedef'd enums in C, omit the redundant enum tag name and declare an anonymous enum like typedef enum { ... } TypeName;
rubric: readability
severity: nit
---

**Rule.** When defining a typedef'd enum in C, omit the redundant enum tag name
and declare an anonymous enum (e.g., typedef enum { ... } TypeName; instead of
typedef enum TypeName { ... } TypeName;). Reserve named tags for struct
definitions that require forward declarations or self-referential pointers.

**Why.** In C, typedef'd enums do not require tag names because enums cannot be
self-referential or forward-declared. Providing an enum tag name redundantly
populates the enum tag namespace and duplicates the type identifier without
providing any semantic or compiler benefit.

## Examples

Bad:

```cpp
typedef enum SKIN_DETECTION_BLOCK_SIZE {
  SKIN_8X8,
  SKIN_16X16
} SKIN_DETECTION_BLOCK_SIZE;
```

Good:

```cpp
typedef enum {
  SKIN_8X8,
  SKIN_16X16
} SKIN_DETECTION_BLOCK_SIZE;
```
