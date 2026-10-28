---
name: libvpx/defer-struct-size-assignment-until-allocation-succeeds
description: When dynamically allocating memory for struct buffer or array fields, assign size and count members only after verifying the allocation succeeded
rubric: readability
severity: nit
---

**Rule.** When dynamically allocating memory for a struct or container field,
update associated size, count, or capacity members only after verifying that the
allocation succeeded. Avoid mutating size fields before allocation or relying on
rollback assignments in error-handling branches.

**Why.** Assigning size or count fields before memory allocation prematurely
modifies struct state and requires explicit rollback if the allocation fails.
Deferring metadata updates until after confirming allocation success keeps error
handling concise and prevents leaving structs in inconsistent states with
non-zero sizes but NULL pointers.

## Examples

Bad:

```cpp
tpl_gop_stats->size = frame_list_size;
tpl_gop_stats->frame_stats_list = (VpxTplFrameStats *)vpx_calloc(
    frame_list_size, sizeof(tpl_gop_stats->frame_stats_list[0]));
if (tpl_gop_stats->frame_stats_list == NULL) {
  tpl_gop_stats->size = 0;
  return VPX_CODEC_MEM_ERROR;
}
```

Good:

```cpp
tpl_gop_stats->frame_stats_list = (VpxTplFrameStats *)vpx_calloc(
    frame_list_size, sizeof(tpl_gop_stats->frame_stats_list[0]));
if (tpl_gop_stats->frame_stats_list == NULL) {
  return VPX_CODEC_MEM_ERROR;
}
tpl_gop_stats->size = frame_list_size;
```
