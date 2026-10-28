---
name: libvpx/avoid-comments-restating-identifier-names
description: When documenting declarations like enum values, struct members, or variables, avoid redundant comments that merely restate identifier names, types, or obvious purposes
rubric: readability
severity: nit
---

**Rule.** Do not add comments to declarations (such as enum constants, struct
fields, or variables) that merely restate or paraphrase identifier names, types,
or self-evident purposes (e.g., stating a flag indicates a condition, a mutex
provides thread safety, or a condition variable is for signalling). Reserve
comments for non-obvious semantics, constraints, thresholds, locking invariants,
or relationships that cannot be inferred from the declaration alone.

**Why.** Restating self-explanatory identifier names, types, or standard library
primitives adds visual clutter without conveying useful context or intent.
Reserving comments for non-obvious thresholds, locking protocols, and invariants
keeps definitions clean and maximizes readability.

## Examples

Bad:

```cpp
typedef enum {
  // encode_breakout is disabled.
  ENCODE_BREAKOUT_DISABLED = 0,
  // encode_breakout is enabled with small max_thresh limit.
  ENCODE_BREAKOUT_LIMITED = 2,
} ENCODE_BREAKOUT_TYPE;

typedef struct {
  /* Flag to indicate jobq has to be terminated */
  int terminate;
  /* Mutex used to keep the functions thread-safe */
  pthread_mutex_t mutex;
  /* Declaration of thread condition variable */
  pthread_cond_t cond;
} JobQueueRowMt;
```

Good:

```cpp
typedef enum {
  ENCODE_BREAKOUT_DISABLED = 0,
  // encode_breakout is enabled with small max_thresh limit.
  ENCODE_BREAKOUT_LIMITED = 2,
} ENCODE_BREAKOUT_TYPE;

typedef struct {
  int terminate;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} JobQueueRowMt;
```
