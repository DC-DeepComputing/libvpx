---
name: libvpx/avoid-global-error-state-in-utility-helpers
description: In utility helpers and example tools, avoid global error variables, dedicated error enums, and duplicate validation functions; validate inline, log to stderr, and return NULL.
rubric: design
severity: should_fix
---

**Rule.** Do not declare mutable global error variables (e.g., Reader_Error,
g_reader_error), dedicated error enums (e.g., VpxVideoReaderError), or redundant
probe/validation helpers (e.g., vpx_video_reader_check_file) to communicate
failure reasons in utility helpers and example tools (such as VpxVideoReader or
VpxVideoWriter). Instead, perform resource opening, header validation, and error
checking directly within primary functions (e.g., vpx_video_reader_open), print
descriptive diagnostic messages directly to stderr, and return standard failure
indicators (such as NULL or error status codes).

**Why.** Declaring mutable global error variables in headers introduces
thread-safety hazards, mutable global state, and duplicate symbol definitions
across translation units. Exposing dedicated error enums for CLI utility helpers
and example tools is unnecessary over-engineering. Furthermore, creating
separate probe or validation functions that duplicate file opening and header
parsing logic creates redundant code paths that can diverge when format
specifications change. Performing validation directly within open/helper
functions and logging actionable diagnostics (such as filenames) to stderr keeps
helper utilities self-contained and simple.

## Examples

Bad:

```cpp
// In video_reader.h: global error state and dedicated error enum
typedef enum VpxVideoReaderError {
  READER_OK,
  READER_OPEN_FAILED,
  READER_CANNOT_READ_HEADER,
} VpxVideoReaderError;

VpxVideoReaderError Reader_Error;

// In video_reader.c: redundant probe helper duplicating open logic
VpxVideoReaderError vpx_video_reader_check_file(const char *filename) {
  char header[32];
  FILE *const file = fopen(filename, "rb");
  if (!file) return READER_OPEN_FAILED;
  if (fread(header, 1, 32, file) != 32) return READER_CANNOT_READ_HEADER;
  fclose(file);
  return READER_OK;
}
```

Good:

```cpp
// In video_reader.c: validation and stderr diagnostics integrated into open function
VpxVideoReader *vpx_video_reader_open(const char *filename) {
  char header[32];
  FILE *const file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "%s can't be opened.\n", filename);
    return NULL;
  }
  if (fread(header, 1, 32, file) != 32) {
    fprintf(stderr, "File header on %s can't be read.\n", filename);
    fclose(file);
    return NULL;
  }
  ...
}
```
