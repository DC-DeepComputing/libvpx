---
name: libvpx/do-not-silently-return-on-invariant-violation
description: When encountering violated invariants, unexpected preconditions, or missing stats (such as NULL frame_stats), report errors via vpx_internal_error() or assert() rather than silently returning or breaking
rubric: correctness
severity: should_fix
---

**Rule.** Do not silently return, break, or bail out when an invariant,
precondition, or expected state is violated (such as when fps_get_frame_stats()
returns NULL unexpectedly in encoder analysis loops or GOP stats sizing
mismatches occur). Signal the error through the appropriate error-reporting
mechanism (such as vpx_internal_error() or returning a vpx_codec_err_t failure
status) or trigger an assertion (assert()) so that unexpected conditions and
upstream indexing bugs surface immediately.

**Why.** Silently returning, breaking, or bailing out on unexpected conditions
or missing frame statistics masks upstream bugs (such as GOP sizing mismatches,
invalid buffer counts, or indexing errors) and leaves rate-control metrics or
codec state partially computed. Explicitly reporting errors via
vpx_internal_error() or asserting ensures contract violations abort cleanly and
pinpoint the failure site immediately.

## Examples

Bad:

```cpp
// Bad: silently returning on unexpected GOP stats size
void trim_tpl_stats(struct vpx_internal_error_info *error_info,
                    VpxTplGopStats *tpl_gop_stats, int extra_frames) {
  if (tpl_gop_stats->size <= extra_frames) return;
  // ...
}

// Bad: silently breaking on unexpected NULL frame stats in GOP analysis
for (j = 0; j < gop_length; ++j) {
  const FIRSTPASS_STATS *frame_stats =
      fps_get_frame_stats(first_pass_info, show_idx + j);
  if (frame_stats == NULL) {
    break;
  }
  gf_group_err += calc_norm_frame_score(oxcf, frame_info, frame_stats, ...);
}
```

Good:

```cpp
// Good: report internal error on unexpected GOP stats size
void trim_tpl_stats(struct vpx_internal_error_info *error_info,
                    VpxTplGopStats *tpl_gop_stats, int extra_frames) {
  if (tpl_gop_stats->size <= extra_frames) {
    vpx_internal_error(
        error_info, VPX_CODEC_ERROR,
        "The number of frames in VpxTplGopStats is fewer than expected.");
  }
  // ...
}

// Good: report internal error on unexpected NULL frame stats before breaking
for (j = 0; j < gop_length; ++j) {
  const FIRSTPASS_STATS *frame_stats =
      fps_get_frame_stats(first_pass_info, show_idx + j);
  if (frame_stats == NULL) {
    vpx_internal_error(&cm->error, VPX_CODEC_ERROR,
                       "In define_gf_group(), frame_stats is NULL when "
                       "calculating gf_group_err.");
    break;
  }
  gf_group_err += calc_norm_frame_score(oxcf, frame_info, frame_stats, ...);
}
```
