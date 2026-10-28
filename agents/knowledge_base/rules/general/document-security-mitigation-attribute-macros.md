---
name: libvpx/document-security-mitigation-attribute-macros
description: When defining compiler attribute macros that bypass security or sanitizer mitigations (such as uninitialized or no_sanitize), document their purpose and strict usage criteria
rubric: readability
severity: nit
---

**Rule.** When defining macro wrappers around compiler attributes or sanitizer
annotations that disable security mitigations (such as `VPX_UNINITIALIZED` for
`__attribute__((uninitialized))` or `VPX_NO_UNSIGNED_OVERFLOW_CHECK` for
`no_sanitize`), document their purpose, safety/security implications, and
criteria for when they may be used. Explicitly note that they should not be
applied preemptively or 'just in case', but only to address measured performance
bottlenecks where refactoring or compiler optimization fails.

**Why.** Attributes that disable compiler security mitigations (like automatic
variable initialization or integer overflow sanitizers) eliminate safety
protections. Documenting their trade-offs and usage criteria prevents misuse and
ensures developers only use them as targeted workarounds for profiled
bottlenecks.

## Examples

Bad:

```cpp
#if __has_attribute(uninitialized)
#define VPX_UNINITIALIZED __attribute__((uninitialized))
#else
#define VPX_UNINITIALIZED
#endif
```

Good:

```cpp
#if __has_attribute(uninitialized)
// Attribute disables -ftrivial-auto-var-init=pattern for specific variables.
// -ftrivial-auto-var-init is a security risk mitigation feature, so the
// attribute should not be used "just in case", but only to fix real performance
// bottlenecks when other approaches do not work. If compiler optimization
// fails and code refactoring is hard, the attribute can be used as a workaround.
#define VPX_UNINITIALIZED __attribute__((uninitialized))
#else
#define VPX_UNINITIALIZED
#endif
```
