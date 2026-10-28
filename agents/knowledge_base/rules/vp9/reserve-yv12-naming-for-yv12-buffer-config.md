---
name: libvpx/vp9/reserve-yv12-naming-for-yv12-buffer-config
description: When naming variables for non-YV12_BUFFER_CONFIG types such as struct buf_2d, name them after stored fields like backup_pre rather than yv12
rubric: readability
severity: nit
---

**Rule.** Do not use type-specific names or abbreviations such as `yv12`
(conventionally reserved for `YV12_BUFFER_CONFIG` structures) when declaring
variables of other types (such as `struct buf_2d` or plane pointers). Instead,
name temporary or backup variables after the specific struct field, plane, or
content they store (such as `backup_pre` or `saved_pre`).

**Why.** In libvpx, `yv12` universally indicates a full `YV12_BUFFER_CONFIG`
frame buffer. Using `yv12` for individual 2D plane buffers (`struct buf_2d`) or
field backups is misleading and obscures the underlying data structure being
manipulated.

## Examples

Bad:

```cpp
struct buf_2d backup_yv12[MAX_MB_PLANE] = { { 0, 0 } };
for (int i = 0; i < MAX_MB_PLANE; i++) backup_yv12[i] = xd->plane[i].pre[0];
```

Good:

```cpp
struct buf_2d backup_pre[MAX_MB_PLANE] = { { 0, 0 } };
for (int i = 0; i < MAX_MB_PLANE; i++) backup_pre[i] = xd->plane[i].pre[0];
```
