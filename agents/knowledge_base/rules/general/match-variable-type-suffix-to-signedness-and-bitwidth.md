---
name: libvpx/match-variable-type-suffix-to-signedness-and-bitwidth
description: When naming variables with signedness or bitwidth suffixes (such as _s64, _u64, _s32, _u32), ensure the suffix matches the variable's actual signedness and element type
rubric: readability
severity: should_fix
---

**Rule.** When naming variables or vector constants with signedness and bitwidth
suffixes (such as _s8, _u8, _s16, _u16, _s32, _u32, _s64, or _u64), verify that
the suffix accurately matches the variable's type and signedness. Do not use an
unsigned suffix for signed types (such as int64x2_t or int64_t), or a signed
suffix for unsigned types.

**Why.** Mismatched signedness or bitwidth suffixes mislead reviewers and future
maintainers, obscuring whether arithmetic, right-shifts, and vector intrinsics
(such as negative shift amounts in NEON vshlq) operate on signed or unsigned
representations.

## Examples

Bad:

```cpp
// strength_u64 is misleadingly named unsigned despite holding signed values
// for negative shift amounts in vshlq_u64.
const int64x2_t strength_u64 = vdupq_n_s64(-strength - 32);
sum_lo = vshlq_u64(sum_lo, strength_u64);
```

Good:

```cpp
// Suffix _s64 accurately reflects the signed int64x2_t type and negative values.
const int64x2_t strength_s64 = vdupq_n_s64(-strength - 32);
sum_lo = vshlq_u64(sum_lo, strength_s64);
```
