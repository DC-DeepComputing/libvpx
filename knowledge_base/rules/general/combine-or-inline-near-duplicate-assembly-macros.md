---
name: libvpx/combine-or-inline-near-duplicate-assembly-macros
description: When writing inline assembly, write individual instructions directly and avoid near-duplicate or single-instruction helper macros; parameterize register operands for shared sequences
rubric: design
severity: should_fix
---

**Rule.** Avoid defining trivial wrapper macros around single inline assembly
instructions or separate near-duplicate macro variants with hardcoded registers.
Write individual assembly instructions directly inside the inline assembly block
(`__asm__`), and reserve helper macros for genuinely repeated multi-instruction
sequences by parameterizing their register operands and immediate shifts (or
inlining them directly if differing register constraints make parameterization
impractical).

**Why.** Wrapping single assembly instructions or creating near-duplicate macro
variants with hardcoded register names introduces unnecessary preprocessor
indirection, obscures register allocation and data flow, and creates redundant
boilerplate. Inlining individual instructions and parameterizing
multi-instruction macros keeps register usage explicit at call sites and
simplifies maintenance.

## Examples

Bad:

```cpp
// Anti-pattern 1: Single-instruction wrapper macro
#define VP8_LOOP_VPSRAB_PUNPCKLBH(reg1, reg2) \
  "punpcklbh " #reg1 ", " #reg1 ", " #reg2 "                      \n\t"

// Anti-pattern 2: Near-duplicate macros with hardcoded registers
#define VP8_LOOP_VPSRAB(reg) \
  "xor       %[ftmp0],    %[ftmp0],           %[ftmp0]            \n\t" \
  "psrah     %[ftmp0],    %[ftmp0],           %[ftmp7]            \n\t" \
  "packsshb  " #reg ",    %[ftmp0],           %[ftmp8]            \n\t"

#define VP8_MBLOOP_VPSRAB(reg) \
  "xor       %[ftmp7],    %[ftmp7],           %[ftmp7]            \n\t" \
  "psrah     %[ftmp7],    %[ftmp7],           %[ftmp12]           \n\t" \
  "packsshb  " #reg ",    %[ftmp7],           %[ftmp8]            \n\t"
```

Good:

```cpp
// Option 1: Write individual instructions directly inside the asm block
__asm__ __volatile__(
  "punpcklbh %[ftmp0], %[ftmp0], %[ftmp12]                        \n\t"
  : [ftmp0] "+f"(ftmp0)
  : [ftmp12] "f"(ftmp12)
);

// Option 2: Parameterize register operands for repeated multi-instruction sequences
#define VP8_VPSRAB(reg, tmp0, tmp1, shift) \
  "xor       " #tmp0 ",   " #tmp0 ",          " #tmp0 "           \n\t" \
  "psrah     " #tmp0 ",   " #tmp0 ",          " #shift "          \n\t" \
  "packsshb  " #reg ",    " #tmp0 ",          " #tmp1 "           \n\t"
```
