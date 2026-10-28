---
name: libvpx/prefer-clock-monotonic-raw-for-elapsed-time
description: 'When measuring elapsed time with clock_gettime(), prefer CLOCK_MONOTONIC_RAW to avoid NTP slewing, guarding with #if defined(CLOCK_MONOTONIC_RAW) and falling back to CLOCK_MONOTONIC'
rubric: correctness
severity: should_fix
---

**Rule.** When measuring elapsed time or benchmarking durations with POSIX
clock_gettime(), prefer CLOCK_MONOTONIC_RAW over CLOCK_MONOTONIC. Because
CLOCK_MONOTONIC_RAW is a Linux-specific extension and not defined across all
target platforms or older toolchains, always guard its use with #if
defined(CLOCK_MONOTONIC_RAW) (or #elif defined(...)) and provide a fallback to
standard CLOCK_MONOTONIC.

**Why.** CLOCK_MONOTONIC is subject to frequency adjustments and slewing by NTP
or adjtime(), which can distort elapsed time measurements. CLOCK_MONOTONIC_RAW
provides raw hardware-based monotonic time unaffected by frequency slewing.
Guarding its usage with a preprocessor check and falling back to CLOCK_MONOTONIC
ensures portability and avoids compilation errors on non-Linux POSIX platforms
(such as macOS, BSDs, or older toolchains).

## Examples

Bad:

```cpp
// Variant 1: Unconditionally using CLOCK_MONOTONIC (subject to NTP slewing)
clock_gettime(CLOCK_MONOTONIC, &t->begin);

// Variant 2: Unconditionally using CLOCK_MONOTONIC_RAW (breaks non-Linux builds)
clock_gettime(CLOCK_MONOTONIC_RAW, &t->begin);
```

Good:

```cpp
// Guard Linux-specific CLOCK_MONOTONIC_RAW with fallback to standard CLOCK_MONOTONIC
#if defined(CLOCK_MONOTONIC_RAW)
clock_gettime(CLOCK_MONOTONIC_RAW, &t->begin);
#else
clock_gettime(CLOCK_MONOTONIC, &t->begin);
#endif
```
