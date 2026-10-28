---
name: libvpx/prefix-local-header-includes-with-dot-slash
description: When including local same-directory or root-level generated/helper headers (such as vpx_config.h, *_rtcd.h, ivfenc.h), prefix the path with './'
rubric: readability
severity: should_fix
---

**Rule.** When including same-directory headers or root-level generated and
helper headers (such as `vpx_config.h`, `vpx_dsp_rtcd.h`, `vp8_rtcd.h`,
`vp9_rtcd.h`, `vpx_scale_rtcd.h`, `ivfenc.h`, `tools_common.h`, `y4menc.h`,
etc.), specify the path with a leading `"./"` prefix (e.g., `#include
"./vpx_config.h"`, `#include "./vpx_dsp_rtcd.h"`, `#include "./ivfenc.h"`). Do
not use bare relative filenames like `#include "vpx_config.h"` or `#include
"ivfenc.h"`.

**Why.** libvpx generates configuration and RTCD headers at the root of the
build tree and places local helper headers alongside tools and tests. Using an
explicit `"./"` prefix maintains project-wide include uniformity and prevents
cpplint style warnings from flagging root-generated and local headers as
non-standard paths.

## Examples

Bad:

```cpp
// 1. Bare root configuration and RTCD headers:
#include "vpx_config.h"
#include "vpx_dsp_rtcd.h"
#include "vpx/vpx_integer.h"

// 2. Bare local tool/helper headers:
#include "ivfenc.h"
#include "tools_common.h"
```

Good:

```cpp
// 1. Leading './' prefix on root configuration and RTCD headers:
#include "./vpx_config.h"
#include "./vpx_dsp_rtcd.h"
#include "vpx/vpx_integer.h"

// 2. Leading './' prefix on local tool/helper headers:
#include "./ivfenc.h"
#include "./tools_common.h"
```
