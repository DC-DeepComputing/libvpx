---
name: libvpx/separate-cosmetic-refactoring-from-functional-patches
description: When submitting changes, separate distinct concerns—such as adding new features or variants versus refactoring, optimizing, or reformatting existing code—into dedicated, independent CLs
rubric: change_scope
severity: should_fix
---

**Rule.** Separate distinct or separable modifications—such as adding new
features or bitdepth variants versus refactoring or optimizing existing
implementations, or cosmetic formatting versus functional changes—into
independent CLs. Do not bundle unrelated refactorings, optimizations, or
formatting cleanups into patches that introduce new functionality or bug fixes.

**Why.** Bundling distinct concerns in a single patch inflates diff complexity,
obscures core functional changes during review, complicates git bisection when
diagnosing regressions, and prevents independently reverting one component
without unnecessarily rolling back another.

## Examples

Bad:

```cpp
// Bad: Bundling new feature variants and refactoring existing code in one CL:
// 1. Adds new high-bitdepth NEON quantize functions.
// 2. Simultaneously refactors existing standard-bitdepth NEON quantize functions.

// Bad: Bundling cosmetic reordering with functional changes in one CL:
-void vpx_idct16x16_10_add_c(...) { ... }
-void vpx_idct16x16_38_add_c(...) { ... }
+void vpx_idct16x16_38_add_c(...) { ... }
+void vpx_idct16x16_10_add_c(...) { ... }
```

Good:

```cpp
// Good: Isolate separable changes into independent CLs:
// CL 1: Refactor or optimize existing standard-bitdepth NEON quantize functions.
// CL 2: Add new high-bitdepth NEON quantize functions.

// Good: Keep function ordering untouched in functional patches:
void vpx_idct16x16_10_add_c(...) { ... }
void vpx_idct16x16_38_add_c(...) { ... }
// Submit cosmetic reordering or formatting normalization in a dedicated cleanup CL.
```
