---
name: libvpx/avoid-duplicate-todo-comments-across-related-functions
description: When tracking cleanups or refactoring across related, split, or copied functions, place a single TODO at the primary implementation and omit duplicate or stale comments
rubric: readability
severity: nit
---

**Rule.** Do not duplicate identical `TODO` comments across related, split, or
adapted helper functions sharing the same task, and do not blindly replicate
existing `TODO` comments when copying or adapting code. Place a single `TODO`
comment at the primary shared entry point or function definition, verify whether
the task applies before copying, and omit redundant or stale `TODO` comments
from coupled call sites or cloned blocks.

**Why.** Duplicating identical `TODO` comments across multiple functions creates
visual clutter, redundant tracking noise, and stale action items that drift out
of sync or outlive their context when refactoring is partially addressed.

## Examples

Bad:

```cpp
// Variant 1: Duplicating identical TODO across split or related functions
// TODO(username): eliminate bsize and subsize in future commits.
static void parse_partition_leaf(TileWorkerData *twd, VP9Decoder *pbi,
                                 BLOCK_SIZE bsize, int n4x4_l2) {
  // ...
}

// TODO(username): eliminate bsize and subsize in future commits.
static void parse_partition(TileWorkerData *twd, VP9Decoder *pbi,
                            BLOCK_SIZE bsize, int n4x4_l2) {
  // ...
}

// Variant 2: Blindly copying existing TODO into new adapted function
// TODO(username): eliminate bsize and subsize in future commits.
static void recon_partition(TileWorkerData *twd, VP9Decoder *const pbi,
                            int mi_row, int mi_col, BLOCK_SIZE bsize,
                            int n4x4_l2) {
  // ...
}
```

Good:

```cpp
// Variant 1: Single TODO at primary shared entry point
static void parse_partition_leaf(TileWorkerData *twd, VP9Decoder *pbi,
                                 BLOCK_SIZE bsize, int n4x4_l2) {
  // ...
}

// TODO(username): eliminate bsize and subsize in future commits.
static void parse_partition(TileWorkerData *twd, VP9Decoder *pbi,
                            BLOCK_SIZE bsize, int n4x4_l2) {
  // ...
}

// Variant 2: Omit redundant or stale TODO from copied/adapted function
static void recon_partition(TileWorkerData *twd, VP9Decoder *const pbi,
                            int mi_row, int mi_col, BLOCK_SIZE bsize,
                            int n4x4_l2) {
  // ...
}
```
