---
name: libvpx/exact-types-for-intrinsics-with-nolint
description: When compiler intrinsics, standard C functions (like fseek/ftell), or external APIs require built-in types like long, declare the exact type with NOLINT
rubric: readability
severity: should_fix
---

**Rule.** When interfacing with compiler intrinsics (such as MSVC
_BitScanForward), standard C library functions (such as fseek or ftell), or
platform APIs that require specific built-in integer types (such as long or
unsigned long), use the exact required type. Do not substitute incompatible
fixed-width types (like size_t or uint32_t) or leave linter warnings
unsuppressed; append '// NOLINT(runtime/int)' or '/* NOLINT */' to suppress
style linter warnings.

**Why.** Compiler intrinsics and C library functions expect exact built-in types
matching their signatures. Substituting fixed-width types like size_t* causes
compilation errors or memory corruption on platforms where type sizes differ
(such as LLP64 64-bit Windows), while omitting NOLINT triggers false-positive
style linter warnings on valid API bindings.

## Examples

Bad:

```cpp
/* Anti-pattern 1: Substituting size_t to silence cpplint */
size_t index;
_BitScanForward(&index, mask);

/* Anti-pattern 2: Leaving C standard library long typedef unannotated */
typedef long FileOffset;
```

Good:

```cpp
/* Example 1: Use exact unsigned long for intrinsic with NOLINT */
unsigned long index;  // NOLINT(runtime/int)
_BitScanForward(&index, mask);

/* Example 2: Use long for standard C API fallback with NOLINT */
typedef long FileOffset; /* NOLINT */
```
