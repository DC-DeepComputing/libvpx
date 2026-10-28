---
name: libvpx/avoid-hungarian-notation-and-type-prefixes
description: When naming variables, parameters, or arrays, avoid Hungarian notation and redundant type or pointer tags (e.g., p_src, buf_ptr_array, _ptr); use clean, idiomatic identifiers.
rubric: readability
severity: nit
---

**Rule.** Do not use Hungarian notation, type-encoded prefixes (such as `p_`,
`lp_`, `pSrc`, `i_`, `b_`), or redundant pointer/type qualifiers (such as
`_ptr_` or `_ptr`) when naming variables, parameters, or arrays. Use standard,
descriptive `snake_case` identifiers (such as `src`, `dst`, `buffer`, `stride`,
`count`, `buf_array`) without redundant type or indirection tags.

**Why.** Encoding type or indirection status into identifier names adds visual
clutter and violates standard libvpx naming conventions. Because types are
explicitly declared in C/C++, redundant prefixes and qualifiers provide no
safety benefits and impair readability and consistency across parallel
constructs.

## Examples

Bad:

```cpp
// Redundant Hungarian prefixes
uint8_t *p_src, *p_dst;
int i_stride;

// Redundant pointer qualifiers
uint8_t *buf_ptr_array[] = { ref_buf->y_buffer, ref_buf->u_buffer,
                             ref_buf->v_buffer };
pre_buf->buf = buf_ptr_array[plane];
```

Good:

```cpp
// Clean, standard identifier names
uint8_t *src, *dst;
int stride;

// Concise array naming without redundant pointer tag
uint8_t *buf_array[] = { ref_buf->y_buffer, ref_buf->u_buffer,
                         ref_buf->v_buffer };
pre_buf->buf = buf_array[plane];
```
