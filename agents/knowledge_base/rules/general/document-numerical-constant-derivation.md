---
name: libvpx/document-numerical-constant-derivation
description: When defining numerical, fixed-point, or scaled constants, document their mathematical formula, derivation method, and scaling factor in an accompanying comment
rubric: readability
severity: should_fix
---

**Rule.** When defining numerical, fixed-point, or lookup constants (such as
scaled trigonometric or irrational values), document the mathematical formula,
derivation method, scaling factor, or rounding approach in an explanatory
comment above or alongside the definition. Avoid leaving precomputed or scaled
values as unexplained magic numbers.

**Why.** Without derivation comments, scaled fixed-point approximations and
precomputed constants appear as arbitrary magic numbers, making it difficult for
reviewers and maintainers to verify numerical correctness, precision limits, or
modify scaling factors.

## Examples

Bad:

```cpp
static const tran_high_t Sqrt2 = 23170;
#define COS_PI_8 30274
```

Good:

```cpp
// 16384 * sqrt(2)
static const tran_high_t Sqrt2 = 23170;

// round(32768 * cos(M_PI / 8))
#define COS_PI_8 30274
```
