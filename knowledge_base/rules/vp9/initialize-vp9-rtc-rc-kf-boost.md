---
name: libvpx/vp9/initialize-vp9-rtc-rc-kf-boost
description: When initializing VP9 RTC rate control structures (RATE_CONTROL), explicitly set rc->kf_boost to DEFAULT_KF_BOOST to ensure accurate keyframe bit allocation
rubric: correctness
severity: should_fix
---

**Rule.** When initializing VP9 rate control state or RTC wrapper structures
(RATE_CONTROL), explicitly initialize rc->kf_boost to DEFAULT_KF_BOOST. Do not
leave kf_boost uninitialized when configuring rate control parameters or
keyframe intervals.

**Why.** In VP9 rate control, keyframe bitrate allocation and quantizer
selection in VBR mode rely on kf_boost to calculate keyframe target bit budgets.
Leaving kf_boost uninitialized causes improper bit distribution and quantizer
selection for keyframes.

## Examples

Bad:

```cpp
bool VP9RateControlRTC::InitRateControl(const VP9RateControlRtcConfig &cfg) {
  RATE_CONTROL *const rc = &cpi_->rc;
  rc->frames_to_key = cfg.key_freq;
  return true;
}
```

Good:

```cpp
bool VP9RateControlRTC::InitRateControl(const VP9RateControlRtcConfig &cfg) {
  RATE_CONTROL *const rc = &cpi_->rc;
  rc->kf_boost = DEFAULT_KF_BOOST;
  rc->frames_to_key = cfg.key_freq;
  return true;
}
```
