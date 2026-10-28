---
name: libvpx/use-standard-memory-ordering-names-for-atomics
description: When defining atomic operations or wrappers with memory ordering semantics, use standard terminology (relaxed, acquire, release, acq_rel, seq_cst) rather than ad-hoc terms like nobarrier
rubric: readability
severity: should_fix
---

**Rule.** When naming atomic wrappers, helper functions, or operations
associated with specific memory ordering semantics, use standard C11/C++11
memory order terminology ('relaxed', 'acquire', 'release', 'acq_rel', 'seq_cst')
as explicit suffixes or identifiers. Avoid legacy or ad-hoc terms such as
'nobarrier', 'no_barrier', or 'unordered'.

**Why.** Standard memory ordering names align directly with language standards
(C11 <stdatomic.h>, C++11 <atomic>) and compiler built-ins, making
synchronization guarantees and memory models immediately clear across
architectures.

## Examples

Bad:

```cpp
// Nonstandard 'nobarrier' obscures standard memory order semantics.
static INLINE int vpx_nobarrier_load_32(volatile atomic_int *x) {
  return atomic_load_explicit(x, memory_order_relaxed);
}

static INLINE void vpx_nobarrier_store_32(volatile atomic_int *x, int val) {
  atomic_store_explicit(x, val, memory_order_relaxed);
}
```

Good:

```cpp
// Explicit 'relaxed' suffix aligns with standard memory order semantics.
static INLINE int vpx_atomic_load_relaxed(volatile atomic_int *x) {
  return atomic_load_explicit(x, memory_order_relaxed);
}

static INLINE void vpx_atomic_store_relaxed(volatile atomic_int *x, int val) {
  atomic_store_explicit(x, val, memory_order_relaxed);
}
```
