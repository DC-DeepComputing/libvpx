---
name: libvpx/use-explicit-casts-over-bitmasks-for-narrowing
description: When converting or truncating integer expressions to narrow integer types (like uint8_t or int8_t), use explicit type casts rather than bitmasking with & 0xff
rubric: readability
severity: nit
---

**Rule.** When converting, narrowing, or assigning integer expressions to narrow
integer types (such as uint8_t, int8_t, uint16_t, or int16_t), use explicit type
casts (e.g., (uint8_t)(...)) consistently rather than applying bitmasks (e.g., &
0xff or & 0xffff) to truncate values. Do not mix explicit type casts and bitwise
truncation masks across sibling assignments or expressions.

**Why.** Explicit type casts directly express the target integer type and handle
signedness conversion, whereas bitwise truncation masks (& 0xff) add redundant
operations, do not convert signed types, and create inconsistent coding styles
when mixed with casts.

## Examples

Bad:

```cpp
// Mixing bitmasks with casts for narrow integer conversion
*oq0 = (signed_char_clamp(qs0 - filter1) ^ 0x80) & 0xff;
*op0 = (uint8_t)(signed_char_clamp(ps0 + filter2) ^ 0x80);
```

Good:

```cpp
// Consistently use explicit type casts for narrowing conversions
*oq0 = (uint8_t)(signed_char_clamp(qs0 - filter1) ^ 0x80);
*op0 = (uint8_t)(signed_char_clamp(ps0 + filter2) ^ 0x80);
```
