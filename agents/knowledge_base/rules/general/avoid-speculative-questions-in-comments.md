---
name: libvpx/avoid-speculative-questions-in-comments
description: When writing code comments or TODOs, state invariants definitively and describe concrete, actionable tasks or investigations rather than observations or speculations
rubric: readability
severity: nit
---

**Rule.** Do not leave speculative questions, uncertain musings, or rhetorical
queries in code comments; state invariants and assumptions definitively or
remove speculative commentary. When adding `TODO` comments, clearly specify the
concrete follow-up task, refactoring, or investigation to be done (referencing
an issue or tracking bug when appropriate) rather than merely noting an
observation, compiler quirk, or temporary workaround.

**Why.** Speculative questions and musings create ambiguity about whether code
operates on verified invariants, while vague TODO comments that only describe
observations fail to inform maintainers what changes are actually needed.
Explicit invariant statements and actionable TODOs ensure expectations and
future follow-up work are clear and trackable.

## Examples

Bad:

```cpp
// Are input frames already extended or should we consider that here?
// In production code I believe it is extended.

// TODO(username): Strangely there is only a conversion warning on
// int64_t to int32_t but not for int64_t to int16_t. Constants fit in int16_t.
```

Good:

```cpp
// Manually extend left border to avoid reading past valid buffer data.

// TODO(webm:1320): Investigate using int16_t for constants to avoid
// bouncing between types and resolve conversion warnings.
```
