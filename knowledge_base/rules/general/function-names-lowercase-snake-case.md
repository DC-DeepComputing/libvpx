---
name: libvpx/function-names-lowercase-snake-case
description: When declaring or defining C functions, static inline helpers, or conversion routines, use lowercase snake_case rather than uppercase, PascalCase, or mixed-case identifiers
rubric: readability
severity: should_fix
---

**Rule.** Name all C functions, static inline helpers, and conversion routines
in lowercase snake_case (e.g., idct8x8_64_1d_bd8, vpx_convolve8_neon,
yv12_buffer_to_image_buffer). Do not use all-caps (ALL_CAPS), PascalCase, or
mixed-case identifiers derived from typedefs or struct names (e.g., IDCT8x8_1D,
ProcessMacroBlock, YV12_BUFFER_to_IMAGE_BUFFER). Reserve all-caps strictly for
preprocessor macros, header include guards, and compile-time constants.

**Why.** In libvpx C code, function names consistently follow lowercase
snake_case. Using all-caps, PascalCase, or mixed-case identifiers derived from
types obscures function calls, confuses functions with macros or type
declarations, violates codebase uniformity, and impairs readability.

## Examples

Bad:

```cpp
// Static inline helper and general function with uppercase or PascalCase names
static INLINE void IDCT8x8_1D(const int16x4_t cospis0, const int16x4_t cospis1,
                              int16x8_t *a0, int16x8_t *a1) {
  // ...
}

void ProcessMacroBlock(int16x8_t *dest, int stride) {
  // ...
}

// Conversion helper using uppercase typedef names
static void YV12_BUFFER_to_IMAGE_BUFFER(const YV12_BUFFER_CONFIG *yv12_buffer,
                                        IMAGE_BUFFER *image_buffer) {
  image_buffer->width = yv12_buffer->y_width;
  image_buffer->height = yv12_buffer->y_height;
}
```

Good:

```cpp
// Static inline helper and general function using lowercase snake_case
static INLINE void idct8x8_64_1d_bd8(const int16x4_t cospis0,
                                     const int16x4_t cospis1,
                                     int16x8_t *a0, int16x8_t *a1) {
  // ...
}

void process_macroblock(int16x8_t *dest, int stride) {
  // ...
}

// Conversion helper using lowercase snake_case
static void yv12_buffer_to_image_buffer(const YV12_BUFFER_CONFIG *yv12_buffer,
                                        IMAGE_BUFFER *image_buffer) {
  image_buffer->width = yv12_buffer->y_width;
  image_buffer->height = yv12_buffer->y_height;
}
```
