---
name: libvpx/avoid-redundant-break-after-return
description: When a switch case or block ends with an unconditional return, omit redundant trailing break statements to avoid unreachable dead code
rubric: readability
severity: nit
---

**Rule.** Do not place a `break;` statement immediately after an unconditional
`return` statement in a `switch` case or control-flow block. Remove unreachable
`break;` statements to keep switch blocks concise and free of dead code.

**Why.** An unconditional `return` immediately exits the enclosing function,
rendering any subsequent `break;` statement unreachable. Redundant `break;`
statements clutter the code, create dead-code noise, and can trigger compiler
dead-code warnings.

## Examples

Bad:

```cpp
switch (img_fmt) {
  case VPX_IMG_FMT_I420:
    return width * height * 3 / 2;
    break;
  case VPX_IMG_FMT_I422:
    return width * height * 2;
    break;
  default:
    return 0;
    break;
}
```

Good:

```cpp
switch (img_fmt) {
  case VPX_IMG_FMT_I420:
    return width * height * 3 / 2;
  case VPX_IMG_FMT_I422:
    return width * height * 2;
  default:
    return 0;
}
```
