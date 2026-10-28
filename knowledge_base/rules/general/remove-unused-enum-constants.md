---
name: libvpx/remove-unused-enum-constants
description: When declaring or modifying enum definitions, remove dead or unused enumerators and avoid creating redundant enum abstractions when a single binary flag suffices
rubric: design
severity: should_fix
---

**Rule.** Do not declare or retain unused enumerator values in enum definitions,
and avoid creating enum or bitmask abstractions when only a single binary state
is needed. Remove dead enum constants that are not referenced anywhere in the
codebase, and track single binary states using simple integer or boolean
variables instead of redundant enum types.

**Why.** Unused enumerator constants and redundant enum types clutter header
interfaces, mislead developers into believing unsupported modes exist, and add
unnecessary abstraction boilerplate over simple binary states.

## Examples

Bad:

```cpp
// Dead enumerator in a multi-state enum:
typedef enum _job_type {
  PARSE_JOB,
  RECON_JOB,
  LPF_JOB,
  FULL_DECODE  // Unreferenced in the codebase.
} job_type;

// Redundant enum bitmask for a single binary flag:
typedef enum {
  NO_FLAG = 0x0,
  RECON_DONE = 0x1,
  LPF_QUEUED = 0x2,  // Unused
  MAX_FLAGS
} FLAG;
```

Good:

```cpp
// Retain only actively used enumerators:
typedef enum _job_type {
  PARSE_JOB,
  RECON_JOB,
  LPF_JOB
} job_type;

// Use a simple integer or boolean for single binary flags:
int recon_done;
```
