---
name: libvpx/parenthesize-macro-arguments-in-expressions
description: When defining function-like preprocessor macros, enclose all parameter references and composite expressions in parentheses, and ensure consistent definition hygiene across conditional compilation branches
rubric: readability
severity: should_fix
---

**Rule.** When defining function-like preprocessor macros with `#define`,
enclose every parameter reference in parentheses wherever it appears in an
expression (e.g., `(W)` instead of `W`). Additionally, wrap composite
replacement expressions in parentheses or use `do { ... } while (0)` for
multi-statement macros, and ensure that all conditional compilation branches
(such as `#if`/`#else` debug vs. release configurations) consistently apply
identical argument parenthesization and macro hygiene.

**Why.** Preprocessor macro expansion performs textual substitution without
respecting expression boundaries. Omitting parentheses around parameter
references or composite expressions allows caller-provided compound expressions
or binary operators to alter operator precedence and evaluation order, causing
subtle computation bugs. Inconsistent macro definitions across conditional
compilation branches leave fallback or release configurations vulnerable to
unparenthesized precedence bugs or compilation errors.

## Examples

Bad:

```cpp
// Macro parameters and composite expressions lack defensive parentheses:
#define SUBPIX_VAR(W, H) \
  uint16_t fdata3[((H) + 1) * W];

#define CALC_STRIDE(base, step) \
  base + step * 4

// Inconsistent argument hygiene across conditional compilation branches:
#if CONFIG_DEBUG
#define CHECK_MEM_ERROR(cm, lval, expr) \
  do { \
    (lval) = (expr); \
    if (!(lval)) \
      vpx_internal_error(&(cm)->error, VPX_CODEC_MEM_ERROR, ...); \
  } while (0)
#else
#define CHECK_MEM_ERROR(cm, lval, expr) \
  do { \
    lval = (expr); \
    if (!lval) \
      vpx_internal_error(&(cm)->error, VPX_CODEC_MEM_ERROR, ...); \
  } while (0)
#endif
```

Good:

```cpp
// Parenthesize every parameter reference and wrap composite expressions:
#define SUBPIX_VAR(W, H) \
  uint16_t fdata3[((H) + 1) * (W)];

#define CALC_STRIDE(base, step) \
  ((base) + (step) * 4)

// Parenthesize arguments consistently across all conditional branches:
#if CONFIG_DEBUG
#define CHECK_MEM_ERROR(cm, lval, expr) \
  do { \
    (lval) = (expr); \
    if (!(lval)) \
      vpx_internal_error(&(cm)->error, VPX_CODEC_MEM_ERROR, ...); \
  } while (0)
#else
#define CHECK_MEM_ERROR(cm, lval, expr) \
  do { \
    (lval) = (expr); \
    if (!(lval)) \
      vpx_internal_error(&(cm)->error, VPX_CODEC_MEM_ERROR, ...); \
  } while (0)
#endif
```
