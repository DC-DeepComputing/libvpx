---
name: libvpx/no-action-verbs-in-assertion-messages
description: When writing assertion failure messages in assert(0 && "...") or conditional assertions, describe the invalid state or condition directly without action verbs like 'detect'
rubric: readability
severity: nit
---

**Rule.** When adding failure messages to assertions such as `assert(0 &&
"...")` or `assert(cond && "...")`, describe the invalid condition or error
state directly (e.g., `"invalid highbd iht input"`, `"unexpected block size"`).
Do not prefix assertion messages with action verbs such as 'detect' or 'check'.

**Why.** Assertion strings serve as diagnostic labels that identify the failing
invariant or unreachable path when triggered. Stating the invalid condition
directly without extraneous action verbs keeps messages concise and consistent
with project conventions.

## Examples

Bad:

```cpp
// Action verb 'detect' in assertion message
assert(0 && "detect invalid iht input");

default:
  assert(0 && "detect unexpected block size");
  break;
```

Good:

```cpp
// Directly describes the invalid condition
assert(0 && "invalid highbd iht input");

default:
  assert(0 && "unexpected block size");
  break;
```
