---
name: libvpx/parenthesize-offset-pointer-cast
description: When casting an offset pointer expression to a wider or SIMD vector pointer type, parenthesize the offset addition before applying the cast
rubric: correctness
severity: blocking
---

**Rule.** When casting an offset pointer to a wider or different pointer type
(such as a SIMD vector pointer `__m128i *`), wrap the offset addition or
subtraction in parentheses `(ptr + offset)` before applying the cast (e.g.,
`(const __m128i *)(ptr + offset)`). Never write `(Type *)ptr + offset` or
`*(Type *)ptr + offset`, because C/C++ cast and dereference operators bind
tighter than binary addition and subtraction.

**Why.** In C and C++, cast and unary dereference operators have higher
precedence than binary addition. Omitting parentheses around `(ptr + offset)`
causes pointer arithmetic to be evaluated after the cast (scaling the offset by
`sizeof(TargetType)` instead of the source element size) or evaluates addition
after dereferencing, leading to invalid memory accesses or data corruption.

## Examples

Bad:

```cpp
// Incorrect precedence: dereferences or scales offset after casting
__m128i v = *(const __m128i *)src + 4;
const __m128i *p = (const __m128i *)src + 4;
return _mm_packs_epi32(a_low, *(const __m128i *)a + 4);
```

Good:

```cpp
// Correct: parenthesizes offset expression before applying cast
__m128i v = *(const __m128i *)(src + 4);
const __m128i *p = (const __m128i *)(src + 4);
return _mm_packs_epi32(a_low, *(const __m128i *)(a + 4));
```
