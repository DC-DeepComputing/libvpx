---
name: libvpx/guard-conditional-includes-with-config
description: 'When including headers required only for specific build configurations, wrap them in #if CONFIG_* guards placed after #include "./vpx_config.h"'
rubric: readability
severity: nit
---

**Rule.** Wrap #include directives that are required only for specific build
configurations inside #if CONFIG_* ... #endif blocks placed after #include
"./vpx_config.h". Do not unconditionally include headers (such as
vpx_util/vpx_debug_util.h, <stdio.h>, or <assert.h>) when their symbols are only
referenced in #if CONFIG_* code.

**Why.** Unconditionally including headers needed only in specific build
configurations introduces unnecessary dependencies, pollutes the header
namespace, and increases compilation overhead for standard builds.

## Examples

Bad:

```cpp
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

#include "./vpx_config.h"
#include "vpx_util/vpx_debug_util.h"
```

Good:

```cpp
#include <limits.h>
#include <stddef.h>

#include "./vpx_config.h"

#if CONFIG_BITSTREAM_DEBUG
#include <stdio.h>

#include "vpx_util/vpx_debug_util.h"
#endif  // CONFIG_BITSTREAM_DEBUG
```
