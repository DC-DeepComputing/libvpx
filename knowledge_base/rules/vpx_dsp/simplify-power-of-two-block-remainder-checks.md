---
name: libvpx/vpx_dsp/simplify-power-of-two-block-remainder-checks
description: When processing power-of-two block dimensions after SIMD chunk loops, use mutually exclusive remainder checks rather than sequential conditional decrements
rubric: readability
severity: nit
---

**Rule.** When processing video block dimensions guaranteed to be powers of two
(e.g., 4, 8, 16, 32, 64), simplify remainder-handling logic after larger vector
chunk loops by using mutually exclusive checks (such as 'if (width == 8) ...
else if (width == 4) ...') instead of sequential non-exclusive 'if (width >=
...)' checks and decrements. Power-of-two block dimensions cannot produce
composite remainders like 12.

**Why.** Codec block dimensions are strictly powers of two (4, 8, 16, 32, 64).
After processing chunks of 16 (or larger powers of two), any remaining width is
mutually exclusive (strictly 8, 4, or 0). Sequential '>=' checks with decrements
add unnecessary branch evaluations and redundant variable updates.

## Examples

Bad:

```cpp
while (width >= 16) {
  process_16(src, dst);
  src += 16;
  dst += 16;
  width -= 16;
}
if (width >= 8) {
  process_8(src, dst);
  src += 8;
  dst += 8;
  width -= 8;
}
if (width >= 4) {
  process_4(src, dst);
}
```

Good:

```cpp
while (width >= 16) {
  process_16(src, dst);
  src += 16;
  dst += 16;
  width -= 16;
}
if (width == 8) {
  process_8(src, dst);
} else if (width == 4) {
  process_4(src, dst);
}
```
