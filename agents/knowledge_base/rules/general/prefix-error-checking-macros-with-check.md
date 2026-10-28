---
name: libvpx/prefix-error-checking-macros-with-check
description: When defining error-checking or validation helper macros, prefix macro identifiers with CHECK_ rather than ad-hoc prefixes like ON_
rubric: readability
severity: nit
---

**Rule.** When defining helper macros that validate operations, check return
codes, or handle error conditions, prefix the macro identifier with CHECK_ (such
as CHECK_FSCANF_ERROR, CHECK_MEM_ERROR, or CHECK_BOUNDS). Avoid introducing
non-standard or ad-hoc prefixes such as ON_ or HANDLE_ for validation macros.

**Why.** Adhering to the standard CHECK_ prefix for validation and
error-handling helper macros maintains naming consistency with established
macros across libvpx (e.g., CHECK_MEM_ERROR) and makes error-checking control
flow immediately recognizable.

## Examples

Bad:

```cpp
#define ON_FSCANF_ERROR(expr)                         \
  do {                                                \
    if ((expr) == EOF) {                              \
      fprintf(stderr, "Reading TPL stats failed.\n"); \
      return VPX_CODEC_ERROR;                         \
    }                                                 \
  } while (0)
```

Good:

```cpp
#define CHECK_FSCANF_ERROR(expr)                      \
  do {                                                \
    if ((expr) == EOF) {                              \
      fprintf(stderr, "Reading TPL stats failed.\n"); \
      return VPX_CODEC_ERROR;                         \
    }                                                 \
  } while (0)
```
