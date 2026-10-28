---
name: libvpx/place-computation-in-loop-body-not-update-clause
description: When writing for-loops, place data computations and accumulations in the loop body rather than using comma operators in the iteration clause
rubric: readability
severity: nit
---

**Rule.** Do not embed primary computations, accumulations, or processing
statements inside the for-loop increment/update clause (such as chaining
statements with the comma operator) while leaving an empty loop body. Keep the
for-loop header focused strictly on loop iteration and bounds control, and place
execution statements inside the loop body.

**Why.** Smuggling processing logic into the loop header or update expression
obscures what the loop executes on each iteration and makes control flow and
state mutations harder to read and debug.

## Examples

Bad:

```cpp
// Bad: Computation and accumulation embedded in for-loop update clause.
for (j = 0; j < 2; sum += s[j], ++j) {
}
```

Good:

```cpp
// Good: Clear separation between loop control and body computation.
for (j = 0; j < 2; ++j) {
  sum += s[j];
}
```
