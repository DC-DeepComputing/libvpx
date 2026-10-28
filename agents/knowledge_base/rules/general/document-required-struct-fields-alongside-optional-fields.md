---
name: libvpx/document-required-struct-fields-alongside-optional-fields
description: When documenting struct members where some fields are marked optional, explicitly label mandatory fields as required in their doc comments
rubric: readability
severity: nit
---

**Rule.** When documenting struct definitions that contain a mix of optional and
required members, explicitly label mandatory fields (e.g., 'Required:')
alongside optional fields ('Optional:') in their doc comments. Do not leave
required fields unlabeled when other fields in the same struct are marked as
optional.

**Why.** Labeling only a subset of struct fields as optional creates ambiguity
about whether unmarked fields are mandatory or have default values. Explicitly
marking both required and optional members clarifies API contracts and struct
initialization requirements for callers and implementers.

## Examples

Bad:

```cpp
typedef struct vpx_rc_encodeframe_decision {
  int q_index; /**< Quantizer step index [0..255] */
  int rdmult;  /**< Frame level Lagrangian multiplier */
  /*!
   * Optional: Superblock quantization parameters.
   * It has no effect if not set by external rate control.
   */
  sb_params *sb_params_list;
} vpx_rc_encodeframe_decision_t;
```

Good:

```cpp
typedef struct vpx_rc_encodeframe_decision {
  int q_index; /**< Required: Quantizer step index [0..255] */
  int rdmult;  /**< Required: Frame level Lagrangian multiplier */
  /*!
   * Optional: Superblock quantization parameters.
   * It has no effect if not set by external rate control.
   */
  sb_params *sb_params_list;
} vpx_rc_encodeframe_decision_t;
```
