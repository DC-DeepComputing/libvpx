---
name: libvpx/avoid-redundant-null-checks-on-element-addresses
description: Do not assert or check addresses of struct members or array elements for nullness; test member values or dereferenced elements directly
rubric: correctness
severity: should_fix
---

**Rule.** Do not assert or check the address of a struct member, field, flag, or
array element (such as assert(&(error)->setjmp) or if (prev_mi) where prev_mi =
&array[idx]), as taking the address of a member or subobject within an allocated
struct or buffer always yields a non-null pointer. When verifying preconditions
or state, test the member's value directly (such as assert((error)->setjmp)).
When validating stored elements within an array or buffer of pointers, check the
dereferenced element (*ptr or ptr[i]) rather than the container element address.

**Why.** Taking the address of a struct member (&(ptr)->member) or an array
element (&array[idx]) evaluates to a non-null address whenever the containing
struct or array resides in memory. Asserting or null-checking such addresses
creates tautologies or dead code that always pass, misleadingly imply the
pointer could be null, and mask bugs where uninitialized state, missing
preconditions, or null stored elements were intended to be checked.

## Examples

Bad:

```cpp
// Bug: taking the address of a struct member tautologically passes.
assert(&(error)->setjmp);

// Bug: prev_mi points to &cm->prev_mi_grid_visible[idx]; &array[idx] is never NULL.
MODE_INFO **prev_mi = &cm->prev_mi_grid_visible[idx_str];
if (prev_mi) {
  for (idy = 0; idy < mi_height; ++idy) {
    for (idx = 0; idx < mi_width; ++idx) {
      MODE_INFO *mi = prev_mi[idy * cm->mi_stride + idx];
      // ...
    }
  }
}
```

Good:

```cpp
// Correct: tests the struct member's actual value directly.
assert((error)->setjmp);

// Correct: omit tautological address check; check dereferenced elements if needed.
MODE_INFO **prev_mi = &cm->prev_mi_grid_visible[idx_str];
for (idy = 0; idy < mi_height; ++idy) {
  for (idx = 0; idx < mi_width; ++idx) {
    MODE_INFO *mi = prev_mi[idy * cm->mi_stride + idx];
    BLOCK_SIZE bs = mi ? mi->sb_type : bsize;
    // ...
  }
}
```
