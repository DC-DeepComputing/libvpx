---
name: libvpx/test/extract-duplicate-test-bodies-into-helpers
description: When multiple unit tests share repetitive setup, configuration, execution, or teardown logic differing only by parameters, factor the common sequence into parameterized helper functions
rubric: design
severity: should_fix
---

**Rule.** When multiple unit tests share identical multi-step initialization,
codec configuration, buffer allocation, execution, or assertion logic and differ
only by test inputs, flags, formats, bit depths, or dimensions, extract the
shared logic into a parameterized helper function (or parameterized test). Pass
differing parameters from individual TEST or TEST_P cases rather than
duplicating the entire test body.

**Why.** Duplicating multi-step test setup, execution, and assertion boilerplate
across test cases bloats test files, obscures the unique conditions or edge
cases under test, increases maintenance overhead, and makes future refactoring
error-prone. Factoring repeated sequences into parameterized helpers centralizes
test mechanics and highlights the distinct test inputs.

## Examples

Bad:

```cpp
// Bad: Duplicating entire test body differing only by input values or config
TEST(EncodeAPI, Vp9UnsafeCastEosCount) {
  vpx_codec_iface_t *const iface = vpx_codec_vp9_cx();
  vpx_codec_enc_cfg_t cfg;
  ASSERT_EQ(vpx_codec_enc_config_default(iface, &cfg, 0), VPX_CODEC_OK);
  cfg.g_pass = VPX_RC_LAST_PASS;

  FIRSTPASS_STATS packets[2] = {};
  packets[1].count = 1e300;

  cfg.rc_twopass_stats_in.buf = packets;
  cfg.rc_twopass_stats_in.sz = sizeof(packets);

  vpx_codec_ctx_t codec;
  EXPECT_EQ(vpx_codec_enc_init(&codec, iface, &cfg, 0),
            VPX_CODEC_INVALID_PARAM);
}

TEST(EncodeAPI, Vp9UnsafeCastEosCountNaN) {
  vpx_codec_iface_t *const iface = vpx_codec_vp9_cx();
  vpx_codec_enc_cfg_t cfg;
  ASSERT_EQ(vpx_codec_enc_config_default(iface, &cfg, 0), VPX_CODEC_OK);
  cfg.g_pass = VPX_RC_LAST_PASS;

  FIRSTPASS_STATS packets[2] = {};
  packets[1].count = atof("NaN");

  cfg.rc_twopass_stats_in.buf = packets;
  cfg.rc_twopass_stats_in.sz = sizeof(packets);

  vpx_codec_ctx_t codec;
  EXPECT_EQ(vpx_codec_enc_init(&codec, iface, &cfg, 0),
            VPX_CODEC_INVALID_PARAM);
}
```

Good:

```cpp
// Good: Extract shared setup and assertions into a parameterized helper
void VerifyVp9UnsafeCastEosCount(double count_value) {
  vpx_codec_iface_t *const iface = vpx_codec_vp9_cx();
  vpx_codec_enc_cfg_t cfg;
  ASSERT_EQ(vpx_codec_enc_config_default(iface, &cfg, 0), VPX_CODEC_OK);
  cfg.g_pass = VPX_RC_LAST_PASS;

  FIRSTPASS_STATS packets[2] = {};
  packets[1].count = count_value;

  cfg.rc_twopass_stats_in.buf = packets;
  cfg.rc_twopass_stats_in.sz = sizeof(packets);

  vpx_codec_ctx_t codec;
  EXPECT_EQ(vpx_codec_enc_init(&codec, iface, &cfg, 0),
            VPX_CODEC_INVALID_PARAM);
}

TEST(EncodeAPI, Vp9UnsafeCastEosCount) { VerifyVp9UnsafeCastEosCount(1e300); }
TEST(EncodeAPI, Vp9UnsafeCastEosCountNaN) { VerifyVp9UnsafeCastEosCount(NAN); }
TEST(EncodeAPI, Vp9UnsafeCastEosCountZero) { VerifyVp9UnsafeCastEosCount(0.0); }
```
