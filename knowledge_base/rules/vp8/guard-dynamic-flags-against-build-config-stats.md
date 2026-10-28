---
name: libvpx/vp8/guard-dynamic-flags-against-build-config-stats
description: 'When updating per-frame encoder or stats calculation flags, guard assignments with build configuration #if checks to avoid overriding compile-time stats settings'
rubric: correctness
severity: should_fix
---

**Rule.** When updating encoder flags or statistics calculation settings (such
as b_calculate_psnr) dynamically per frame, guard the assignment with
build-configuration checks (e.g., #if CONFIG_INTERNAL_STATS). Ensure that
runtime or per-frame flags do not unconditionally overwrite compile-time
statistics tracking initialized at encoder startup.

**Why.** Under compile-time configurations like CONFIG_INTERNAL_STATS,
statistics calculation is enabled globally at encoder initialization.
Overwriting internal state with dynamic per-frame flags resets tracking flags to
0 whenever runtime flags are omitted, breaking statistics collection.

## Examples

Bad:

```cpp
cpi->b_calculate_psnr = (enc_flags & VPX_EFLAG_CALCULATE_PSNR) ||
                        (ctx->base.init_flags & VPX_CODEC_USE_PSNR);
```

Good:

```cpp
#if CONFIG_INTERNAL_STATS
assert(cpi->b_calculate_psnr == 1);
#else
cpi->b_calculate_psnr = (ctx->base.init_flags & VPX_CODEC_USE_PSNR) ||
                        (enc_flags & VPX_EFLAG_CALCULATE_PSNR);
#endif
```
