---
name: libvpx/normalize-integer-types-across-parallel-branches
description: When implementing parallel or related branches handling the same conceptual values, normalize integer types, range limits (INT_MAX vs. UINT_MAX), and casts consistently
rubric: readability
severity: should_fix
---

**Rule.** Normalize integer types, range limit constants (e.g., INT_MAX vs.
UINT_MAX), and explicit type casts across parallel or alternative control-flow
branches that validate, store, or process the same conceptual values or data
structures. Do not mix signed and unsigned representations or differing integer
widths between sibling branches.

**Why.** Inconsistent integer representations across parallel branches increase
cognitive overhead, make logic harder to maintain, and introduce risks of subtle
signed/unsigned comparison bugs, overflow mismatches, or unexpected wrap-around
behaviors.

## Examples

Bad:

```cpp
// Multi-layer branch uses unsigned int and UINT_MAX:
unsigned int n_packets_per_layer[VPX_SS_MAX_LAYERS] = { 0 };
if (count_rounded > (double)UINT_MAX ||
    (unsigned int)count_rounded != n_packets_per_layer[layer_id] - 1) {
  return VPX_CODEC_CORRUPT_FRAME;
}

// Parallel single-layer branch uses int and INT_MAX:
if (count_rounded > (double)INT_MAX ||
    (int)count_rounded != n_packets - 1) {
  return VPX_CODEC_CORRUPT_FRAME;
}
```

Good:

```cpp
// Both parallel branches normalized to int and INT_MAX:
int n_packets_per_layer[VPX_SS_MAX_LAYERS] = { 0 };
if (count_rounded > (double)INT_MAX ||
    (int)count_rounded != n_packets_per_layer[layer_id] - 1) {
  return VPX_CODEC_CORRUPT_FRAME;
}

if (count_rounded > (double)INT_MAX ||
    (int)count_rounded != n_packets - 1) {
  return VPX_CODEC_CORRUPT_FRAME;
}
```
