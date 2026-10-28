---
name: libvpx/concise-macro-local-variable-names
description: When declaring macro-local temporary variables to avoid shadowing, use concise abbreviations with `_m` suffixes rather than full macro names, prepending underscores before leading digits.
rubric: readability
severity: should_fix
---

**Rule.** When declaring internal temporary or local variables within macro
definitions (such as statement expressions `({ ... })` or `do { ... } while (0)`
blocks) to prevent variable shadowing (`-Wshadow`) warnings or identifier
collisions across nested expansions, do not mechanically copy or lowercase the
entire macro name as a prefix. Instead, use concise, distinctive abbreviations
or short prefixes with a disambiguating tag (such as an `_m` suffix or compact
`_tmp`/`_vec` tokens). If a distinguishing prefix begins with a digit, prepend a
leading underscore (e.g., `_6tap_vec0_m` or `_4tap_vec0_m`) or spell out the
number to maintain valid C identifier syntax.

**Why.** Mechanically prepending or lowercasing full macro names for internal
temporary variables creates excessively long, unwieldy identifiers that clutter
macro bodies, bloat SIMD/DSP implementations, obscure arithmetic and vector data
flow, and cause awkward line wrapping. A concise, distinctive abbreviation
combined with an `_m` or `_tmp` suffix is sufficient to prevent variable
shadowing against caller and nested scopes while keeping macro bodies readable
and compact. Adding a leading underscore ensures valid C identifier syntax when
prefixes start with numbers.

## Examples

Bad:

```cpp
// Bad: Mechanically copying or lowercasing full macro names clutters macro bodies
#define HORIZ_6TAP_4WID_4VECS_FILT(src0, src1, src2, src3, mask0, mask1, out0) \
  do {                                                                          \
    v16i8 horiz_6tap_4wid_4vecs_filt_vec0_m, horiz_6tap_4wid_4vecs_filt_vec1_m; \
    VSHF_B2_SB(src0, src1, src2, src3, mask0, mask0,                            \
               horiz_6tap_4wid_4vecs_filt_vec0_m,                               \
               horiz_6tap_4wid_4vecs_filt_vec1_m);                              \
  } while (0)

#define FILT_4TAP_DPADD_S_H(vec0, vec1, filt0, filt1)                         \
  ({                                                                          \
    v8i16 filt_4tap_dpadd_s_h_tmp0;                                           \
    filt_4tap_dpadd_s_h_tmp0 = __msa_dotp_s_h((v16i8)vec0, (v16i8)filt0);     \
    filt_4tap_dpadd_s_h_tmp0;                                                 \
  })
```

Good:

```cpp
// Good: Use concise abbreviations or short prefixes with _m / _tmp suffixes
#define HORIZ_6TAP_4WID_4VECS_FILT(src0, src1, src2, src3, mask0, mask1, out0) \
  do {                                                                          \
    v16i8 _6tap_4wid_vec0_m, _6tap_4wid_vec1_m;                                 \
    VSHF_B2_SB(src0, src1, src2, src3, mask0, mask0, _6tap_4wid_vec0_m,        \
               _6tap_4wid_vec1_m);                                              \
  } while (0)

#define FILT_4TAP_DPADD_S_H(vec0, vec1, filt0, filt1)                 \
  ({                                                                  \
    v8i16 _4tap_dpadd_tmp0;                                           \
    _4tap_dpadd_tmp0 = __msa_dotp_s_h((v16i8)vec0, (v16i8)filt0);     \
    _4tap_dpadd_tmp0;                                                 \
  })
```
