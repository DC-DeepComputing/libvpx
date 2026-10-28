---
name: libvpx/vpx/add-public-api-symbols-to-exports-files
description: When declaring new public API functions or interface symbols in vpx/ headers, add them to the corresponding exports file (such as vpx/exports_com)
rubric: correctness
severity: should_fix
---

**Rule.** When declaring new public API functions or interface symbols in vpx/
headers (such as vpx/vpx_image.h or vpx/vpx_codec.h), add the symbol names to
the appropriate shared library export definitions file (such as vpx/exports_com,
vpx/exports_enc, vpx/exports_dec, vp8/exports_*, or vp9/exports_*).

**Why.** libvpx generates shared library symbol visibility maps and module
definition files (.so, .def, .syms, .ver) from these export files. If a new
public symbol is declared in headers but omitted from the exports files, dynamic
library builds will not export the function and automated ABI checking tools
will fail to detect API additions.

## Examples

Bad:

```cpp
// In vpx/vpx_image.h (declared but not added to vpx/exports_com):
int vpx_img_plane_width(const vpx_image_t *img, int plane);
int vpx_img_plane_height(const vpx_image_t *img, int plane);
```

Good:

```cpp
// In vpx/vpx_image.h:
int vpx_img_plane_width(const vpx_image_t *img, int plane);
int vpx_img_plane_height(const vpx_image_t *img, int plane);

// In vpx/exports_com:
text vpx_img_plane_width
text vpx_img_plane_height
```
