---
name: libvpx/const-qualify-extern-declarations
description: When declaring external arrays, lookup tables, or global variables matching const definitions, use extern const to maintain const-correctness and prevent mutation.
rubric: readability
severity: should_fix
---

**Rule.** When declaring external variables, tables, or arrays whose definition
is const, include the const qualifier in the extern declaration (such as extern
const Type name[] or extern const Type *const ptr). Do not declare external
read-only constants without const.

**Why.** Omitting const from an extern declaration causes a type signature
mismatch with the underlying const definition, violates const-correctness, and
can lead to accidental mutations resulting in runtime faults or undefined
behavior when modifying read-only memory.

## Examples

Bad:

```cpp
// Bad: extern declarations omit const qualifier of underlying definition
extern int16_t vpx_rv[];
extern int vp8_mode_contexts[6][4];
extern short *vp8_dct_value_cost_ptr;
```

Good:

```cpp
// Good: extern declarations match const definition
extern const int16_t vpx_rv[];
extern const int vp8_mode_contexts[6][4];
extern const short *const vp8_dct_value_cost_ptr;
```
