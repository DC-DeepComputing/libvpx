---
name: libvpx/prefer-scoped-enums-in-cpp
description: When defining C++ enumerations, prefer scoped enum class types with PascalCase names over unscoped enums, prefixing unscoped enumerators with the enum type name if required.
rubric: readability
severity: should_fix
---

**Rule.** When defining enumerations in C++ code, prefer scoped enumerations
(`enum class`) over unscoped C-style `enum` declarations. Name the scoped enum
type in `PascalCase` (e.g., `RcFrameType` rather than `RC_FRAME_TYPE`) and its
enumerators in constant style (`kConstantName` such as `kKeyFrame`). If unscoped
enums must be used (e.g., for C-interoperability in shared headers), prefix each
enumerator constant with the enum type name in CamelCase (`k<EnumName><Value>`,
e.g., `kRefFrameTypeLast` rather than `kLastRefFrame`) to prevent identifier
collisions in the enclosing scope.

**Why.** Scoped enumerations (`enum class`) isolate enumerator names within the
enum's scope, prevent namespace pollution in enclosing namespaces or classes,
eliminate accidental implicit conversions to integer types, and conform to
Google C++ Style Guide conventions. When unscoped enums cannot be avoided,
explicitly qualifying enumerator names with the enum type name prevents naming
collisions with common identifiers (such as `kOk` or `kError`) across modules.

## Examples

Bad:

```cpp
// Bad: Unscoped C-style enum with macro-style naming and implicit int conversion
enum RC_FRAME_TYPE {
  RC_KEY_FRAME = 0,
  RC_INTER_FRAME = 1,
};

// Bad: Unscoped enum constants omit the type name and pollute the enclosing scope
enum RefFrameType {
  kLastRefFrame = 0,
  kPastRefFrame = 1,
  kFutureRefFrame = 2,
};
```

Good:

```cpp
// Good: Scoped enum class with PascalCase type name and constant-style enumerators
enum class RcFrameType {
  kKeyFrame = 0,
  kInterFrame = 1,
};

// Good: If unscoped enums are required for C-interop, prefix with the enum type name
enum RefFrameType {
  kRefFrameTypeLast = 0,
  kRefFrameTypePast = 1,
  kRefFrameTypeFuture = 2,
};
```
