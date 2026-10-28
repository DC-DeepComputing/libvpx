---
name: libvpx/no-dummy-cli-flags-for-api-testing
description: When introducing new codec controls or library APIs, verify functionality in dedicated unit tests rather than adding dummy flags to CLI tools
rubric: design
severity: should_fix
---

**Rule.** Do not add placeholder or non-functional command-line flags and dummy
test routines to user-facing CLI tools (such as vpxenc or vpxdec) solely to
exercise new library features. Verify new codec controls, configuration
parameters, and library APIs through dedicated unit tests in test/ (e.g.,
test/encode_api_test.cc or test/decode_api_test.cc) instead.

**Why.** Adding temporary or non-functional flags clutters user-facing CLI tools
with dead options and throwaway test logic that confuses users. Dedicated unit
tests directly validate API behavior and edge cases without polluting production
tool interfaces.

## Examples

Bad:

```cpp
// In vpxenc.c:
static const arg_def_t export_tpl_stats =
    ARG_DEF(NULL, "export-tpl-stats", 0, "Export TPL stats of vp9 encoder");

static void get_tpl_stats(struct stream_state *stream) {
  TplFrameStats *tpl_stats = NULL;
  vpx_allocate_tpl_frame_stats_list(&tpl_stats);
  vpx_codec_control(&stream->encoder, VP9E_GET_TPL_STATS, tpl_stats);
  // Free the memory right away as this is only a test.
  vpx_free_tpl_frame_stats_list(tpl_stats);
}
```

Good:

```cpp
// In test/encode_api_test.cc:
TEST_P(EncodeApiGetTplStatsTest, GetTplStats) {
  vpx_codec_ctx_t enc;
  vpx_codec_enc_cfg_t cfg;
  ASSERT_EQ(vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, 0), VPX_CODEC_OK);
  ASSERT_EQ(vpx_codec_enc_init(&enc, vpx_codec_vp9_cx(), &cfg, 0), VPX_CODEC_OK);
  TplFrameStats *tpl_stats = NULL;
  ASSERT_EQ(vpx_codec_control(&enc, VP9E_GET_TPL_STATS, tpl_stats), VPX_CODEC_OK);
  vpx_codec_destroy(&enc);
}
```
