---
name: libvpx/use-assert-zero-and-message-for-unreachable
description: When marking unreachable branches or invariant failures with an explanation, use assert(0 && "message") instead of printing with printf before assert(0)
rubric: readability
severity: nit
---

**Rule.** When asserting an unreachable code path or invariant failure with a
descriptive error message, use the `assert(0 && "error message")` idiom. Do not
pair a separate `printf()` or `fprintf()` statement with `assert(0)`.

**Why.** Embedding the diagnostic string within the assertion condition ensures
the message is printed only when assertions are enabled, avoids polluting
stdout/stderr in library code, and cleanly compiles away along with the string
literal in release (NDEBUG) builds.

## Examples

Bad:

```cpp
default:
  square_bsize = BLOCK_INVALID;
  printf("ERROR: invalid block size %d\n", bsize);
  assert(0);
  break;
```

Good:

```cpp
default:
  square_bsize = BLOCK_INVALID;
  assert(0 && "ERROR: invalid block size");
  break;
```
