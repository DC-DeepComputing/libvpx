---
name: libvpx/wrap-block-macros-in-do-while-zero
description: When defining multi-statement preprocessor macros, wrap macro bodies in do { ... } while (0) without trailing semicolons instead of bare blocks or statement expressions
rubric: readability
severity: should_fix
---

**Rule.** Wrap multi-statement or compound-statement preprocessor macro bodies
in `do { ... } while (0)` without a trailing semicolon at the definition end. Do
not use bare block scopes `{ ... }`, non-standard GNU statement expressions `({
... })` (pass output variables as macro parameters instead), or loose unbraced
statement sequences without semicolons. Ensure invocations at call sites require
and use standard trailing semicolons.

**Why.** A bare compound statement `{ ... }` or loose multi-statement macro body
breaks syntax or introduces dangling-`else` bugs when invoked inside conditional
statements (such as unbraced `if (cond) MACRO(); else ...`). GNU statement
expressions `({ ... })` rely on non-standard compiler extensions that harm
portability. Wrapping in `do { ... } while (0)` without a trailing semicolon
guarantees the macro expands syntactically as a single standard statement
requiring a semicolon at the invocation site.

## Examples

Bad:

```cpp
// Bad: Bare compound block breaks in 'if-else' conditionals
#define VPX_SWAP(type, a, b) \
  {                          \
    type temp__ = (b);       \
    (b) = (a);               \
    (a) = temp__;            \
  }

// Bad: GNU statement expression ({ ... }) is non-portable
#define HADD_SW_S32(in)                        \
  ({                                           \
    __m128i res0_m;                            \
    int32_t sum_m;                             \
    res0_m = __lsx_vhaddw_d_w(in, in);         \
    res0_m = __lsx_vhaddw_q_d(res0_m, res0_m); \
    sum_m = __lsx_vpickve2gr_w(res0_m, 0);     \
    sum_m;                                     \
  })

// Bad: Loose unbraced statements without do-while(0)
#define SHUF(name)                                                         \
  name##2 = _mm_blend_epi16(_mm_bslli_si128(name##0, 2), name##1, 0x01);   \
  name##3 = _mm_blend_epi16(_mm_bsrli_si128(name##1, 2), name##0, 0x80);
```

Good:

```cpp
// Good: Wrap compound block in do { ... } while (0)
#define VPX_SWAP(type, a, b) \
  do {                       \
    type c = (b);            \
    (b) = (a);               \
    (a) = c;                 \
  } while (0)

// Good: Pass output parameter and wrap in do { ... } while (0)
#define HADD_SW_S32(in0, in1)                  \
  do {                                         \
    __m128i res0_m;                            \
    res0_m = __lsx_vhaddw_d_w(in0, in0);       \
    res0_m = __lsx_vhaddw_q_d(res0_m, res0_m); \
    in1 = __lsx_vpickve2gr_w(res0_m, 0);       \
  } while (0)

// Good: Multi-statement macro wrapped in do-while(0), invoked with ';'
#define SHUF(name)                                                         \
  do {                                                                     \
    name##2 = _mm_blend_epi16(_mm_bslli_si128(name##0, 2), name##1, 0x01); \
    name##3 = _mm_blend_epi16(_mm_bsrli_si128(name##1, 2), name##0, 0x80); \
  } while (0)
```
