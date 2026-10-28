---
name: libvpx/remove-dead-function-parameters
description: When function parameters are obsolete, remove them from signatures; when unused parameters are retained for dispatch compatibility, assert any assumed preconditions alongside (void) casts
rubric: readability
severity: nit
---

**Rule.** Remove dead or obsolete parameters from function prototypes, RTCD
definitions, call sites, and architecture implementations rather than retaining
them in the interface and silencing compiler unused-parameter warnings with
`(void)` casts or `VPX_UNUSED`. When unused parameters must be retained to
satisfy fixed callback or multi-target dispatch signatures (such as RTCD), use
`(void)` casts or `VPX_UNUSED` and assert any assumed caller preconditions
(e.g., `assert(!param);`) to document invariants and catch unexpected caller
usage in debug builds.

**Why.** Removing dead parameters prevents confusion about whether an argument
has an effect and avoids passing dummy values across call sites and SIMD
backends. When parameters must be retained for dispatch compatibility but an
implementation relies on specific caller assumptions (such as a flag being false
or zero), asserting those preconditions documents the invariant for future
maintainers and catches invalid caller invocations during debug builds.

## Examples

Bad:

```cpp
// Dead parameter across all implementations:
void vpx_plane_add_noise_msa(uint8_t *start_ptr, const int8_t *noise,
                             int blackclamp, int whiteclamp,
                             int bothclamp, int width,
                             int height, int32_t pitch) {
  (void)bothclamp;
  // ...
}

// Retained dispatch parameter assumed to be false:
void vpx_quantize_b_neon(const tran_low_t *coeff, intptr_t count,
                         int skip_block, const int16_t *zbin, ...) {
  (void)skip_block;
  // ...
}
```

Good:

```cpp
// Remove dead parameter from signature, RTCD, and callers:
void vpx_plane_add_noise_msa(uint8_t *start_ptr, const int8_t *noise,
                             int blackclamp, int whiteclamp,
                             int width, int height, int32_t pitch) {
  // ...
}

// Retained dispatch parameter: assert assumed precondition:
void vpx_quantize_b_neon(const tran_low_t *coeff, intptr_t count,
                         int skip_block, const int16_t *zbin, ...) {
  (void)skip_block;
  assert(!skip_block);
  // ...
}
```
