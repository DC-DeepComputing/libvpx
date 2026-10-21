---
name: libvpx/vpx_dsp/match-neon-vector-reinterpret-signedness
rubric: correctness
severity: blocking
---

**Rule.** Ensure ARM NEON vector reinterpret intrinsics (vreinterpret_* and
vreinterpretq_*) match the signedness and element type of the destination vector
and downstream operations (e.g., use vreinterpret_s32_s64 rather than
vreinterpret_u32_s64 when producing int32x2_t or feeding signed arithmetic like
vadd_s32). Do not use unsigned reinterpret intrinsics for signed vector targets
or vice versa.

**Why.** In ARM NEON (<arm_neon.h>), signed and unsigned vector types (such as
int32x2_t and uint32x2_t) are distinct types. Mismatching signedness in
vreinterpret_* creates vector type mismatches that cause compiler warnings or
strict type-check build failures across different toolchains.

## Examples

Bad:

```cpp
// Bad: vreinterpret_u32_s64 produces uint32x2_t, causing type mismatch with vadd_s32 and int32x2_t.
const int32x2_t c = vadd_s32(vreinterpret_u32_s64(vget_low_s64(b)),
                             vreinterpret_u32_s64(vget_high_s64(b)));
```

Good:

```cpp
// Good: vreinterpret_s32_s64 produces int32x2_t, matching vadd_s32 and int32x2_t.
const int32x2_t c = vadd_s32(vreinterpret_s32_s64(vget_low_s64(b)),
                             vreinterpret_s32_s64(vget_high_s64(b)));
```
