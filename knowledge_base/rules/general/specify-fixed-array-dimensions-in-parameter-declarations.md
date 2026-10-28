---
name: libvpx/specify-fixed-array-dimensions-in-parameter-declarations
description: When helper functions or DSP kernels operate on fixed-size buffers, specify explicit array dimensions in parameter declarations rather than bare pointers
rubric: readability
severity: should_fix
---

**Rule.** When declaring or defining helper functions or DSP kernels that accept
fixed-size input or output buffers (e.g., const __m128i in[16], __m128i out[32],
or const tran_low_t in[16]), specify expected array dimensions directly in the
parameter signature (Type name[N]) instead of using bare pointers (Type *name)
or relying on inline comments. This makes buffer size expectations
self-documenting in the function prototype.

**Why.** Bare pointer parameters (such as const __m128i *in or tran_low_t *out)
obscure expected buffer bounds and force callers to inspect implementation
details or risk reading stale comments. Declaring fixed array dimensions
directly in the function signature makes buffer sizing requirements
self-documenting at both declaration and call sites.

## Examples

Bad:

```cpp
// Process 8x32 block; bare pointers obscure expected buffer bounds
static void idct32_8x32_quarter_3_4(const __m128i *in, __m128i *out) {
  // ...
}

// Fixed-size transform kernel with dimension in comments
static void fdct16(const tran_low_t *in /*[16]*/, tran_low_t *out /*[16]*/) {
  // ...
}
```

Good:

```cpp
// Process 8x32 block; buffer dimensions are self-documenting in signature
static void idct32_8x32_quarter_3_4(const __m128i in[16], __m128i out[32]) {
  // ...
}

// Explicit array parameter syntax specifies exact buffer requirements
static void fdct16(const tran_low_t in[16], tran_low_t out[16]) {
  // ...
}
```
