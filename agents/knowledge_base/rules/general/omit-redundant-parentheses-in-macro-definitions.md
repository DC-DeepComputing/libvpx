---
name: libvpx/omit-redundant-parentheses-in-macro-definitions
description: When defining preprocessor macros for single numeric literals or simple tokens, omit redundant enclosing parentheses around the literal value
rubric: readability
severity: nit
---

**Rule.** Do not enclose single numeric literals, character constants, or simple
single-token values in unnecessary outer parentheses in #define macro
definitions. Use parentheses around compound expressions where operator
precedence could be ambiguous, but write simple constant macros as #define NAME
value directly.

**Why.** While parentheses in macro expansions are necessary for compound
arithmetic or bitwise expressions to protect operator precedence, they are
redundant on single numeric literals and add unnecessary syntax noise to header
declarations and constant definitions.

## Examples

Bad:

```cpp
#define VPX_TPL_ABI_VERSION (4) /**<\hideinitializer*/
#define VPX_MAX_PLANES (3)
```

Good:

```cpp
#define VPX_TPL_ABI_VERSION 4 /**<\hideinitializer*/
#define VPX_MAX_PLANES 3
```
