---
name: libvpx/prefer-memset-over-chained-assignments-to-zero-buffers
description: When zeroing fixed-size arrays or buffers, use memset() with sizeof rather than manual chained scalar assignments like a[0] = a[1] = 0
rubric: readability
severity: nit
---

**Rule.** When clearing or zeroing fixed-size arrays or buffers, use
`memset(buf, 0, N * sizeof(*buf))` instead of manual chained scalar assignments
(such as `buf[0] = buf[1] = ... = 0`). Rely on compiler memset expansion to
generate optimal inline instructions for constant buffer sizes.

**Why.** Chaining scalar assignments to zero an array creates visual clutter,
increases manual unrolling complexity, and is error-prone when array bounds
change. Compilers reliably optimize and inline fixed-size memset() calls into
efficient architecture-specific instructions.

## Examples

Bad:

```cpp
output[0] = output[1] = output[2] = output[3] = 0;
```

Good:

```cpp
memset(output, 0, 4 * sizeof(*output));
```
