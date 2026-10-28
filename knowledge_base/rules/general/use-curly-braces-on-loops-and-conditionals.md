---
name: libvpx/use-curly-braces-on-loops-and-conditionals
description: Enclose all conditional (if/else) and loop (for, while, do) bodies in curly braces `{}` even for single-line statements, and omit redundant condition parentheses
rubric: readability
severity: should_fix
---

**Rule.** Always enclose conditional (`if`, `else`) and loop (`for`, `while`,
`do`-`while`) bodies in curly braces `{}` in new and modified code. This applies
unconditionally to single-line statements, multi-line blocks, statements
wrapping across multiple lines, and nested control structures. Do not omit
braces on single-statement branches or outer loop structures. Additionally, when
writing multiline conditional expressions, omit redundant parentheses around
sub-expressions in flat logical `&&` or `||` conditions where operator
precedence makes grouping unambiguous.

**Why.** Omitting curly braces around control-flow bodies violates project style
conventions, harms code readability, risks dangling-else ambiguities, and
creates subtle bugs when statements are added or modified during future edits.
Enclosing bodies in braces clearly separates conditions from execution blocks.
Omitting unnecessary inner parentheses in flat logical conditions reduces visual
clutter without altering precedence.

## Examples

Bad:

```cpp
// Bad: Omitting braces on single-line if/else, nested loops, and multiline conditions.
if (yoffset == 0)
  src_ptr -= 2;
else
  src_ptr -= 2 + (src_pixels_per_line * 2);

for (i = 0; i < 2; ++i, s += p)
  for (j = 0; j < 2; ++j)
    sum += s[j];

if (cpi->oxcf.speed >= 9 &&
    (tmp_mv->as_mv.row == 0 && tmp_mv->as_mv.col == 0))
  subpel_force_stop = 1;
```

Good:

```cpp
// Good: All conditional branches and loop bodies are enclosed in curly braces, with redundant parens removed.
if (yoffset == 0) {
  src_ptr -= 2;
} else {
  src_ptr -= 2 + (src_pixels_per_line * 2);
}

for (i = 0; i < 2; ++i, s += p) {
  for (j = 0; j < 2; ++j) {
    sum += s[j];
  }
}

if (cpi->oxcf.speed >= 9 &&
    tmp_mv->as_mv.row == 0 && tmp_mv->as_mv.col == 0) {
  subpel_force_stop = 1;
}
```
