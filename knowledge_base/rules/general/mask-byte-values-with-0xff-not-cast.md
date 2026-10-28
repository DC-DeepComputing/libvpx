---
name: libvpx/mask-byte-values-with-0xff-not-cast
description: When assigning wider integer expressions into uint8_t buffers or variables, mask with & 0xff rather than using explicit type casts
rubric: readability
severity: should_fix
---

**Rule.** When assigning a wider integer expression (such as shifted values or
multi-byte calculations) into a uint8_t buffer or variable, mask the value with
& 0xff instead of using explicit type casts like (int8_t) or (uint8_t). Explicit
masking ensures value truncation is well-defined, avoids signed/unsigned
conversion pitfalls, and satisfies integer sanitizers.

**Why.** Casting wider or signed expressions to (int8_t) or (uint8_t) can
trigger integer sanitizer warnings or introduce signed conversion bugs when
writing to unsigned byte arrays. Using bitwise masking (& 0xff) explicitly
constrains the value to an unsigned byte across all architectures and compilers.

## Examples

Bad:

```cpp
// Anti-pattern: Type casting wider or signed values when writing to a uint8_t buffer
br->buffer[br->pos++] = (int8_t)(lowvalue >> (24 - offset));
```

Good:

```cpp
// Recommended: Masking with & 0xff explicitly truncates to an unsigned byte
br->buffer[br->pos++] = (lowvalue >> (24 - offset)) & 0xff;
```
