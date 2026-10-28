---
name: libvpx/test/place-tested-expression-first-in-assertions
description: When writing GoogleTest assertions like EXPECT_EQ or ASSERT_EQ, pass the expression under test first and the expected constant or literal second
rubric: readability
severity: nit
---

**Rule.** In GoogleTest assertions (such as EXPECT_EQ, ASSERT_EQ, EXPECT_NE, and
ASSERT_NE), pass the expression, variable, or function call under test as the
first argument (actual) and the expected constant, literal, or baseline value as
the second argument (expected). Avoid Yoda-style ordering where literal
constants appear as the first argument.

**Why.** Placing the expression under test first and the expected constant
second maintains natural reading order, avoids Yoda-style comparisons, and
ensures consistent assertion failure formatting across the test suite.

## Examples

Bad:

```cpp
EXPECT_EQ(static_cast<unsigned int>(0), GetMismatchFrames());
EXPECT_EQ(nullptr, vpx_img_wrap(&img, format, width, height, align, buf));
EXPECT_EQ(0, fseek(f.file(), 0, 0));
```

Good:

```cpp
EXPECT_EQ(GetMismatchFrames(), 0u);
EXPECT_EQ(vpx_img_wrap(&img, format, width, height, align, buf), nullptr);
EXPECT_EQ(fseek(f.file(), 0, 0), 0);
```
