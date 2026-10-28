---
name: libvpx/prefer-plain-int-for-local-variables
description: Prefer standard int for local variables, loop counters, image plane indices, and bounded counters instead of fixed-width types (int32_t, int64_t) or size_t
rubric: readability
severity: nit
---

**Rule.** Prefer standard int (or unsigned int where non-negativity is required
by an API contract) over fixed-width types (int32_t, uint32_t, int64_t,
uint64_t) or size_t for local intermediate arithmetic variables, temporaries,
arithmetic casts, loop counters, array indices, image plane indices/parameters
(such as plane in image accessor helpers like vpx_img_plane_width), and bounded
counts/dimensions (such as tile, row, column, block, or worker counts/indices).

- Reserve <stdint.h> exact-width types (int32_t, uint32_t, int16_t, uint8_t, etc.) for struct memory buffer layouts, SIMD vector registers, bitstream definitions, or binary interface contracts.
- Reserve 64-bit types (int64_t, uint64_t) strictly for values genuinely requiring 64-bit range, such as timestamps, high-capacity bit budgets, or rate-control accumulators.
- Reserve size_t for byte sizes, buffer allocations, capacities, and byte offset calculations rather than small bounded counters or plane indices.

**Why.** In C and C++, standard int is the native, idiomatic scalar type for
intermediate arithmetic and loop iteration. Using fixed-width types like int32_t
or uint32_t for local temporaries introduces visual clutter and style
inconsistency without adding portability or overflow safety. Using 64-bit types
(int64_t, uint64_t) for small, bounded quantities inflates struct footprints,
increases cache pressure, and introduces 64-bit arithmetic overhead and
register-pair spilling on 32-bit architectures. Similarly, image planes
(typically 0..2 or 0..3) and dimension counts are naturally small bounded
integers; using int matches libvpx conventions, avoids unnecessary unsigned
conversions, and reserves size_t strictly for memory and byte buffer sizing.

## Examples

Bad:

```cpp
// Bad: using fixed-width types for local variables, casts, and loop counters
const int32_t diff = (int32_t)(src[i] - ref[i]);
for (int32_t i = 0; i < num_coeffs; ++i) {
  process_coeff(diff, i);
}

// Bad: using 64-bit integer types for small, bounded counters and indices
struct DecoderContext {
  int64_t num_tiles_done;
  uint64_t row_count;
  int64_t worker_idx;
};

// Bad: using size_t or unsigned types for image plane indices and parameters
static int vpx_img_plane_width(const vpx_image_t *img, size_t plane);
for (size_t plane = 0; plane < 3; ++plane) {
  uint8_t *buf = img->planes[plane];
}
```

Good:

```cpp
// Good: use standard int for local variables, casts, and loop counters
const int diff = (int)(src[i] - ref[i]);
for (int i = 0; i < num_coeffs; ++i) {
  process_coeff(diff, i);
}

// Good: use standard int for small, bounded counters and indices
struct DecoderContext {
  int num_tiles_done;
  int row_count;
  int worker_idx;
};

// Good: use standard int for image plane indices and parameters
static int vpx_img_plane_width(const vpx_image_t *img, int plane);
for (int plane = 0; plane < 3; ++plane) {
  uint8_t *buf = img->planes[plane];
}
```
