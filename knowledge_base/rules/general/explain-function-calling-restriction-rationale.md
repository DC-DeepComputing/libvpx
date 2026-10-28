---
name: libvpx/explain-function-calling-restriction-rationale
description: When documenting function calling restrictions or prohibitions in comments, explain the underlying technical reason or lifetime invariant rather than leaving an unexplained prohibition
rubric: readability
severity: nit
---

**Rule.** When documenting calling restrictions, invocation prerequisites, or
caller prohibitions on functions (such as stating that a function must not be
called from certain entry points), explain the underlying technical rationale,
state machine precondition, or lifetime invariant. Do not leave bare,
unexplained prohibitions (e.g. 'Must not be called by X'); specify the
reason—such as memory ownership, cleanup order, or error handling lifecycles—so
future maintainers understand why the restriction exists.

**Why.** Unexplained calling restrictions leave maintainers uncertain about why
a constraint exists, increasing the risk that future refactorings violate subtle
lifetime or state machine invariants. Documenting the underlying technical
reason clarifies caller contracts and prevents regressions.

## Examples

Bad:

```cpp
// Called by vp8e_set_config() and vp8e_encode() only. Must not be called
// by vp8e_init().
static vpx_codec_err_t update_error_state(vpx_codec_alg_priv_t *ctx,
                                          const char *detail) {
  // ...
}
```

Good:

```cpp
// Called by vp8e_set_config() and vp8e_encode() only. Must not be called
// by vp8e_init() because ctx->cpi->common.error will be destroyed by
// vpx_codec_enc_init_ver() if vp8e_init() returns an error.
static vpx_codec_err_t update_error_state(vpx_codec_alg_priv_t *ctx,
                                          const char *detail) {
  // ...
}
```
