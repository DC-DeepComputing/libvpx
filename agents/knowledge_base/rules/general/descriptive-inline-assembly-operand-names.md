---
name: libvpx/descriptive-inline-assembly-operand-names
description: When defining inline assembly operands and loop counters, use descriptive semantic operand names and dedicated initialized C variables rather than generic scratch arrays or placeholders
rubric: readability
severity: should_fix
---

**Rule.** When writing inline assembly blocks (`__asm__`), assign descriptive,
role-specific symbolic operand names (such as `[loop_count]`, `[stride]`,
`[src_ptr]`, or `[step]`) to registers rather than generic temporary names (such
as `[tmp2]`, `[ftmp0]`, or `[temp]`). Do not lump semantic control variables
(such as loop counters or step counts) into generic scratch arrays (e.g.,
`tmp[]`); instead, declare them as distinct, descriptively named C variables,
initialize them in C, and pass them via read-write operand constraints (e.g.,
`[loop_count]"+&r"(loop_count)`) rather than loading immediate constants inside
the assembly body with instructions like `MMI_LI`.

**Why.** Using generic scratch arrays and temporary names like `[tmp2]` obscures
register roles and control flow in inline assembly. Binding descriptive,
C-initialized variables to explicit read-write operand constraints clarifies
loop termination conditions, avoids redundant load-immediate instructions inside
assembly routines, and simplifies register allocation and maintenance.

## Examples

Bad:

```cpp
// Bad: Bundling loop counter in scratch array, loading immediate in asm, and using generic operand names
uint64_t tmp[3];

__asm__ volatile (
  MMI_LI(%[tmp2], 0x04)
  "1:                                                     \n\t"
  COPY_MEM_16X2
  COPY_MEM_16X2
  MMI_ADDIU(%[tmp2], %[tmp2], -0x01)
  "bnez       %[tmp2],    1b                              \n\t"
  : [tmp0]"=&r"(tmp[0]), [tmp1]"=&r"(tmp[1]), [tmp2]"=&r"(tmp[2])
  : [src]"r"(src), [dst]"r"(dst)
  : "memory"
);
```

Good:

```cpp
// Good: Distinct C variable initialized in C, descriptive operand name, and read-write constraint
uint64_t tmp[2];
uint8_t loop_count = 4;

__asm__ volatile (
  "1:                                                     \n\t"
  COPY_MEM_16X2
  COPY_MEM_16X2
  MMI_ADDIU(%[loop_count], %[loop_count], -0x01)
  "bnez       %[loop_count],    1b                        \n\t"
  : [tmp0]"=&r"(tmp[0]), [tmp1]"=&r"(tmp[1]), [loop_count]"+&r"(loop_count)
  : [src]"r"(src), [dst]"r"(dst)
  : "memory"
);
```
