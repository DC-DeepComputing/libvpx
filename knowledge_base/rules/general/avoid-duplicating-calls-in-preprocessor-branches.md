---
name: libvpx/avoid-duplicating-calls-in-preprocessor-branches
description: 'When function calls or operations differ only by arguments across #if/#else preprocessor guards, isolate varying variables inside the conditional and call the function once unconditionally.'
rubric: readability
severity: nit
---

**Rule.** When an expression, statement, or function call differs only by an
argument or variable value across preprocessor configuration branches (such as
#if CONFIG_* / #else / #elif), isolate and assign only the differing value to a
local variable inside the preprocessor conditional block. Perform the function
call or common operation once unconditionally after the #endif rather than
duplicating the entire call across preprocessor branches.

**Why.** Duplicating entire function calls, initializations, or shared
operations across #if and #else branches introduces redundant boilerplate and
increases maintenance overhead. If function signatures or surrounding logic
change, duplicated call sites risk divergent updates. Isolating only the
configuration-dependent arguments keeps code DRY and readable.

## Examples

Bad:

```cpp
#if CONFIG_VP9_HIGHBITDEPTH
  const uint16_t *cat6_high_cost = vp9_get_high_cost_table(xd->bd);
#else
  const uint16_t *cat6_high_cost = vp9_get_high_cost_table(8);
#endif
```

Good:

```cpp
#if CONFIG_VP9_HIGHBITDEPTH
  const int bit_depth = xd->bd;
#else
  const int bit_depth = 8;
#endif
  const uint16_t *cat6_high_cost = vp9_get_high_cost_table(bit_depth);
```
