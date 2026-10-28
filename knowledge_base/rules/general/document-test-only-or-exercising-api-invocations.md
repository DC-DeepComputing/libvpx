---
name: libvpx/document-test-only-or-exercising-api-invocations
description: When invoking APIs or codec controls solely to exercise functionality where returned data is immediately freed or discarded, add an explanatory comment
rubric: readability
severity: should_fix
---

**Rule.** When invoking codec controls, query methods, or APIs solely to test or
exercise functionality where the resulting data is immediately freed, ignored,
or discarded rather than consumed or written out, add an explanatory comment
stating the testing purpose. Document why the data is being requested and
discarded so the call is not mistaken for redundant or dead logic.

**Why.** Querying API controls and immediately freeing or ignoring the returned
data resembles dead code, an incomplete refactoring, or a missing output write.
An explanatory comment clarifies that the call deliberately exercises or
validates the API control, preventing maintainers from inadvertently removing
the test logic.

## Examples

Bad:

```cpp
if (got_data && global.export_tpl_stats) {
  TplFrameStats *tpl_stats = NULL;
  FOREACH_STREAM(vpx_codec_control(&stream->encoder, VP9E_GET_TPL_STATS,
                                   &tpl_stats));
  free(tpl_stats);
}
```

Good:

```cpp
if (got_data && global.export_tpl_stats) {
  TplFrameStats *tpl_stats = NULL;
  /* Exercise the control for testing purposes. */
  FOREACH_STREAM(vpx_codec_control(&stream->encoder, VP9E_GET_TPL_STATS,
                                   &tpl_stats));
  free(tpl_stats);
}
```
