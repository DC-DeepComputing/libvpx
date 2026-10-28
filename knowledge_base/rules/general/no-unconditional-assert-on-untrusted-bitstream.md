---
name: libvpx/no-unconditional-assert-on-untrusted-bitstream
description: When validating untrusted bitstream inputs or transform coefficients, use standard error handling or guard debugging assertions with CONFIG_COEFFICIENT_RANGE_CHECKING
rubric: correctness
severity: should_fix
---

**Rule.** Do not use unconditional assert() or assert(0) to validate or reject
untrusted bitstream data, corrupted frame input, or out-of-range transform
coefficients. Handle malformed bitstream input gracefully through regular
control flow and error returns, or guard debugging range assertions behind
dedicated build configuration flags (such as #if
CONFIG_COEFFICIENT_RANGE_CHECKING).

**Why.** In debug builds and fuzzer targets where assertions are enabled, an
assertion failure aborts process execution as a crash. Untrusted or corrupted
bitstream data legitimately reaches decoding and inverse transform routines;
such inputs are external failures rather than violated programmer invariants and
must be handled without crashing.

## Examples

Bad:

```cpp
if (detect_invalid_iht_input(input, 16)) {
  assert(0 && "invalid highbd iht input");
  return;
}
```

Good:

```cpp
if (detect_invalid_iht_input(input, 16)) {
#if CONFIG_COEFFICIENT_RANGE_CHECKING
  assert(0 && "invalid highbd iht input");
#endif
  return;
}
```
