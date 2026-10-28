---
name: libvpx/return-expression-directly
description: When returning computed results or booleans, return expressions directly instead of declaring single-use temporaries, mutating variables before return, or using compound assignments in return statements
rubric: readability
severity: nit
---

**Rule.** Do not introduce unnecessary single-use temporary variables solely to
store and return a value, and do not perform mutating compound assignments (such
as `*=`, `+=`, `-=`, `/=`) either immediately before or directly inside a
`return` statement. Return the evaluated expression or arithmetic computation
directly unless an intermediate variable is required for debug assertions or
substantially clarifies an overly complex expression.

**Why.** Declaring redundant temporary variables or mutating local variables
immediately prior to returning them introduces dead stores, confusing side
effects, and unnecessary boilerplate. Returning pure expressions directly
simplifies data flow, eliminates mutable state, and makes calculation helpers
and predicate functions cleaner and easier to read.

## Examples

Bad:

```cpp
// Redundant temporary variable assigned and returned immediately:
int denoise_svc_pickmode = denoise_svc(cpi) && !lc->is_key_frame;
return denoise_svc_pickmode;

// Mutating local variable immediately before return:
static double get_bits_percent_diff(int target_bits, int actual_bits) {
  double diff = abs(target_bits - actual_bits) * 1.0 / target_bits;
  diff *= 100.0;
  return diff;
}

// Compound assignment directly inside return statement:
return diff *= 100.0;
```

Good:

```cpp
// Return boolean expression directly:
return denoise_svc(cpi) && !lc->is_key_frame;

// Return arithmetic computation directly without intermediate mutation:
static double get_bits_percent_diff(int target_bits, int actual_bits) {
  return abs(target_bits - actual_bits) * 100.0 / target_bits;
}

// Return pure arithmetic expression without side effects:
return diff * 100.0;
```
