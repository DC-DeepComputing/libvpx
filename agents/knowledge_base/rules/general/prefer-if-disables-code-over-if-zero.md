---
name: libvpx/prefer-if-disables-code-over-if-zero
description: 'When disabling debug or diagnostic blocks inside functions, prefer if (/*DISABLES CODE*/ (0)) over #if 0 to prevent bitrot and suppress warnings'
rubric: readability
severity: nit
---

**Rule.** Prefer 'if (/*DISABLES CODE*/ (0))' or 'if ((0))' over '#if 0' when
disabling debugging, diagnostic, or visualization code inside functions. Keeping
the disabled block in an 'if (0)' statement ensures the compiler continues to
type-check and validate the code as surrounding types and APIs evolve, while the
'/*DISABLES CODE*/' comment or nested parentheses explicitly silence Clang
'-Wunreachable-code' warnings.

**Why.** '#if 0' completely strips code from the compilation unit, allowing it
to silently bitrot and break when surrounding data structures, variables, or
function signatures change. Using 'if (/*DISABLES CODE*/ (0))' keeps the block
compiled and type-checked across refactorings without triggering unreachable
code compiler diagnostics.

## Examples

Bad:

```cpp
// View first pass reconstruction frame for debugging.
#if 0
  char filename[512];
  FILE *recon_file;
  snprintf(filename, sizeof(filename), "enc%04d.yuv",
           (int)cm->current_video_frame);
  recon_file = fopen(filename, "wb");
  ...
#endif
```

Good:

```cpp
// View first pass reconstruction frame for debugging.
if (/*DISABLES CODE*/ (0)) {
  char filename[512];
  FILE *recon_file;
  snprintf(filename, sizeof(filename), "enc%04d.yuv",
           (int)cm->current_video_frame);
  recon_file = fopen(filename, "wb");
  ...
}
```
