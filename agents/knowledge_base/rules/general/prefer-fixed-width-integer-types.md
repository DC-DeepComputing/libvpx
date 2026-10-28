---
name: libvpx/prefer-fixed-width-integer-types
description: Prefer explicit fixed-width types (uint8_t, int8_t, int16_t) or int over plain char, unsigned char, or short for buffers, struct members, and numeric variables.
rubric: readability
severity: should_fix
---

**Rule.** Prefer standard fixed-width integer types from <stdint.h> or
vpx/vpx_integer.h (uint8_t, int8_t, int16_t, uint16_t, int32_t, uint32_t) or
standard int over primitive C types (char, unsigned char, signed char, short,
unsigned short) for buffer pointers, function parameters, struct members, and
numeric variables.

- Use uint8_t* for pixel or unsigned byte buffers, and int8_t* for signed sample, noise, or count arrays (e.g., eobs).
- Qualify read-only input buffer parameters with const (e.g., const uint8_t* or const int8_t*).
- Use standard int for loop counters, array indices, and arithmetic iteration (or fixed-width types when exact bit-width or register constraints apply).
- Reserve plain char strictly for human-readable text strings and null-terminated character data.

**Why.** In C, plain char has implementation-defined signedness across target
architectures and compilers (e.g., signed on x86 but unsigned on ARM, MIPS, and
PowerPC), causing unexpected sign-extension bugs, arithmetic discrepancies, and
DSP warnings. Primitive types like short and unsigned char obscure bit width.
Using standard <stdint.h> types guarantees portability and consistent signedness
across all target platforms while matching libvpx codec and DSP interface
conventions, and const enforces buffer immutability.

## Examples

Bad:

```cpp
// Primitive types for buffers and missing const qualification
void vp8_dequant_idct_add_uv_block_mmi(int16_t *q, int16_t *dq,
                                       unsigned char *dstu, unsigned char *dstv,
                                       int stride, char *eobs);
void vpx_plane_add_noise_c(uint8_t *start, char *noise, int width, int height);

// Plain char used for loop counters
for (char i = 0; i < 4; ++i) {
  process_block(i);
}
```

Good:

```cpp
// Fixed-width stdint types with const qualification for read-only buffers
void vp8_dequant_idct_add_uv_block_mmi(int16_t *q, int16_t *dq,
                                       uint8_t *dstu, uint8_t *dstv,
                                       int stride, int8_t *eobs);
void vpx_plane_add_noise_c(uint8_t *start, const int8_t *noise, int width, int height);

// Standard int used for numeric loop counters
for (int i = 0; i < 4; ++i) {
  process_block(i);
}
```
