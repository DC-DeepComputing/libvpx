---
name: libvpx/test/guard-psnr-test-expectations-with-config-internal-stats
description: 'When testing conditional PSNR packet generation in encoder tests, guard test expectations with #if CONFIG_INTERNAL_STATS to handle unconditional PSNR output'
rubric: correctness
severity: nit
---

**Rule.** When writing or updating encoder tests that verify conditional PSNR
packet generation (such as toggling VPX_EFLAG_CALCULATE_PSNR), guard test
expectations with #if CONFIG_INTERNAL_STATS. Unconditionally expect PSNR packets
(VPX_CODEC_PSNR_PKT) when CONFIG_INTERNAL_STATS is enabled, and only condition
expectations on frame flags in standard builds.

**Why.** When libvpx is compiled with --enable-internal-stats
(CONFIG_INTERNAL_STATS), encoders unconditionally compute and output PSNR
packets for every frame regardless of per-frame request flags. Without
preprocessor guards, tests asserting that PSNR packets are absent when
VPX_EFLAG_CALCULATE_PSNR is unset will fail in internal stats build
configurations.

## Examples

Bad:

```cpp
// Fails when compiled with --enable-internal-stats (CONFIG_INTERNAL_STATS):
expect_psnr_ = (frame_flags_ & VPX_EFLAG_CALCULATE_PSNR) != 0;
...
EXPECT_EQ(had_psnr, expect_psnr_);
```

Good:

```cpp
#if CONFIG_INTERNAL_STATS
// CONFIG_INTERNAL_STATS unconditionally generates PSNR.
expect_psnr_ = true;
#else
expect_psnr_ = (frame_flags_ & VPX_EFLAG_CALCULATE_PSNR) != 0;
#endif  // CONFIG_INTERNAL_STATS
...
EXPECT_EQ(had_psnr, expect_psnr_);
```
