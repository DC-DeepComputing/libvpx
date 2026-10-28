---
name: libvpx/check-vpx-codec-control-return-value
description: When calling vpx_codec_control to configure or query codec properties, check the returned status or invoke error handlers instead of ignoring return values
rubric: correctness
severity: should_fix
---

**Rule.** Always check the return value or context error status when invoking
vpx_codec_control(). Verify the returned vpx_codec_err_t against VPX_CODEC_OK or
call error-handling utilities (such as ctx_exit_on_error() or die_codec()),
especially when passing out-pointers that remain uninitialized on failure.

**Why.** vpx_codec_control() returns a vpx_codec_err_t status code and records
errors in the codec context. Ignoring return values silently masks unsupported
controls or internal failures and can leave out-pointer arguments dangling or
uninitialized, causing memory safety issues and subtle logic bugs.

## Examples

Bad:

```cpp
TplFrameStats *tpl_stats = NULL;
FOREACH_STREAM(vpx_codec_control(&stream->encoder, VP9E_GET_TPL_STATS,
                                 &tpl_stats));
free(tpl_stats);
```

Good:

```cpp
TplFrameStats *tpl_stats = NULL;
FOREACH_STREAM({
  vpx_codec_control(&stream->encoder, VP9E_GET_TPL_STATS, &tpl_stats);
  ctx_exit_on_error(&stream->encoder, "Failed to get TPL stats");
});
free(tpl_stats);
```
