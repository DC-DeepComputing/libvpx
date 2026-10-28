---
name: libvpx/avoid-non-ascii-characters
description: When writing comments, documentation, or code in source and header files, use plain 7-bit ASCII characters rather than Unicode or non-ASCII symbols
rubric: readability
severity: nit
---

**Rule.** Use standard 7-bit ASCII characters in source files, headers, and
comments; do not include non-ASCII Unicode characters such as trademark symbols
(e.g., '®', '™'), typographic quotes, or em dashes. Replace special symbols with
plain ASCII equivalents or spelled-out words.

**Why.** Non-ASCII characters can cause rendering glitches, encoding mismatches,
and compatibility issues across diverse terminal environments, diff tools,
compilers, and ASCII-only developer tooling.

## Examples

Bad:

```cpp
// Intel® 64 and IA-32 Architectures Developer's Manual: Vol. 1
// Frame rate — default is 30 fps
const char *msg = "Copyright © 2026 Google LLC";
```

Good:

```cpp
// Intel 64 and IA-32 Architectures Developer's Manual: Vol. 1
// Frame rate - default is 30 fps
const char *msg = "Copyright (c) 2026 Google LLC";
```
