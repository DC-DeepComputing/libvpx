---
name: libvpx/vp9/use-invalid-mv-for-motion-vector-sentinels
description: When setting, checking, or resetting motion vector sentinels in VP9, use INVALID_MV on int_mv.as_int rather than buffer index sentinel INVALID_IDX
rubric: correctness
severity: should_fix
---

**Rule.** When initializing, checking, or invalidating motion vectors (int_mv,
MV) in VP9, use INVALID_MV (0x80008000) for int_mv.as_int comparisons and
assignments. Do not use INVALID_IDX (-1), which is reserved strictly for invalid
frame buffer indices.

**Why.** Comparing or assigning INVALID_IDX (-1) to int_mv.as_int causes
sentinel checks against INVALID_MV (0x80008000) to fail, mistakenly treating
invalid or uninitialized motion vectors as valid candidates during motion search
and rate-distortion optimization.

## Examples

Bad:

```cpp
int_mv nb_full_mvs[NB_MVS_NUM];
nb_full_mvs[i].as_int = INVALID_IDX;
```

Good:

```cpp
int_mv nb_full_mvs[NB_MVS_NUM];
nb_full_mvs[i].as_int = INVALID_MV;
```
