---
name: libvpx/vp8/symmetric-cpu-used-range-check
description: When validating or clamping signed encoder parameters like cpu_used, clamp bounds symmetrically and preserve CONFIG_REALTIME_ONLY outward snapping for non-realtime speeds
rubric: correctness
severity: blocking
---

**Rule.** When validating or clamping configuration parameters that accept
signed valid ranges (such as cpu_used in RANGE_CHECK or VP8E_SET_CPUUSED control
handlers), clamp and validate both lower and upper bounds symmetrically (e.g.,
clamp(cpu_used, -9, 9) and RANGE_CHECK(extra_cfg, cpu_used, -9, 9)). Do not use
one-sided capping (such as VPXMIN(9, cpu_used)) or leave negative bounds
narrower than positive bounds. Additionally, under CONFIG_REALTIME_ONLY, ensure
speeds within the non-realtime range (-5, 5) are snapped outward to 5 or -5
after clamping to [-9, 9], rather than incorrectly clamping into [-5, 5].

**Why.** In libvpx encoder interfaces, cpu_used and certain signed parameters
accept negative values representing valid speed configurations processed via
abs(cpu_used). One-sided capping allows invalid negative values to pass
unchecked, while asymmetric range checks reject valid configurations.
Furthermore, VP9 realtime-only mode does not support speeds in (-5, 5); clamping
to [-5, 5] inverts the requirement and restricts execution to unsupported slow
speeds.

## Examples

Bad:

```cpp
// Asymmetric range check when extending supported speed from 8 to 9:
RANGE_CHECK(extra_cfg, cpu_used, -8, 9);

// Capping only upper bound allows invalid negative values:
extra_cfg.cpu_used = VPXMIN(9, extra_cfg.cpu_used);

// Clamping to [-5, 5] under CONFIG_REALTIME_ONLY cuts off valid realtime speeds:
#if CONFIG_REALTIME_ONLY
  extra_cfg.cpu_used = clamp(extra_cfg.cpu_used, -5, 5);
#endif
```

Good:

```cpp
// Update both lower and upper bounds symmetrically:
RANGE_CHECK(extra_cfg, cpu_used, -9, 9);

// Clamp both minimum and maximum bounds symmetrically:
extra_cfg.cpu_used = clamp(extra_cfg.cpu_used, -9, 9);

// Snap unsupported non-realtime speeds in (-5, 5) outward under CONFIG_REALTIME_ONLY:
#if CONFIG_REALTIME_ONLY
  if (extra_cfg.cpu_used > -5 && extra_cfg.cpu_used < 5)
    extra_cfg.cpu_used = (extra_cfg.cpu_used > 0) ? 5 : -5;
#endif
```
