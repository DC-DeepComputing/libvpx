---
name: libvpx/todo-use-imperative-mood
description: When writing TODO comments describing pending work, phrase the task using imperative action verbs instead of infinitive phrases like 'To turn on'.
rubric: readability
severity: nit
---

**Rule.** Phrase TODO comments using imperative action verbs (e.g., 'Turn on
...', 'Investigate ...', 'Check ...') rather than infinitive clauses (e.g., 'To
turn on ...', 'To investigate ...') or descriptive phrases. Ensure the comment
clearly states the direct action to be performed.

**Why.** TODO comments represent pending action items; using concise imperative
verbs makes tasks immediately actionable, clear, and consistent across the
codebase.

## Examples

Bad:

```cpp
// TODO(zoeliu): To turn on the option to disable extra ALTREFs for still GF groups
```

Good:

```cpp
// TODO(zoeliu): Turn on the option to disable extra ALTREFs for still GF groups.
```
