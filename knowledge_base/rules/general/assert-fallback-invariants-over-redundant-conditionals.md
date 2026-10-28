---
name: libvpx/assert-fallback-invariants-over-redundant-conditionals
description: In multi-branch conditionals where prior branches handle larger cases and remaining inputs are guaranteed, use unconditional else with assert() instead of redundant else if
rubric: readability
severity: should_fix
---

**Rule.** In multi-branch conditionals where prior branches handle larger cases
or specific partitions and all remaining valid inputs satisfy a known invariant
or precondition, use an unconditional else block containing an assert() rather
than a redundant else if runtime check. Do not write runtime else if checks on
the final fallback branch when earlier branches and function preconditions have
already partitioned the input space.

**Why.** An else if check on the final branch adds an unnecessary runtime branch
instruction in release builds and misleadingly implies to readers that unhandled
valid cases exist. Using an unconditional else with assert() documents and
validates the expected invariant during debug builds with zero release overhead.

## Examples

Bad:

```cpp
if (output_width >= 8) {
  process_8_pixels(src, dst);
} else if (output_width >= 4) {
  process_4_pixels(src, dst);
}
```

Good:

```cpp
if (output_width >= 8) {
  process_8_pixels(src, dst);
} else {
  assert(output_width >= 4);
  process_4_pixels(src, dst);
}
```
