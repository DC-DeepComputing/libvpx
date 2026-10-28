---
name: libvpx/standard-data-structure-helper-names
description: When defining helper functions for custom data structures, use standard terminology such as top or push rather than ad-hoc verbs like read or write
rubric: readability
severity: nit
---

**Rule.** When defining helper functions or macros for custom data structures
(such as stacks, queues, or ring buffers), use standard container terminology
(e.g., stack_top, stack_push, stack_pop) rather than ad-hoc or generic names
like stack_read or stack_write.

**Why.** Standard container nomenclature immediately conveys access patterns,
element ordering, and state mutation semantics to readers, whereas generic verbs
like read or write obscure whether an operation is non-destructive or modifies
container state.

## Examples

Bad:

```cpp
// Ambiguous helper name obscures whether it peeks or consumes elements
static INLINE int stack_read(int *stack) {
  return stack[0];
}
```

Good:

```cpp
// Standard container terminology clearly indicates inspecting top element
static INLINE int stack_top(const int *stack) {
  return stack[0];
}
```
