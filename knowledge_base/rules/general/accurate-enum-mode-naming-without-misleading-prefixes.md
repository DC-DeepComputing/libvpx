---
name: libvpx/accurate-enum-mode-naming-without-misleading-prefixes
description: When defining enum constants or configuration modes, name enumerators to accurately reflect per-item semantics without misleading narrowing prefixes like SINGLE_
rubric: readability
severity: nit
---

**Rule.** Name enum constants and configuration modes to accurately convey their
operational semantics without misleading narrowing prefixes. Avoid prefixes like
`SINGLE_` when the mode permits any item, stream, or layer to operate
independently rather than restricting behavior to an isolated single entity.

**Why.** Misleading prefixes such as `SINGLE_` imply that an operation is
exclusively restricted to one item in isolation (e.g., dropping only one spatial
layer). Using accurate, representative names like `LAYER_DROP` clarifies that
the mode applies to any layer independently and prevents misuse across the API.

## Examples

Bad:

```cpp
typedef enum {
  CONSTRAINED_LAYER_DROP,
  SINGLE_LAYER_DROP, /**< Misleading: implies only one layer drops */
  FULL_SUPERFRAME_DROP,
} SVC_LAYER_DROP_MODE;
```

Good:

```cpp
typedef enum {
  CONSTRAINED_LAYER_DROP,
  LAYER_DROP, /**< Accurate: any spatial layer can drop independently */
  FULL_SUPERFRAME_DROP,
} SVC_LAYER_DROP_MODE;
```
