---
name: libvpx/vp9/update-counterpart-encoder-mode-decision-routines
description: When fixing uninitialized variables or logic bugs in encoder mode-decision routines, update counterpart RD, non-RD, and sub8x8 mode-picking functions consistently
rubric: correctness
severity: should_fix
---

**Rule.** When fixing uninitialized variables, buffer initializations, or
mode-evaluation logic bugs in encoder mode-decision routines (such as
vp9_rdopt.c), inspect and update counterpart mode-picking files and sibling
routines (such as vp9_pickmode.c and _sub8x8 / block-level variants). Ensure
identical variable initializations, buffer resets, and search constraints are
applied across both full rate-distortion optimization (RD) and fast non-RD
decision paths.

**Why.** Mode-selection logic in the encoder is split across parallel
implementations: full RD search (vp9_rdopt.c), fast non-RD search
(vp9_pickmode.c), and sub-block routines (_sub8x8). Fixing an uninitialized
buffer or logic bug in only one path leaves counterpart mode-picking routines
vulnerable to undefined behavior, valgrind errors, and inconsistent mode
decisions.

## Examples

Bad:

```cpp
// In vp9/encoder/vp9_rdopt.c: Initialized in RD mode search...
struct buf_2d yv12_mb[4][MAX_MB_PLANE] = { 0 };

// In vp9/encoder/vp9_pickmode.c: ...but left uninitialized in counterpart non-RD / sub8x8 routine:
struct buf_2d yv12_mb[4][MAX_MB_PLANE];
```

Good:

```cpp
// In vp9/encoder/vp9_rdopt.c:
struct buf_2d yv12_mb[4][MAX_MB_PLANE] = { 0 };

// In vp9/encoder/vp9_pickmode.c: Initialized across counterpart mode-picking routines:
struct buf_2d yv12_mb[4][MAX_MB_PLANE] = { 0 };
```
