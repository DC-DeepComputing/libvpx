---
name: libvpx/avoid-assertions-on-unverified-range-assumptions
description: When adding assertions on calculated values or rates, verify bounds hold across all input paths rather than asserting unverified range assumptions
rubric: correctness
severity: blocking
---

**Rule.** Do not add assertions on calculated values, rates, or metrics based on
unverified range assumptions. Verify that asserted bounds are strictly
guaranteed across all valid input paths and edge cases (such as extreme
timestamps, zero durations, or sub-unit intervals), or rely on safe arithmetic,
explicit sanitization, and clamping when bounds cannot be guaranteed.

**Why.** Assertions should only verify true invariants that always hold across
all valid inputs. Asserting unverified range assumptions on calculated
quantities triggers false assertion failures in debug builds on rare but
legitimate inputs, even when downstream code already handles out-of-range values
safely using clamping or wider integer types.

## Examples

Bad:

```cpp
// Framerate derived from timestamp deltas may exceed TICKS_PER_SEC for small durations.
assert(framerate <= TICKS_PER_SEC);
kf_boost = VPXMAX(kf_boost, (int)round(2 * framerate - 16));
```

Good:

```cpp
// Rely on clamping and safe bounds rather than asserting unverified range assumptions.
kf_boost = VPXMAX(kf_boost, (int)round(2 * framerate - 16));
```
