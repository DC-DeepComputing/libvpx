---
name: libvpx/hoist-identical-operations-from-conditional-branches
description: When multiple branches of a conditional (if-else or switch) execute identical leading or trailing statements, hoist the common operations outside the conditional
rubric: design
severity: should_fix
---

**Rule.** Extract common prefix or suffix operations that are identical across
all branches of a conditional (such as if-else or switch statements) outside of
the conditional block. Keep only the branch-dependent statements inside the
conditional branches.

**Why.** Duplicating identical statements inside every branch of a conditional
obscures which logic actually depends on the condition, inflates code size, and
risks subtle behavioral divergence if one branch is later modified without
updating the others.

## Examples

Bad:

```cpp
if (bd == 10) {
  transpose_s32_4x4(&c[0], &c[1], &c[2], &c[3]);
  highbd_iadst4(c);
  idct4x4_16_kernel_bd10(cospis, c);
} else {
  transpose_s32_4x4(&c[0], &c[1], &c[2], &c[3]);
  highbd_iadst4(c);
  idct4x4_16_kernel_bd12(cospis, c);
}
```

Good:

```cpp
transpose_s32_4x4(&c[0], &c[1], &c[2], &c[3]);
highbd_iadst4(c);
if (bd == 10) {
  idct4x4_16_kernel_bd10(cospis, c);
} else {
  idct4x4_16_kernel_bd12(cospis, c);
}
```
