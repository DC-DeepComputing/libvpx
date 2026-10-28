---
name: libvpx/declare-variables-before-statements-c89
description: In C source and header files and macros, declare local variables and loop counters at the beginning of the block before executable statements for C89/C90 compatibility
rubric: readability
severity: should_fix
---

**Rule.** In C source and header files and preprocessor macros, declare all
local variables, loop counters, and indices at the beginning of the enclosing
function or block scope before any executable statements. Do not declare
variables inside for loop headers (such as 'for (int i = 0; ...)'), and do not
mix variable declarations after executable logic.

**Why.** libvpx targets ISO C89 (C90 / ANSI C) standard compliance for broad
toolchain and embedded platform portability. In C89/C90, variable declarations
inside for loop headers and declarations placed after executable statements are
invalid and cause compilation errors or warnings on strict C89 toolchains.

## Examples

Bad:

```cpp
#define PACK_STORE(v0, v1)                          \
  vec_vsx_st(output_v, 0, tmp_dest);                \
  for (int i = 0; i < 4; i++)                       \
    dest[i] = tmp_dest[i];

const int gf_index = cpi->twopass.gf_group.index;
if (gf_index >= MAX_ARF_GOP_SIZE) return orig_rdmult;
TplDepFrame *tpl_frame = &cpi->tpl_stats[gf_index];
```

Good:

```cpp
int i;
#define PACK_STORE(v0, v1)                          \
  vec_vsx_st(output_v, 0, tmp_dest);                \
  for (i = 0; i < 4; i++)                           \
    dest[i] = tmp_dest[i];

const int gf_index = cpi->twopass.gf_group.index;
TplDepFrame *tpl_frame;
if (gf_index >= MAX_ARF_GOP_SIZE) return orig_rdmult;
tpl_frame = &cpi->tpl_stats[gf_index];
```
