---
name: libvpx/omit-redundant-parentheses-in-address-of-member
description: When taking the address of a struct or class member using -> or ., omit redundant parentheses such as &(ptr->member) or &(obj.member)
rubric: readability
severity: nit
---

**Rule.** Do not use redundant parentheses around member access expressions when
taking their address; write `&ptr->member` or `&obj.member` instead of
`&(ptr->member)` or `&(obj.member)`.

**Why.** In C and C++, the member access operators `->` and `.` have higher
operator precedence than the unary address-of operator `&`. Parenthesizing the
member access adds unnecessary syntactic noise without changing precedence or
semantics.

## Examples

Bad:

```cpp
vp9_denoiser_free(&(cpi->denoiser));
vpx_free(&(ctx.buffer));
```

Good:

```cpp
vp9_denoiser_free(&cpi->denoiser);
vpx_free(&ctx.buffer);
```
