---
name: libvpx/group-buffer-pointer-and-size-parameters
description: When declaring or defining functions with buffer pointers and associated sizes, group each pointer directly adjacent to its size parameter instead of separating them.
rubric: readability
severity: nit
---

**Rule.** When declaring or defining functions that accept a buffer pointer and
its associated size, length, or count parameter, place the pointer and its
corresponding size parameter directly adjacent to each other. Do not separate
them with unrelated configuration or input parameters.

**Why.** Keeping buffer pointers and their associated size/length arguments
adjacent makes function signatures intuitive to read and reduces the risk of
callers passing arguments out of order or mismatching multiple buffer sizes.

## Examples

Bad:

```cpp
// Bad: 'size' is separated from the 'noise' buffer by an unrelated 'sigma' argument.
int vpx_setup_noise(int size, double sigma, char *noise);
```

Good:

```cpp
// Good: 'noise' buffer pointer and its 'size' parameter are grouped together.
int vpx_setup_noise(double sigma, char *noise, int size);
```
