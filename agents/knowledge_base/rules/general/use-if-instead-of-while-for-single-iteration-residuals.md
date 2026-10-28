---
name: libvpx/use-if-instead-of-while-for-single-iteration-residuals
description: When handling residual elements or descending chunks after loops, use if branches instead of while loops and omit dead trailing updates at function exit
rubric: readability
severity: nit
---

**Rule.** When processing residual elements or tail buffers where preceding
chunk loops guarantee the remaining count is strictly less than twice the chunk
size (such as descending power-of-two chunks 8 and 4 following a 16-element
loop), use 'if' conditional branches instead of 'while' loops because each
remainder chunk executes at most once. Additionally, in the final residual
handling block or at function exit, omit dead pointer increments (e.g., src +=
N, dst += N) and remaining count decrements (e.g., width -= N) when control flow
immediately returns or goes out of scope without reading the modified values.

**Why.** Using a 'while' loop for remainder logic that is mathematically
guaranteed to execute at most once miscommunicates control-flow intent and
falsely suggests repetitive looping. Using an 'if' branch explicitly signals a
single-step residual handler. Furthermore, updating local pass-by-value pointer
arguments, offset variables, or remaining size counters immediately before
function exit is dead code; omitting these trailing mutations keeps functions
concise and avoids misleading readers into expecting subsequent operations.

## Examples

Bad:

```cpp
while (width >= 16) {
  process_16(src, dst);
  src += 16;
  dst += 16;
  width -= 16;
}
// Preceding loop guarantees width < 16, so width >= 8 and width >= 4 run <= 1 time
while (width >= 8) {
  process_8(src, dst);
  src += 8;
  dst += 8;
  width -= 8;
}
while (width >= 4) {
  process_4(src, dst);
  // Dead pointer and counter mutations at function exit
  src += 4;
  dst += 4;
  width -= 4;
}
```

Good:

```cpp
while (width >= 16) {
  process_16(src, dst);
  src += 16;
  dst += 16;
  width -= 16;
}
// Use 'if' because width < 16 ensures each descending chunk runs <= 1 time
if (width >= 8) {
  process_8(src, dst);
  src += 8;
  dst += 8;
  width -= 8;
}
if (width >= 4) {
  process_4(src, dst);
  // Omit dead pointer/size updates in the final residual block before return
}
```
