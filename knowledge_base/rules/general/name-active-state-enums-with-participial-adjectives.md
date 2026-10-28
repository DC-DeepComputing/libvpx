---
name: libvpx/name-active-state-enums-with-participial-adjectives
description: When defining enum constants representing active or ongoing object states or thread status, use progressive or participial adjectives rather than bare verbs or nouns
rubric: readability
severity: nit
---

**Rule.** When defining enum constants that represent active, ongoing, or
transitional states of an object, worker thread, or asynchronous task, name them
using progressive or present-participial adjectives (such as WORKING, RUNNING,
PROCESSING, WAITING) rather than bare verbs or nouns (such as WORK, RUN, WAIT).

**Why.** Progressive and participial adjectives clearly denote an active,
ongoing condition or background activity rather than a command, action
directive, or generic noun, improving clarity during state transitions and
condition checks.

## Examples

Bad:

```cpp
typedef enum {
  VPX_WORKER_STATUS_NOT_OK = 0,
  VPX_WORKER_STATUS_OK,
  VPX_WORKER_STATUS_WORK
} VPxWorkerStatus;
```

Good:

```cpp
typedef enum {
  VPX_WORKER_STATUS_NOT_OK = 0,
  VPX_WORKER_STATUS_OK,
  VPX_WORKER_STATUS_WORKING
} VPxWorkerStatus;
```
