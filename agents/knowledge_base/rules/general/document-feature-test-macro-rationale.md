---
name: libvpx/document-feature-test-macro-rationale
description: When defining platform or feature test macros like _GNU_SOURCE or _POSIX_C_SOURCE before includes, add a comment explaining which specific API requires them
rubric: readability
severity: nit
---

**Rule.** When defining platform or feature test macros (such as `_GNU_SOURCE`,
`_POSIX_C_SOURCE`, `_XOPEN_SOURCE`, or `_DEFAULT_SOURCE`) at file scope before
system headers, precede the definition with a comment documenting which specific
APIs, types, or compiler extensions require the macro. Do not define feature
test macros without explaining why default library symbol visibility is
insufficient.

**Why.** Feature test macros alter standard library header visibility and system
symbol exposure across compilation units. Documenting the specific required API
(such as `pthread_setname_np()`) clarifies why the definition is necessary, aids
portability reviews on other platforms, and prevents accidental deletion or
redundant definitions.

## Examples

Bad:

```cpp
// This must be before any #include statements.
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
```

Good:

```cpp
// Enable GNU extensions in glibc so that we can call pthread_setname_np().
// This must be before any #include statements.
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
```
