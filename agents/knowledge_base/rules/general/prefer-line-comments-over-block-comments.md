---
name: libvpx/prefer-line-comments-over-block-comments
description: When adding single-line, multi-line, or inline code comments in C and C++ source and header files, prefer `//` line comments over `/* ... */` block comments.
rubric: readability
severity: nit
---

**Rule.** In C and C++ source and header files, prefer `//` line comments over
C-style `/* ... */` block comments for single-line, multi-line, and inline
explanatory code comments. Reserve `/* ... */` block comments for Doxygen or
public API header documentation where required by existing file conventions.

**Why.** Using `//` line comments maintains uniform formatting across libvpx,
avoids cumbersome multi-line `/* ... */` delimiter syntax for standard notes,
and prevents syntax errors or nesting conflicts when commenting out surrounding
code blocks during development.

## Examples

Bad:

```cpp
/* Per-frame PSNR is not supported when g_lag_in_frames is greater than 0.
 */
if (ctx->cfg.g_lag_in_frames > 0) {
  return VPX_CODEC_INVALID_PARAM;
}

int stride = 32; /* frame stride in bytes */
```

Good:

```cpp
// Per-frame PSNR is not supported when g_lag_in_frames is greater than 0.
if (ctx->cfg.g_lag_in_frames > 0) {
  return VPX_CODEC_INVALID_PARAM;
}

int stride = 32;  // frame stride in bytes
```
