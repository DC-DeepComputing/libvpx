---
name: libvpx/inline-or-const-qualify-single-use-locals
description: When computing simple single-use expressions for loop bounds or call arguments, inline them directly or declare retained local variables as const
rubric: readability
severity: nit
---

**Rule.** Inline simple single-use local expressions directly at their point of
use (such as in loop termination bounds or function call arguments) rather than
introducing unnecessary temporary variables. If retaining a named local variable
is necessary for readability or complex subexpressions, qualify the variable as
const.

**Why.** Declaring non-const temporary variables for simple single-use
expressions introduces mutable state, cognitive overhead, and unnecessary
declaration boilerplate. Inlining simple expressions keeps loop headers and
function bodies concise, while const-qualification explicitly communicates
immutability when intermediate variables are retained.

## Examples

Bad:

```cpp
int elements = width * height;
int i;
for (i = 0; i < elements; i += 16) {
  // Process elements...
}
```

Good:

```cpp
int i;
// Inline simple single-use expressions directly:
for (i = 0; i < width * height; i += 16) {
  // Process elements...
}

// Or declare as const if retaining the variable for clarity:
const int total_elements = width * height;
```
