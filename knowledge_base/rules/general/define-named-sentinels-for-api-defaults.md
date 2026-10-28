---
name: libvpx/define-named-sentinels-for-api-defaults
description: When allowing external API callers or models to request default behavior, define and check explicit named sentinel constants instead of silently swallowing out-of-range inputs
rubric: correctness
severity: should_fix
---

**Rule.** Define and check an explicit, named sentinel constant (such as
`#define VPX_DEFAULT_Q -1` or `VPX_DEFAULT_RDMULT`) when allowing external API
callers, models, or callbacks to request default library behavior. Do not
silently fall back to default behavior on arbitrary invalid or out-of-range
values; reserve sentinel values specifically for requesting defaults and
validate or reject out-of-range inputs.

**Why.** Silently falling back to default behavior for all out-of-range or
invalid values masks caller bugs, configuration typos, and upstream calculation
errors. An explicit named sentinel clearly documents the caller's intent to
request default behavior while allowing unexpected values to be caught and
reported.

## Examples

Bad:

```cpp
// External rate control decision: silently fall back to default q if out of range
if (encode_frame_decision.q_index <= MAXQ &&
    encode_frame_decision.q_index >= MINQ) {
  q = encode_frame_decision.q_index;
}
```

Good:

```cpp
// In API header: #define VPX_DEFAULT_Q -1
// Explicit sentinel used to request default library behavior
if (encode_frame_decision.q_index != VPX_DEFAULT_Q) {
  q = encode_frame_decision.q_index;
}
```
