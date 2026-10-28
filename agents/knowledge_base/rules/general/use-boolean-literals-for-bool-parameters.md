---
name: libvpx/use-boolean-literals-for-bool-parameters
description: When passing arguments to bool parameters or assigning values to bool variables, use explicit true and false literals instead of integer 1 and 0
rubric: readability
severity: nit
---

**Rule.** Use boolean literals (`true` and `false`) instead of integer literals
(`1` and `0`) when passing arguments to `bool` parameters, assigning to `bool`
variables or struct members, or specifying default argument values for boolean
parameters.

**Why.** Using integer literals 1 and 0 for boolean types relies on implicit
numeric conversions, obscures variable and parameter types at call sites, and
degrades code readability. Explicit true and false literals clearly communicate
boolean intent and conform to modern C/C++ style standards.

## Examples

Bad:

```cpp
// Passing integer literals (1 and 0) to bool parameters or assigning to bool
bool is_enabled = 1;
VerifyVp9EncoderMidstreamReconfig(64, 128, 1, 128, 64, 1, 128, 64, 0);
```

Good:

```cpp
// Using explicit boolean literals (true and false)
bool is_enabled = true;
VerifyVp9EncoderMidstreamReconfig(64, 128, true, 128, 64, true, 128, 64, false);
```
