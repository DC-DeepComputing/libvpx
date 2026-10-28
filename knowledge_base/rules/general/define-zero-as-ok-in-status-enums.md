---
name: libvpx/define-zero-as-ok-in-status-enums
description: When defining status or error code enums, define the success or no-error enumerator (such as VPX_CODEC_OK or READER_OK) as 0
rubric: readability
severity: should_fix
---

**Rule.** When defining status or error code enums, define the success or
no-error enumerator (such as VPX_CODEC_OK or READER_OK) as 0 (the first
enumerator). Do not assign 0 to an error code or place the success enumerator at
a non-zero value.

**Why.** In C and libvpx conventions, 0 represents success, enabling standard
conditional idioms such as if (err) for failure and if (!err) for success.
Defining an error code at 0 or a success code at a non-zero value contradicts
codebase conventions (such as vpx_codec_err_t) and leads to call-site confusion
or inverted logic bugs.

## Examples

Bad:

```cpp
typedef enum VpxVideoError {
  ERR_OPENING_FILE,
  ERR_FILE_HEADER,
  ERR_IVF_SIGNATURE,
  ERR_IVF_VERSION,
  ERR_ALLOCATION,
  ERR_NONE
} VpxVideoError;
```

Good:

```cpp
typedef enum VpxVideoReaderError {
  READER_OK = 0,
  READER_OPEN_FAILED,
  READER_CANNOT_READ_HEADER,
  READER_INVALID_IVF_SIGNATURE,
  READER_INVALID_IVF_VERSION,
  READER_OUT_OF_MEMORY
} VpxVideoReaderError;
```
