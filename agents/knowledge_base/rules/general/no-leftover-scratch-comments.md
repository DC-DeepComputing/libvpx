---
name: libvpx/no-leftover-scratch-comments
description: When writing, refactoring, or cleaning up source files, delete commented-out code, `#if 0` guards, temporary scratch markers, and obsolete or satisfied TODO comments
rubric: readability
severity: nit
---

**Rule.** Do not leave commented-out code, disabled function calls, dead code
blocks, obsolete function definitions, `#if 0` preprocessor guards, temporary
developer scratch notes or debug markers (such as `// remove`, `// temp`, `//
WIP`), or obsolete/satisfied TODO or bug-tracking comments in source and header
files. Completely delete superseded, inactive, or resolved comments and code
blocks rather than commenting them out. Rely on version control (git) history to
recover reference implementations or past logic if needed. If documenting why an
alternative approach was rejected or non-viable, explain it with clear prose
comments instead of disabled code blocks.

**Why.** Commented-out code, disabled declarations, leftover scratch markers,
and obsolete TODO comments create visual clutter, generate noise in diffs, and
quickly rot as surrounding APIs evolve. They also confuse reviewers and
maintainers regarding active runtime logic or the resolution status of
referenced bugs and disabled tests. Version control cleanly preserves historical
code without polluting active source files.

## Examples

Bad:

```cpp
// Bad: Commented-out logic, disabled declarations, and #if 0 guards
vp9_zero(svc->update_reference);
// vp9_zero(svc->update_golden);
#if 0
int VP8RateControlRTC::GetLoopfilterLevel() const { return lf->filter_level; }
#endif

// Bad: Temporary scratch notes and leftover debug markers
// temp / remove
s1_10 = vqsubq_s16(s2_9, s2_10);

// Bad: Resolved or obsolete TODO comments left after fixing a bug or re-enabling a test
// TODO: b/442105459 - Re-enable after fix msan
TEST(EncodeAPI, Buganizer442105459_4RowTiles) {
  // Test implementation...
}
```

Good:

```cpp
// Good: Keep only active logic; rely on version control for history
vp9_zero(svc->update_reference);
int VP8RateControlRTC::GetQP() const { return q_; }

// Good: Clean active implementation; explain non-obvious decisions in prose
s1_10 = vqsubq_s16(s2_9, s2_10);
// Can't use transpose_s16_8x8() because values are in two 4x8 columns.

// Good: Delete satisfied TODO comments when the bug is resolved or test re-enabled
TEST(EncodeAPI, Buganizer442105459_4RowTiles) {
  // Test implementation...
}
```
