---
name: libvpx/vp9/override-gop-params-with-external-rate-control
description: When external rate control is active, ensure external GOP decision parameters like gop_coding_frames and use_alt_ref override the encoder's internal GOP calculations
rubric: correctness
severity: should_fix
---

**Rule.** When external rate control is enabled and a valid GOP decision is
received (vpx_rc_gop_decision_t / gop_decision_ready), ensure external GOP
structure parameters (such as gop_coding_frames and use_alt_ref) override the
encoder's internal heuristic GOP calculations (such as
get_gop_coding_frame_num). Do not proceed with rate control or frame structure
configuration using only internally computed GOP parameters when external rate
control decisions are active.

**Why.** External rate control models specify custom GOP lengths and alternate
reference frame usage via vpx_rc_gop_decision_t. If internally computed values
for gop_coding_frames and use_alt_ref are not overridden by the external
decision, the encoder will bypass or corrupt the external rate controller's
intended GOP structure and frame allocation.

## Examples

Bad:

```cpp
gop_coding_frames = get_gop_coding_frame_num(
    &use_alt_ref, frame_info, twopass, rc, gf_start_show_idx,
    &active_gf_interval, gop_intra_factor, cpi->oxcf.lag_in_frames,
    &end_of_sequence);
use_alt_ref &= allow_alt_ref;

// gop_coding_frames and use_alt_ref are not overridden with gop_decision
rc->constrained_gf_group = (gop_coding_frames >= rc->frames_to_key) ? 1 : 0;
```

Good:

```cpp
gop_coding_frames = get_gop_coding_frame_num(
    &use_alt_ref, frame_info, twopass, rc, gf_start_show_idx,
    &active_gf_interval, gop_intra_factor, cpi->oxcf.lag_in_frames,
    &end_of_sequence);
use_alt_ref &= allow_alt_ref;

if (gop_decision_ready) {
  gop_coding_frames = gop_decision.gop_coding_frames;
  use_alt_ref = gop_decision.use_alt_ref;
}
rc->constrained_gf_group = (gop_coding_frames >= rc->frames_to_key) ? 1 : 0;
```
