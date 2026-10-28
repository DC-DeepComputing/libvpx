---
name: libvpx/use-validated-size-variable-in-allocations
description: When computing and validating allocation sizes in a dedicated variable, pass the validated size variable to allocation calls rather than recomputing expressions
rubric: correctness
severity: should_fix
---

**Rule.** When computing and validating an allocation size in a dedicated
variable (e.g., after an overflow check or alignment calculation), pass that
validated variable to the subsequent allocation call (`malloc()`, `realloc()`,
`vpx_malloc()`, `vpx_memalign()`). Do not recompute or duplicate the size
expression at the allocation site.

**Why.** Duplicating or recalculating size expressions at allocation call sites
can introduce discrepancies, such as off-by-one errors or omitted alignment
offsets, and risks allocating a different amount of memory than was validated
for overflow.

## Examples

Bad:

```cpp
const uint64_t total_size =
    (uint64_t)size + align - 1 + ADDRESS_STORAGE_SIZE;
if (!check_size_argument_overflow(1, total_size)) return NULL;

new_addr = realloc(addr, size + align + ADDRESS_STORAGE_SIZE);
```

Good:

```cpp
const uint64_t total_size =
    (uint64_t)size + align - 1 + ADDRESS_STORAGE_SIZE;
if (!check_size_argument_overflow(1, total_size)) return NULL;

new_addr = realloc(addr, (size_t)total_size);
```
