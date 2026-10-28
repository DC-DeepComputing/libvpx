---
name: libvpx/vp9/trim-tpl-gop-stats-in-forward-order
description: When trimming trailing lookahead frames from GOP statistics (VpxTplGopStats), copy leading frame statistics in forward chronological order rather than reverse-indexing from the tail
rubric: correctness
severity: should_fix
---

**Rule.** When resizing or trimming trailing lookahead frames from
VpxTplGopStats, retain the leading GOP frames by copying elements from
frame_stats_list in forward chronological order ([0 ... new_size - 1]). Do not
index backward from the tail (size - i - 1), which reverses frame order and
discards leading GOP statistics.

**Why.** VpxTplGopStats::frame_stats_list stores frame statistics in forward
chronological order with lookahead frames appended at the tail. Indexing
backward copies the trailing lookahead frames in reverse order instead of
preserving the initial GOP frames, corrupting TPL model statistics for
downstream rate control.

## Examples

Bad:

```cpp
for (i = 0; i < new_size; i++) {
  VpxTplFrameStats *frame_stats =
      &tpl_gop_stats->frame_stats_list[tpl_gop_stats->size - i - 1];
  new_frame_stats[i] = *frame_stats;
}
```

Good:

```cpp
for (i = 0; i < new_size; i++) {
  VpxTplFrameStats *frame_stats = &tpl_gop_stats->frame_stats_list[i];
  new_frame_stats[i] = *frame_stats;
}
```
