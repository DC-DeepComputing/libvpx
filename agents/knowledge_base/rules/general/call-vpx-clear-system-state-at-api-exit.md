---
name: libvpx/call-vpx-clear-system-state-at-api-exit
description: Call vpx_clear_system_state() after MMX instructions or at public API exits before floating-point math, omitting redundant calls after scalar calculations
rubric: correctness
severity: should_fix
---

**Rule.** Call `vpx_clear_system_state()` after MMX/SIMD routines or at the exit
points of public API functions, rate control wrappers (such as
`VP8RateControlRTC` or `VpxRateControlRTC`), and codec entry points before
executing floating-point math or returning to callers. Do not call it solely at
function entry before MMX execution, and do not add redundant
`vpx_clear_system_state()` calls after scalar floating-point or integer
calculations.

**Why.** `vpx_clear_system_state()` executes `emms` (`_mm_empty()`) to clear the
x87 FPU tag word after MMX instructions that alias the x87 register stack.
Failing to invoke it after MMX operations or before returning from API
boundaries leaves the FPU stack tagged as busy/dirty, causing subsequent
floating-point operations in external callers or downstream code to produce
NaNs, overflow, or trigger floating-point exceptions. Conversely, scalar
floating-point and integer instructions do not set the MMX state, making
trailing calls after scalar math redundant overhead.

## Examples

Bad:

```cpp
// Variant 1: Redundant call after scalar float operations
vpx_clear_system_state();
rk = (double)intra_cost / mc_dep_cost;
beta = r0 / rk;
dr = vp9_get_adaptive_rdmult(cpi, beta);
dr = VPXMAX(1, dr);
vpx_clear_system_state();  // Redundant trailing call after scalar math
return dr;

// Variant 2: Calling at API entry instead of API exit
bool VP8RateControlRTC::UpdateRateControl(
    const VP8RateControlRtcConfig &rc_cfg) {
  vpx_clear_system_state();  // Premature: MMX code runs in vp8_change_config
  if (!ValidateConfig(rc_cfg)) return false;
  cpi_->oxcf.target_bandwidth = rc_cfg.target_bandwidth;
  vp8_change_config(cpi_, &cpi_->oxcf);
  return true;
}
```

Good:

```cpp
// Variant 1: Clear before float math, omit after scalar operations
vpx_clear_system_state();
rk = (double)intra_cost / mc_dep_cost;
beta = r0 / rk;
dr = vp9_get_adaptive_rdmult(cpi, beta);
dr = VPXMAX(1, dr);
return dr;

// Variant 2: Clear at API exit before returning to caller
bool VP8RateControlRTC::UpdateRateControl(
    const VP8RateControlRtcConfig &rc_cfg) {
  if (!ValidateConfig(rc_cfg)) return false;
  cpi_->oxcf.target_bandwidth = rc_cfg.target_bandwidth;
  vp8_change_config(cpi_, &cpi_->oxcf);
  vpx_clear_system_state();
  return true;
}
```
