---
name: libvpx/avoid-unnecessary-flag-parameters
description: When helper functions read or update state with only a single active value, avoid flag parameters and bitwise masks in favor of direct scalar checks and assignments
rubric: design
severity: should_fix
---

**Rule.** Do not add or retain flag parameters, bitmask masks, or bitwise
operations (such as |= or & flag) in internal helper functions when only a
single state or fixed value is ever tracked across call sites. Simplify the
helper signature by removing dead flag parameters, and evaluate or assign the
state directly using scalar checks (e.g., !(*map)) or direct assignments (e.g.,
= 1).

**Why.** Passing redundant flag arguments and performing bitwise mask operations
when only a single state is evaluated introduces dead configurability,
misleading interfaces, and unnecessary runtime overhead. Keeping helper
functions focused on their actual usage simplifies signatures, reduces cognitive
load, and keeps state inspections and updates direct and readable.

## Examples

Bad:

```cpp
// Bad: Redundant flag parameters and bitwise masks when reading and updating a single state.
static void map_read(VP9Decoder *pbi, int idx, int flag) {
  volatile int8_t *map = pbi->recon_map + idx;
  while (!((*map) & flag)) {
    sched_yield();
  }
}

static void map_write(VP9Decoder *pbi, int idx, int flag) {
  pbi->recon_map[idx] |= flag;
}
```

Good:

```cpp
// Good: Direct scalar check and assignment without unused flag parameters.
static void map_read(VP9Decoder *pbi, int idx) {
  volatile int8_t *map = pbi->recon_map + idx;
  while (!(*map)) {
    sched_yield();
  }
}

static void map_write(VP9Decoder *pbi, int idx) {
  pbi->recon_map[idx] = 1;
}
```
