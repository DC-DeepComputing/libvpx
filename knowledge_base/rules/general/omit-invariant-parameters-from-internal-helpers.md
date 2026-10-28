---
name: libvpx/omit-invariant-parameters-from-internal-helpers
description: When defining internal static helper functions or intermediate processing passes, omit parameters that take an invariant constant value across all call sites
rubric: design
severity: should_fix
---

**Rule.** Do not pass invariant values as parameters to internal helper routines
or intermediate multi-pass functions when the argument is always a fixed
constant across every call site. Hardcode or compute the fixed constant
internally to simplify the helper's signature and eliminate dead
configurability.

**Why.** Passing parameters that never vary across call sites adds unnecessary
signature complexity, register/stack overhead, and misleading configurability to
private helper functions.

## Examples

Bad:

```cpp
// Bad: output_stride is always 16 for the intermediate pass1 buffer.
static void idct16x16_pass1(const int16_t *in, int16_t *out,
                            int output_stride) {
  for (int i = 0; i < 16; ++i) {
    idct16_1d(in + i * 16, out + i * output_stride);
  }
}
```

Good:

```cpp
// Good: Hardcode fixed intermediate stride internally.
static void idct16x16_pass1(const int16_t *in, int16_t *out) {
  const int output_stride = 16;
  for (int i = 0; i < 16; ++i) {
    idct16_1d(in + i * 16, out + i * output_stride);
  }
}
```
