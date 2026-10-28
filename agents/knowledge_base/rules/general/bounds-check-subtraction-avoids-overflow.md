---
name: libvpx/bounds-check-subtraction-avoids-overflow
description: When checking if an offset or chunk length exceeds remaining buffer capacity, formulate bounds checks as chunk > size - used to prevent integer overflow
rubric: correctness
severity: nit
---

**Rule.** When checking whether adding an offset or chunk length to an
accumulated count exceeds a total buffer size, formulate the bounds check as
`chunk > size - used` rather than `used + chunk > size`. Ensure the invariant
`used <= size` is preserved prior to subtraction to prevent underflow while
avoiding integer overflow.

**Why.** Computing `used + chunk` can overflow integer or `size_t` limits when
inputs are large or untrusted, wrapping around and bypassing the bounds check.
Subtracting consumed bytes from total capacity (`size - used`) avoids integer
overflow when `used <= size` holds.

## Examples

Bad:

```cpp
// Prone to integer overflow if `used + nb` wraps around:
if (used + nb > size) {
  nb = size - used;
}
```

Good:

```cpp
// Safe from integer overflow when invariant `used <= size` holds:
if (nb > size - used) {
  nb = size - used;
}
```
