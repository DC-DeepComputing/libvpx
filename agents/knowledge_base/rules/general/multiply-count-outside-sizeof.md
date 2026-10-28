---
name: libvpx/multiply-count-outside-sizeof
description: When calculating buffer byte sizes for memory operations like memcpy or malloc, multiply the count outside sizeof rather than inside sizeof(elem * count)
rubric: correctness
severity: should_fix
---

**Rule.** When computing buffer byte sizes for memory copying or allocation
functions (such as memcpy, memmove, malloc, or vpx_malloc), multiply the element
count outside the sizeof operator. Write sizeof(elem) * count rather than
placing the multiplication inside parentheses like sizeof(elem * count).

**Why.** Passing sizeof(elem * count) evaluates the size of the expression's
resulting data type rather than multiplying the element size by count. For
example, sizeof(int_val * 4) evaluates to sizeof(int) (typically 4 bytes),
causing memory copies and allocations to operate on far fewer bytes than
intended and leading to data truncation or memory corruption.

## Examples

Bad:

```cpp
// Incorrect: evaluates sizeof(type) instead of total byte size
memcpy(&cpi->svc.spatial_layer_sync, data,
       sizeof(data[0] * cpi->svc.number_spatial_layers));

int *buf = vpx_malloc(sizeof(buf[0] * num_elements));
```

Good:

```cpp
// Correct: multiplies element size by the number of elements
memcpy(&cpi->svc.spatial_layer_sync, data,
       sizeof(data[0]) * cpi->svc.number_spatial_layers);

int *buf = vpx_malloc(sizeof(buf[0]) * num_elements);
```
