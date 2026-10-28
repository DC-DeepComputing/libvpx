---
name: libvpx/keep-unrelated-fixes-in-separate-cls
description: When making focused functional or refactoring changes, submit unrelated drive-by bug fixes or cleanups in separate CLs instead of bundling them
rubric: change_scope
severity: should_fix
---

**Rule.** Do not combine unrelated drive-by fixes, refactorings, or cleanups
into a focused change. Submit unrelated modifications in separate, dedicated CLs
so each change remains self-contained, reviewable, and independently revertible.

**Why.** Mixing unrelated fixes into a single change creates review noise,
muddies commit history and git blame, complicates regression bisection, and
makes safe reverts difficult.

## Examples

Bad:

```cpp
// In a CL updating block error calculation to use tran_low_t:
// [vpx_dsp/x86/bitdepth_conversion_sse2.h]
-#ifndef VPX_DSP_X86_BITDEPTH_CONVERSION_H
+#ifndef VPX_DSP_X86_BITDEPTH_CONVERSION_SSE2_H
```

Good:

```cpp
// Keep the CL strictly focused on the tran_low_t refactoring.
// Submit the unrelated header guard fix in a separate, dedicated CL.
```
