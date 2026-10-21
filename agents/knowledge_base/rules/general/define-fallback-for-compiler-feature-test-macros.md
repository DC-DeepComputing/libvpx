---
name: libvpx/define-fallback-for-compiler-feature-test-macros
rubric: correctness
severity: blocking
---

**Rule.** Before invoking compiler feature-test macros (such as __has_attribute,
__has_builtin, or __has_feature) in preprocessor #if or #elif expressions,
provide a fallback definition expanding to 0 (e.g., #if
!defined(__has_attribute) / #define __has_attribute(x) 0 / #endif) or ensure the
check is guarded by defined(). Do not invoke undefined function-like
feature-test macros directly in preprocessor expressions without a fallback.

**Why.** Compilers such as MSVC or older toolchains do not define Clang/GCC
feature-test macros natively. Invoking an undefined function-like macro directly
in an #if expression causes preprocessor syntax errors and breaks MSVC builds.

## Examples

Bad:

```cpp
// Direct invocation without fallback breaks MSVC / non-supporting compilers
#if __has_attribute(uninitialized)
#define VPX_UNINITIALIZED __attribute__((uninitialized))
#else
#define VPX_UNINITIALIZED
#endif
```

Good:

```cpp
#if !defined(__has_attribute)
#define __has_attribute(x) 0
#endif  // !defined(__has_attribute)

#if __has_attribute(uninitialized)
#define VPX_UNINITIALIZED __attribute__((uninitialized))
#else
#define VPX_UNINITIALIZED
#endif  // __has_attribute(uninitialized)
```
