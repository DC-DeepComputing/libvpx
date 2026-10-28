---
name: libvpx/document-hardcoded-limits-and-bounds-checks
description: When using hardcoded numerical limits, capacity caps, or heuristic threshold cutoffs in conditions, document their derivation or make them configurable named parameters
rubric: readability
severity: nit
---

**Rule.** When applying hardcoded numerical limits, capacity caps, or heuristic
threshold constants (such as layer bounds, debug dump limits, or rate control
convergence tolerances), add an explanatory comment clarifying the derivation
and rationale behind the value. Where appropriate, prefer exposing experimental
or tuning thresholds as configurable options or defining descriptive named
constants.

**Why.** Hardcoded numeric literals and ad-hoc thresholds look arbitrary without
context, making it difficult for maintainers to know whether a value is a
protocol constraint, a hardware limit, or an empirical heuristic. Explaining the
derivation or allowing configuration eases tuning, testing, and future
maintenance.

## Examples

Bad:

```cpp
// Example 1: Undocumented debug frame dump cap
#ifdef OUTPUT_YUV_SVC_SRC
  if (is_one_pass_cbr_svc(cpi) && cpi->svc.spatial_layer_id < 3) {
    vpx_write_yuv_frame(yuv_svc_src[cpi->svc.spatial_layer_id], cpi->Source);
  }
#endif

// Example 2: Arbitrary hardcoded heuristic threshold
if (percent_diff <= 15 ||
    rq_history.recode_count >= RATE_CTRL_MAX_RECODE_NUM ||
    rq_history.q_index_low >= rq_history.q_index_high) {
  break;
}
```

Good:

```cpp
// Example 1: Documented limit clarifying intent
#ifdef OUTPUT_YUV_SVC_SRC
  // Write out at most 3 spatial layers for debugging.
  if (is_one_pass_cbr_svc(cpi) && cpi->svc.spatial_layer_id < 3) {
    vpx_write_yuv_frame(yuv_svc_src[cpi->svc.spatial_layer_id], cpi->Source);
  }
#endif

// Example 2: Configurable threshold with documented derivation
// 15% tolerance chosen based on experimental bitrate convergence tests.
if (percent_diff <= cpi->oxcf.target_percent_diff ||
    rq_history.recode_count >= RATE_CTRL_MAX_RECODE_NUM ||
    rq_history.q_index_low >= rq_history.q_index_high) {
  break;
}
```
