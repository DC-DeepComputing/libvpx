---
name: libvpx/scope-module-internal-variables-to-source-files
description: When declaring module-internal variables or file pointers, define them with static internal linkage in source files rather than exposing globals in headers
rubric: readability
severity: should_fix
---

**Rule.** Do not declare or define global variables in header files when their
usage is confined to a single module or translation unit. Define internal
variables and file handles with static internal linkage directly inside the .c
or .cc source file where they are used.

**Why.** Exposing module-internal variables in header files pollutes the global
namespace, breaks encapsulation, and risks multiple definition or link errors
across translation units. Giving them static internal linkage in the source file
restricts visibility and allows compiler optimizations.

## Examples

Bad:

```cpp
// In header file (e.g., vpx_dsp/skin_detection.h)
#ifdef OUTPUT_YUV_SKINMAP
FILE *yuv_skinmap_file;
#endif
```

Good:

```cpp
// In source file (e.g., vpx_dsp/skin_detection.c)
#ifdef OUTPUT_YUV_SKINMAP
static FILE *yuv_skinmap_file = NULL;
#endif
```
