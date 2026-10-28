---
name: libvpx/vp8/no-arbitrary-dimension-limits-for-allocation-overflow
description: When validating frame dimensions in codec configuration, do not impose arbitrary resolution limits; validate against bitstream specification bounds and return VPX_CODEC_MEM_ERROR on allocation failure.
rubric: correctness
severity: should_fix
---

**Rule.** Do not impose arbitrary resolution or dimension limits during
encoder/decoder configuration validation to prevent memory allocation overflows
when the codec specification permits those dimensions. Validate frame dimensions
(e.g., g_w and g_h) against bitstream format specification limits using
RANGE_CHECK macros, perform internal buffer size calculations with overflow-safe
or 64-bit arithmetic, and return VPX_CODEC_MEM_ERROR gracefully if memory
allocation fails.

**Why.** Artificially capping frame dimensions in configuration validation
breaks specification compliance for valid high-resolution inputs. Proper
overflow protection requires safe arithmetic in size computations and graceful
out-of-memory error handling rather than rejecting spec-compliant resolutions at
configuration time.

## Examples

Bad:

```cpp
// Arbitrary cap on total pixels to prevent downstream allocation overflow
if ((int64_t)cfg->g_w * cfg->g_h > (int64_t)700000000)
  ERROR("frame size too large");
```

Good:

```cpp
// Validate dimensions against bitstream spec limits (e.g. 16-bit max in VP9);
// downstream allocation uses 64-bit math and returns VPX_CODEC_MEM_ERROR on OOM.
RANGE_CHECK(cfg, g_w, 1, 65536);  // 16 bits available
RANGE_CHECK(cfg, g_h, 1, 65536);  // 16 bits available
```
