---
name: libvpx/test/use-minimal-dimensions-for-overflow-tests
description: When writing regression tests for integer overflow or large allocations, use minimal boundary dimensions to prevent 32-bit runner OOMs
rubric: correctness
severity: should_fix
---

**Rule.** When writing regression or unit tests that verify integer overflow
handling, dimension limits, or large-allocation error paths, calculate and use
the minimal input dimensions or parameters required to trigger the boundary
condition. Avoid using arbitrarily large dimensions from fuzzer reproducers.

**Why.** Excessively large test dimensions can allocate massive memory buffers
or exhaust virtual address space, causing test runner processes to be killed or
run out of memory on 32-bit test targets, ASan builds, or resource-constrained
CI environments. Using the minimum boundary dimensions tests the fix safely
across all targets.

## Examples

Bad:

```cpp
// Arbitrarily large dimensions from fuzz reproducers can cause OOM kills on 32-bit CI targets.
cfg.g_w = 26575;
cfg.g_h = 30747;
EXPECT_EQ(vpx_codec_enc_init(&enc, codec, &cfg, 0), VPX_CODEC_INVALID_PARAM);
```

Good:

```cpp
// Calculate minimal dimensions that satisfy the overflow threshold (e.g. w * h > limit).
cfg.g_w = 26625;
cfg.g_h = 26625;
EXPECT_EQ(vpx_codec_enc_init(&enc, codec, &cfg, 0), VPX_CODEC_INVALID_PARAM);
```
