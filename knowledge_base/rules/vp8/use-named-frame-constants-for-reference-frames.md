---
name: libvpx/vp8/use-named-frame-constants-for-reference-frames
description: When checking whether a reference frame is intra or inter, compare against named constants like INTRA_FRAME or LAST_FRAME instead of literal 0
rubric: readability
severity: should_fix
---

**Rule.** When checking whether a reference frame index represents an inter
frame or intra frame (such as `ref_frame` or `second_ref_frame`), compare
against named constants such as `INTRA_FRAME` or `LAST_FRAME` (e.g., `ref_frame
> INTRA_FRAME`, `ref_frame >= LAST_FRAME`, or `ref_frame == INTRA_FRAME`) rather
than bare integer literals like `0`. When inspecting a `MODE_INFO` structure
directly, prefer standard predicate helpers such as `is_inter_block(mi)` or
`has_second_ref(mi)`.

**Why.** In VP8 and VP9, `INTRA_FRAME` is defined as 0, while inter reference
frames begin at `LAST_FRAME == 1`. Comparing against named frame constants or
predicate helpers avoids magic numbers, makes intra/inter intent explicit, and
maintains consistency across the codebase.

## Examples

Bad:

```cpp
// Magic literal 0 obscures whether the index represents an inter frame.
int ref_scaled = ref_frame != 0 &&
                 vp9_is_scaled(&cm->frame_refs[ref_frame - 1].sf);

if (mi->ref_frame[0] != 0) {
  // ...
}
```

Good:

```cpp
// Using named frame constants documents intent clearly.
int ref_scaled = ref_frame > INTRA_FRAME &&
                 vp9_is_scaled(&cm->frame_refs[ref_frame - 1].sf);

// Use standard helper when checking mode info.
if (is_inter_block(mi)) {
  // ...
}
```
