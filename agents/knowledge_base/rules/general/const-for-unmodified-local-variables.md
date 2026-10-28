---
name: libvpx/const-for-unmodified-local-variables
description: Qualify unmodified local variables, structs, SIMD registers, function parameters, and pointer aliases with const (`const Type`, `Type *const`, `const Type *const`), and declare separate const intermediates
rubric: readability
severity: should_fix
---

**Rule.** Qualify function-local variables (including primitive scalars,
dimensions, strides, flags, structs like MV, unpacked va_arg values, and SIMD
vector registers such as __m128i, __m256i, __m512i, NEON
uint8x16_t/int16x8_t/int32x4_t, or VSX vector/v128_t), function/helper
parameters, and pointer aliases with const whenever their values or referenced
data are not modified after initialization. Specifically:
1. Qualify read-only input buffer pointers, lookup tables, and struct members as const Type * (e.g., const int16_t *coeff_ptr = b->coeff;), reserving non-const pointer types strictly for mutable destination buffers and output arrays (e.g., int16_t *qcoeff_ptr = d->qcoeff;).
2. Qualify immutable pointer variables and aliases as Type *const ptr (or const Type *const ptr when pointed-to data is also immutable, placing const after the asterisk, e.g., LAYER_CONTEXT *const lc = &svc->layer_context[layer];, const VP9_COMMON *const cm = &cpi->common;) to prevent accidental pointer rebinding.
3. In static inline helpers and SIMD kernels, qualify all unmodified by-value scalar and vector parameters as const.
4. Initialize local variables and cached dereferences (such as const int eob = *pd->eob; or struct member fetches) directly at their declaration point as const rather than leaving uninitialized declarations or repeatedly dereferencing.
5. Introduce separate const local variables for intermediate computation stages and separate channels rather than reassigning input registers or reusing a single mutable variable.

**Why.** Qualifying immutable local variables, structs, SIMD vector registers,
function parameters, and pointer aliases with const, const Type *, or Type
*const explicitly documents data flow and intent, prevents accidental mutation,
buffer corruption, state carryover, or unintended pointer rebinding at compile
time, and conforms to libvpx const-correctness style. Initializing values
directly upon declaration eliminates uninitialized read hazards and avoids
redundant repeated dereferencing. Creating separate const intermediate variables
rather than reusing mutable accumulator registers prevents state carryover bugs
across calculation stages and aids compiler optimization (such as register
allocation, caching, and alias analysis).

## Examples

Bad:

```cpp
// 1. Unmodified scalars, unpacked params, struct instances, uninitialized declarations:
int ystride = cpi->Source->y_stride;
int data = va_arg(args, int);
MV zero_mv = { 0, 0 };
int eob;
if (*pd->eob > 0) {
  inverse_transform_block_inter(..., *pd->eob);
}
eob = *pd->eob;

// 2. Read-only buffers and pointer aliases lacking const / *const:
int16_t *coeff_ptr = b->coeff;
LAYER_CONTEXT *lc = &svc->layer_context[layer];
const VP9_COMMON *cm = &cpi->common;

// 3. Mutable register reuse across intermediate computation steps:
__m256i p = _mm256_loadu_si256((const __m256i *)src);
const __m256i u = _mm256_loadu_si256((const __m256i *)dst);
p = _mm256_avg_epu16(p, u);
_mm256_storeu_si256((__m256i *)dst, p);

// 4. Helper parameters missing const for by-value vector and read-only pointee:
static INLINE void filter_sub(uint8x16_t a, uint8x8_t filter, uint8_t *src,
                              uint16x8_t *out) {
  uint16x8_t acc = vmovl_u8(vget_low_u8(a));
  *out = vpadalq_u8(*out, vmul_u8(vget_low_u8(a), filter));
}
```

Good:

```cpp
// 1. Qualify unmodified scalars, structs, params, and cached values with const:
const int ystride = cpi->Source->y_stride;
const int data = va_arg(args, int);
const MV zero_mv = { 0, 0 };
const int eob = *pd->eob;
if (eob > 0) {
  inverse_transform_block_inter(..., eob);
}

// 2. Read-only sources marked const Type *; pointer variables qualified with *const:
const int16_t *coeff_ptr = b->coeff;
int16_t *qcoeff_ptr = d->qcoeff;  // mutable destination remains non-const
LAYER_CONTEXT *const lc = &svc->layer_context[layer];
const VP9_COMMON *const cm = &cpi->common;

// 3. Declare separate const variables for intermediate computation stages:
const __m256i p = _mm256_loadu_si256((const __m256i *)src);
const __m256i u = _mm256_loadu_si256((const __m256i *)dst);
const __m256i avg = _mm256_avg_epu16(p, u);
_mm256_storeu_si256((__m256i *)dst, avg);

// 4. Qualify helper by-value vector inputs and read-only buffer pointers with const:
static INLINE void filter_sub(const uint8x16_t a, const uint8x8_t filter,
                              const uint8_t *src, uint16x8_t *out) {
  const uint16x8_t acc = vmovl_u8(vget_low_u8(a));
  *out = vpadalq_u8(*out, vmul_u8(vget_low_u8(a), filter));
}
```
