---
name: libvpx/include-self-guarding-headers-unconditionally
description: 'When including headers that internally guard their contents with configuration macros, include them unconditionally rather than wrapping #include in #if guards'
rubric: readability
severity: nit
---

**Rule.** Do not wrap #include directives in preprocessor conditional blocks
(#if CONFIG_* or #if HAVE_*) when the included header internally guards its own
definitions or provides no-op fallback stubs. Include self-guarding headers
unconditionally in the standard project header section.

**Why.** Wrapping #include directives in #if blocks is redundant when the header
already evaluates configuration macros internally. Unconditional inclusion keeps
include lists clean, consistent, and maintainable across translation units.

## Examples

Bad:

```cpp
#if CONFIG_MULTITHREAD
#include "vp8/common/threading.h"
#include "vp8/encoder/ethreading.h"
#endif
```

Good:

```cpp
#include "vp8/common/threading.h"

#if CONFIG_MULTITHREAD
#include "vp8/encoder/ethreading.h"
#endif
```
