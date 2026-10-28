---
name: libvpx/declare-file-local-constants-as-static-const
description: Declare file-scope constants, lookup tables, and mask arrays in C source files as static const to ensure internal linkage.
rubric: readability
severity: should_fix
---

**Rule.** In C source files, declare file-scope constants, lookup tables, and
mask arrays as `static const` to give them internal linkage, unless they are
part of an explicitly exported public API. When a constant is only used within a
single helper function, scope it locally inside that function (as `static const`
or `const`) rather than declaring it at file scope. Do not define file-local
constants as plain `const` at file scope.

**Why.** In C, unlike C++, global `const` variables and arrays have external
linkage by default unless explicitly qualified with `static`. Defining
non-static `const` tables across source files pollutes the global symbol table,
risks duplicate symbol name collisions at link time, and prevents compiler
optimizations enabled by internal visibility.

## Examples

Bad:

```cpp
// Bad: Non-static const variables and lookup tables in .c files have external linkage
const uint8_t mc_filt_mask_arr[16 * 3] = {
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8,
  /* ... */
};

const size_t kOffsetSize = 100;
const int kLookupTable[4] = { 0, 1, 2, 3 };

static int frame_alloc_size(VP9_COMP *const cpi) {
  return cpi->initial_width + kOffsetSize;
}
```

Good:

```cpp
// Good: Declared static const for internal linkage
static const uint8_t mc_filt_mask_arr[16 * 3] = {
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8,
  /* ... */
};

static const int kLookupTable[4] = { 0, 1, 2, 3 };

// Or scoped locally within the helper function that uses it:
static int frame_alloc_size(VP9_COMP *const cpi) {
  static const size_t kOffsetSize = 100;
  return cpi->initial_width + kOffsetSize;
}
```
