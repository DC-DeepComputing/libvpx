---
name: libvpx/direct-boolean-assignment
description: When assigning, returning, or evaluating boolean conditions, flags, or binary results, use direct expressions instead of redundant if-else branches, ternary literals, or bitwise-OR assignments
rubric: readability
severity: nit
---

**Rule.** Assign, return, or pass boolean variables, error indicators, and
binary flag fields directly using the condition expression, boolean function
result, or logical negation (!cond) instead of redundant if-else branches (if
(cond) flag = 1; else flag = 0;), boolean return patterns (if (!cond) return
false; return true;), or redundant ternary operators (cond ? 1 : 0, cond ? 0 :
1, cond ? true : false, cond ? false : true). When setting non-boolean
multi-state flags conditionally, use direct ternary expressions (flags = cond ?
FLAG : 0;). Reserve bitwise-OR assignment (|=, |) strictly for accumulating
multi-bit integer bitmasks across operations rather than setting binary flags or
error indicators, and avoid superfluous parentheses around comparison operands
when operator precedence is clear.

**Why.** Comparisons, relational operators, and logical expressions in C and C++
already evaluate to integer 0/1 or boolean true/false. Wrapping them in
redundant if-else branches or ternary operators (? 1 : 0, ? true : false) adds
visual noise, introduces dead branching or superfluous parentheses, and obscures
control flow. Directly assigning or returning the condition expression clarifies
data flow. Furthermore, using bitwise-OR assignment (|= 1 or |= FLAG) implies
multi-bit accumulation across operations; when a flag or error status is binary
or recomputed per frame/operation, bitwise-ORing obscures data flow and risks
carrying over stale state.

## Examples

Bad:

```cpp
// Redundant branching for boolean condition assignment
if (cm->intra_only)
  cpi->svc.previous_frame_is_intra_only = 1;
else
  cpi->svc.previous_frame_is_intra_only = 0;

// Redundant ternary wrapping on comparisons or binary function returns
int drop_from_rate_control = (vp9_rc_drop_frame(cpi)) ? 1 : 0;
const int is_last_row =
    ((sb_rows - 1) == (mi_row >> MI_BLOCK_SIZE_LOG2)) ? 1 : 0;
force_split[0] = (scene_change_detected) ? 1 : 0;
return is_valid ? true : false;

// Redundant branching and temporary on boolean return
const bool allocated = init_image_buffer(&encode_frame_result->coded_frame,
                                         frame_width, frame_height, img_fmt);
if (!allocated) {
  return false;
}
return true;

// Bitwise OR for binary error flag or per-frame flags
error |= 1;
cpi_->frame_flags = cpi_->frame_flags | FRAMEFLAGS_KEY;
```

Good:

```cpp
// Direct assignment of condition and comparison
cpi->svc.previous_frame_is_intra_only = cm->intra_only;
const int is_last_row = (sb_rows - 1 == mi_row >> MI_BLOCK_SIZE_LOG2);
force_split[0] = scene_change_detected;

// Direct assignment of binary function return value
int drop_from_rate_control = vp9_rc_drop_frame(cpi);

// Direct return of condition or function result
return is_valid;
return init_image_buffer(&encode_frame_result->coded_frame, frame_width,
                         frame_height, img_fmt);

// Direct assignment for binary error flag and per-frame flags
error = 1;
cpi_->frame_flags = (cm->frame_type == KEY_FRAME) ? FRAMEFLAGS_KEY : 0;
```
