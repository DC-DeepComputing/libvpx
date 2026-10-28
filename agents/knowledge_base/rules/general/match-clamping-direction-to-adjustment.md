---
name: libvpx/match-clamping-direction-to-adjustment
description: Use VPXMIN for upper bounds or increments and VPXMAX for lower bounds or decrements, matching fixed-width integer limits like INT64_MAX.
rubric: correctness
severity: should_fix
---

**Rule.** When clamping or bounding numerical values, ensure the clamping macro
matches the intended boundary direction:
1. Upper bounds / ceilings: Use VPXMIN(val, max_val) to prevent exceeding maximum thresholds, such as after increments (+=, ++) or to avoid arithmetic overflow before multiplication/scaling operations. Do not use VPXMAX when capping an upper bound.
2. Lower bounds / floors: Use VPXMAX(val, min_val) to guard against underflowing minimum thresholds, such as after decrements (-=, --). Do not use VPXMIN after decrements.
3. Multi-sided clamping: Alternatively, use two-sided clamp(val, min_val, max_val), fclamp(), or lclamp().

Additionally, ensure limit constants match the exact-width integer type being bounded (e.g., INT64_MAX or UINT64_MAX for 64-bit integers instead of platform-dependent LONG_MAX or ULONG_MAX).

**Why.** VPXMAX() establishes a lower floor rather than an upper ceiling, which
fails to protect against arithmetic overflow or upper-bound overflow while
inadvertently increasing small values. Conversely, VPXMIN() establishes an upper
ceiling rather than a lower floor, leaving decremented values vulnerable to
underflow (e.g., negative array indexing). Furthermore, using platform-dependent
constants like LONG_MAX introduces 32-bit vs. 64-bit portability bugs because
long is only 32 bits on LLP64 platforms such as 64-bit Windows.

## Examples

Bad:

```cpp
// Variant 1: Inverted bounds after adjustments
if (actual_bits < target_bits) {
  q_index -= gap;
  q_index = VPXMIN(q_index, 255);  // Underflow unchecked
} else {
  q_index += gap;
  q_index = VPXMAX(q_index, 0);    // Overflow unchecked
}

// Variant 2: VPXMAX used for upper ceiling with platform-dependent limit
this_duration = VPXMAX(this_duration, LONG_MAX / 10);
```

Good:

```cpp
// Variant 1: Correct bound matching adjustment direction
if (actual_bits < target_bits) {
  q_index -= gap;
  q_index = VPXMAX(q_index, 0);    // Guards lower bound
} else {
  q_index += gap;
  q_index = VPXMIN(q_index, 255);  // Guards upper bound
}

// Variant 2: VPXMIN caps upper bound using matching 64-bit integer limit
this_duration = VPXMIN(this_duration, INT64_MAX / 10);
```
