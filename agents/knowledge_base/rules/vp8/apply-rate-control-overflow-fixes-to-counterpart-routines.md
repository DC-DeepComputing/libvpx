---
name: libvpx/vp8/apply-rate-control-overflow-fixes-to-counterpart-routines
description: When fixing integer overflow in rate-control calculations, apply 64-bit casts or shift bounds checks consistently across all counterpart routines and frame-type helpers
rubric: correctness
severity: should_fix
---

**Rule.** When fixing arithmetic or integer overflow in rate-control
calculations (such as casting multiplicands to int64_t before multiplying
avg_frame_bandwidth by percentage factors, or bounds-checking bit shifts like
BPER_MB_NORMBITS against INT_MAX), verify and apply the fix consistently across
all counterpart calculation routines and frame-type helpers (e.g.,
vp8_regulate_q/vp9_rc_regulate_q vs.
vp8_drop_encodedframe_overshoot/vp9_drop_encodedframe_overshoot, and intra vs.
inter/golden target clamping functions like vp9_rc_clamp_iframe_target_size and
vp9_rc_clamp_pframe_target_size). Do not leave duplicated formulas in sibling
rate-control routines vulnerable to arithmetic overflow or wrap-around.

**Why.** Rate-control formulas (such as scaling target frame bits by
BPER_MB_NORMBITS, dividing by macroblock count to determine target_bits_per_mb,
or multiplying avg_frame_bandwidth by rc_max_inter_bitrate_pct /
rc_max_intra_bitrate_pct) are duplicated across frame Q selection, overshoot
drop handlers, and frame-type target clamping helpers. Fixing 32-bit integer
overflow in only one routine leaves counterpart paths susceptible to integer
wrap-around and erratic rate-control decisions on high-bitrate or
high-resolution frames.

## Examples

Bad:

```cpp
// Variant 1: Fixed in regulate_q, but counterpart drop handler left unchecked
if (target_bits_per_frame > (INT_MAX >> BPER_MB_NORMBITS)) {
  int temp = target_bits_per_frame / cpi->common.MBs;
  target_bits_per_mb = (temp > (INT_MAX >> BPER_MB_NORMBITS))
                       ? INT_MAX
                       : (temp << BPER_MB_NORMBITS);
} else {
  target_bits_per_mb =
      (target_bits_per_frame << BPER_MB_NORMBITS) / cpi->common.MBs;
}
// BAD: Counterpart in drop_encodedframe_overshoot has unchecked shift
target_bits_per_mb = (target_size << BPER_MB_NORMBITS) / cpi->common.MBs;

// Variant 2: Fixed in intra clamping, but inter clamping has 32-bit overflow
int vp9_rc_clamp_pframe_target_size(const VP9_COMP *const cpi, int target) {
  const RATE_CONTROL *const rc = &cpi->rc;
  const VP9EncoderConfig *const oxcf = &cpi->oxcf;
  if (oxcf->rc_max_inter_bitrate_pct) {
    const int max_rate =
        rc->avg_frame_bandwidth * oxcf->rc_max_inter_bitrate_pct / 100;
    target = VPXMIN(target, max_rate);
  }
  return target;
}
```

Good:

```cpp
// Variant 1: Counterpart routines use identical overflow protection
if (target_size > (INT_MAX >> BPER_MB_NORMBITS)) {
  int temp = target_size / cpi->common.MBs;
  target_bits_per_mb = (temp > (INT_MAX >> BPER_MB_NORMBITS))
                       ? INT_MAX
                       : (temp << BPER_MB_NORMBITS);
} else {
  target_bits_per_mb = (target_size << BPER_MB_NORMBITS) / cpi->common.MBs;
}

// Variant 2: Both intra and inter clamping helpers cast to int64_t
int vp9_rc_clamp_pframe_target_size(const VP9_COMP *const cpi, int target) {
  const RATE_CONTROL *const rc = &cpi->rc;
  const VP9EncoderConfig *const oxcf = &cpi->oxcf;
  if (oxcf->rc_max_inter_bitrate_pct) {
    const int64_t max_rate =
        (int64_t)rc->avg_frame_bandwidth * oxcf->rc_max_inter_bitrate_pct / 100;
    target = (int)VPXMIN((int64_t)target, max_rate);
  }
  return target;
}
```
