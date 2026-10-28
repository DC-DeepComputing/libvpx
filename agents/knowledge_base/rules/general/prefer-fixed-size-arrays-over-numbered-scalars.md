---
name: libvpx/prefer-fixed-size-arrays-over-numbered-scalars
description: When declaring, initializing, or accessing homogeneous values or array elements, use fixed-size arrays with loops or initializers instead of numbered scalars or chained assignments
rubric: readability
severity: nit
---

**Rule.** When managing, initializing, or sequentially consuming a series of
homogeneous values (such as multi-row context, line buffers, intermediate
calculation stages, or SIMD vector registers like uint8x8_t or __m128i), declare
a fixed-size array (type name[N]) instead of separate numbered scalar variables
(e.g., name_0, name_1, val0, val1). Initialize or populate elements using loops
(e.g., for), memset (for zero or byte values), standard array initializer
syntax, or direct array indexing (arr[i]), rather than chaining assignments
across scalar variables or array elements (such as a = b = c = val or arr[0] =
arr[1] = val) or selecting between numbered variables inside loop bodies using
conditional branch ladders (if (i == 0) ...) or switch statements.

**Why.** Using fixed-size arrays paired with structured loops, memset, or
standard initializers keeps variable declarations concise, eliminates repetitive
chained assignments across multi-line breaks, flattens loop control flow, and
decouples iteration logic from hardcoded variable names, reducing the risk of
copy-paste index errors.

## Examples

Bad:

```cpp
// Variant 1: Numbered scalar variables or chained assignments
uint8x8_t above_context_0, above_context_1, above_context_2,
    above_context_3, above_context_4, above_context_5, above_context_6,
    above_context_7, s;
s = vld1_u8(dst);
above_context_0 = above_context_1 = above_context_2 = above_context_3 =
    above_context_4 = above_context_5 = above_context_6 = above_context_7 = s;

// Chained assignments across multiple array indices
above_context[0] = above_context[1] = above_context[2] = above_context[3] =
    above_context[4] = above_context[5] = above_context[6] = above_context[7] = s;

// Variant 2: Branch ladder selecting among numbered scalar registers in a loop
const __m128i avg3_left_0 = avg3_epu16(&XL0, &L0, &L0_);
const __m128i avg3_left_1 = avg3_epu16(&XL1, &L1, &L1_);
const __m128i avg3_left_2 = avg3_epu16(&XL2, &L2, &L2_);
const __m128i avg3_left_3 = avg3_epu16(&XL3, &L3, &L3_);
for (i = 0; i < 4; ++i) {
  __m128i avg_left;
  if (i == 0) avg_left = avg3_left_0;
  else if (i == 1) avg_left = avg3_left_1;
  else if (i == 2) avg_left = avg3_left_2;
  else avg_left = avg3_left_3;
  // process avg_left...
}
```

Good:

```cpp
// Variant 1: Fixed-size array initialized in a compact loop or initializer
uint8x8_t s, above_context[8];
int i;

s = vld1_u8(dst);
for (i = 0; i < 8; ++i) {
  above_context[i] = s;
}

// Variant 2: Fixed-size array indexed directly by the loop counter
__m128i avg3_left[4];
avg3_left[0] = avg3_epu16(&XL0, &L0, &L0_);
avg3_left[1] = avg3_epu16(&XL1, &L1, &L1_);
avg3_left[2] = avg3_epu16(&XL2, &L2, &L2_);
avg3_left[3] = avg3_epu16(&XL3, &L3, &L3_);
for (i = 0; i < 4; ++i) {
  const __m128i avg_left = avg3_left[i];
  // process avg_left...
}
```
