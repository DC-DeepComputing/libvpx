---
name: libvpx/sync-companion-header-platform-guards
description: When updating or generalizing platform preprocessor guards in implementation files, ensure companion headers providing platform includes maintain matching conditional guards
rubric: correctness
severity: should_fix
---

**Rule.** When generalizing or modifying platform preprocessor guards (such as
replacing compiler-specific _MSC_VER with OS-level _WIN32) in a source file,
ensure matching conditional guards in companion headers that provide necessary
platform headers (such as <windows.h>) are updated consistently. Do not leave
companion headers with narrower or divergent preprocessor guards when
implementation files rely on their declarations across other toolchains.

**Why.** If an implementation file uses platform APIs under a generalized guard
like _WIN32 while its companion header includes the required system header only
under a compiler-specific macro like _MSC_VER, builds targeting alternative
toolchains (such as MinGW or Clang on Windows) will fail due to missing
declarations or incomplete types.

## Examples

Bad:

```cpp
// arm_cpudetect.h
#if defined(_MSC_VER)
#include <windows.h>
#endif

// aarch64_cpudetect.c
#elif defined(_WIN32)
static int arm_get_cpu_caps(void) {
  // Uses IsProcessorFeaturePresent() from <windows.h>
  return IsProcessorFeaturePresent(PF_ARM_V8_INSTRUCTIONS_AVAILABLE);
}
```

Good:

```cpp
// arm_cpudetect.h
#if defined(_WIN32)
#include <windows.h>
#endif

// aarch64_cpudetect.c
#elif defined(_WIN32)
static int arm_get_cpu_caps(void) {
  // Uses IsProcessorFeaturePresent() from <windows.h>
  return IsProcessorFeaturePresent(PF_ARM_V8_INSTRUCTIONS_AVAILABLE);
}
```
