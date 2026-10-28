---
name: libvpx/match-declaration-parameter-names-to-definitions
description: When declaring functions in header files, ensure parameter names match their corresponding source definitions and the struct fields they populate.
rubric: readability
severity: nit
---

**Rule.** When declaring functions in header files, keep parameter names
consistent with their corresponding source file definitions and any struct
fields they initialize or populate. Avoid using different or outdated parameter
names in declarations that diverge from the implementation.

**Why.** Mismatched parameter names between header declarations and source
definitions create confusion for callers, mask API intent, and increase the risk
of passing arguments in the wrong order or misunderstanding the function
contract.

## Examples

Bad:

```cpp
// In header (.h):
void vp9_alloc_motion_field_info(MotionFieldInfo *motion_field_info,
                                 int gop_size, int mi_rows, int mi_cols);

// In source (.c):
void vp9_alloc_motion_field_info(MotionFieldInfo *motion_field_info,
                                 int frame_num, int mi_rows, int mi_cols) {
  motion_field_info->frame_num = frame_num;
  ...
}
```

Good:

```cpp
// In header (.h):
void vp9_alloc_motion_field_info(MotionFieldInfo *motion_field_info,
                                 int frame_num, int mi_rows, int mi_cols);

// In source (.c):
void vp9_alloc_motion_field_info(MotionFieldInfo *motion_field_info,
                                 int frame_num, int mi_rows, int mi_cols) {
  motion_field_info->frame_num = frame_num;
  ...
}
```
