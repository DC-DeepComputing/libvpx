---
name: libvpx/place-early-return-guards-before-state-mutations
description: When implementing functions with early-return or bypass conditions, evaluate guard checks before performing state mutations, buffer updates, or pointer swaps
rubric: correctness
severity: should_fix
---

**Rule.** Place early-return and bypass guard checks before any state mutations,
buffer updates, or pointer swaps in a function or update routine. Do not perform
partial state modifications or variable exchanges prior to a condition that
skips or aborts the operation, unless those mutations are strictly required for
the guard check itself or explicitly undone on early exit.

**Why.** Mutating state or swapping pointers before an early return leaves data
structures partially modified or desynchronized when the routine is bypassed,
leading to corrupted state or incorrect behavior in subsequent operations.

## Examples

Bad:

```cpp
static void update_frame_buffers(VP9_COMMON *cm) {
  // Anti-pattern: mutating pointers before checking bypass condition
  MODE_INFO *temp = cm->prev_mip;
  cm->prev_mip = cm->mip;
  cm->mip = temp;

  if (cm->show_existing_frame) return;

  cm->mi = cm->mip + cm->mi_stride + 1;
}
```

Good:

```cpp
static void update_frame_buffers(VP9_COMMON *cm) {
  // Check bypass condition before mutating state
  if (cm->show_existing_frame) return;

  MODE_INFO *temp = cm->prev_mip;
  cm->prev_mip = cm->mip;
  cm->mip = temp;

  cm->mi = cm->mip + cm->mi_stride + 1;
}
```
