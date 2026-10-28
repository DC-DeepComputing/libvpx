---
name: libvpx/vp9/guard-all-layering-branches-against-zero-division
description: When guarding rate-control or layer-configuration calculations against division by zero or invalid inputs, update all sibling layering branches and mode paths consistently
rubric: correctness
severity: should_fix
---

**Rule.** When adding an edge-case guard or input validation (such as checking
for zero target_bandwidth or framerate before division) in rate-control or
layer-configuration calculations, ensure all sibling branches and layering modes
(such as temporal layering loops versus spatial-only or single-layer paths)
performing the calculation are updated consistently. Do not guard only one
layering branch while leaving counterpart paths vulnerable to division by zero
or invalid bit allocations.

**Why.** Rate control and layer configuration routines frequently partition bit
allocation and buffer sizing across separate branches for different operating
modes (such as temporal layering vs. spatial-only layering). Adding a
zero-division guard in only one branch leaves other supported configurations
susceptible to crashes or calculation errors when zero bitrate is configured.

## Examples

Bad:

```cpp
// In temporal layering branch:
if (target_bandwidth != 0) {
  bitrate_alloc = (float)lc->target_bandwidth / target_bandwidth;
}

// In else / spatial-only branch:
bitrate_alloc = (float)lc->target_bandwidth / target_bandwidth;  // Missed guard; divides by zero if target_bandwidth == 0
```

Good:

```cpp
// In temporal layering branch:
if (target_bandwidth != 0) {
  bitrate_alloc = (float)lc->target_bandwidth / target_bandwidth;
}

// In else / spatial-only branch:
if (target_bandwidth != 0) {
  bitrate_alloc = (float)lc->target_bandwidth / target_bandwidth;
}
```
