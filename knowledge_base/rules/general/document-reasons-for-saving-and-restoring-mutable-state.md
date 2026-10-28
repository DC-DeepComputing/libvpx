---
name: libvpx/document-reasons-for-saving-and-restoring-mutable-state
description: When saving and restoring mutable context, buffers, or struct fields across a temporary operation, document why the backup and restore is necessary.
rubric: readability
severity: nit
---

**Rule.** When backing up and restoring mutable context, buffer pointers, or
struct fields across a temporary operation (such as a motion search or scaling
override), accompany the backup/restore logic with an explanatory comment
detailing why saving and restoring that state is necessary and what behavior
would break without it.

**Why.** Temporary state overrides and bug workarounds across context structs
(like MACROBLOCKD or VP9_COMP) are non-obvious to future readers. Without inline
documentation explaining what downstream operations depend on the original
state, the backup and restore logic risks being inadvertently removed or altered
during refactoring.

## Examples

Bad:

```cpp
if (scaled_ref_frame) {
  yv12 = scaled_ref_frame;
  for (int i = 0; i < MAX_MB_PLANE; i++) backup_pre[i] = xd->plane[i].pre[0];
}
```

Good:

```cpp
if (scaled_ref_frame) {
  yv12 = scaled_ref_frame;
  // The reference buffer pointer needs to be saved and restored after the
  // search; otherwise, reference frame scaling corrupts plane pointers.
  for (int i = 0; i < MAX_MB_PLANE; i++) backup_pre[i] = xd->plane[i].pre[0];
}
```
