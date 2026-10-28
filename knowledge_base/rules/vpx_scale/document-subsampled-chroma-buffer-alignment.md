---
name: libvpx/vpx_scale/document-subsampled-chroma-buffer-alignment
description: When calculating frame buffer stride, border, or alignment to accommodate subsampled chroma planes, add comments explaining why luma alignment is scaled for SIMD instructions
rubric: readability
severity: should_fix
---

**Rule.** When configuring frame buffer row stride, border padding, or byte
alignment to accommodate subsampled chroma planes (such as 4:2:0 or 4:2:2) or
high-bitdepth pixels, add an explanatory comment detailing why the luma
alignment or border is scaled. Explicitly document the target SIMD instruction
requirements (such as 32-byte or 64-byte AVX-512 vector alignment) and how
halving the stride for chroma planes dictates the doubled luma alignment.

**Why.** Without explanatory comments, doubling row alignment or border padding
constraints when handling high bitdepth or wider SIMD architectures appears
redundant or arbitrary. Explaining that subsampled chroma planes have half the
stride clarifies why luma dimensions must be doubled to guarantee that chroma
rows meet SIMD load and store alignment boundaries.

## Examples

Bad:

```cpp
if (use_highbitdepth) {
  // two bytes per pixel
  border <<= 1;
  row_alignment <<= 1;
}
```

Good:

```cpp
if (use_highbitdepth) {
  // Two bytes per pixel; align chroma rows to 64-byte boundaries
  // so AVX-512 can access 32 pixels at a time with aligned loads/stores.
  border <<= 1;
  row_alignment <<= 1;
}
```
