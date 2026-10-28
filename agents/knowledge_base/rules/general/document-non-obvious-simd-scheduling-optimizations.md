---
name: libvpx/document-non-obvious-simd-scheduling-optimizations
description: When implementing non-obvious SIMD or microarchitectural scheduling choices like splitting dependency chains across parallel execution pipelines, document the performance rationale
rubric: readability
severity: nit
---

**Rule.** When SIMD or assembly routines use non-obvious instruction scheduling,
unrolling, or register allocation strategies—such as splitting an accumulation
dependency chain across multiple parallel execution pipelines rather than
coalescing into fewer operations—precede the logic with a comment explaining the
microarchitectural rationale. Document why the split or ordering was chosen over
mathematically minimal or simpler serial alternatives.

**Why.** Without explanatory comments, future contributors or reviewers may
attempt to coalesce independent dependency chains to minimize instruction counts
or register usage, unknowingly serializing operations and degrading throughput
on multi-issue or dual-pipe execution hardware.

## Examples

Bad:

```cpp
// diff is 15-bits, the squares 30, so we can store 2 in 31-bits before
// accumulating them in 64-bits.
err0 = vmull_u16(vget_low_u16(diff0), vget_low_u16(diff0));
err0 = vmlal_u16(err0, vget_high_u16(diff0), vget_high_u16(diff0));
err1 = vmull_u16(vget_low_u16(diff1), vget_low_u16(diff1));
err1 = vmlal_u16(err1, vget_high_u16(diff1), vget_high_u16(diff1));
```

Good:

```cpp
// diff is 15-bits, the squares 30, so in theory we can store 4 in 32-bits
// before accumulating them in 64-bits. However splitting into 2 mull, mlal
// pairs is beneficial since it allows us to use both Neon
// multiply-accumulate pipes - on CPUs that have them - rather than having
// a single chain of 4 instructions executing serially.
err0 = vmull_u16(vget_low_u16(diff0), vget_low_u16(diff0));
err0 = vmlal_u16(err0, vget_high_u16(diff0), vget_high_u16(diff0));
err1 = vmull_u16(vget_low_u16(diff1), vget_low_u16(diff1));
err1 = vmlal_u16(err1, vget_high_u16(diff1), vget_high_u16(diff1));
```
