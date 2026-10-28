---
name: libvpx/vp8/populate-vbr-params-in-constrained-quality-mode
description: When configuring rate control structs for Constrained Quality (VPX_CQ or VPX_RC_CQ) mode, populate VBR overshoot and undershoot parameters alongside quantizers
rubric: correctness
severity: should_fix
---

**Rule.** When configuring rate control structs or external rate controller
parameters (such as vpx_rc_config_t) for Constrained Quality (VPX_CQ /
VPX_RC_CQ) mode, populate Variable Bit Rate (VBR) rate control
parameters—including overshoot and undershoot percentages—in addition to
quantizer bounds. Do not restrict VBR parameter population exclusively to
VPX_VBR / VPX_RC_VBR branches.

**Why.** Constrained Quality (CQ) is a hybrid rate control mode where quality is
capped by a maximum quantizer but falls back to Variable Bit Rate (VBR) when
target bitrate is constrained. Omitting VBR parameters leaves rate control
misconfigured during bitrate-constrained operations.

## Examples

Bad:

```cpp
if (oxcf->rc_mode == VPX_VBR) {
  ratectrl_config.rc_mode = VPX_RC_VBR;
  ratectrl_config.overshoot_percent = oxcf->over_shoot_pct;
  ratectrl_config.undershoot_percent = oxcf->under_shoot_pct;
} else if (oxcf->rc_mode == VPX_CQ) {
  ratectrl_config.rc_mode = VPX_RC_CQ;
}
```

Good:

```cpp
ratectrl_config.overshoot_percent = oxcf->over_shoot_pct;
ratectrl_config.undershoot_percent = oxcf->under_shoot_pct;
if (oxcf->rc_mode == VPX_VBR) {
  ratectrl_config.rc_mode = VPX_RC_VBR;
} else if (oxcf->rc_mode == VPX_CQ) {
  ratectrl_config.rc_mode = VPX_RC_CQ;
}
```
