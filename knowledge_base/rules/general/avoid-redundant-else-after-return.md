---
name: libvpx/avoid-redundant-else-after-return
description: When an if or else-if branch unconditionally exits via return, break, continue, or goto, omit redundant else blocks and flatten subsequent control flow
rubric: readability
severity: should_fix
---

**Rule.** Do not use an `else` block or `else if` clause immediately following
an `if` or preceding `else if` branch that unconditionally exits or terminates
control flow (via `return`, `break`, `continue`, or `goto`). Omit the redundant
`else` keyword and place subsequent statements or fallback returns directly at
the outer function scope to reduce nesting depth. Additionally, flatten nested
`if` statements inside an `else` block into `else if` chains or early guard
returns rather than introducing extra indentation levels.

**Why.** When an `if` or `else if` branch terminates unconditionally, code in a
trailing `else` block is already guaranteed not to run if earlier conditions
matched. Wrapping subsequent statements in an `else` block adds redundant
nesting and visual clutter without altering execution semantics. Flattening
control flow and un-indenting subsequent statements keeps the primary logic at
the outermost scope, simplifies early exits, and makes independent execution
paths clearer and easier to follow.

## Examples

Bad:

```cpp
// Redundant else after return:
if (step == 1) {
  return single_step_search(...);
} else {
  return multi_step_search(...);
}

// Redundant else in multi-branch chain:
if (has_above && has_left) {
  return left_intra && above_intra ? 3 : left_intra || above_intra;
} else if (has_above || has_left) {
  return 2 * !is_inter_block(has_above ? above_mi : left_mi);
} else {
  return 0;
}

// Redundant else after break:
if (ext_ratectrl->ready) {
  break;
} else {
  vpx_internal_error(&cm->error, VPX_CODEC_ERROR, ...);
  break;
}

// Nested if inside else block:
if (y < y_low || y > y_high) {
  return 0;
} else {
  if (mode == MODE_A) {
    return evaluate_a(cb, cr);
  } else {
    return 0;
  }
}
```

Good:

```cpp
// Return at outer scope:
if (step == 1) {
  return single_step_search(...);
}
return multi_step_search(...);

// Multi-branch chain with fallback return at outer scope:
if (has_above && has_left) {
  return left_intra && above_intra ? 3 : left_intra || above_intra;
} else if (has_above || has_left) {
  return 2 * !is_inter_block(has_above ? above_mi : left_mi);
}
return 0;

// Unnested after break:
if (ext_ratectrl->ready) {
  break;
}
vpx_internal_error(&cm->error, VPX_CODEC_ERROR, ...);
break;

// Flattened with else if / guard checks:
if (y < y_low || y > y_high) {
  return 0;
} else if (mode == MODE_A) {
  return evaluate_a(cb, cr);
} else {
  return 0;
}
```
