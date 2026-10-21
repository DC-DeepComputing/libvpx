---
name: libvpx/annotate-intentional-unsigned-overflow-wraparound
rubric: correctness
severity: should_fix
---

**Rule.** Account for integer promotion to signed int when subtracting unsigned
types narrower than int (e.g., uint16_t or uint8_t). When intentionally relying
on modulo-2^32 unsigned wraparound for negative differences (such as squaring
cast differences), cast values explicitly to unsigned types and annotate the
function with VPX_NO_UNSIGNED_OVERFLOW_CHECK.

**Why.** In C, subtracting unsigned operands narrower than int yields a signed
int when src < ref. Casting negative signed values to uint32_t and multiplying
them produces mathematically correct squared results modulo 2^32, but triggers
unsigned-integer-overflow sanitizers unless suppressed with
VPX_NO_UNSIGNED_OVERFLOW_CHECK.

## Examples

Bad:

```cpp
static uint32_t square_diff(uint16_t a, uint16_t b) {
  const int diff = a - b;
  // Triggers -fsanitize=unsigned-integer-overflow when diff is negative:
  return (uint32_t)diff * (uint32_t)diff;
}
```

Good:

```cpp
static VPX_NO_UNSIGNED_OVERFLOW_CHECK uint32_t square_diff(uint16_t a,
                                                           uint16_t b) {
  const int diff = a - b;
  const uint32_t unsigned_diff = (uint32_t)diff;
  return unsigned_diff * unsigned_diff;
}
```
