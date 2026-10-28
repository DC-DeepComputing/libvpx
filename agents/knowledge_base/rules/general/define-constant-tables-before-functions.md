---
name: libvpx/define-constant-tables-before-functions
description: When defining file-scope static constant or lookup tables in C source files, place table definitions before the functions that reference them
rubric: readability
severity: nit
---

**Rule.** Define file-scope static constant tables (such as lookup, permutation,
or filter coefficient arrays) before the helper functions that use or reference
them in the translation unit, rather than defining functions above the tables.

**Why.** Placing constant and lookup tables above the functions that use them
maintains a clean top-down declaration order, avoids forward declarations or
out-of-order references, and preserves a consistent file layout across the
codebase.

## Examples

Bad:

```cpp
static INLINE void transpose_concat_4x4(...) {
  // Uses 'dot_prod_tran_concat_tbl' defined below.
  ...
}

DECLARE_ALIGNED(16, static const uint8_t, dot_prod_tran_concat_tbl[32]) = {
  ...
};
```

Good:

```cpp
DECLARE_ALIGNED(16, static const uint8_t, dot_prod_tran_concat_tbl[32]) = {
  ...
};

static INLINE void transpose_concat_4x4(...) {
  // Uses 'dot_prod_tran_concat_tbl' defined above.
  ...
}
```
