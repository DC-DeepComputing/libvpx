---
name: libvpx/vpx_dsp/align-multiline-commented-reference-code
description: When including multi-line commented reference C code in SIMD or DSP implementations, align wrapped arguments with the opening parenthesis and maintain clean indentation.
rubric: readability
severity: nit
---

**Rule.** When documenting SIMD or DSP optimizations with commented-out
reference C code that spans multiple lines, maintain standard C indentation and
align wrapped arguments with the opening parenthesis of the call or expression.
Do not leave subsequent lines of commented code arbitrarily indented or
misaligned.

**Why.** Commented reference C code serves as the specification for complex
vector intrinsics or assembly implementations. Keeping the commented code
properly formatted and aligned preserves readability and makes it easy to
compare the SIMD implementation against the reference logic.

## Examples

Bad:

```cpp
/* const int8_t mask = filter_mask(*limit, *blimit, */
/*                 p3, p2, p1, p0, q0, q1, q2, q3); */
```

Good:

```cpp
/* const int8_t mask = filter_mask(*limit, *blimit, */
/*                                 p3, p2, p1, p0, q0, q1, q2, q3); */
```
