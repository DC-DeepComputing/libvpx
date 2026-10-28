---
name: libvpx/test/avoid-underscores-in-googletest-names
description: When defining Googletest test suites or test cases with TEST, TEST_F, or TEST_P, use CamelCase without underscores in names
rubric: readability
severity: should_fix
---

**Rule.** Do not use underscores (`_`) in Googletest test suite, fixture, or
test case names defined with `TEST`, `TEST_F`, `TEST_P`, or `TYPED_TEST`. Use
`CamelCase` or `PascalCase` instead to separate words, values, or dimensions.

**Why.** Googletest uses underscores as internal delimiters for synthesized
class and method names, test filtering (`--gtest_filter`), and disabled prefixes
(`DISABLED_`). Underscores in suite or test names can cause name collisions,
test runner errors, or filter matching issues.

## Examples

Bad:

```cpp
TEST_P(DatarateOnePassCbrSvcFrameDropMultiBR,
       OnePassCbrSvc3SL3TL4Threads_1284x770) {
  // ...
}
```

Good:

```cpp
TEST_P(DatarateOnePassCbrSvcFrameDropMultiBR,
       OnePassCbrSvc3SL3TL4Threads1284x770) {
  // ...
}
```
