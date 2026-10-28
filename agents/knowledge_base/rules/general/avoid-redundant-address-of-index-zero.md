---
name: libvpx/avoid-redundant-address-of-index-zero
description: When computing pointer offsets from an array, use direct pointer arithmetic or indexed addressing instead of redundant (&arr[0]) + offset expressions
rubric: readability
severity: nit
---

**Rule.** Use direct array pointer arithmetic (`arr + offset`) or indexed
addressing (`&arr[offset]`) instead of redundant `(&arr[0]) + offset` or
`&arr[0] + offset` expressions. In C and C++, array identifiers naturally decay
to a pointer to their first element in value expressions.

**Why.** Writing `(&arr[0]) + offset` is needlessly verbose and adds redundant
parentheses and address-of operations compared to clean pointer arithmetic or
direct array indexing.

## Examples

Bad:

```cpp
rv2[i] = (&vpx_rv[0]) + (i & 7);
```

Good:

```cpp
rv2[i] = vpx_rv + (i & 7);
```
