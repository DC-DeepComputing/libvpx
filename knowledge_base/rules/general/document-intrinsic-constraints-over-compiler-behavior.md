---
name: libvpx/document-intrinsic-constraints-over-compiler-behavior
description: When commenting on manual unrolling or code structure necessitated by SIMD intrinsics, state the technical constraint directly rather than compiler-specific unrolling behavior
rubric: readability
severity: nit
---

**Rule.** When documenting code structures, manual loop unrolling, or explicit
constant indexing necessitated by SIMD intrinsics (such as ARM NEON vext or x86
_mm_alignr_epi8), concisely state the fundamental technical requirement (e.g.,
that the intrinsic requires a compile-time immediate operand) rather than
discussing compiler-specific unrolling heuristics, optimization quirks, or
temporary build failures.

**Why.** SIMD intrinsics that require compile-time literals or immediate
arguments have strict API requirements across all compilers. Documenting the
underlying technical constraint directly provides clear, durable rationale
without cluttering comments with ephemeral compiler-specific optimization
details.

## Examples

Bad:

```cpp
// Shift x and xy to the right and sum. The compiler doesn't always unroll
// this and use constants which causes a build failure because vext doesn't
// accept variables.
*sum = vadd_s16(*sum, vext_s16(zero, x, 1));
```

Good:

```cpp
// Shift x and xy to the right and sum. vext requires an immediate.
*sum = vadd_s16(*sum, vext_s16(zero, x, 1));
```
