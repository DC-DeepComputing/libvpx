---
name: libvpx/vpx_dsp/consume-simd-load-helper-types-directly
description: When refactoring SIMD routines to use vector load helpers, remove obsolete intermediate variables and redundant reinterpret casts, consuming native vector return types directly
rubric: readability
severity: should_fix
---

**Rule.** When refactoring manual SIMD load sequences to use dedicated load
helpers (such as load_u8(), load_u16(), or load_tran_low_to_s16q()), assign and
consume the helper's native return type (such as uint8x8_t) directly. Remove
obsolete intermediate variables, dead initializations (such as vdup_n_*), and
redundant vector reinterpret casts (such as vreinterpret_*).

**Why.** Dedicated load helpers encapsulate multi-lane loading and return the
properly typed SIMD vector directly. Retaining legacy intermediate variables and
unnecessary reinterpret casts adds dead code, clutters arithmetic expressions,
and risks compiler type mismatch errors on strict vector targets.

## Examples

Bad:

```cpp
// Retaining legacy intermediate variable and redundant reinterpret cast
uint32x2_t dest01_u32 = vdup_n_u32(0);
dest01_u32 = load_u8(dst, stride);
d01_u16 = vaddw_u8(vreinterpretq_u16_s16(a0), vreinterpret_u8_u32(dest01_u32));
```

Good:

```cpp
// Consume load helper's native uint8x8_t return type directly
const uint8x8_t s = load_u8(dst, stride);
sum[0] = vaddw_u8(vreinterpretq_u16_s16(a[0]), s);
```
