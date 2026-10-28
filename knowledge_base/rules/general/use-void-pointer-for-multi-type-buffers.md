---
name: libvpx/use-void-pointer-for-multi-type-buffers
description: When defining helper functions or multi-stage transform subroutines that accept buffers of varying types or bit depths, type parameters as const void * rather than forcing call-site casts
rubric: safety
severity: should_fix
---

**Rule.** Use const void * (or void * / const void *const) rather than casting
between incompatible concrete pointer types when a helper function or
multi-stage transform subroutine (such as a 1D row/column IDCT, IADST, or
multi-pass filter) accepts buffer inputs of varying types or bit depths across
passes (e.g., tran_low_t* or int32_t* in pass 1 and int16_t* in subsequent
passes). At call sites, pass pointers directly to the void * parameter without
explicit C-style casts.

**Why.** Casting pointers between incompatible non-character types (such as
tran_low_t* and int16_t*) violates C strict-aliasing rules and can lead to
compiler warnings, erroneous optimization assumptions, or undefined behavior.
Using const void* for multi-type buffer interfaces documents polymorphic input
handling and avoids aliasing hazards across call sites.

## Examples

Bad:

```cpp
// Bad: Helper forces concrete int16_t* pointer type despite handling multi-pass types
static void idct16x16_38_add_half1d(const int16_t *input, int16_t *output,
                                    uint8_t *dest, int stride) {
  if (output) {
    const tran_low_t *inputT = (const tran_low_t *)input;
    // Process first pass...
  }
}

void vpx_idct16x16_38_add_neon(const tran_low_t *input, uint8_t *dest,
                               int stride) {
  int16_t row_idct_output[16 * 16];
  // Caller forced to cast tran_low_t* to incompatible int16_t*
  idct16x16_38_add_half1d((const int16_t *)input, row_idct_output, dest, stride);
  idct16x16_38_add_half1d(row_idct_output, NULL, dest, stride);
}
```

Good:

```cpp
// Good: Parameter typed as const void *const; casts isolated internally
static void idct16x16_38_add_half1d(const void *const input, int16_t *output,
                                    uint8_t *dest, int stride) {
  if (output) {
    const tran_low_t *const inputT = (const tran_low_t *)input;
    // Process first pass...
  }
}

void vpx_idct16x16_38_add_neon(const tran_low_t *input, uint8_t *dest,
                               int stride) {
  int16_t row_idct_output[16 * 16];
  // Passed cleanly without incompatible call-site pointer casts
  idct16x16_38_add_half1d(input, row_idct_output, dest, stride);
  idct16x16_38_add_half1d(row_idct_output, NULL, dest, stride);
}
```
