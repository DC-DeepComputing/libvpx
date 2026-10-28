---
name: libvpx/prefix-enum-constants-with-subsystem-or-type
description: When defining C enum types and constants, prefix enumerators with a specific subsystem, component, or type qualifier and align the enum type name with its component.
rubric: readability
severity: nit
---

**Rule.** When defining C enum types and constants, prefix enumerator names with
a specific subsystem, component, or type qualifier (e.g., VPX_WORKER_STATUS_*
for VPxWorkerStatus, READER_* for VpxVideoReaderError, or VPX_CODEC_* for
vpx_codec_err_t). Avoid generic prefixes (such as ERR_* or VPX_STATUS_*) or
un-prefixed names that pollute the global namespace. Additionally, align the
enum type name with the specific component or subsystem it serves.

**Why.** In C, enum constants lack scoping and occupy the flat global namespace.
Generic prefixes like ERR_* or STATUS_* and insufficiently qualified names risk
identifier collisions across headers, internal modules, and third-party
libraries, while obscuring component ownership and type provenance.

## Examples

Bad:

```cpp
// Bad: Generic ERR_ prefix lacks component provenance and risks collisions
typedef enum VpxVideoError {
  ERR_OPENING_FILE,
  ERR_FILE_HEADER,
  ERR_ALLOCATION,
} VpxVideoError_t;

// Bad: Generic VPX_STATUS_ prefix does not identify the specific worker subsystem
typedef enum {
  VPX_STATUS_NOT_OK = 0,
  VPX_STATUS_OK,
  VPX_STATUS_WORK
} VPxWorkerStatus;
```

Good:

```cpp
// Good: Prefix enumerators with component name and align type identifier
typedef enum VpxVideoReaderError {
  READER_OK,
  READER_OPEN_FAILED,
  READER_CANNOT_READ_HEADER,
  READER_OUT_OF_MEMORY
} VpxVideoReaderError;

// Good: Specific subsystem qualifier identifies the worker component
typedef enum {
  VPX_WORKER_STATUS_NOT_OK = 0,
  VPX_WORKER_STATUS_OK,
  VPX_WORKER_STATUS_WORKING
} VPxWorkerStatus;
```
