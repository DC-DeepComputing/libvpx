---
name: libvpx/declare-immutable-tables-as-static-const
description: When defining immutable static lookup tables, arrays, or function dispatch tables, declare them as static const rather than non-const static
rubric: readability
severity: should_fix
---

**Rule.** Declare immutable static lookup tables, arrays, and function dispatch
tables as 'static const' rather than non-const 'static'. Ensure all tables
initialized at definition time and never mutated at runtime include the 'const'
qualifier on the declaration.

**Why.** Marking static lookup and dispatch tables as 'const' ensures they are
allocated in read-only data sections (.rodata), explicitly signals immutability
to readers and the compiler, and prevents accidental runtime mutation or
cross-thread data corruption.

## Examples

Bad:

```cpp
static blend_fn blend[9] = {
  blend_a64_vmask_w16n_sse4_1,
  vpx_blend_a64_vmask_c,
  vpx_blend_a64_vmask_c,
  NULL,
  blend_a64_vmask_w4_sse4_1,
  NULL,
  NULL,
  NULL,
  blend_a64_vmask_w8_sse4_1,
};
```

Good:

```cpp
static const blend_fn blend[9] = {
  blend_a64_vmask_w16n_sse4_1,
  vpx_blend_a64_vmask_c,
  vpx_blend_a64_vmask_c,
  NULL,
  blend_a64_vmask_w4_sse4_1,
  NULL,
  NULL,
  NULL,
  blend_a64_vmask_w8_sse4_1,
};
```
