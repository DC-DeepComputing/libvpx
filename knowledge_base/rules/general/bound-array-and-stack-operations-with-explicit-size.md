---
name: libvpx/bound-array-and-stack-operations-with-explicit-size
description: When manipulating fixed-size arrays or stacks, pass explicit size or count parameters instead of relying on unbounded sentinel values like -1 to terminate iteration.
rubric: safety
severity: should_fix
---

**Rule.** When manipulating fixed-size arrays, buffers, or stack data
structures, pass explicit size or count parameters to bound iteration. Do not
rely on unbounded sentinel values (such as -1 or NULL) without length checks to
terminate loops or pop operations.

**Why.** Sentinel values can be omitted, overwritten, or corrupted by invalid
inputs, leading to unbounded loop iteration and out-of-bounds memory reads or
writes. Explicit size bounds guarantee safe and predictable loop termination.

## Examples

Bad:

```cpp
static INLINE int stack_pop(int *stack) {
  int idx = 1;
  int r = stack[0];
  while (stack[idx] != -1) {
    stack[idx - 1] = stack[idx];
    ++idx;
  }
  return r;
}
```

Good:

```cpp
static INLINE int stack_pop(int *stack, int stack_size) {
  int idx;
  int r = stack[0];
  for (idx = 1; idx < stack_size; ++idx) stack[idx - 1] = stack[idx];
  return r;
}
```
