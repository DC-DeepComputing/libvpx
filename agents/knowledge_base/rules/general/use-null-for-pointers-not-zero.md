---
name: libvpx/use-null-for-pointers-not-zero
description: When initializing, assigning, resetting, or checking pointer variables and struct members in C/C++, use NULL (or nullptr) instead of integer literal 0.
rubric: readability
severity: nit
---

**Rule.** When initializing, assigning, resetting, or comparing pointer
variables, arguments, or struct members, use `NULL` (or `nullptr` in C++ code)
rather than the bare integer literal `0`. For null pointer checks in C, compare
against `NULL` or use logical negation (e.g., `!ptr`). Reserve `0` strictly for
numeric and integer values.

**Why.** Using `NULL` (or `nullptr` in C++) and boolean negation (`!ptr`)
explicitly communicates pointer semantics and nullability intent to human
readers and static analyzers, avoiding confusion between integer values and null
pointer constants.

## Examples

Bad:

```cpp
if (frame_stats == 0) {
  return 0;
}
uint8_t *buffer = 0;

vpx_free(oci->postproc_state.generated_noise);
oci->postproc_state.generated_noise = 0;
```

Good:

```cpp
if (frame_stats == NULL) {
  return 0;
}
uint8_t *buffer = NULL;

vpx_free(oci->postproc_state.generated_noise);
oci->postproc_state.generated_noise = NULL;
```
