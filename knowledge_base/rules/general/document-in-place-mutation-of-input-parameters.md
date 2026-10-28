---
name: libvpx/document-in-place-mutation-of-input-parameters
description: When a function mutates pointer or buffer parameters intended as inputs in place, document the in-place modification and side effects in a comment
rubric: readability
severity: nit
---

**Rule.** When a function modifies an input buffer or pointer parameter in place
(such as an array or vector named `in`, `src`, or `input` that cannot be
qualified with `const`), explicitly document the in-place mutation and side
effects in a function comment. Prefer keeping input buffers unmodified and
`const`-qualified whenever possible, but if in-place computation is necessary,
warn callers in the doc comment so they do not assume input data remains intact.

**Why.** Callers expect input pointer parameters (especially those named `in` or
`src`) to be read-only. Mutating input buffers in place without documentation
introduces hidden side effects that corrupt caller data if the buffer is reused
or referenced after the call.

## Examples

Bad:

```cpp
// Mutates `in` in place during transposition without documenting side effects:
static INLINE void idct4_kernel_sse2(__m128i *const in, __m128i *const out) {
  transpose_16bit_4x4(in, in);
  // ...
}
```

Good:

```cpp
// Note: 'in' is modified in place during transposition.
static INLINE void idct4_kernel_sse2(__m128i *const in, __m128i *const out) {
  transpose_16bit_4x4(in, in);
  // ...
}
```
