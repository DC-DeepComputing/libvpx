---
name: libvpx/test/disable-unfixed-bug-reproducer-tests
description: When adding GoogleTest test cases that reproduce known unfixed bugs ahead of their fixes, prefix test names with DISABLED_ to prevent CI failures
rubric: readability
severity: should_fix
---

**Rule.** When adding a GoogleTest test case that reproduces a known unfixed
bug, assertion failure, or crash ahead of its accompanying fix, prefix the test
case or test suite name with DISABLED_. Remove the DISABLED_ prefix once the fix
is submitted to enable continuous regression testing.

**Why.** Active tests that reproduce unfixed bugs fail automated CI test runs
and presubmit bots. Prefixing the test name with DISABLED_ allows the reproducer
test to be checked in safely for tracking and local verification without
breaking continuous testing until the fix is ready.

## Examples

Bad:

```cpp
// Reproducer for buganizer issue 442105459 landed before the fix.
TEST(EncodeAPI, Buganizer442105459) {
  vpx_codec_ctx_t enc;
  vpx_codec_enc_cfg_t cfg;
  InitEncoder(&enc, &cfg);
  // Fails/crashes until the fix is landed, breaking CI test runs.
  EXPECT_EQ(VPX_CODEC_OK, vpx_codec_encode(&enc, nullptr, 0, 0, 0, 0));
}
```

Good:

```cpp
// Reproducer for buganizer issue 442105459 landed before the fix.
TEST(EncodeAPI, DISABLED_Buganizer442105459) {
  vpx_codec_ctx_t enc;
  vpx_codec_enc_cfg_t cfg;
  InitEncoder(&enc, &cfg);
  // Safely disabled in continuous test runs until the fix enables it.
  EXPECT_EQ(VPX_CODEC_OK, vpx_codec_encode(&enc, nullptr, 0, 0, 0, 0));
}
```
