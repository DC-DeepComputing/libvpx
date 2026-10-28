---
name: libvpx/return-directly-from-branches-to-eliminate-dead-stores
description: When conditional branches or switch cases compute the final return value, return directly from each branch instead of assigning to a temporary local variable.
rubric: readability
severity: nit
---

**Rule.** When every branch of a conditional statement or switch block computes
the final return value of a function, return the value directly from each branch
(using return expr;). Avoid declaring a temporary local variable with a dead
initial value (e.g., int result = 0;), assigning to it across branches, and
returning it at the end of the function.

**Why.** Returning directly eliminates unnecessary intermediate local variables,
avoids dead initial stores or dummy assignments, simplifies control flow, and
makes data flow immediately obvious to readers and static analysis tools.

## Examples

Bad:

```cpp
int filter_weight = 0;
if (i < block_height / 2) {
  if (j < block_width / 2) {
    filter_weight = blk_fw[0];
  } else {
    filter_weight = blk_fw[1];
  }
} else {
  if (j < block_width / 2) {
    filter_weight = blk_fw[2];
  } else {
    filter_weight = blk_fw[3];
  }
}
return filter_weight;
```

Good:

```cpp
if (i < block_height / 2) {
  if (j < block_width / 2) {
    return blk_fw[0];
  }
  return blk_fw[1];
}
if (j < block_width / 2) {
  return blk_fw[2];
}
return blk_fw[3];
```
