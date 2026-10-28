---
name: libvpx/avoid-single-use-boolean-temporaries
description: When evaluating boolean predicates, helper functions, or relational conditions in conditionals, test them directly inline rather than declaring single-use intermediate variables
rubric: readability
severity: nit
---

**Rule.** Do not declare redundant single-use local variables solely to capture
the result of a boolean helper function, predicate expression, or relational
condition before an `if` statement. Evaluate the predicate or helper call
directly within the conditional expression (e.g., `if (!is_flat(...))`).
Additionally, avoid intermediate boolean variables defined with inverted
polarity (e.g., `const int less8x8 = bsize < BLOCK_8X8; if (!less8x8)`);
evaluate the inverted relational condition directly inline (e.g., `if (bsize >=
BLOCK_8X8)`).

**Why.** Inlining simple predicate calls and relational checks directly into
conditional expressions avoids unnecessary variable declarations, reduces scope
pollution and cognitive overhead, and eliminates redundant indirection from
negated temporary flags.

## Examples

Bad:

```cpp
// Bad: Single-use temporary capturing helper predicate
int flat_static_blocks =
    is_superblock_flat_static(cpi, sb_row_index, sb_col_index);
if (!flat_static_blocks) {
  process_block(cpi);
}

// Bad: Single-use temporary with inverted polarity
const int less8x8 = bsize < BLOCK_8X8;
if (!less8x8 && eobtotal == 0) {
  mi->skip = 1;
}
```

Good:

```cpp
// Good: Helper predicate evaluated directly inline
if (!is_superblock_flat_static(cpi, sb_row_index, sb_col_index)) {
  process_block(cpi);
}

// Good: Relational condition checked directly inline
if (bsize >= BLOCK_8X8 && eobtotal == 0) {
  mi->skip = 1;
}
```
