---
name: libvpx/check-fopen-return-value-for-null
description: When opening files with fopen(), verify the returned FILE pointer is non-NULL before performing I/O operations and handle or propagate failure
rubric: correctness
severity: should_fix
---

**Rule.** Always check the return value of fopen() (or related file-opening
functions) for NULL before using the resulting FILE pointer. Handle or propagate
the error (such as returning an error status or reporting failure) rather than
continuing with subsequent I/O operations on an unchecked handle.

**Why.** fopen() returns NULL when opening a file fails due to invalid paths,
non-existent directories, or permission issues. Passing a NULL FILE pointer to
subsequent stdio functions causes segmentation faults and undefined behavior.

## Examples

Bad:

```cpp
if (funcs.rate_ctrl_log_path != NULL) {
  ext_ratectrl->log_file = fopen(funcs.rate_ctrl_log_path, "w");
}
```

Good:

```cpp
if (funcs.rate_ctrl_log_path != NULL) {
  ext_ratectrl->log_file = fopen(funcs.rate_ctrl_log_path, "w");
  if (!ext_ratectrl->log_file) {
    return VPX_CODEC_ERROR;
  }
}
```
