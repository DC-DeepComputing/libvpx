---
name: libvpx/test/call-reset-model-before-custom-test-parameters
description: When configuring test fixtures or test loops with reset helpers like ResetModel(), invoke the reset method before assigning custom parameter overrides
rubric: correctness
severity: should_fix
---

**Rule.** When reinitializing test state across test cases or loop iterations
using fixture reset helpers (such as ResetModel()), invoke the reset helper
before applying custom test parameter overrides or member assignments. Do not
assign custom configuration values or test parameters before calling reset
methods that reinitialize those fields to defaults.

**Why.** Fixture reset methods such as ResetModel() reinitialize encoder
settings, rate control models, and test member variables to default values.
Assigning custom parameter overrides prior to calling reset silently overwrites
the custom values with defaults, causing the test loop to execute with incorrect
parameters.

## Examples

Bad:

```cpp
for (int j = 1; j < 5; ++j) {
  denoiser_on_ = j;
  cfg_.rc_target_bitrate = 300;
  ResetModel();
  ASSERT_NO_FATAL_FAILURE(RunLoop(&video));
}
```

Good:

```cpp
for (int j = 1; j < 5; ++j) {
  cfg_.rc_target_bitrate = 300;
  ResetModel();
  denoiser_on_ = j;
  ASSERT_NO_FATAL_FAILURE(RunLoop(&video));
}
```
