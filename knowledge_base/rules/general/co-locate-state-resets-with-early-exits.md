---
name: libvpx/co-locate-state-resets-with-early-exits
description: 'When handling early-exit or bailout conditions with state updates, co-locate continuation resets with the exit check in the same #if block'
rubric: design
severity: should_fix
---

**Rule.** When an early-exit or bailout check updates state on exit (such as
incrementing a drop or skip counter) and requires resetting state on
continuation, place the continuation state reset immediately after the
early-exit check within the same #if conditional compilation block. Do not defer
the state reset to a separate #if block later in the function.

**Why.** Co-locating counter resets and state transitions with their
corresponding early-exit logic keeps related control flow together, avoids
scattering redundant conditional compilation (#if) guards across a function, and
makes state management clearer.

## Examples

Bad:

```cpp
#if defined(DROP_UNCODED_FRAMES)
  if (cpi->common.MBs == cpi->mb.skip_true_count &&
      (cpi->drop_frame_count & 7) != 7) {
    cpi->drop_frame_count++;
    return;
  }
#endif
  /* ... frame encoding logic ... */
#if defined(DROP_UNCODED_FRAMES)
  cpi->drop_frame_count = 0;
#endif
```

Good:

```cpp
#if defined(DROP_UNCODED_FRAMES)
  if (cpi->common.MBs == cpi->mb.skip_true_count &&
      (cpi->drop_frame_count & 7) != 7) {
    cpi->drop_frame_count++;
    return;
  }
  cpi->drop_frame_count = 0;
#endif
```
