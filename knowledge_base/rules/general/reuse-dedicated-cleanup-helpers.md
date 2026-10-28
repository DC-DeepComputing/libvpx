---
name: libvpx/reuse-dedicated-cleanup-helpers
description: When releasing dynamic structures or handling allocation failures across cleanup, error, and teardown paths, invoke dedicated deallocation helpers rather than duplicating manual free loops
rubric: design
severity: should_fix
---

**Rule.** When releasing memory for structs containing nested buffers, dynamic
arrays, or multi-level allocations—across teardown, error handling, reset, or
allocation failure paths—invoke dedicated cleanup and deallocation helpers (such
as vpx_free_* or free_*) rather than open-coding manual freeing loops or writing
redundant static cleanup functions. On allocation failure or error return paths,
ensure all previously allocated buffers are freed before returning an error
code, leveraging zero-initialized memory (calloc) so standard deallocators can
safely clean up partially constructed structures.

**Why.** Duplicating manual free loops or redundant static helpers creates
maintenance hazards when struct layouts change, increasing the risk of memory
leaks or double frees. Reusing centralized deallocation helpers guarantees
consistent resource reclamation across normal teardown and error paths,
preventing memory leaks when allocations fail midway.

## Examples

Bad:

```cpp
// Anti-pattern 1: Leaking previously allocated memory on error return
if (frame_stats->block_stats_list == NULL) return VPX_CODEC_MEM_ERROR;

// Anti-pattern 2: Redundant static cleanup helper duplicating existing deallocator
static void free_allocated_tpl_memory(VpxTplGopStats *tpl_gop_stats, int num_frame) {
  for (int f = 0; f < num_frame; f++) {
    free(tpl_gop_stats->frame_stats_list[f].block_stats_list);
  }
  free(tpl_gop_stats->frame_stats_list);
}

// Anti-pattern 3: Open-coding manual deallocation loop in teardown or reset path
for (frame = 0; frame < cpi->tpl_gop_stats.size; ++frame) {
  vpx_free(cpi->tpl_gop_stats.frame_stats_list[frame].block_stats_list);
}
vpx_free(cpi->tpl_gop_stats.frame_stats_list);
cpi->tpl_gop_stats.frame_stats_list = NULL;
```

Good:

```cpp
// Clean up all allocated memory on error return using dedicated helper
if (frame_stats->block_stats_list == NULL) {
  vpx_free_tpl_gop_stats(tpl_gop_stats);
  return VPX_CODEC_MEM_ERROR;
}

// Reuse dedicated cleanup helper in teardown and reset paths
free_tpl_frame_stats_list(&cpi->tpl_gop_stats);
```
