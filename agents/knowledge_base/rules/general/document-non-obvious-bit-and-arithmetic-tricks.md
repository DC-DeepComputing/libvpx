---
name: libvpx/document-non-obvious-bit-and-arithmetic-tricks
description: When writing non-obvious bit manipulation or arithmetic transformations—such as mapping zero inputs for bit-scanning functions—add comments explaining the logic and edge-case handling.
rubric: readability
severity: nit
---

**Rule.** Precede non-obvious bit manipulation, mask transformations, and
arithmetic tricks with an explanatory comment detailing the underlying logic,
edge-case behavior, and coordinate/index mappings. In particular, when
transforming inputs passed to bit-scanning functions (such as `get_msb()`,
`get_lsb()`, `__builtin_clz`, or `_mm_movemask_*`) to safely handle zero inputs
or convert between 0-based positions and 1-based counts, document why the
transformation is safe and necessary.

**Why.** Bit-level tricks and mathematical transformations (such as `mask * 2 +
1` to map zero to one and offset bit positions) are opaque and error-prone to
maintain. Explanatory comments prevent future maintainers from accidentally
breaking subtle zero-case handling or introducing undefined behavior in
bit-scanning intrinsics.

## Examples

Bad:

```cpp
mask = _mm_movemask_epi8(x);
eob = get_msb(mask * 2 + 1);
```

Good:

```cpp
mask = _mm_movemask_epi8(x);
/* x2 is needed to increase the result from non-zero masks by 1,
 * +1 is needed to mask undefined behavior for a null argument,
 * the result of get_msb(1) is 0 */
eob = get_msb(mask * 2 + 1);
```
