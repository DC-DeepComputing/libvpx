---
name: libvpx/document-in-place-buffer-stride-safety
description: When updating frame buffer strides or dimensions in place without reallocating plane buffer pointers, document the capacity invariants ensuring the modification is safe
rubric: readability
severity: nit
---

**Rule.** When updating frame buffer layout metadata (such as y_stride,
uv_stride, or image dimensions in YV12_BUFFER_CONFIG or vpx_image_t) in place
without reallocating or adjusting the underlying plane buffer pointers
(y_buffer, u_buffer, v_buffer), add an explanatory comment documenting the
invariant that makes the in-place modification safe (such as dimensions being
bounded by existing allocated buffer capacity).

**Why.** Frame buffer plane pointers and their strides are tightly coupled.
Modifying strides or dimensions in place without updating or reallocating the
backing memory can easily be mistaken for an out-of-bounds bug or memory
corruption hazard unless the safety invariant is explicitly documented.

## Examples

Bad:

```cpp
} else if (new_dimensions) {
  buf->img.y_width = src->y_width;
  buf->img.y_height = src->y_height;
  buf->img.y_stride =
      ALIGN_POWER_OF_TWO((aligned_width + 2 * buf->img.border), 5);
  buf->img.uv_stride = buf->img.y_stride >> subsampling_x;
}
```

Good:

```cpp
} else if (new_dimensions) {
  buf->img.y_width = src->y_width;
  buf->img.y_height = src->y_height;
  // New width is <= previous width (otherwise larger_dimensions branch is
  // taken), so updating stride in place within existing buffer capacity is safe.
  buf->img.y_stride =
      ALIGN_POWER_OF_TWO((aligned_width + 2 * buf->img.border), 5);
  buf->img.uv_stride = buf->img.y_stride >> subsampling_x;
}
```
