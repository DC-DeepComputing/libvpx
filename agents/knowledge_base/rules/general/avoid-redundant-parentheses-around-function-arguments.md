---
name: libvpx/avoid-redundant-parentheses-around-function-arguments
description: When writing expressions, assignments, pointer offsets, dereference arithmetic, type casts, function arguments, or ternary branches, omit redundant parentheses where operator precedence is unambiguous
rubric: readability
severity: nit
---

**Rule.** Do not wrap variable assignments, function arguments, cast operands,
unary dereferences and negations, equality/relational comparisons, ternary
conditions and branches, or pointer offset and dereference arithmetic in
redundant parentheses when operator precedence and associativity are
unambiguous. Specifically:
- **Function arguments & assignments**: Pass arguments and assign right-hand side expressions directly without superfluous outer parentheses (e.g., func(a << b, c >> d) instead of func((a << b), (c >> d)), and dst = width >> 1; instead of dst = (width >> 1);).
- **Type casts & intrinsics**: Cast variables directly without wrapping identifiers in macro-style parentheses (e.g., (Type *)var or (uint64_t)var << 32 instead of (Type *)(var) or (uint64_t)(var) << 32, and _mm256_storeu_si256((__m256i *)b, a) instead of ((__m256i *)(b))).
- **Unary operations & dereferences**: Write unary operations and dereferences directly without wrapping inner expressions or pointer arithmetic (e.g., !*ptr, !cond && ..., *dst + offset, or (Type *)(*dst + offset) instead of !(*ptr), (!cond) && ..., ((*dst) + offset), or (Type *)((*dst) + offset)). Unary dereference (*) and negation (!) bind tighter than binary addition, subtraction, or logical operators.
- **Logical & relational operators**: Rely on standard precedence where relational (<, <=, >, >=) and equality (==, !=) operators bind tighter than logical AND (&&) and OR (||) (e.g., a == b || c == d instead of (a == b) || (c == d)).
- **Pointer offsets & arithmetic**: Rely on multiplicative operators (*, /, %) binding tighter than additive operators (+, -) and left-to-right associativity (e.g., src + y * src_stride instead of src + (y * src_stride), and a * b * c / d instead of (((a * b) * c) / d)).
- **Ternary expressions**: Omit parentheses around uncomplicated conditions and branches in ternary operators (e.g., source ? source + source_sz : source instead of source ? (source + source_sz) : source, and is_intra ? ac_thr : 1 instead of (is_intra) ? ac_thr : 1).

Reserve grouping parentheses for sub-expressions where they alter evaluation order or disambiguate complex mixed operators (such as bitwise operators combined with relational comparisons).

**Why.** In C and C++ regular code bodies (unlike preprocessor macros), named
identifiers, explicit casts, unary operators (!, *), multiplicative arithmetic,
and equality/relational comparisons have well-defined precedence and
associativity relative to additive arithmetic, logical operators (&&, ||),
assignment operators, and ternary conditionals (?:). Specifically, unary
dereference (*) binds tighter than binary addition and subtraction (+, -). Comma
and colon delimiters also establish clear syntactic boundaries. Superfluous
parentheses introduce visual noise and clutter without altering evaluation
semantics, preventing bugs, or improving safety.

## Examples

Bad:

```cpp
// Redundant parentheses in logical comparisons, negations, and dereferences:
if ((tx_type == DCT_DCT) || (tx_type == ADST_DCT)) {
while ((!xd->corrupted) && mi_row < cm->mi_rows)
while (!(*map))

// Redundant inner parentheses around dereferenced pointers in pointer arithmetic and casts:
_mm_store_si128((__m128i *)((*dst) + 8), val);
uint16_t *next = ((*src) + offset);

// Redundant parentheses in pointer offsets, casts, and arithmetic:
const uint8_t *next_block = src + (y * src_stride);
raw_target_rate = (unsigned int)(((int64_t)w * h * 8) * 3 * fps / 1000);
_mm256_storeu_si256((__m256i *)(b), a);

// Redundant parentheses around assignments, arguments, and ternary expressions:
base_mv->as_mv.col = (candidate->mv[0].as_mv.col * 2);
vp9_set_row(lf_sync, (1 << cm->log2_tile_cols), (mi_row >> MI_BLOCK_SIZE_LOG2));
br->user_buffer_end = source ? (source + source_sz) : source;
unsigned int var_thresh = (is_intra) ? (unsigned int)ac_thr : 1;
```

Good:

```cpp
// Clean logical conditions, negations, and unary dereferences:
if (tx_type == DCT_DCT || tx_type == ADST_DCT) {
while (!xd->corrupted && mi_row < cm->mi_rows)
while (!*map)

// Clean pointer arithmetic on dereferenced pointers:
_mm_store_si128((__m128i *)(*dst + 8), val);
uint16_t *next = *src + offset;

// Clean pointer offsets, casts, and arithmetic chains:
const uint8_t *next_block = src + y * src_stride;
raw_target_rate = (unsigned int)((int64_t)w * h * 8 * 3 * fps / 1000);
_mm256_storeu_si256((__m256i *)b, a);

// Clean assignments, arguments, and ternary expressions:
base_mv->as_mv.col = candidate->mv[0].as_mv.col * 2;
vp9_set_row(lf_sync, 1 << cm->log2_tile_cols, mi_row >> MI_BLOCK_SIZE_LOG2);
br->user_buffer_end = source ? source + source_sz : source;
unsigned int var_thresh = is_intra ? (unsigned int)ac_thr : 1;
```
