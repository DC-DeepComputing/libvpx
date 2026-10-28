---
name: libvpx/remove-temporary-debug-code-before-submitting
description: When modifying library and codec code, remove temporary debug prints (printf, fprintf), ad-hoc file logging (fopen), and diagnostics before submitting, or guard them with CONFIG_* macros.
rubric: readability
severity: should_fix
---

**Rule.** Do not leave stray or temporary debug print statements (such as
`printf`, `fprintf`, `puts`, `putchar`, or `std::cout`) or ad-hoc debugging
instrumentation (such as `fopen`, `fclose`, or local diagnostic file dumps) in
codec and library implementation code. Remove all temporary development
scaffolding before submitting patches, or guard intentional persistent
diagnostic logging behind appropriate build configuration macros (such as `#if
CONFIG_...`).

**Why.** Unconditional debug prints and ad-hoc file I/O pollute `stdout` and
`stderr` for client applications embedding libvpx, degrade encoding and decoding
throughput, introduce security and side-effect risks from local file operations,
and break automated output parsers and tests. Diagnostic and profiling
instrumentation must be controlled at build time via `CONFIG_*` macros.

## Examples

Bad:

```cpp
// Bad: Leftover ad-hoc file logging and stray print statements.
FILE *pfile = fopen("vp9_mv.txt", "a");
if (pfile != NULL) {
  fprintf(pfile, "(%d, %d)\n", mv->as_mv.row, mv->as_mv.col);
  fclose(pfile);
}

printf("frame=%d type=%d q=%d\n", cm->current_video_frame, cm->frame_type,
       cm->base_qindex);
fprintf(stderr, "frame size = (%d, %d)\n", frame->y_height, frame->y_width);
```

Good:

```cpp
// Good: Remove ad-hoc logging and prints, or guard with CONFIG_* macros.
process_motion_vector(&mv->as_mv);
update_frame_rate_estimate(cpi);

#if CONFIG_INTERNAL_STATS
fprintf(stderr, "frame size = (%d, %d)\n", frame->y_height, frame->y_width);
#endif
```
