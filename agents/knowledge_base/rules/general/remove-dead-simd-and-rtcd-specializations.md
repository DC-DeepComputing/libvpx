---
name: libvpx/remove-dead-simd-and-rtcd-specializations
description: When disabling or deprecating unused SIMD optimizations, remove their source files, build rules, and RTCD specialization entries completely rather than bypassing with _c
rubric: readability
severity: should_fix
---

**Rule.** When disabling an underperforming or unused SIMD optimization, remove
the SIMD source file, its build rules (*.mk/CMakeLists.txt), and its RTCD
specialization entry (*_rtcd_defs.pl) completely rather than leaving dead code
and overriding function pointers or calls to the _c baseline at runtime. If all
SIMD specializations for a function are removed, eliminate the RTCD declaration
as well.

**Why.** Bypassing SIMD implementations at runtime by hardcoding _c leaves dead
assembly and C code in the repository, wastes compilation time, bloats binary
size, and creates ongoing maintenance overhead.

## Examples

Bad:

```cpp
// Bypassing AVX implementation at runtime while leaving dead code and RTCD entry:
cpi->diamond_search_sad = vp9_diamond_search_sad_c;
```

Good:

```cpp
// Remove unused AVX implementation, remove avx from vp9_rtcd_defs.pl, and retain RTCD symbol:
cpi->diamond_search_sad = vp9_diamond_search_sad;
```
