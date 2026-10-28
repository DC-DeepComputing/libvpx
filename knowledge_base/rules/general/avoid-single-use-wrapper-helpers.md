---
name: libvpx/avoid-single-use-wrapper-helpers
description: When encapsulating simple operations like bitwise flag updates, avoid creating single-use setter wrapper functions; provide minimal conversion helpers and mutate at call sites
rubric: design
severity: should_fix
---

**Rule.** Do not create single-use wrapper or setter helper functions that
encapsulate trivial operations like bitwise flag setting or clearing. Instead,
define minimal mapping or conversion helpers (such as enum-to-flag lookups) and
perform the mutation directly at call sites using standard operators.

**Why.** Dedicated wrapper functions for basic operations add unnecessary API
surface, obscure standard idiom semantics, and increase cognitive overhead when
called in only one place. Providing a minimal, reusable mapping helper is more
versatile and keeps state mutations explicit and readable at caller sites.

## Examples

Bad:

```cpp
// Bad: Single-use setter helper encapsulating simple bitwise flag manipulation.
static INLINE void set_ref_flags(int *ref_frame_flags, int8_t ref_frame,
                                 int referenced) {
  assert(ref_frame >= LAST_FRAME && ref_frame <= ALTREF_FRAME);
  if (referenced) {
    *ref_frame_flags |= kVp9RefFlagList[ref_frame];
  } else {
    *ref_frame_flags &= ~kVp9RefFlagList[ref_frame];
  }
}

// Call site:
set_ref_flags(&cpi->ref_frame_flags, ref_frame, 0);
```

Good:

```cpp
// Good: Minimal mapping helper with direct bitwise manipulation at the call site.
static INLINE int ref_frame_to_flag(int8_t ref_frame) {
  static const int kVp9RefFlagList[4] = { 0, VP9_LAST_FLAG, VP9_GOLD_FLAG,
                                          VP9_ALT_FLAG };
  assert(ref_frame >= LAST_FRAME && ref_frame <= ALTREF_FRAME);
  return kVp9RefFlagList[ref_frame];
}

// Call site:
cpi->ref_frame_flags &= ~ref_frame_to_flag(ref_frame);
```
