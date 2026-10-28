---
name: libvpx/accurate-todo-component-descriptions
description: When adding or modifying TODO comments, ensure the description accurately names the specific local component rather than copy-pasting descriptions from other subsystems
rubric: readability
severity: nit
---

**Rule.** Ensure TODO comments, fixme notes, and inline tracking comments
accurately describe the specific component, algorithm, or feature being modified
in the current file. Do not leave copy-pasted comments that reference unrelated
subsystems or algorithms from other modules.

**Why.** Copy-pasting temporary workarounds or tracking notes across files
leaves misleading comments that reference wrong subsystems, confusing future
maintainers and obscuring the actual follow-up task.

## Examples

Bad:

```cpp
// In vp9_aq_cyclicrefresh.c:
// TODO(jianj): Look into issue of noise estimation with high bitdepth.
if (cm->bit_depth > 8 || cm->frame_type == KEY_FRAME) {
  cr->apply_cyclic_refresh = 0;
  return;
}
```

Good:

```cpp
// In vp9_aq_cyclicrefresh.c:
// TODO(jianj): Look into issue of cyclic refresh with high bitdepth.
if (cm->bit_depth > 8 || cm->frame_type == KEY_FRAME) {
  cr->apply_cyclic_refresh = 0;
  return;
}
```
