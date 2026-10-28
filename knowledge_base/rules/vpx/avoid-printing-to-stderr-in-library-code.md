---
name: libvpx/vpx/avoid-printing-to-stderr-in-library-code
description: When handling errors or diagnostics in library code, return status error codes such as vpx_codec_err_t instead of printing directly to stderr
rubric: readability
severity: should_fix
---

**Rule.** Do not output error messages or diagnostic logs directly to stderr or
stdout from core library code (vpx/, vp8/, vp9/, vpx_dsp/). Return an error
status code (such as vpx_codec_err_t or vpx_rc_status_t) to the caller instead,
or gate diagnostic printing strictly behind debug configuration macros (such as
CONFIG_DEBUG or CONFIG_BITSTREAM_DEBUG).

**Why.** Libvpx is embedded as a shared or static library across diverse host
applications and runtimes. Writing directly to stderr bypasses host logging
frameworks, pollutes standard error streams, and breaks library encapsulation,
whereas returning error codes allows callers to handle failures appropriately.

## Examples

Bad:

```cpp
#define CHECK_WRITE_OR_RETURN(expr)                   \
  do {                                                \
    if ((expr) < 0) {                                 \
      fprintf(stderr, "Writing TPL stats failed.\n"); \
      return VPX_CODEC_ERROR;                         \
    }                                                 \
  } while (0)
```

Good:

```cpp
#define CHECK_WRITE_OR_RETURN(expr) \
  do {                              \
    if ((expr) < 0) {               \
      return VPX_CODEC_ERROR;       \
    }                               \
  } while (0)
```
