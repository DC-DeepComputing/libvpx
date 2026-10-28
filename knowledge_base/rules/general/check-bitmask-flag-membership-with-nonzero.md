---
name: libvpx/check-bitmask-flag-membership-with-nonzero
description: When testing bitmask flags with bitwise AND (`val & FLAG`), compare against `!= 0` or `== 0` rather than relational operators like `> 0` or redundant equality against `FLAG`
rubric: readability
severity: should_fix
---

**Rule.** When checking whether a bit flag is set or unset in a bitmask
expression (e.g., `val & FLAG`), compare against `!= 0` (or evaluate boolean
truthiness directly) and `== 0` for unset flags. Do not use relational magnitude
comparisons such as `> 0` or `>= 0`. Additionally, do not redundantly duplicate
the flag constant on both sides of the comparison (e.g., avoid `(flags & FLAG)
== FLAG`).

**Why.** Bitwise expressions yield discrete flag bit patterns rather than
numeric magnitudes. Testing flag membership with relational operators like `> 0`
obscures intent and can produce incorrect results if signed integers or sign
bits are involved. Furthermore, comparing against `== FLAG` redundantly repeats
the flag constant without adding safety. Comparing against `!= 0` (or `== 0` for
unset flags) is the standard, concise, and portable C/C++ idiom.

## Examples

Bad:

```cpp
// Bad: Using relational magnitude comparison to test a bitmask flag
if (cpi->ext_ratectrl.ready &&
    (cpi->ext_ratectrl.funcs.rc_type & VPX_RC_QP) > 0) {
  apply_external_qp(cpi);
}

// Bad: Redundantly repeating the flag constant
bool expect_psnr = (frame_flags_ & VPX_EFLAG_CALCULATE_PSNR) ==
                   VPX_EFLAG_CALCULATE_PSNR;
```

Good:

```cpp
// Good: Using != 0 to test bitmask flag membership
if (cpi->ext_ratectrl.ready &&
    (cpi->ext_ratectrl.funcs.rc_type & VPX_RC_QP) != 0) {
  apply_external_qp(cpi);
}

// Good: Comparing against != 0 without duplicating the flag constant
bool expect_psnr = (frame_flags_ & VPX_EFLAG_CALCULATE_PSNR) != 0;
```
