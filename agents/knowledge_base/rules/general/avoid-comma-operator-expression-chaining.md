---
name: libvpx/avoid-comma-operator-expression-chaining
description: When writing sequential assignments, operations, or statements, place each on its own line instead of chaining multiple expressions with comma operators
rubric: readability
severity: nit
---

**Rule.** Write each assignment or statement on its own line rather than
chaining multiple expressions into a single statement using the comma operator.
Initialize local variables at declaration where possible rather than declaring
them uninitialized and assigning across chained expressions.

**Why.** Chaining multiple assignments or expressions on a single line with
comma operators obscures individual operations, hinders line-by-line debugging
and breakpoint stepping, and harms code readability.

## Examples

Bad:

```cpp
void *temp;
temp = *a_ptr, *a_ptr = *b_ptr, *b_ptr = temp;
```

Good:

```cpp
void *temp = *a_ptr;
*a_ptr = *b_ptr;
*b_ptr = temp;
```
