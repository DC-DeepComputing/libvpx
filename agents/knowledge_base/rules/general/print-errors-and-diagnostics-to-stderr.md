---
name: libvpx/print-errors-and-diagnostics-to-stderr
description: When printing error, warning, or diagnostic failure messages, direct output to stderr (using fprintf or vfprintf) rather than stdout or printf
rubric: readability
severity: should_fix
---

**Rule.** Direct error, warning, diagnostic, and assertion-failure messages to
stderr (e.g., using fprintf(stderr, ...), vfprintf(stderr, ...), or fputs(...,
stderr)) rather than stdout (printf). Reserve stdout for normal program output
and piped data streams.

**Why.** Directing diagnostics and error information to stderr prevents
polluting standard output, which is often redirected, piped to downstream tools,
or parsed as raw data streams.

## Examples

Bad:

```cpp
if (prob != ref_prob) {
  printf("prob error prob %d ref_prob %d queue_r %d\n", prob, ref_prob,
         queue_r);
  assert(0);
}
```

Good:

```cpp
if (prob != ref_prob) {
  fprintf(stderr, "prob error prob %d ref_prob %d queue_r %d\n", prob,
          ref_prob, queue_r);
  assert(0);
}
```
