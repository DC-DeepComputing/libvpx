---
name: libvpx/pascal-case-typedef-and-tag-names
description: When declaring struct, enum, union, or typedef names in C code, use PascalCase and avoid snake_case, leading-underscore tags, or POSIX-reserved _t suffixes
rubric: readability
severity: should_fix
---

**Rule.** When defining new structs, unions, enums, or `typedef` aliases in
libvpx C/C++ code, name types using PascalCase (`UpperCamelCase`, e.g.,
`JobType`, `ThreadData`, `FrameDimension`) rather than `snake_case`. Do not use
a `_t` suffix for user-defined types (reserving `_t` for standard POSIX/C
library types like `size_t`, `uint32_t`, or `pthread_mutex_t`). Additionally,
avoid leading underscores for struct, union, or enum tag names (e.g.,
`_job_type`)—either omit the tag name or match the PascalCase typedef name.

**Why.** In POSIX (IEEE Std 1003.1) and standard C, identifier names ending in
`_t` as well as leading-underscore identifiers are reserved for system and
compiler namespaces, creating collision risks across platform headers. In
libvpx, user-defined types consistently use PascalCase to clearly distinguish
type identifiers from variables and functions while avoiding reserved
namespaces.

## Examples

Bad:

```cpp
// Bad: snake_case with POSIX-reserved _t suffix
typedef struct {
  pthread_mutex_t mutex;
  int is_done;
} jobq_t;

// Bad: leading-underscore tag name and snake_case typedef
typedef enum _job_type {
  PARSE_JOB,
  RECON_JOB,
} job_type;

// Bad: snake_case typedef with _t suffix
typedef struct {
  int width;
  int height;
} frame_dimension_t;
```

Good:

```cpp
// Good: PascalCase typedef without _t suffix
typedef struct {
  pthread_mutex_t mutex;
  int is_done;
} JobQueueRowMt;

// Good: PascalCase tag and typedef without leading underscores
typedef enum JobType {
  PARSE_JOB,
  RECON_JOB,
} JobType;

// Good: PascalCase typedef
typedef struct {
  int width;
  int height;
} FrameDimension;
```
