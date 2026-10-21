---
name: libvpx/vpx/include-extracted-headers-in-original-public-headers
rubric: correctness
severity: blocking
---

**Rule.** When extracting types, data structures, or typedefs from an existing
public API header in vpx/ into a new header file, #include the new header in the
original header (or its dependent interface headers). Do not move public type
definitions into a separate header without maintaining include dependencies in
the original public headers.

**Why.** Downstream applications and external callers rely on including
top-level public headers (such as vpx_encoder.h or vpx_ext_ratectrl.h) for
library type definitions. Omitting the #include of the newly extracted header
breaks source compatibility and causes compiler errors in existing client code.

## Examples

Bad:

```cpp
// In vpx/vpx_encoder.h (after extracting VpxTpl* definitions to vpx_tpl.h):
#include "./vpx_codec.h"
#include "./vpx_ext_ratectrl.h"
// Missing #include "./vpx_tpl.h" breaks callers relying on vpx_encoder.h for VpxTpl types
```

Good:

```cpp
// In vpx/vpx_encoder.h:
#include "./vpx_codec.h"
#include "./vpx_ext_ratectrl.h"
#include "./vpx_tpl.h"  // Preserves API compatibility for existing callers
```
