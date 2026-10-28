---
name: libvpx/validate-payload-length-at-consumption-point
description: When parsing bitstream buffers with variable-length headers, validate payload bounds at the consumption point after advancing headers rather than precalculating composite offsets
rubric: design
severity: should_fix
---

**Rule.** When parsing bitstream or packet buffers containing variable-length or
conditional headers followed by payloads or partitions, validate payload length
against remaining buffer capacity (e.g., data_end - data < payload_len) at the
start of payload decoding after consuming header bytes and advancing the buffer
pointer. Do not precalculate prospective composite length expressions (such as
payload_len + (is_key_frame ? 7 : 0)) prior to header parsing.

**Why.** Precalculating composite length expressions upfront creates
prospective, conditional offset arithmetic that obscures boundary checks and
risks desynchronization or integer overflow if header structures change.
Validating remaining buffer bounds sequentially as each section is consumed
keeps bounds checks local to the data being parsed and simplifies validation
logic.

## Examples

Bad:

```cpp
// Anti-pattern: prospective composite length check before header parsing
const int min_data_length =
    first_partition_length_in_bytes + (pc->frame_type == KEY_FRAME ? 7 : 0);
if (!pbi->ec_active && (data_end - data < min_data_length ||
                        first_partition_length_in_bytes == 0)) {
  vpx_internal_error(&pc->error, VPX_CODEC_CORRUPT_FRAME,
                     "Truncated packet or corrupt partition 0 length");
}
```

Good:

```cpp
// Preferred: validate payload length sequentially after header parsing advances data
if (pc->frame_type == KEY_FRAME) {
  if (data_end - data < 7) {
    vpx_internal_error(&pc->error, VPX_CODEC_CORRUPT_FRAME, "Truncated header");
  }
  data += 7;
}
if (!pbi->ec_active && data_end - data < first_partition_length_in_bytes) {
  vpx_internal_error(&pc->error, VPX_CODEC_CORRUPT_FRAME,
                     "Truncated packet or corrupt partition 0 length");
}
```
