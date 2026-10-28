---
name: libvpx/lowercase-variable-and-parameter-names
description: When declaring function parameters, local variables, or local constants in C/C++ code, use lowercase snake_case names without capitalized identifiers or k/k__ prefixes
rubric: readability
severity: should_fix
---

**Rule.** Name function parameters, local variables, and function-local const
variables (including SIMD vector constants) using all-lowercase snake_case
identifiers (e.g., sum, sse, src_ptr, nonzero_bias_a) rather than capitalized,
PascalCase, or ALL_CAPS names (e.g., Sum, SSE, IntermediateVal). Do not use `k`,
`k_`, or `k__` prefixes for function-local constants or vectors; reserve `k` and
`kCamelCase` prefixes strictly for global or file-scope constants with static
storage duration. Additionally, keep parameter names consistent across header
declarations and function definitions.

**Why.** Standard lowercase snake_case naming maintains consistent style across
libvpx source and headers, clearly distinguishing local variables and parameters
from capitalized macros, constants, and typedef types. In libvpx conventions,
`k` and `kCamelCase` prefixes signal static or global storage duration constants
(such as `kCospi`); using `k` or Hungarian-style prefixes like `k__` on
function-local constants obscures variable scope and lifetime, creating
inconsistency across DSP and codec implementations.

## Examples

Bad:

```cpp
// Capitalized or ALL_CAPS parameters and local variables
void vpx_get16x16var_avx2(const uint8_t *src_ptr, int src_stride,
                          const uint8_t *ref_ptr, int ref_stride,
                          unsigned int *SSE, int *Sum) {
  int IntermediateVal = 0;
  *((int *)Sum) = _mm_cvtsi128_si32(...);
}

// Function-local constants with k__ prefix
static INLINE void load_buffer_4x4(const int16_t *input, int16x8_t *in, int stride) {
  const int16x8_t k__nonzero_bias_a = { 0, 1, 1, 1, 1, 1, 1, 1 };
  const int16x8_t k__nonzero_bias_b = { 1, 0, 0, 0, 0, 0, 0, 0 };
}
```

Good:

```cpp
// Lowercase snake_case parameters and local variables
void vpx_get16x16var_avx2(const uint8_t *src_ptr, int src_stride,
                          const uint8_t *ref_ptr, int ref_stride,
                          unsigned int *sse, int *sum) {
  int intermediate_val = 0;
  *((int *)sum) = _mm_cvtsi128_si32(...);
}

// Function-local constants using standard snake_case without k prefix
static INLINE void load_buffer_4x4(const int16_t *input, int16x8_t *in, int stride) {
  const int16x8_t nonzero_bias_a = { 0, 1, 1, 1, 1, 1, 1, 1 };
  const int16x8_t nonzero_bias_b = { 1, 0, 0, 0, 0, 0, 0, 0 };
}
```
