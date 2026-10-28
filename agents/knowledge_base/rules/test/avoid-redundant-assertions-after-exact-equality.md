---
name: libvpx/test/avoid-redundant-assertions-after-exact-equality
description: When a GoogleTest assertion already validates an exact value with EXPECT_EQ or ASSERT_EQ, avoid adding redundant inequality checks like EXPECT_GT or EXPECT_LT
rubric: readability
severity: nit
---

**Rule.** When a test already verifies an exact result using an equality
assertion (such as `EXPECT_EQ` or `ASSERT_EQ`), do not add redundant inequality
or range assertions (such as `EXPECT_GT`, `EXPECT_LT`, `EXPECT_GE`, or
`EXPECT_LE`) on the same variable for thresholds already satisfied by that exact
value. If the relationship to a threshold or constant (such as exceeding
`INT_MAX`) is non-obvious, document the derivation in a comment instead of
asserting it separately.

**Why.** An exact equality assertion already strictly checks the computed value
and guarantees any looser inequality condition. Adding redundant inequality
assertions adds visual noise without increasing test coverage or catching
additional failure modes.

## Examples

Bad:

```cpp
EXPECT_EQ(tokens, (int64_t)1668 * 1668 * (16 * 16 * 3 + 4));
EXPECT_GT(tokens, INT_MAX);
```

Good:

```cpp
// 1668 * 1668 * 772 = 2147876928, which is > INT_MAX.
EXPECT_EQ(tokens, (int64_t)1668 * 1668 * (16 * 16 * 3 + 4));
```
