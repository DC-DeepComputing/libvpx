---
name: libvpx/check-file-io-return-values
description: Check file I/O and formatted parsing return values (fprintf, fscanf, sscanf, fread, fwrite) to detect write errors and verify exact parsed field counts rather than EOF
rubric: correctness
severity: should_fix
---

**Rule.** Check the return values of file I/O and formatted parsing functions
(such as `fprintf`, `fscanf`, `sscanf`, `scanf`, `fread`, and `fwrite`) to
handle write errors, end-of-file conditions, and malformed input. Verify that
write functions return non-negative counts or the expected item count, and
verify that parsing functions match the exact expected number of conversion
specifiers (e.g., `fscanf(...) != N`) rather than checking only against `EOF`.

**Why.** File I/O operations can fail due to disk full errors, broken streams,
EOF, or corrupted inputs. Ignoring write return values causes silent data
truncation. Furthermore, `fscanf()`, `sscanf()`, and `scanf()` return the count
of successfully matched and assigned items, which can be less than expected (or
0) without returning `EOF` when a format mismatch occurs; checking only against
`EOF` allows partial or failed reads to pass undetected, leaving variables
uninitialized.

## Examples

Bad:

```cpp
// Writing without verifying success:
fprintf(tpl_file, "%d %d %d\n", stats.width, stats.height, num_blocks);

// Parsing checking only EOF, failing to catch partial reads or format mismatches:
if (fscanf(tpl_file, "%d %d %d\n", &width, &height, &num_blocks) == EOF) {
  return VPX_CODEC_ERROR;
}
```

Good:

```cpp
// Verify write operation succeeded:
if (fprintf(tpl_file, "%d %d %d\n", stats.width, stats.height, num_blocks) < 0) {
  return VPX_CODEC_ERROR;
}

// Verify exact expected conversion count was matched and assigned:
if (fscanf(tpl_file, "%d %d %d\n", &width, &height, &num_blocks) != 3) {
  return VPX_CODEC_CORRUPT_FRAME;
}
```
