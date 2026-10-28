---
name: libvpx/include-simd-benchmark-speedups-in-commit-message
description: When adding SIMD or architecture-specific optimizations, include benchmark speedup numbers against baseline C code across relevant compilers and microarchitectures in the commit message
rubric: readability
severity: should_fix
---

**Rule.** When adding or modifying SIMD or architecture-specific optimizations
(such as ARM NEON, x86 SSE/AVX, or MIPS MSA kernels), include benchmark speedup
numbers measured against the baseline C implementation in the commit message.
Document speedup metrics across relevant compiler versions and CPU
microarchitectures to substantiate the performance improvement.

**Why.** Quantifying performance gains over the baseline C implementation in the
commit message justifies the added complexity of SIMD kernels, provides
verifiable benchmark records for reviewers and maintainers, and helps detect
future performance regressions.

## Examples

Bad:

```cpp
Implement d63_predictor using Neon

Add Neon implementations of the d63 predictor for 4x4, 8x8, 16x16 and
32x32 block sizes. Also update tests to add new corresponding cases.
```

Good:

```cpp
Implement d63_predictor using Neon

Add Neon implementations of the d63 predictor for 4x4, 8x8, 16x16 and
32x32 block sizes. Also update tests to add new corresponding cases.

Speedups over the C code (higher is better):

Microarch.  | Compiler | Block | Speedup
Neoverse N1 |  LLVM 15 |   4x4 |    2.10x
Neoverse N1 |  LLVM 15 |   8x8 |    4.45x
Cortex-A78  |   GCC 12 |   4x4 |    1.95x
```
