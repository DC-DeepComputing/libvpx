---
name: libvpx/sync-reference-c-and-tests-with-simd-fixed-point-math
description: When implementing SIMD routines using fixed-point multiplier tables instead of integer division, update C reference implementations and unit tests to use matching arithmetic
rubric: correctness
severity: should_fix
---

**Rule.** When introducing or modifying a SIMD routine that replaces integer
division with fixed-point multiplier constants, lookup tables, or reciprocal
shifts (such as via _mm_mulhi_epu16 or NEON vqdmulhq_s16), update the
corresponding C reference implementation and unit test reference models to use
the exact same multiplier constants and fixed-point shift arithmetic rather than
exact division (/).

**Why.** Fixed-point multiplier approximations and shift truncation differ
subtly from exact integer division, particularly on large input values or
boundary conditions. Keeping reference C and test arithmetic in sync with SIMD
fixed-point math ensures bit-exact consistency and prevents cross-architecture
test failures.

## Examples

Bad:

```cpp
// SIMD implementation uses fixed-point multiplier table and shift (sum * mult) >> 16,
// but the C reference implementation and test still use exact integer division:
static INLINE int mod_index(int sum_dist, int index, int rounding, int strength,
                            int filter_weight) {
  int mod = (sum_dist * 3) / index;
  mod += rounding;
  mod >>= strength;
  return mod;
}
```

Good:

```cpp
// C reference and test use the exact same multiplier table and shift arithmetic:
static const int index_mult[14] = { -1,    -1,    -1,    -1,    49152, 39322, 32768,
                                    28087, 24576, 21846, 19661, 17874, -1,    15124 };

static INLINE int mod_index(int sum_dist, int index, int rounding, int strength,
                            int filter_weight) {
  int mod;
  assert(index >= 0 && index <= 13 && index_mult[index] != -1);
  mod = (sum_dist * index_mult[index]) >> 16;
  mod += rounding;
  mod >>= strength;
  return mod;
}
```
