---
name: libvpx/document-reserved-header-bytes-and-buffer-skips
description: When reserving fixed-size header bytes or advancing past unconsumed buffer space for future extensibility, add a comment explaining why bytes are skipped
rubric: readability
severity: should_fix
---

**Rule.** When reserving fixed-size header bytes, padding, or skipping
unconsumed buffer space for future extensibility or corpus/format compatibility,
add an explanatory comment. Explicitly state that the unread bytes are reserved
to preserve compatibility when adding new configuration options or fields.

**Why.** When code consumes only a fraction of a fixed-size header or buffer and
advances past the rest, readers and maintainers cannot tell if unread bytes were
accidentally omitted or intentionally reserved. A comment clarifies the design
intent and prevents accidental removal or format-breaking changes.

## Examples

Bad:

```cpp
// Flawed: advances past 32 header bytes after reading 1 byte without explanation
nalloc_start(data, size);
data += FUZZ_HDR_SZ;
size -= FUZZ_HDR_SZ;
```

Good:

```cpp
// Correct: documents that unused header space is reserved for corpus compatibility
nalloc_start(data, size);
// We reserve FUZZ_HDR_SZ=32 bytes so future config options can be added
// while maintaining format compatibility with existing generated corpora.
data += FUZZ_HDR_SZ;
size -= FUZZ_HDR_SZ;
```
