---
name: libvpx/simplify-bitmask-non-zero-check
description: When testing whether any bit is set in an integer bitmask, check mask != 0 directly instead of iterating across bit positions
rubric: readability
severity: should_fix
---

**Rule.** Do not iterate across bit indices or bit positions with bit-shift
tests (such as `mask & (1 << i)`) solely to check if any bit is active in a
bitmask. Test `mask != 0` (or `mask == 0` for absence) directly instead.

**Why.** An integer bitmask is non-zero if and only if at least one bit is set.
Iterating over bit positions with loop constructs and shifts adds unnecessary
code verbosity, branches, and execution overhead for a condition that is
directly evaluated with a single comparison.

## Examples

Bad:

```cpp
int ref;
for (ref = 0; ref < REF_FRAMES; ++ref) {
  if (svc->update_buffer_slot[svc->spatial_layer_id] & (1 << ref)) {
    svc->non_reference_frame = 0;
  }
}
```

Good:

```cpp
if (svc->update_buffer_slot[svc->spatial_layer_id] != 0) {
  svc->non_reference_frame = 0;
}
```
