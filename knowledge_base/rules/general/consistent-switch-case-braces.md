---
name: libvpx/consistent-switch-case-braces
description: When any case block in a switch statement uses curly braces, enclose all other case and default clauses in curly braces as well
rubric: readability
severity: should_fix
---

**Rule.** In a `switch` statement, if any `case` block uses curly braces (`{ ...
}`), enclose all other `case` and `default` blocks in curly braces as well. Do
not mix braced and unbraced `case` or `default` clauses within the same `switch`
statement.

**Why.** Enclosing all case and default blocks in braces consistently when any
case requires a local scope ensures uniform formatting and indentation, improves
readability, and avoids scoping confusion.

## Examples

Bad:

```cpp
switch (cols) {
  case left_16: {
    int i;
    array_transpose_16x16(in0, in1);
    for (i = 0; i < 16; ++i) {
      store[i] = in0[16 + i];
      store[16 + i] = in1[16 + i];
    }
    break;
  }
  case right_16:
    array_transpose_16x16_2(store, &store[16], in0, in1);
    break;
  default:
    assert(0);
}
```

Good:

```cpp
switch (cols) {
  case left_16: {
    int i;
    array_transpose_16x16(in0, in1);
    for (i = 0; i < 16; ++i) {
      store[i] = in0[16 + i];
      store[16 + i] = in1[16 + i];
    }
    break;
  }
  case right_16: {
    array_transpose_16x16_2(store, &store[16], in0, in1);
    break;
  }
  default: {
    assert(0);
    break;
  }
}
```
