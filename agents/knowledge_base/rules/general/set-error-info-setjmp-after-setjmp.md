---
name: libvpx/set-error-info-setjmp-after-setjmp
description: When using vpx_internal_error_info with setjmp, set error.setjmp = 1 only after setjmp() and always reset it to 0 on all function exit paths
rubric: correctness
severity: should_fix
---

**Rule.** When establishing error recovery with struct vpx_internal_error_info
(error.setjmp / error_info.setjmp), set setjmp = 1 only after successfully
executing setjmp(error.jmp), keep it active throughout all fallible
initialization and callee operations, and always reset setjmp = 0 on every
function exit path (including normal returns, early returns, and error exits).
Do not enable setjmp = 1 prior to calling setjmp(), do not clear it prematurely
midway through setup, and never leave an active setjmp flag set across function
boundaries.

**Why.** vpx_internal_error() checks info->setjmp before calling longjmp().
Setting error.setjmp = 1 before calling setjmp() risks jumping to an
uninitialized jmp_buf. Conversely, clearing error.setjmp = 0 prematurely leaves
intermediate operations unprotected by error recovery. Finally, failing to reset
error.setjmp = 0 on all function exits leaves error.jmp pointing to an expired
stack frame, causing stack corruption and undefined behavior if a subsequent
callee invokes vpx_internal_error().

## Examples

Bad:

```cpp
// Variant 1: Enabling setjmp = 1 before calling setjmp()
xd->error_info.setjmp = 1;
if (setjmp(xd->error_info.jmp)) {
  xd->error_info.setjmp = 0;
  return -1;
}
mt_decode_mb_rows(pbi, xd, 0);

// Variant 2: Leaving setjmp == 1 on early return or function exit
if (setjmp(cpi->common.error.jmp)) {
  cpi->common.error.setjmp = 0;
  return NULL;
}
cpi->common.error.setjmp = 1;
if (!init_rate_control(cfg)) return NULL;  // Exits with error.setjmp == 1!
return rc_api;                             // Exits with error.setjmp == 1!
```

Good:

```cpp
// Variant 1: Enable setjmp = 1 only after setjmp() is established
if (setjmp(xd->error_info.jmp)) {
  xd->error_info.setjmp = 0;
  return -1;
}
xd->error_info.setjmp = 1;
mt_decode_mb_rows(pbi, xd, 0);
xd->error_info.setjmp = 0;

// Variant 2: Always reset setjmp = 0 on all return paths
if (setjmp(cpi->common.error.jmp)) {
  cpi->common.error.setjmp = 0;
  return NULL;
}
cpi->common.error.setjmp = 1;
const int success = init_rate_control(cfg);
cpi->common.error.setjmp = 0;
if (!success) return NULL;
return rc_api;
```
