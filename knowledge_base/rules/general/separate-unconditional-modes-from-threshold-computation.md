---
name: libvpx/separate-unconditional-modes-from-threshold-computation
description: When handling unconditional execution modes or external overrides, decouple them upfront from complex heuristic analysis loops and conditional threshold computations
rubric: design
severity: should_fix
---

**Rule.** Do not embed unconditional execution mode checks or external
configuration override logic inside complex heuristic analysis loops or nested
threshold computation blocks. When execution is determined unconditionally by a
mode, preset, or external configuration, evaluate or return the parameters
directly upfront via a separate branch or helper, and execute heuristic
statistics accumulation and threshold calculations only on conditional paths.

**Why.** Embedding unconditional mode bypasses or external configuration checks
inside heuristic analysis loops and threshold calculations complicates control
flow, muddies analysis heuristics with ad-hoc breakout logic, and computes
threshold metrics or statistics unnecessarily on unconditional paths.

## Examples

Bad:

```cpp
// Variant 1: Unconditional mode embedded in threshold calculation
if (method == MESH || (sf->exhaustive_searches_thresh < INT_MAX &&
                       !cpi->rc.is_src_frame_alt_ref)) {
  int64_t exhaustive_thr = sf->exhaustive_searches_thresh;
  exhaustive_thr >>= 8 - (b_width_log2_lookup[bsize] + b_height_log2_lookup[bsize]);
  if (var > exhaustive_thr || method == MESH) {
    var_ex = full_pixel_exhaustive(cpi, x, tmp_mv, error_per_bit, cost_list, fn_ptr, ref_mv, &tmp_mv_ex);
  }
}

// Variant 2: External override embedded in heuristic statistics loop
while (gop_coding_frames < rc->static_scene_max_gf_interval) {
  ++gop_coding_frames;
  // ... accumulate statistics ...
  if (encode_command != NULL && encode_command->use_external_arf) {
    if (encode_command->external_arf_indexes[gf_start_show_idx + gop_coding_frames] == 1) break;
    continue;
  }
}
```

Good:

```cpp
// Variant 1: Separate unconditional path from threshold computation
int run_exhaustive_search = 0;
if (method == NSTEP) {
  if (sf->exhaustive_searches_thresh < INT_MAX && !cpi->rc.is_src_frame_alt_ref) {
    const int64_t exhaustive_thr = get_exhaustive_threshold(sf->exhaustive_searches_thresh, bsize);
    if (var > exhaustive_thr) run_exhaustive_search = 1;
  }
} else if (method == MESH) {
  run_exhaustive_search = 1;
}
if (run_exhaustive_search) {
  var_ex = full_pixel_exhaustive(cpi, x, tmp_mv, error_per_bit, cost_list, fn_ptr, ref_mv, &tmp_mv_ex);
}

// Variant 2: Evaluate external override upfront before heuristic loop
if (encode_command != NULL && encode_command->use_external_arf) {
  *use_alt_ref = 1;
  return get_external_gop_coding_frames(encode_command, gf_start_show_idx);
}
while (gop_coding_frames < rc->static_scene_max_gf_interval) {
  ++gop_coding_frames;
  // ... accumulate statistics and evaluate heuristic breakouts ...
}
```
