---
name: libvpx/vpx/keep-tool-helpers-in-tools-common
description: Place CLI helpers, format parsers, and file I/O functions using FILE* in tools_common rather than public library headers under vpx/
rubric: design
severity: should_fix
---

**Rule.** Do not declare or expose tool-, CLI-, or example-specific helper
functions, format parsers, or file I/O utilities (such as functions operating on
FILE * or stream parsing helpers) in public library headers under vpx/ (e.g.,
vpx/vpx_image.h). Keep CLI and sample utilities encapsulated in
tools_common.{c,h} (or dedicated example helper headers) unless there is an
intentional requirement to expand the public libvpx ABI.

**Why.** Public headers in vpx/ define the core libvpx API, which is designed
for in-memory codec and image processing without coupling to filesystem streams
or platform I/O mechanisms. Exposing tool utilities or stdio dependencies (FILE
*) in public headers bloats the shared library interface, burdens embedded or
memory-only consumers with unnecessary I/O dependencies, and creates permanent
ABI backward-compatibility obligations.

## Examples

Bad:

```cpp
// In public header vpx/vpx_image.h:
#include <stdio.h>

void vpx_img_write(const vpx_image_t *img, FILE *file);
int vpx_img_read(vpx_image_t *img, FILE *file);
```

Good:

```cpp
// In tools_common.h:
#include <stdio.h>
#include "vpx/vpx_image.h"

void vpx_img_write(const vpx_image_t *img, FILE *file);
int vpx_img_read(vpx_image_t *img, FILE *file);
```
