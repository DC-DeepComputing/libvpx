---
name: libvpx/close-opened-file-streams-with-fclose
description: When opening files with fopen() or related APIs, ensure all opened FILE pointers are closed with fclose() across all exit paths and teardown routines
rubric: safety
severity: should_fix
---

**Rule.** Ensure every FILE * stream opened with fopen() (or vpx_fopen(),
fdopen(), freopen()) is explicitly closed with fclose() across all control-flow
paths, including early returns, error exits, and normal completion.
Additionally, for long-lived streams stored in state structs or configs across
processing passes, ensure stream pointers are checked for null, closed during
teardown and cleanup routines, and reset to NULL.

**Why.** Failing to invoke fclose() on opened file streams causes file
descriptor and stream buffer memory leaks, risking descriptor exhaustion in
multi-stream or batch encoding runs. Failing to close output files also risks
leaving buffered write data unflushed.

## Examples

Bad:

```cpp
// Variant 1: Local file stream leaks on return
FILE *fp = fopen(config_path, "r");
if (!fp) return -1;
while (fscanf(fp, "%d,%d", &x, &y) == 2) {
  process_point(x, y);
}
return 0;  // Leaks open file descriptor and buffer

// Variant 2: Struct stream pointer omitted during teardown
if (pass && stream->config.tpl_stats_fn) {
  stream->config.tpl_stats_file = fopen(stream->config.tpl_stats_fn, "w");
}
// stream->config.tpl_stats_file never closed during teardown
```

Good:

```cpp
// Variant 1: Explicit fclose before return
FILE *fp = fopen(config_path, "r");
if (!fp) return -1;
while (fscanf(fp, "%d,%d", &x, &y) == 2) {
  process_point(x, y);
}
fclose(fp);
return 0;

// Variant 2: Checked and closed during stream cleanup
if (stream->config.tpl_stats_file) {
  fclose(stream->config.tpl_stats_file);
  stream->config.tpl_stats_file = NULL;
}
```
