# Prefix Sum — The Complete Reference

> One precomputation turns "sum of any range" from O(n) per query into O(1) per query.
> Combined with a hash map, it turns almost every *exact-target subarray* problem from O(n^2) into O(n).

---

## Table of Contents

1. [The Core Idea](#1-the-core-idea)
2. [Building a Prefix Sum Array](#2-building-a-prefix-sum-array)
3. [Range Sum Queries](#3-range-sum-queries)
4. [Prefix Sum + Hash Map — The Big Pattern](#4-prefix-sum--hash-map--the-big-pattern)
5. [The Two Map Flavours](#5-the-two-map-flavours-count-vs-first-index)
6. [Generalising the Operation — XOR, Product, Count](#6-generalising-the-operation--xor-product-count)
7. [Suffix Sums and Prefix + Suffix Combinations](#7-suffix-sums-and-prefix--suffix-combinations)
8. [2D Prefix Sums](#8-2d-prefix-sums)
9. [Difference Array — The Inverse Trick](#9-difference-array--the-inverse-trick)
10. [Worked Examples](#10-worked-examples)
11. [Common Pitfalls](#11-common-pitfalls)
12. [Related Problems in This Module](#12-related-problems-in-this-module)

---

## 1. The Core Idea

A **prefix sum** at index `i` is the sum of everything before / up to `i`. Once you have them, the sum of any range is a subtraction:

```
sum(arr[i..j]) = prefix(j) - prefix(i-1)
```

That single identity is the whole topic. Everything below is a variation on it.

```
arr     =  [ 2,  4,  1,  3,  5]
prefix  =  [ 2,  6,  7, 10, 15]      (inclusive form)

sum(arr[1..3]) = 4 + 1 + 3 = 8
               = prefix[3] - prefix[0] = 10 - 2 = 8
```

---

## 2. Building a Prefix Sum Array

### 2.1 Inclusive form — `P[i] = arr[0] + ... + arr[i]`

```cpp
vector<long long> P(n);
P[0] = arr[0];
for (int i = 1; i < n; i++) P[i] = P[i - 1] + arr[i];

// sum(arr[i..j]) = P[j] - (i > 0 ? P[i - 1] : 0)
```

Compact, but every query needs the `i > 0` guard.

### 2.2 Exclusive / padded form — `P[i] = arr[0] + ... + arr[i-1]`, size `n+1`

```cpp
vector<long long> P(n + 1, 0);
for (int i = 0; i < n; i++) P[i + 1] = P[i] + arr[i];

// sum(arr[i..j]) = P[j + 1] - P[i]     -- no special case, ever
```

**Prefer this one.** `P[0] = 0` represents the empty prefix, which removes the boundary check and is the same `0` you seed the hash map with in §4. Almost every off-by-one bug in this topic comes from mixing the two forms — pick the padded form and stay with it.

### 2.3 Running variable — O(1) space

When you only make a single left-to-right pass and never need to revisit an arbitrary prefix, you do not need the array at all:

```cpp
long long sum = 0;
for (int i = 0; i < n; i++) {
    sum += arr[i];      // sum is now prefix up to i
    // ... use it
}
```

This is what [09-longest_subarray_with_sum_k.cpp](../01-Easy/09-longest_subarray_with_sum_k.cpp) and [14-subarray_sum.cpp](../02-Medium/14-subarray_sum.cpp) do. The hash map still costs O(n) space, but the prefix array itself is avoided.

| Form | Build | Query | Space |
| --- | --- | --- | --- |
| Recompute each time | — | O(n) | O(1) |
| Prefix array | O(n) | O(1) | O(n) |
| Running variable | — | streaming only | O(1) |

---

## 3. Range Sum Queries

The classic use: `q` queries of the form "sum of `arr[l..r]`".

```cpp
// Precompute once - O(n)
vector<long long> P(n + 1, 0);
for (int i = 0; i < n; i++) P[i + 1] = P[i] + arr[i];

// Answer each query in O(1)
long long rangeSum(int l, int r) {   // inclusive l, r
    return P[r + 1] - P[l];
}
```

Total: **O(n + q)** instead of **O(n·q)**.

> Prefix sums are for **static** arrays. If elements change between queries, every update invalidates a suffix of `P` (O(n) per update). Use a Fenwick tree / segment tree for O(log n) update and query.

---

## 4. Prefix Sum + Hash Map — The Big Pattern

This is the pattern that answers most exact-target subarray questions.

### 4.1 The derivation

We scan left to right. At index `j`, let `cur` be the prefix sum up to `j`. A subarray `arr[i..j]` has sum `k` exactly when:

```
cur - P[i] = k        =>        P[i] = cur - k
```

So: **how many (or which) earlier prefixes equal `cur - k`?** A hash map of the prefixes seen so far answers that in O(1).

```
             i                    j
arr:  [ . . . | . . . . . . . . . ]
      \___ P[i] ___/\____ k ______/
      \__________ cur ____________/
```

### 4.2 Template — count subarrays with sum k

```cpp
int subarraySum(vector<int>& arr, int k) {
    unordered_map<long long, int> freq;
    freq[0] = 1;                    // the empty prefix - do not omit this
    long long cur = 0;
    int count = 0;

    for (int x : arr) {
        cur += x;
        if (freq.count(cur - k)) count += freq[cur - k];
        freq[cur]++;                // record AFTER querying
    }
    return count;
}
```

Time O(n), space O(n).

### 4.3 Template — longest subarray with sum k

```cpp
int longestSubarray(vector<int>& arr, int k) {
    unordered_map<long long, int> firstIdx;   // prefix sum -> earliest index
    long long cur = 0;
    int best = 0;

    for (int i = 0; i < (int)arr.size(); i++) {
        cur += arr[i];

        if (cur == k) best = max(best, i + 1);            // whole prefix works

        if (firstIdx.count(cur - k))
            best = max(best, i - firstIdx[cur - k]);

        if (!firstIdx.count(cur)) firstIdx[cur] = i;      // FIRST occurrence only
    }
    return best;
}
```

Storing only the first occurrence is the crux: an earlier start index yields a longer subarray, so overwriting would shrink the answer.

### 4.4 Why the two orderings matter

- **Query before insert.** Inserting `cur` first would let a subarray match itself, producing a phantom empty subarray of sum 0.
- **Seed with `{0: 1}` (or `firstIdx[0] = -1`).** Without it, subarrays that start at index 0 are invisible, because their "earlier prefix" is the empty one.

---

## 5. The Two Map Flavours (count vs first-index)

| Goal | Map type | Insert policy | Seed |
| --- | --- | --- | --- |
| **Count** subarrays with target | `unordered_map<sum, int>` frequency | always `freq[cur]++` | `freq[0] = 1` |
| **Longest** subarray with target | `unordered_map<sum, int>` first index | only if absent | `firstIdx[0] = -1` (or handle `cur == k` explicitly) |
| **Shortest** subarray with target | `unordered_map<sum, int>` latest index | always overwrite | `lastIdx[0] = -1` |
| **Does one exist** (yes/no) | `unordered_set<sum>` | always insert | insert `0` |

Getting these four rows straight removes most of the confusion around this pattern.

---

## 6. Generalising the Operation — XOR, Product, Count

The prefix trick works for any operation with an **inverse**.

### 6.1 Prefix XOR

XOR is its own inverse (`a ^ a = 0`), so:

```
xor(arr[i..j]) = prefXor(j) ^ prefXor(i-1)
```

To count subarrays with XOR `= k`, we need earlier prefixes `y` with `cur ^ y = k`, i.e. **`y = cur ^ k`**:

```cpp
int subarraysWithXorK(vector<int>& arr, int k) {
    unordered_map<int, int> freq;
    freq[0] = 1;
    int cur = 0, count = 0;
    for (int x : arr) {
        cur ^= x;
        count += freq[cur ^ k];     // note: ^ replaces the subtraction
        freq[cur]++;
    }
    return count;
}
```

Same skeleton, one operator changed. → [06-subarrays_with_xor_k.cpp](../03-Hard/06-subarrays_with_xor_k.cpp)

### 6.2 Prefix sum = 0 (longest)

Sum zero is just `k = 0` in §4.3. Two equal prefix sums bracket a zero-sum block:

```
P[i] == P[j]   =>   sum(arr[i+1..j]) == 0
```

→ [05-largest_subarray_sum_zero.cpp](../03-Hard/05-largest_subarray_sum_zero.cpp)

### 6.3 Binary encoding tricks

Many problems become prefix-sum problems after a re-labelling:

| Problem | Transform | Then |
| --- | --- | --- |
| Longest subarray with equal 0s and 1s | map `0 → -1`, `1 → +1` | longest subarray with sum 0 |
| Subarray with sum divisible by k | store `cur % k` (normalise negatives) | equal remainders bracket a valid block |
| Longest subarray with #even = #odd | `even → -1`, `odd → +1` | longest subarray with sum 0 |
| Count subarrays with exactly K odds | `odd → 1, even → 0` | count subarrays with sum K |

The last row is why "nice subarrays" problems look identical to "subarray sum equals K" once transformed.

### 6.4 What does **not** work

Prefix **product** cannot be undone by division when the array contains `0` (and integer division loses precision anyway). That is why [12-maximum_product_subarray.cpp](../03-Hard/12-maximum_product_subarray.cpp) uses a prefix/suffix sweep with resets instead of division — see §7.

---

## 7. Suffix Sums and Prefix + Suffix Combinations

A **suffix** array is the mirror image, built right to left:

```cpp
vector<long long> S(n + 1, 0);
for (int i = n - 1; i >= 0; i--) S[i] = S[i + 1] + arr[i];
```

Two sweeps together answer "best thing on the left AND best thing on the right of each split", which is a very common shape:

- **Product of array except self** — `answer[i] = prefixProduct[i-1] * suffixProduct[i+1]`, no division needed.
- **Maximum product subarray** — sweep prefix products left to right and suffix products right to left, resetting to `1` at every zero; the answer is the largest value seen in either sweep. Two negatives anywhere in a block are caught because at least one of the two sweeps includes them both. → [12-maximum_product_subarray.cpp](../03-Hard/12-maximum_product_subarray.cpp)
- **Leaders in an array** — a suffix maximum scan; an element is a leader iff it is `>=` every element to its right. → [09-leaders.cpp](../02-Medium/09-leaders.cpp)
- **Equilibrium / pivot index** — the index where `prefix[i-1] == suffix[i+1]`.

---

## 8. 2D Prefix Sums

For a matrix, `P[i][j]` = sum of the rectangle from `(0,0)` to `(i-1, j-1)`:

```cpp
vector<vector<long long>> P(m + 1, vector<long long>(n + 1, 0));
for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
        P[i+1][j+1] = mat[i][j] + P[i][j+1] + P[i+1][j] - P[i][j];
```

The `- P[i][j]` is inclusion–exclusion: the top-left block was added twice.

Sum of the rectangle with corners `(r1,c1)` to `(r2,c2)` inclusive:

```cpp
long long rect(int r1, int c1, int r2, int c2) {
    return P[r2+1][c2+1] - P[r1][c2+1] - P[r2+1][c1] + P[r1][c1];
}
```

```
        c1        c2
        |         |
 r1 --  +---------+
        |  WANT   |
 r2 --  +---------+

 WANT = BigRect - TopStrip - LeftStrip + TopLeftCorner
```

Build O(m·n), query O(1). This is the foundation of "count submatrices with sum = target" (fix a pair of rows, collapse each column, then run the 1D §4.2 template on the collapsed row).

---

## 9. Difference Array — The Inverse Trick

Prefix sum's dual: apply many **range updates** in O(1) each, then materialise the array once.

To add `val` to every element of `arr[l..r]`:

```cpp
vector<long long> diff(n + 1, 0);

// each update: O(1)
diff[l]     += val;
diff[r + 1] -= val;

// after all updates, one prefix-sum pass reconstructs the result: O(n)
long long run = 0;
for (int i = 0; i < n; i++) { run += diff[i]; arr[i] += run; }
```

`q` range updates cost **O(q + n)** instead of **O(q·n)**. Classic uses: flight-booking / car-pooling problems, marking intervals, counting overlaps at each point.

---

## 10. Worked Examples

### 10.1 Longest subarray with sum k

`arr = [10, 5, 2, 7, 1, -10]`, `k = 15` (the exact case in [01-Easy/09](../01-Easy/09-longest_subarray_with_sum_k.cpp))

| i | arr[i] | cur | cur == k? | cur−k | firstIdx has cur−k? | length | best | map insert |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0 | 10 | 10 | no | −5 | no | — | 0 | {10:0} |
| 1 | 5 | 15 | **yes** | 0 | — | 2 | 2 | {15:1} |
| 2 | 2 | 17 | no | 2 | no | — | 2 | {17:2} |
| 3 | 7 | 24 | no | 9 | no | — | 2 | {24:3} |
| 4 | 1 | 25 | no | 10 | **yes @0** | 4−0 = 4 | **4** | {25:4} |
| 5 | −10 | 15 | **yes** | 0 | — | 6 | **6** | 15 already present → skip |

**Answer: 6** — the whole array. Two things to notice: the negative at the end makes the sliding window unusable, and `15` was *not* overwritten at step 5, which is exactly the "first occurrence only" rule.

### 10.2 Count subarrays with XOR = k

`arr = [4, 2, 2, 6, 4]`, `k = 6`

| i | arr[i] | cur (prefix XOR) | need `cur ^ k` | freq of need | answer | map |
| --- | --- | --- | --- | --- | --- | --- |
| — | — | 0 | — | — | 0 | {0:1} |
| 0 | 4 | 4 | 2 | 0 | 0 | {0:1, 4:1} |
| 1 | 2 | 6 | 0 | 1 | 1 | + 6:1 |
| 2 | 2 | 4 | 2 | 0 | 1 | 4:2 |
| 3 | 6 | 2 | 4 | 2 | 3 | + 2:1 |
| 4 | 4 | 6 | 0 | 1 | 4 | 6:2 |

**Answer: 4** — the subarrays `[4,2]`, `[2,2,6]`, `[6]`, `[4,2,2,6,4]`.

### 10.3 Difference array

`n = 5`, updates: `+3` on `[1,3]`, `+2` on `[0,2]`, `-1` on `[3,4]`

```
diff after updates:  [ 2,  3,  0, -2,  0,  -1... ]   (index 5 is the pad)
                       ^   ^        ^        ^
                     +2   +3      -3,-2,-1   +1 (from -1 at r+1=5)

running prefix:      [ 2,  5,  5,  3,  2]
```

Three range updates, one O(n) pass, done.

---

## 11. Common Pitfalls

1. **Missing `freq[0] = 1` / `firstIdx[0] = -1`.** Every subarray starting at index 0 is dropped. The single most common bug in this topic.
2. **Inserting before querying.** Creates a spurious count when `k = 0`.
3. **Overwriting the first index** in the "longest" variant, or **not** overwriting in the "shortest" variant.
4. **Mixing the inclusive and padded prefix forms** in the same function. Pick one (padded) and stay with it.
5. **Overflow.** Use `long long` for the prefix; `10^5 × 10^9` overflows `int` by four orders of magnitude.
6. **Negative modulo.** For "sum divisible by k", C++ gives `-7 % 3 == -1`. Normalise with `((cur % k) + k) % k` before using it as a map key.
7. **Applying prefix sums to a mutable array.** Any element update forces an O(n) rebuild — use a Fenwick / segment tree instead.
8. **Trying to invert a prefix product with division.** Breaks on zeros and on integer division. Use prefix + suffix sweeps.
9. **Forgetting `diff` needs size `n+1`** so that `diff[r+1]` is in range when `r == n-1`.

---

## 12. Related Problems in This Module

| Problem | Variant | File |
| --- | --- | --- |
| Longest subarray with sum k | Prefix + first-index map | [01-Easy/09](../01-Easy/09-longest_subarray_with_sum_k.cpp) |
| Count subarrays with sum k | Prefix + frequency map | [02-Medium/14](../02-Medium/14-subarray_sum.cpp) |
| Leaders in an array | Suffix maximum | [02-Medium/09](../02-Medium/09-leaders.cpp) |
| Largest subarray with sum zero | `k = 0` special case | [03-Hard/05](../03-Hard/05-largest_subarray_sum_zero.cpp) |
| Count subarrays with XOR = k | Prefix XOR + frequency map | [03-Hard/06](../03-Hard/06-subarrays_with_xor_k.cpp) |
| Maximum product subarray | Prefix + suffix products | [03-Hard/12](../03-Hard/12-maximum_product_subarray.cpp) |
| Missing number | Sum formula (a prefix-sum identity) | [01-Easy/06](../01-Easy/06-missing_number.cpp) |
| Missing and repeating number | Sum + sum-of-squares equations | [03-Hard/09](../03-Hard/09-missing_and_repeating_number.cpp) |

See also: [01-Subarrays.md](01-Subarrays.md), [04-Two-Pointers.md](04-Two-Pointers.md)

---

## Cheat Sheet

| Task | Tool | Complexity |
| --- | --- | --- |
| Range sum, static array | Prefix array (padded) | O(n) build, O(1) query |
| Range sum, mutable array | Fenwick / segment tree | O(log n) both |
| Count subarrays, sum = k | Prefix + frequency map, seed `{0:1}` | O(n) / O(n) |
| Longest subarray, sum = k | Prefix + first-index map | O(n) / O(n) |
| Count subarrays, XOR = k | Prefix XOR, look up `cur ^ k` | O(n) / O(n) |
| Longest, equal 0s and 1s | Map `0 → -1`, then sum = 0 | O(n) / O(n) |
| Sum divisible by k | Map on `((cur%k)+k)%k` | O(n) / O(k) |
| Range sum on a matrix | 2D prefix + inclusion–exclusion | O(mn) build, O(1) query |
| Many range updates | Difference array | O(q + n) |
