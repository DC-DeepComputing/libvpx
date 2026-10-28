---
name: libvpx/vp9/apply-external-rc-qp-in-tpl-pass-across-modes
description: When external rate control provides frame QP decisions (VPX_RC_QP), apply external QP across all rate control modes in TPL passes instead of restricting overrides
rubric: correctness
severity: should_fix
---

**Rule.** When external rate control is configured to supply frame QP decisions
(VPX_RC_QP), respect and query external QP decisions during encoding across all
rate control modes, including Q mode during TPL analysis passes. Do not bypass
external QP overrides for specific rate control modes during intermediate or TPL
passes.

**Why.** Temporal dependency (TPL) modeling and reference frame reconstruction
rely on frames being encoded with the intended QP. Skipping external QP
overrides during TPL passes in constant-quality (Q) mode leads to reconstructed
reference frame mismatch and corrupts subsequent TPL dependency statistics.

## Examples

Bad:

```cpp
if (rc_mode == VPX_Q) {
  // In TPL pass in Q mode, we don't override the QP for encoding.
  return !tpl_with_external_rc;
} else if (rc_mode == VPX_VBR) {
  return 1;
}
```

Good:

```cpp
if (cpi->ext_ratectrl.ready &&
    (cpi->ext_ratectrl.funcs.rc_type & VPX_RC_QP) != 0 &&
    cpi->ext_ratectrl.funcs.get_encodeframe_decision != NULL) {
  codec_status = vp9_extrc_get_encodeframe_decision(
      &cpi->ext_ratectrl, gf_group->index, &encode_frame_decision);
}
```
