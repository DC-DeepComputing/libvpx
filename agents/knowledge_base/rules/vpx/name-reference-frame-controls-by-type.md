---
name: libvpx/vpx/name-reference-frame-controls-by-type
description: When naming reference-frame variables, struct members, codec controls, macros, or dispatch handlers, identify concrete buffer types (LAST, GF, ARF) rather than ordinals or speculative lifecycles.
rubric: readability
severity: should_fix
---

**Rule.** Name reference-frame variables, struct members/fields, codec control
enums, type-mapping macros (such as VPX_CTRL_USE_TYPE), and interface dispatch
handlers explicitly after their concrete frame buffer type or slot (such as
LAST, GF for golden frame, or ARF / ALTREF for alternate reference frame). Avoid
naming them after speculative temporal lifecycles (such as longterm_ref or
shortterm_ref) or generic ordinals (such as FIRST_REF, SECOND_REF, or
SECOND_TEMPORAL_REF).

**Why.** In VP8 and VP9 codecs (including SVC configurations), reference buffers
serve distinct architectural roles (LAST being the primary temporal reference,
GF being the golden frame, and ARF being the alternate reference). Temporal
designations like 'longterm' are speculative because golden or altref buffers
may be updated frequently or irregularly depending on dynamic layer schedules.
Generic ordinal names obscure which buffer slot is being manipulated. Concrete
buffer names ensure clarity and consistency across standard and SVC pipelines.

## Examples

Bad:

```cpp
// Bad: Speculative lifecycle names for struct members and controls
svc->buffer_longterm_ref.idx = 7;
cpi->svc.use_longterm_ref = 1;
vpx_codec_control(&codec, VP9E_SET_SVC_USE_LONGTERM_REF, 1);

// Bad: Generic ordinal naming for controls, macros, and handlers
VP9E_SET_SVC_SECOND_TEMPORAL_REF,
VPX_CTRL_USE_TYPE(VP9E_SET_SVC_SECOND_TEMPORAL_REF, unsigned int)
#define VPX_CTRL_VP9E_SET_SVC_SECOND_TEMPORAL_REF

static vpx_codec_err_t ctrl_set_svc_second_temporal_ref(
    vpx_codec_alg_priv_t *ctx, va_list args) { ... }
```

Good:

```cpp
// Good: Explicit buffer type (GF / golden frame) for struct members and controls
svc->buffer_gf_temporal_ref[1].idx = 7;
cpi->svc.use_gf_temporal_ref = 1;
vpx_codec_control(&codec, VP9E_SET_SVC_GF_TEMPORAL_REF, 1);

// Good: Explicit frame acronyms (GF, ARF, LAST) for controls, macros, and handlers
VP9E_SET_SVC_GF_TEMPORAL_REF,
VPX_CTRL_USE_TYPE(VP9E_SET_SVC_GF_TEMPORAL_REF, unsigned int)
#define VPX_CTRL_VP9E_SET_SVC_GF_TEMPORAL_REF

static vpx_codec_err_t ctrl_set_svc_gf_temporal_ref(
    vpx_codec_alg_priv_t *ctx, va_list args) { ... }
```
