---
name: libvpx/vp8/avoid-range-checks-for-downstream-clamped-config
description: When validating codec configurations in validate_config, do not add RANGE_CHECK macros for parameters already capped or clamped downstream in set_encoder_config.
rubric: correctness
severity: should_fix
---

**Rule.** When validating encoder configuration parameters in codec interface
functions (such as validate_config), do not add redundant or conflicting
RANGE_CHECK, RANGE_CHECK_HI, or RANGE_CHECK_LO macros for values that are
already capped or clamped downstream in internal configuration setup (such as
capping rc_target_bitrate or raw stream rates via VPXMIN in set_encoder_config /
update_encoder_cfg). Rely on downstream clamping logic or ensure interface
validation bounds directly align with internal limits.

**Why.** If an encoder configuration parameter is already clamped to a safe
operational maximum downstream (e.g. via VPXMIN), adding a redundant or looser
RANGE_CHECK_HI in validate_config() introduces conflicting expectations,
unnecessary interface validation failures, and dead validation logic.

## Examples

Bad:

```cpp
// In validate_config():
RANGE_CHECK_HI(cfg, rc_target_bitrate, 2000000);

// In set_encoder_config():
oxcf->target_bandwidth = VPXMIN(cfg.rc_target_bitrate, 1000000);
```

Good:

```cpp
// In validate_config():
// Do not add redundant or conflicting RANGE_CHECK_HI for rc_target_bitrate.

// In set_encoder_config():
// Parameter is safely clamped downstream to the maximum supported bitrate.
cfg->rc_target_bitrate =
    VPXMIN(VPXMIN(raw_target_rate, cfg->rc_target_bitrate), 1000000);
oxcf->target_bandwidth = cfg->rc_target_bitrate;
```
