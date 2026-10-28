---
name: libvpx/omit-redundant-parentheses-in-sizeof
description: When computing element size with sizeof on dereferenced pointers or member expressions inside function bodies, omit redundant inner parentheses, writing sizeof(*ptr) or sizeof(ptr[0]).
rubric: readability
severity: nit
---

**Rule.** Do not add redundant defensive inner parentheses around dereferenced
pointers or member access expressions in sizeof(...) operations inside function
bodies. Write direct expressions such as sizeof(*ptr), sizeof(*expr->member), or
sizeof(expr[0]) rather than sizeof(*(ptr)) or sizeof(*(expr->member)). Reserve
defensive parameter parenthesizing exclusively for preprocessor macro
definitions.

**Why.** In C and C++, postfix member access (-> and .) naturally binds tighter
than unary dereference (*), and pointer dereferences inside sizeof(*ptr) have
unambiguous precedence within function bodies. Defensive parenthesizing such as
*(arg) is only necessary in macro definitions to guard against arbitrary
argument expansion. In normal code, redundant inner parentheses add visual noise
and syntactic clutter without improving safety or changing evaluation semantics.

## Examples

Bad:

```cpp
// Redundant defensive parentheses around dereferenced pointer or member
src[1] = load_tran_low(8 * sizeof(*(input)), input);
memset(dst, 0, count * sizeof(*(dst)));
CHECK_MEM_ERROR(cm, cpi->skin_map,
    vpx_calloc(cm->mb_rows * cm->mb_cols, sizeof(*(cpi->skin_map))));
```

Good:

```cpp
// Clean sizeof expressions omitting redundant inner parentheses
src[1] = load_tran_low(8 * sizeof(*input), input);
memset(dst, 0, count * sizeof(*dst));
CHECK_MEM_ERROR(cm, cpi->skin_map,
    vpx_calloc(cm->mb_rows * cm->mb_cols, sizeof(cpi->skin_map[0])));
```
