---
name: libvpx/bundle-array-pointers-and-counts-in-structs
description: When defining or passing structs containing dynamic array pointers, include element counts inside the struct rather than as loose function parameters
rubric: design
severity: should_fix
---

**Rule.** When designing structs that contain pointers to dynamically sized
array buffers or lists, include the element count, length, or buffer size as a
member within the struct alongside the pointer. Do not pass the element count as
a separate loose parameter to functions or callback signatures that already
receive the struct pointer.

**Why.** Encapsulating array buffers together with their element counts or sizes
in the same data structure maintains clear object boundaries, simplifies
function and callback signatures, and prevents bounds decoupling during
refactoring or callback invocations.

## Examples

Bad:

```cpp
typedef struct vpx_rc_encodeframe_decision {
  int q_index;
  int rdmult;
  sb_params *sb_params_list;
} vpx_rc_encodeframe_decision_t;

typedef vpx_rc_status_t (*vpx_rc_get_encodeframe_decision_cb_fn_t)(
    vpx_rc_model_t rate_ctrl_model, const int frame_gop_index, int num_sb,
    vpx_rc_encodeframe_decision_t *frame_decision);
```

Good:

```cpp
typedef struct vpx_rc_encodeframe_decision {
  int q_index;
  int rdmult;
  int num_sb;
  sb_params *sb_params_list;
} vpx_rc_encodeframe_decision_t;

typedef vpx_rc_status_t (*vpx_rc_get_encodeframe_decision_cb_fn_t)(
    vpx_rc_model_t rate_ctrl_model, const int frame_gop_index,
    vpx_rc_encodeframe_decision_t *frame_decision);
```
