---
name: libvpx/arch-simd-suffix-in-filenames
description: When naming architecture-specific SIMD source files, suffix filenames with their instruction set extension and omit _intrin_ unless disambiguating from coexisting assembly
rubric: readability
severity: should_fix
---

**Rule.** When adding or renaming architecture-specific source files (such as
files under arm/, x86/, ppc/, or mips/), append the architecture or SIMD
instruction set suffix (e.g., _neon.c, _sse2.c, _vsx.c, _sve.c, _msa.c) to the
filename rather than reusing the base file name. Do not include _intrin_ in
intrinsics source filenames unless both assembly (.asm/.s) and C intrinsics (.c)
implementations coexist and require disambiguation for build systems.

**Why.** Certain build tools and flattened object directory configurations
collide when multiple source files share identical base names across different
directories (e.g., vp9_dct.c and arm/neon/vp9_dct.c). Suffixing filenames with
their SIMD extension prevents collisions, while reserving _intrin_ strictly for
assembly/intrinsics disambiguation keeps source naming uniform across the
repository.

## Examples

Bad:

```cpp
// Missing SIMD suffix:
// vp9/encoder/arm/neon/vp9_dct.c
VP9_CX_SRCS-$(HAVE_NEON) += encoder/arm/neon/vp9_dct.c

// Superfluous '_intrin_' infix when no assembly counterpart exists:
// vp9/common/ppc/vp9_idct_intrin_vsx.c
VP9_COMMON_SRCS-$(HAVE_VSX) += common/ppc/vp9_idct_intrin_vsx.c
```

Good:

```cpp
// Suffix with SIMD extension:
// vp9/encoder/arm/neon/vp9_dct_neon.c
VP9_CX_SRCS-$(HAVE_NEON) += encoder/arm/neon/vp9_dct_neon.c

// Direct SIMD extension without '_intrin_':
// vp9/common/ppc/vp9_idct_vsx.c
VP9_COMMON_SRCS-$(HAVE_VSX) += common/ppc/vp9_idct_vsx.c
```
