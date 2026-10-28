---
name: libvpx/match-variable-type-to-target-api
description: When declaring variables passed to or returned by specific APIs (such as FileOffset for fseeko/ftello), use the API type directly rather than casting at call sites
rubric: readability
severity: should_fix
---

**Rule.** Declare variables using the specific typedef or type expected by the
APIs they interact with (such as FileOffset for file positions passed to fseeko
and returned by ftello) rather than using generic integer types and casting at
call sites.

**Why.** Declaring variables with the exact API type avoids redundant,
error-prone casting at call sites, prevents platform-specific signedness or
conversion warnings across 32-bit and 64-bit builds, and clearly communicates
the variable's intended use.

## Examples

Bad:

```cpp
static int64_t ivf_header_pos = 0;
...
const int64_t currpos = ftello(stream->file);
fseeko(stream->file, (fileoffset_t)ivf_header_pos, SEEK_SET);
ivf_write_frame_size(stream->file, fsize);
fseeko(stream->file, (fileoffset_t)currpos, SEEK_SET);
```

Good:

```cpp
static FileOffset ivf_header_pos = 0;
...
const FileOffset currpos = ftello(stream->file);
fseeko(stream->file, ivf_header_pos, SEEK_SET);
ivf_write_frame_size(stream->file, fsize);
fseeko(stream->file, currpos, SEEK_SET);
```
