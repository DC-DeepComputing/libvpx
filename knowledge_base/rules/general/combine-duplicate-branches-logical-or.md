---
name: libvpx/combine-duplicate-branches-logical-or
description: When consecutive if and else-if branches execute identical statements, combine their conditions using logical OR (||) relying on short-circuit evaluation
rubric: readability
severity: nit
---

**Rule.** When consecutive if and else-if branches execute identical statements
or assign the same value, combine their conditions into a single if statement
using logical OR (||). Rely on standard short-circuit evaluation so that
subsequent dereferences or operations safely follow earlier pointer or validity
checks.

**Why.** Duplicating the same action across multiple branch bodies introduces
unnecessary boilerplate and increases maintenance overhead. Merging them into a
single condition with short-circuit logical OR makes the intent clearer and
preserves safety without nested or duplicated logic.

## Examples

Bad:

```cpp
if (ref == NULL)
  is_key_frame = 1;
else if (ref->y_crop_height != cm->height || ref->y_crop_width != cm->width)
  is_key_frame = 1;
```

Good:

```cpp
if (ref == NULL || ref->y_crop_height != cm->height ||
    ref->y_crop_width != cm->width)
  is_key_frame = 1;
```
