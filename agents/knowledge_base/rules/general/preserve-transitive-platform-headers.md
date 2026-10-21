---
name: libvpx/preserve-transitive-platform-headers
rubric: correctness
severity: blocking
---

**Rule.** Do not remove platform abstraction or compatibility headers (such as
vpx_util/vpx_thread.h, vpx_util/vpx_atomics.h, or vpx_ports/*.h) from common
headers without auditing indirect callers across all target platforms. If
callers rely on transitive includes on secondary platforms not tested by
standard CI trybots (such as MSVC or Windows threading), keep the header
included or update all indirect callers simultaneously.

**Why.** In codebases where callers rely on transitive includes for platform
abstractions (such as pthread emulation or architecture-specific atomics on
Windows), removing an include that seems locally unused will silently break
compilation on non-trybot platforms.

## Examples

Bad:

```cpp
// vp8/common/threading.h
#if ARCH_X86 || ARCH_X86_64
#include "vpx_ports/x86.h"
#else
#define x86_pause_hint()
#endif

// Removed vpx_util/vpx_thread.h because threading.h doesn't use it directly;
// breaks downstream files on Windows/MSVC that relied on transitive inclusion.
#include "vpx_util/vpx_atomics.h"
```

Good:

```cpp
// vp8/common/threading.h
#if ARCH_X86 || ARCH_X86_64
#include "vpx_ports/x86.h"
#else
#define x86_pause_hint()
#endif

// Retain platform abstraction header required by indirect callers across platforms.
#include "vpx_util/vpx_thread.h"
#include "vpx_util/vpx_atomics.h"
```
