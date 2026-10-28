---
name: libvpx/size-buffers-for-simd-overreads-not-sanitizers
description: When allocating buffers for architecture-specific SIMD vector operations, size for full vector over-reads in all target-architecture builds rather than gating buffer size on sanitizer flags
rubric: safety
severity: should_fix
---

**Rule.** Allocate sufficient buffer capacity for architecture-specific SIMD
vector over-reads (e.g., 8 or 16 bytes for NEON, SSE, or VSX loads) across all
builds of that target architecture, rather than gating buffer sizes behind
sanitizer macros like VPX_WITH_ASAN. Reserve sanitizer conditional guards
strictly for optional zero-initialization or padding hygiene, ensuring
non-sanitizer builds never execute out-of-bounds vector reads.

**Why.** SIMD instructions and intrinsics often load fixed-width vectors (such
as 8 or 16 bytes) even when only a subset of elements is logically required. If
buffer sizing is conditioned on sanitizer macros like VPX_WITH_ASAN,
non-sanitizer builds of that architecture will still execute the vector load and
perform an out-of-bounds read, causing memory safety issues, page faults, or
failures under other dynamic analysis tools.

## Examples

Bad:

```cpp
#if HAVE_NEON && VPX_WITH_ASAN
  unsigned char Left[8];
  vp8_zero_array(Left, 8);
#elif HAVE_VSX
  unsigned char Left[16];
#else
  unsigned char Left[4];
#endif
```

Good:

```cpp
#if HAVE_NEON
  unsigned char Left[8];
#if VPX_WITH_ASAN
  vp8_zero_array(Left, 8);
#endif  // VPX_WITH_ASAN
#elif HAVE_VSX
  unsigned char Left[16];
#else
  unsigned char Left[4];
#endif  // HAVE_NEON
```
