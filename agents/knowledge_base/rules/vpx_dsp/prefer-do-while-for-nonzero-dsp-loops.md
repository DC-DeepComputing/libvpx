---
name: libvpx/vpx_dsp/prefer-do-while-for-nonzero-dsp-loops
rubric: safety
severity: should_fix
---

**Rule.** In performance-critical DSP and SIMD kernel loops where the iteration
count is guaranteed to be non-zero (such as known block dimensions), prefer 'do
{ ... } while (...)' over 'for' loops to eliminate the redundant initial entry
condition check.

**Why.** When block dimensions are guaranteed to be positive, the first
iteration always executes. Using a do-while loop eliminates the initial
comparison and branch before loop entry, improving instruction throughput in
tight DSP kernels.

## Examples

Bad:

```cpp
for (i = 0; i < h; i += 2) {
  // process rows
}
```

Good:

```cpp
int i = 0;
do {
  // process rows
  i += 2;
} while (i < h);
```
