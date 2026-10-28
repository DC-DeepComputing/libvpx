---
name: libvpx/register-new-files-in-module-makefiles
description: When adding a new header or source file, register it in the subsystem module makefile under the appropriate architecture or feature configuration guard
rubric: correctness
severity: should_fix
---

**Rule.** When adding a new source (.c, .cc, .asm) or header (.h) file to any
subsystem, register it in the subsystem's module Makefile (*.mk, such as
vpx_dsp/vpx_dsp.mk or vp9/vp9_common.mk). Add the file path to the appropriate
*_SRCS- list using the corresponding architecture or feature configuration
variable (e.g., DSP_SRCS-$(HAVE_SSE2) or VP9_COMMON_SRCS-yes).

**Why.** Even if a header or source file compiles in local test builds when
directly included, omitting it from the subsystem makefile breaks build
dependency tracking, IDE project file generation, source distribution packaging,
and non-default build configurations.

## Examples

Bad:

```cpp
// vpx_dsp/x86/convolve_sse2.h is created and included by other files,
// but omitted from vpx_dsp/vpx_dsp.mk
```

Good:

```cpp
// In vpx_dsp/vpx_dsp.mk:
DSP_SRCS-$(HAVE_SSE2) += x86/convolve_sse2.h
```
