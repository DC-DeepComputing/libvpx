---
name: libvpx/omit-redundant-inner-casts-in-expressions
description: When casting the result of a bitwise or arithmetic expression to an integer type, omit redundant inner casts on operands that undergo standard integer promotion
rubric: readability
severity: nit
---

**Rule.** When explicitly casting the evaluated result of a bitwise or
arithmetic expression to a target integer type (such as int8_t or uint8_t), do
not add redundant inner casts to the individual operands. Rely on standard
integer promotion during expression evaluation and use a single outer cast on
the overall expression to convert or narrow the result.

**Why.** In C and C++, operands of bitwise and arithmetic operations
automatically undergo integer promotion to int prior to evaluation. Adding inner
casts to operands has no semantic effect on the operation and introduces
unnecessary visual noise compared to a clean outer cast on the parenthesized
expression.

## Examples

Bad:

```cpp
// Redundant inner cast on the operand before bitwise XOR or addition
const int8_t ps1 = (int8_t)((int8_t)*op1 ^ 0x80);
const uint8_t val = (uint8_t)((uint8_t)sample + 4);
```

Good:

```cpp
// Single outer cast converts the promoted expression result
const int8_t ps1 = (int8_t)(*op1 ^ 0x80);
const uint8_t val = (uint8_t)(sample + 4);
```
