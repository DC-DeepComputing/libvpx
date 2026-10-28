---
name: libvpx/document-vpx-cbr-minrate-maxrate-constraints
description: When documenting, configuring, or exposing Constant Bit Rate (VPX_CBR) mode parameters, explicitly specify that CBR requires setting minrate and maxrate equal to target bitrate
rubric: readability
severity: should_fix
---

**Rule.** When documenting, exposing, or configuring Constant Bit Rate (VPX_CBR)
mode parameters in encoder interfaces, wrapper classes, CLI tools, or comments,
explicitly specify that CBR requires constraining both minimum and maximum
section bitrate bounds to 100% of the target bitrate (e.g., -minrate <bit_rate>
-maxrate <bit_rate> or two_pass_vbrmin_section == 100 and
two_pass_vbrmax_section == 100). Do not document or configure VPX_CBR using only
the target bitrate without the bounding constraints.

**Why.** In libvpx rate control, setting the target bitrate or rc_end_usage =
VPX_CBR alone does not enforce constant bitrate without bounding section bitrate
variations. Without explicit min and max constraints (such as
rc_2pass_vbr_minsection_pct = 100 and rc_2pass_vbr_maxsection_pct = 100), the
rate controller permits section bitrate fluctuations, failing to achieve true
CBR encoding.

## Examples

Bad:

```cpp
// 1 - Constant Bit Rate (VPX_CBR)  -b:v bit_rate
cfg.rc_end_usage = VPX_CBR;
cfg.rc_target_bitrate = bitrate;
```

Good:

```cpp
// 1 - Constant Bit Rate (VPX_CBR)  -b:v <bit_rate> -minrate <bit_rate> \
//                                 -maxrate <bit_rate>
// two_pass_vbrmin_section == 100   i.e. bit_rate == minrate == maxrate
// two_pass_vbrmax_section == 100
cfg.rc_end_usage = VPX_CBR;
cfg.rc_target_bitrate = bitrate;
cfg.rc_2pass_vbr_minsection_pct = 100;
cfg.rc_2pass_vbr_maxsection_pct = 100;
```
