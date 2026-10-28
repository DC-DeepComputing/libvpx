---
name: libvpx/no-aggregate-initializer-for-arm-neon-vector-types
description: When initializing ARM NEON vector types (such as int16x8_t or uint8x16_t), use NEON intrinsics or vld1_* loads instead of aggregate initializer lists
rubric: correctness
severity: blocking
---

**Rule.** Do not initialize ARM NEON vector types (such as int16x8_t,
uint8x16_t, or float32x4_t) using aggregate initializer lists (e.g. { 0, 1, ...
}) or compound literals. Instead, construct vector constants using NEON
intrinsics (such as vdupq_n_*, vcombine_*, vextq_*, or vcreate_*) or load them
from static constant arrays using vld1_* / vld1q_*.

**Why.** ARM NEON vector types are compiler-specific typedefs whose internal
representations differ across toolchains. Aggregate initializer lists and vector
compound literals are GCC/Clang extensions that fail to compile under MSVC
(Visual Studio), causing cross-platform build breakage.

## Examples

Bad:

```cpp
// Aggregate initializer lists for NEON vectors fail to compile on MSVC.
const int16x8_t nonzero_bias_a = { 0, 1, 1, 1, 1, 1, 1, 1 };
const int16x8_t nonzero_bias_b = { 1, 0, 0, 0, 0, 0, 0, 0 };
```

Good:

```cpp
// Construct vectors using portable NEON intrinsics:
const int16x8_t nonzero_bias_a = vextq_s16(vdupq_n_s16(0), vdupq_n_s16(1), 7);
const int16x8_t nonzero_bias_b = vextq_s16(vdupq_n_s16(1), vdupq_n_s16(0), 7);

// Or load from static constant arrays using vld1q_*:
static const int16_t kNonzeroBiasA[8] = { 0, 1, 1, 1, 1, 1, 1, 1 };
const int16x8_t bias_a = vld1q_s16(kNonzeroBiasA);
```
