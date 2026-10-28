---
name: libvpx/test/name-bug-regression-tests-with-issue-id
description: When adding bug-specific regression test cases in GoogleTest, include the issue tracker ID (e.g., Buganizer<IssueId> or Issue<IssueId>) rather than generic test names
rubric: readability
severity: should_fix
---

**Rule.** Name bug-specific regression tests using the issue tracker ID prefix
(e.g., Buganizer<IssueId>, AomediaIssue<IssueId>, or ChromiumIssue<IssueId>,
optionally followed by a descriptive suffix like ScaledRefs) rather than generic
placeholders such as BuganizerTest... or RegressionTest....

**Why.** Naming regression tests with the specific issue tracker ID ensures
consistency across the test suite and allows developers to directly trace test
cases to their upstream bug reports, reproduction steps, and discussions.

## Examples

Bad:

```cpp
// Generic placeholder test name without the issue number
TEST(EncodeAPI, BuganizerTestScalingIssue) {
  // ...
}
```

Good:

```cpp
// Specific issue ID prefix with optional descriptive suffix
TEST(EncodeAPI, Buganizer487259772ScaledRefs) {
  // ...
}
```
