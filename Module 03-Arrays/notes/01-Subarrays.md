# Subarrays — The Complete Reference

> A subarray is the single most common object in array interview questions.
> Get the definition, the counting formulas, and the three scan patterns right, and half of this module becomes mechanical.

---

## Table of Contents

1. [Definition](#1-definition)
2. [Subarray vs Subsequence vs Subset](#2-subarray-vs-subsequence-vs-subset)
3. [Counting Subarrays](#3-counting-subarrays)
4. [Generating All Subarrays — Brute Force](#4-generating-all-subarrays--brute-force)
5. [The Three Optimisation Patterns](#5-the-three-optimisation-patterns)
6. [Choosing the Right Pattern](#6-choosing-the-right-pattern)
7. [Worked Examples](#7-worked-examples)
8. [Common Pitfalls](#8-common-pitfalls)
9. [Related Problems in This Module](#9-related-problems-in-this-module)

---

## 1. Definition

A **subarray** of an array `arr` of size `n` is a **contiguous, non-empty** block of elements:

```
arr[i], arr[i+1], ..., arr[j]     where 0 <= i <= j <= n-1
```

It is fully described by two indices — the start `i` and the end `j`. Order is preserved and nothing may be skipped.

```
arr = [3, 1, 4, 1, 5]

Valid subarrays:   [3]   [1, 4]   [4, 1, 5]   [3, 1, 4, 1, 5]
Not subarrays:     [3, 4]      -> skips index 1
                   [4, 1, 3]   -> order changed
```

---

## 2. Subarray vs Subsequence vs Subset

| Term | Contiguous? | Order preserved? | Count for size `n` |
| --- | --- | --- | --- |
| **Subarray** | Yes | Yes | `n(n+1)/2` (non-empty) |
| **Subsequence** | No | Yes | `2^n - 1` (non-empty) |
| **Subset** | No | No | `2^n` |

For `arr = [1, 2, 3]`:

- Subarrays: `[1] [2] [3] [1,2] [2,3] [1,2,3]` → 6
- Subsequences: the 6 above **plus** `[1,3]` → 7
- Subsets: the 7 above **plus** `[]` → 8

> **Interview trap:** "longest subarray with sum k" and "longest subsequence with sum k" are completely different problems. The first is prefix-sum / sliding-window; the second is DP or knapsack.

---

## 3. Counting Subarrays

### 3.1 Total number

Choose a start `i` and an end `j >= i`. Equivalently, count by length: `n` subarrays of length 1, `n-1` of length 2, ..., `1` of length `n`.

```
n + (n-1) + ... + 1 = n(n+1)/2
```

| `n` | Subarrays |
| --- | --- |
| 5 | 15 |
| 10 | 55 |
| 1,000 | 500,500 |
| 10^5 | ~5 x 10^9 → brute force is hopeless |

That last row is the whole reason the O(n) patterns below exist.

### 3.2 Subarrays containing a fixed index `k`

The start can be any of `k+1` choices (`0..k`), the end any of `n-k` choices (`k..n-1`):

```
count(k) = (k + 1) * (n - k)
```

This formula powers "sum of all subarray minimums / maximums" style problems (the *contribution* technique: instead of iterating subarrays, ask how many subarrays each element is responsible for).

### 3.3 The "at most" trick

Many problems ask for **exactly K** of something (distinct elements, odd numbers, ...). Direct counting is awkward; the difference of two easy quantities is not:

```
exactly(K) = atMost(K) - atMost(K - 1)
```

`atMost(K)` is a plain sliding window in O(n), so `exactly(K)` is O(n) too.

---

## 4. Generating All Subarrays — Brute Force

Always know the baseline before optimising. It is the correctness oracle you test the fast solution against.

```cpp
// O(n^3) - recompute the sum for every subarray
for (int i = 0; i < n; i++) {
    for (int j = i; j < n; j++) {
        int sum = 0;
        for (int k = i; k <= j; k++) sum += arr[k];
        // subarray arr[i..j] has value 'sum'
    }
}
```

```cpp
// O(n^2) - carry the running sum as j expands. Always prefer this.
for (int i = 0; i < n; i++) {
    int sum = 0;
    for (int j = i; j < n; j++) {
        sum += arr[j];               // sum of arr[i..j]
        // process subarray arr[i..j]
    }
}
```

The second form is the honest brute force: the inner sum is free because extending the right end by one adds exactly one element.

---

## 5. The Three Optimisation Patterns

Nearly every efficient subarray solution is one of these three.

### 5.1 Prefix Sum + Hash Map — O(n)

**Use when:** the array contains **negative numbers**, and you need a count, or a longest/shortest length, for an *exact* target.

Key identity — with `P[i] = arr[0] + ... + arr[i-1]`:

```
sum(arr[i..j]) = P[j+1] - P[i]
```

So a subarray ending at `j` with sum `k` exists for every earlier prefix equal to `P[j+1] - k`. Store prefixes in a hash map as you scan.

→ Full treatment in [02-Prefix-Sum.md](02-Prefix-Sum.md)

Problems: [09-longest_subarray_with_sum_k.cpp](../01-Easy/09-longest_subarray_with_sum_k.cpp), [14-subarray_sum.cpp](../02-Medium/14-subarray_sum.cpp), [05-largest_subarray_sum_zero.cpp](../03-Hard/05-largest_subarray_sum_zero.cpp), [06-subarrays_with_xor_k.cpp](../03-Hard/06-subarrays_with_xor_k.cpp)

### 5.2 Sliding Window (Two Pointers) — O(n)

**Use when:** the array is **all non-negative** (so the sum grows monotonically with the window), or the constraint is "at most K distinct / at most K zeros / no repeats".

Growing the right end can only increase the sum; shrinking the left end can only decrease it. That monotonicity is exactly what lets both pointers move forward only, never backward.

```cpp
int left = 0, sum = 0, best = 0;
for (int right = 0; right < n; right++) {
    sum += arr[right];
    while (sum > k) sum -= arr[left++];   // shrink until valid
    if (sum == k) best = max(best, right - left + 1);
}
```

→ Full treatment in [04-Two-Pointers.md](04-Two-Pointers.md)

Problems: [07-max_consecutive_ones.cpp](../01-Easy/07-max_consecutive_ones.cpp), [09-longest_subarray_with_sum_k.cpp](../01-Easy/09-longest_subarray_with_sum_k.cpp) (positive-only variant)

### 5.3 Kadane / Running-State DP — O(n)

**Use when:** you want an **optimum** (max/min sum, max product, max profit) rather than an exact target.

Define the answer *ending exactly at index i*, express it from the answer ending at `i-1`, and keep a global best.

```
best_ending_here(i) = max(arr[i], best_ending_here(i-1) + arr[i])
answer              = max over all i of best_ending_here(i)
```

→ Full treatment in [03-Kadanes-Algorithm.md](03-Kadanes-Algorithm.md)

Problems: [04-max_subarray_sum.cpp](../02-Medium/04-max_subarray_sum.cpp), [06-buy_and_sell_stock.cpp](../02-Medium/06-buy_and_sell_stock.cpp), [12-maximum_product_subarray.cpp](../03-Hard/12-maximum_product_subarray.cpp)

---

## 6. Choosing the Right Pattern

```
Is the question about a CONTIGUOUS block?
|
+-- No  -> subsequence / subset problem (DP, not this file)
|
+-- Yes
    |
    +-- Asking for a max/min VALUE (sum, product, profit)?
    |       -> Kadane / running-state DP
    |
    +-- Asking for a COUNT or a LENGTH with an EXACT target?
    |   |
    |   +-- Array has negatives, or target is XOR / mod  -> Prefix Sum + Hash Map
    |   +-- All values strictly positive                 -> Sliding Window
    |
    +-- Constraint is "at most K" / "at least K"?
            -> Sliding Window, with exactly(K) = atMost(K) - atMost(K-1)
```

### Why negatives break the sliding window

```
arr = [4, -1, -1, 4], k = 6
```

Window `[4]` has sum 4 < 6, so the rule says expand → `[4,-1]` = 3. The sum **went down** while the window grew. There is no longer a valid rule saying "sum too small ⇒ expand, sum too large ⇒ shrink", so the two-pointer invariant collapses. Prefix sums do not care about sign, so they still work.

---

## 7. Worked Examples

### 7.1 Count subarrays with sum = k (negatives allowed)

`arr = [1, 2, 3, -3, 1, 1, 1, 4, 2, -3]`, `k = 3`

| i | arr[i] | prefix | prefix−k | count of (prefix−k) in map | running answer | map after step |
| --- | --- | --- | --- | --- | --- | --- |
| — | — | 0 | — | — | 0 | {0:1} |
| 0 | 1 | 1 | −2 | 0 | 0 | {0:1, 1:1} |
| 1 | 2 | 3 | 0 | 1 | 1 | + 3:1 |
| 2 | 3 | 6 | 3 | 1 | 2 | + 6:1 |
| 3 | −3 | 3 | 0 | 1 | 3 | 3:2 |
| 4 | 1 | 4 | 1 | 1 | 4 | + 4:1 |
| 5 | 1 | 5 | 2 | 0 | 4 | + 5:1 |
| 6 | 1 | 6 | 3 | 2 | 6 | 6:2 |
| 7 | 4 | 10 | 7 | 0 | 6 | + 10:1 |
| 8 | 2 | 12 | 9 | 0 | 6 | + 12:1 |
| 9 | −3 | 9 | 6 | 2 | 8 | + 9:1 |

**Answer: 8.** Note how steps 3, 6 and 9 pick up matches only because negatives let a prefix sum revisit an old value — precisely the case a sliding window cannot handle.

### 7.2 Longest subarray with sum <= k, all positive

`arr = [2, 1, 5, 1, 3, 2]`, `k = 8`

| right | element | window | sum | action | best |
| --- | --- | --- | --- | --- | --- |
| 0 | 2 | [2] | 2 | ok | 1 |
| 1 | 1 | [2,1] | 3 | ok | 2 |
| 2 | 5 | [2,1,5] | 8 | ok | 3 |
| 3 | 1 | [2,1,5,1] | 9 > 8 | drop 2 → [1,5,1] = 7 | 3 |
| 4 | 3 | [1,5,1,3] | 10 > 8 | drop 1 → [5,1,3] = 9 > 8, drop 5 → [1,3] = 4 | 3 |
| 5 | 2 | [1,3,2] | 6 | ok | 3 |

**Answer: 3.** Each index enters the window once and leaves at most once, so the total pointer movement is `2n` → O(n), even though there is a `while` inside a `for`.

### 7.3 Contribution counting

"Sum of `min` over all subarrays" of `arr = [3, 1, 2]`:

Instead of listing all 6 subarrays, ask for each element how many subarrays it is the minimum of:

| element | index | subarrays where it is the min | contribution |
| --- | --- | --- | --- |
| 3 | 0 | [3] | 3 × 1 = 3 |
| 1 | 1 | [1], [3,1], [1,2], [3,1,2] | 1 × 4 = 4 |
| 2 | 2 | [2] | 2 × 1 = 2 |

Total = 9. The count for element `k` comes from §3.2, restricted to the span where it stays the minimum — a monotonic stack finds those spans in O(n).

---

## 8. Common Pitfalls

1. **Forgetting the empty prefix.** When counting subarrays with sum `k`, the map must start as `{0: 1}`, otherwise every subarray that starts at index 0 is missed.
2. **Storing the last index instead of the first** when you want the *longest* subarray. For longest, keep the **earliest** occurrence of each prefix sum. For counting, keep **frequencies**.
3. **Mixing up the two goals.** Counting needs `unordered_map<sum, count>`; longest needs `unordered_map<sum, firstIndex>`. Never one map trying to do both.
4. **Sliding window on an array with negatives.** See §6.
5. **Integer overflow.** A prefix sum over 10^5 elements each up to 10^9 needs `long long`.
6. **Off-by-one in the sum identity.** With a 0-indexed `P` of length `n+1`, `sum(arr[i..j]) = P[j+1] - P[i]`. Writing `P[j] - P[i]` silently drops the last element.
7. **Assuming a subarray must have length >= 2.** Single elements are subarrays; Kadane's answer on an all-negative array is the largest single element, not 0.

---

## 9. Related Problems in This Module

| Problem | Technique | File |
| --- | --- | --- |
| Max consecutive ones | Running counter / window | [01-Easy/07](../01-Easy/07-max_consecutive_ones.cpp) |
| Longest subarray with sum k | Prefix sum + hash map | [01-Easy/09](../01-Easy/09-longest_subarray_with_sum_k.cpp) |
| Maximum subarray sum | Kadane | [02-Medium/04](../02-Medium/04-max_subarray_sum.cpp) |
| Best time to buy and sell stock | Kadane variant | [02-Medium/06](../02-Medium/06-buy_and_sell_stock.cpp) |
| Count subarrays with sum k | Prefix sum + hash map | [02-Medium/14](../02-Medium/14-subarray_sum.cpp) |
| Largest subarray with sum zero | Prefix sum + hash map | [03-Hard/05](../03-Hard/05-largest_subarray_sum_zero.cpp) |
| Count subarrays with XOR = k | Prefix XOR + hash map | [03-Hard/06](../03-Hard/06-subarrays_with_xor_k.cpp) |
| Maximum product subarray | Prefix / suffix products | [03-Hard/12](../03-Hard/12-maximum_product_subarray.cpp) |

---

## Cheat Sheet

| Quantity | Formula / Complexity |
| --- | --- |
| Number of subarrays | `n(n+1)/2` |
| Number of subarrays containing index `k` | `(k+1)(n-k)` |
| Number of subsequences | `2^n - 1` (non-empty) |
| Brute force scan | O(n^2) with a running sum |
| Prefix sum + hash map | O(n) time, O(n) space |
| Sliding window | O(n) time, O(1) space, non-negative only |
| Kadane | O(n) time, O(1) space |
