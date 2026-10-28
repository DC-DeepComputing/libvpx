---
name: libvpx/keep-compiler-name-and-qualifier-together-in-comments
description: When documenting compiler bug workarounds with version constraints, keep the compiler product name and executable qualifier together before comparison operators
rubric: readability
severity: nit
---

**Rule.** When documenting compiler bug workarounds and version thresholds, keep
the compiler product name and its executable qualifier together before any
version comparison operators (e.g., 'Visual Studio 2022 (cl.exe) < 17.7' rather
than 'Visual Studio 2022 < 17.7 (cl.exe)').

**Why.** Keeping the toolchain name and executable qualifier adjacent preserves
searchability across the codebase and cleanly binds the version comparison
operator to the toolchain version number.

## Examples

Bad:

```cpp
// Visual Studio 2022 < 17.7 (cl.exe) targeting AArch64 with optimizations
// enabled will fail with an internal compiler error. See:
```

Good:

```cpp
// Visual Studio 2022 (cl.exe) < 17.7 targeting AArch64 with optimizations
// enabled will fail with an internal compiler error. See:
```
