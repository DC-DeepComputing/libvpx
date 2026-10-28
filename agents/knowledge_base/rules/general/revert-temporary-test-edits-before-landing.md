---
name: libvpx/revert-temporary-test-edits-before-landing
description: When submitting build configuration or toolchain changes, revert temporary proof-of-concept test modifications and drive-by edits in production source files before landing.
rubric: change_scope
severity: should_fix
---

**Rule.** Do not include temporary proof-of-concept edits, test modifications,
or unrelated drive-by changes in production source code when submitting build
configuration or compiler flag updates. Revert any local scratch edits or bot
verification tweaks made in production source files prior to landing, keeping
build changes isolated in dedicated commits.

**Why.** Temporary test edits left in production code introduce unnecessary
churn, complicate code reviews, and risk landing unvetted or non-standard
syntax. Keeping build and toolchain changes isolated ensures clean revision
history and focused review scope.

## Examples

Bad:

```cpp
/* In a build configuration change modifying compiler warning flags: */
/* Unrelated temporary test edit left in vp9_cx_iface.c: */
stats = (const FIRSTPASS_STATS *)cfg->rc_twopass_stats_in.buf +
        n_packets - cfg->ss_number_layers + i;
const unsigned int layer_id = (int)stats->spatial_layer_id;
```

Good:

```cpp
/* Revert temporary production edits before landing build changes: */
unsigned int layer_id;
/* ... */
stats = (const FIRSTPASS_STATS *)cfg->rc_twopass_stats_in.buf +
        n_packets - cfg->ss_number_layers + i;
layer_id = (int)stats->spatial_layer_id;
```
