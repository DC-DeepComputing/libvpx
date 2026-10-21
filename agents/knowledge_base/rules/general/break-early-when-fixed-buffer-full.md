---
name: libvpx/break-early-when-fixed-buffer-full
rubric: safety
severity: should_fix
---

**Rule.** When populating a fixed-capacity buffer or lookup table across nested
loops, break, return, or jump out of the loops as soon as the buffer is full. Do
not guard each individual write with a bounds check while continuing outer loop
iterations when subsequent computations are unused.

**Why.** Continuing loop iterations after a fixed-size buffer or table is full
wastes CPU cycles executing calculations whose results will never be stored.
Exiting early eliminates unnecessary iterations and bounds-check overhead.

## Examples

Bad:

```cpp
for (i = -32; i < 32; ++i) {
  const int a_i = (int)(0.5 + 256 * gaussian(sigma, 0, i));
  if (a_i) {
    for (j = 0; j < a_i; ++j) {
      if (next + j < 256) char_dist[next + j] = (int8_t)i;
    }
    next = next + j;
  }
}
```

Good:

```cpp
for (i = -32; i < 32; ++i) {
  const int a_i = (int)(0.5 + 256 * gaussian(sigma, 0, i));
  if (a_i) {
    for (j = 0; j < a_i; ++j) {
      if (next + j >= 256) goto set_noise;
      char_dist[next + j] = (int8_t)i;
    }
    next = next + j;
  }
}
```
