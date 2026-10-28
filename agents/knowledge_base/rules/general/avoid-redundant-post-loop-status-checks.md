---
name: libvpx/avoid-redundant-post-loop-status-checks
description: When evaluating success or failure after a processing loop, avoid redundant checks on per-iteration flags already covered by cumulative counters or error bailouts
rubric: readability
severity: should_fix
---

**Rule.** Do not add redundant post-loop condition checks on per-iteration
status or error variables when cumulative error counters or explicit error
bailouts already account for all failure states. After a loop exits, verify the
overall outcome using cumulative error counts or aggregate status instead of
inspecting variables that only preserve the state of the final iteration.

**Why.** Per-iteration variables only retain the value from the last iteration
executed by the loop, making post-loop checks on them misleading and redundant
when errors throughout all iterations are already aggregated into counters or
handled via immediate error jumps.

## Examples

Bad:

```cpp
if (frames_corrupted) {
  fprintf(stderr, "WARNING: %d frames corrupted.\n", frames_corrupted);
} else if (!corrupted) {
  ret = EXIT_SUCCESS;
}
```

Good:

```cpp
if (frames_corrupted) {
  fprintf(stderr, "WARNING: %d frames corrupted.\n", frames_corrupted);
} else {
  ret = EXIT_SUCCESS;
}
```
