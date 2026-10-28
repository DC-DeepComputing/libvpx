---
name: libvpx/check-remaining-buffer-with-pointer-subtraction
description: When verifying buffer capacity against an end pointer, check available bytes using pointer subtraction end - ptr >= N rather than pointer addition or offset comparisons
rubric: safety
severity: nit
---

**Rule.** When validating that at least N bytes or elements remain in a buffer
bounded by [ptr, end) (or [ptr, ptr_end)), check remaining capacity using
pointer subtraction (end - ptr >= N, or end - ptr < N for early exit) rather
than pointer addition (ptr + N <= end) or offset comparisons (ptr + (N - 1) <
end). Ensure inequality operators correctly handle boundary values to prevent
off-by-one errors.

**Why.** Adding an offset to a pointer before verifying bounds can compute an
out-of-bounds pointer beyond end + 1, invoking undefined behavior in C/C++ and
risking address overflow. Furthermore, pointer offset comparisons like ptr + 6 <
end for 7 bytes obscure the required byte count, do not match subsequent
advances (e.g., data += 7), and invite off-by-one errors.

## Examples

Bad:

```cpp
// Anti-pattern 1: pointer offset with adjusted offset obscures required 7 bytes
if (data + 6 < data_end) {
  pc->Width = (clear[3] | (clear[4] << 8)) & 0x3fff;
  pc->Height = (clear[5] | (clear[6] << 8)) & 0x3fff;
  data += 7;
}

// Anti-pattern 2: pointer addition risks forming out-of-bounds pointer and off-by-one error
if (data + 3 < data_end) {
  if (clear[0] != 0x9d || clear[1] != 0x01 || clear[2] != 0x2a) {
    vpx_internal_error(&pc->error, VPX_CODEC_UNSUP_BITSTREAM,
                       "Invalid frame sync code");
  }
}
```

Good:

```cpp
// Preferred: direct pointer subtraction explicitly checks 7 bytes
if (data_end - data >= 7) {
  pc->Width = (clear[3] | (clear[4] << 8)) & 0x3fff;
  pc->Height = (clear[5] | (clear[6] << 8)) & 0x3fff;
  data += 7;
}

// Preferred: pointer subtraction safely checks 3 bytes without invalid pointer arithmetic
if (data_end - data >= 3) {
  if (clear[0] != 0x9d || clear[1] != 0x01 || clear[2] != 0x2a) {
    vpx_internal_error(&pc->error, VPX_CODEC_UNSUP_BITSTREAM,
                       "Invalid frame sync code");
  }
}
```
