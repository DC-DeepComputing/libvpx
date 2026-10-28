---
name: libvpx/test/register-test-data-in-mk-and-sha1
description: When adding or referencing new test video clips or test vectors in tests, register them in test/test-data.mk and test/test-data.sha1
rubric: correctness
severity: should_fix
---

**Rule.** When adding, referencing, or updating test video clips or test vector
files (`.y4m`, `.yuv`, `.ivf`, `.webm`) in unit tests or test scripts, register
the filename in `test/test-data.mk` under the appropriate `LIBVPX_TEST_DATA-*`
configuration variable and add its SHA-1 hash to `test/test-data.sha1`.

**Why.** The libvpx test infrastructure tracks test media assets via
`test/test-data.mk` and verifies their integrity with `test/test-data.sha1` when
downloading test data for automated test runs. Omitting either entry will cause
test data downloads or checksum verification to fail.

## Examples

Bad:

```cpp
// In test/svc_datarate_test.cc:
::libvpx_test::Y4mVideoSource video("niklas_1284_770_30.y4m", 0, 60);
// test/test-data.mk and test/test-data.sha1 are not updated
```

Good:

```cpp
// In test/svc_datarate_test.cc:
::libvpx_test::Y4mVideoSource video("niklas_1284_770_30.y4m", 0, 60);

# In test/test-data.mk:
LIBVPX_TEST_DATA-$(CONFIG_VP9_ENCODER) += niklas_1284_770_30.y4m

# In test/test-data.sha1:
01838b78920e7d6c26ac7926a6ae2f2b26d335a9 *niklas_1284_770_30.y4m
```
