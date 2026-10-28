---
name: libvpx/no-empty-fallback-memory-barriers
description: 'When defining architecture-specific memory barriers, use compiler barriers rather than hardware fences on x86, and emit #error instead of empty fallbacks'
rubric: safety
severity: should_fix
---

**Rule.** Do not define architecture-specific memory barrier macros as empty
no-ops, and do not use full hardware memory fences (such as Windows
MemoryBarrier(), _mm_mfence(), or mfence) on x86 or x86_64. On x86/x86_64, use a
lightweight compiler barrier (such as __asm__ __volatile__("" ::: "memory") or
MSVC _ReadWriteBarrier()) to prevent compiler reordering without runtime
serialization overhead. For unhandled or unsupported architectures, always emit
a #error directive rather than providing an empty fallback.

**Why.** On x86 and x86_64 processors, the hardware memory model (TSO)
guarantees acquire semantics for loads and release semantics for stores, so full
hardware memory fences introduce unnecessary pipeline stalls when only compiler
instruction reordering needs to be prevented. Conversely, providing empty
fallback macros permits compiler instruction reordering on x86 and silently
compiles unsynchronized memory accesses on weak memory models instead of
explicitly failing at build time.

## Examples

Bad:

```cpp
#if defined(_WIN32)
#include <winnt.h>
// Bad: MemoryBarrier() emits an expensive hardware fence instruction on x86.
#define vpx_atomic_memory_barrier() MemoryBarrier()
#elif VPX_ARCH_ARM
#define vpx_atomic_memory_barrier() __asm__ __volatile__("dmb ish" ::: "memory")
#else
// Bad: Empty fallback permits compiler reordering and silently breaks weak architectures.
#define vpx_atomic_memory_barrier()
#endif
```

Good:

```cpp
#if defined(_MSC_VER)
#include <intrin.h>
// Good: Compiler barrier prevents reordering on x86 with zero runtime cost.
#define vpx_atomic_memory_barrier() _ReadWriteBarrier()
#elif VPX_ARCH_X86 || VPX_ARCH_X86_64
#define vpx_atomic_memory_barrier() __asm__ __volatile__("" ::: "memory")
#elif VPX_ARCH_ARM
#define vpx_atomic_memory_barrier() __asm__ __volatile__("dmb ish" ::: "memory")
#elif VPX_ARCH_MIPS
#define vpx_atomic_memory_barrier() __asm__ __volatile__("sync" ::: "memory")
#else
#error Unsupported architecture!
#endif
```
