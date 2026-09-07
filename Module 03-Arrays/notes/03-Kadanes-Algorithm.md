# Kadane's Algorithm — The Complete Reference

> The maximum subarray problem in O(n) time and O(1) space.
> More importantly: the *thinking pattern* — "compute the answer that ends exactly here, then take the best over all endings" — which solves a whole family of problems.

---

## Table of Contents

1. [The Problem](#1-the-problem)
2. [The Key Insight](#2-the-key-insight)
3. [The Algorithm](#3-the-algorithm)
4. [Correctness Argument](#4-correctness-argument)
5. [The All-Negative Edge Case](#5-the-all-negative-edge-case)
6. [Printing the Subarray Itself](#6-printing-the-subarray-itself)
7. [Variants](#7-variants)
8. [Maximum Product Subarray — Why Kadane Needs Modification](#8-maximum-product-subarray--why-kadane-needs-modification)
9. [Worked Examples](#9-worked-examples)
10. [Common Pitfalls](#10-common-pitfalls)
11. [Related Problems in This Module](#11-related-problems-in-this-module)

---

## 1. The Problem

> Given an integer array (which may contain negative numbers), find the contiguous subarray with the largest sum, and return that sum.

```
arr = [-2, 1, -3, 4, -1, 2, 1, -5, 4]
answer = 6   from the subarray [4, -1, 2, 1]
```

### Complexity ladder

| Approach | Time | Space | Idea |
| --- | --- | --- | --- |
| Brute force (3 loops) | O(n^3) | O(1) | try every `(i, j)`, sum from scratch |
| Brute force (2 loops) | O(n^2) | O(1) | try every `i`, extend `j` with a running sum |
| Divide & conquer | O(n log n) | O(log n) | best-left, best-right, best-crossing |
| **Kadane** | **O(n)** | **O(1)** | running state, single pass |

---

## 2. The Key Insight

Do not think about "all subarrays". Think about the subarrays that **end at index `i`** — there are only `i+1` of them, and their best is computable from the answer at `i-1`.

Let `curr(i)` = the maximum sum of a subarray **ending exactly at `i`**. Such a subarray either

- extends the best subarray ending at `i-1`, giving `curr(i-1) + arr[i]`, or
- starts fresh at `i`, giving `arr[i]`.

There is no third option, so:

```
curr(i) = max( arr[i], curr(i-1) + arr[i] )
answer  = max over all i of curr(i)
```

That is the entire algorithm. It is a one-state dynamic program, and because `curr(i)` only depends on `curr(i-1)`, the DP table collapses to a single variable.

### The equivalent "reset" view

`curr(i-1) + arr[i] < arr[i]` happens exactly when `curr(i-1) < 0`. So:

> **If the running sum ever goes negative, throw it away and restart from 0.**

A negative prefix can only hurt whatever follows it — dropping it is always at least as good.

---

## 3. The Algorithm

### Algorithm Steps

1. Initialise `curr = arr[0]` and `best = arr[0]`.
2. For each `i` from `1` to `n-1`:
   a. `curr = max(arr[i], curr + arr[i])` — extend or restart.
   b. `best = max(best, curr)` — record the global optimum.
3. Return `best`.

```cpp
int maxSubArray(vector<int>& nums) {
    int curr = nums[0];   // best sum of a subarray ending at the current index
    int best = nums[0];   // best sum seen anywhere so far

    for (int i = 1; i < (int)nums.size(); i++) {
        curr = max(nums[i], curr + nums[i]);   // extend, or start fresh here
        best = max(best, curr);                // update global answer
    }
    return best;
}
```

This is exactly the implementation in [04-max_subarray_sum.cpp](../02-Medium/04-max_subarray_sum.cpp).

### The reset form (equivalent, seen more often in the wild)

```cpp
int maxSubArray(vector<int>& nums) {
    int curr = 0, best = INT_MIN;
    for (int x : nums) {
        curr += x;
        best = max(best, curr);
        if (curr < 0) curr = 0;    // discard a negative prefix
    }
    return best;
}
```

> Note the order: update `best` **before** resetting. Resetting first would make an all-negative array return 0, which is wrong unless empty subarrays are allowed.

**Time O(n)** — one pass, constant work per element. **Space O(1)** — two integers.

---

## 4. Correctness Argument

**Claim.** After processing index `i`, `curr` holds the maximum sum over all subarrays ending at `i`, and `best` holds the maximum over all subarrays ending at any index `<= i`.

*Base case.* At `i = 0` the only subarray ending at 0 is `[arr[0]]`, and both variables equal `arr[0]`. ✓

*Inductive step.* Assume the claim at `i-1`. Any subarray ending at `i` has the form `arr[s..i]`. Either `s == i` (sum `arr[i]`), or `s < i`, in which case `arr[s..i-1]` is a subarray ending at `i-1` and its best possible sum is `curr(i-1)` by hypothesis, giving `curr(i-1) + arr[i]`. Taking the max over the two cases is exactly line (a). Line (b) then extends the global max from "ending at `<= i-1`" to "ending at `<= i`". ✓

*Conclusion.* At `i = n-1`, `best` is the maximum over all subarrays ending at any index — that is, over all subarrays. ∎

The optimal subarray is guaranteed to be found because every subarray ends *somewhere*, and every ending index is examined.

---

## 5. The All-Negative Edge Case

```
arr = [-5, -2, -8, -1]
```

- **If a subarray must be non-empty** (the standard LeetCode 53 statement), the answer is `-1` — the least-bad single element. The `max(arr[i], curr + arr[i])` form and the reset form (with `best` updated before the reset) both produce this correctly.
- **If the empty subarray is allowed**, the answer is `0`.

The classic bug is initialising `best = 0`:

```cpp
int best = 0;              // WRONG for the non-empty version
```

This returns `0` for the array above. Initialise with `arr[0]` or `INT_MIN` instead.

> Always ask the interviewer which convention applies. It is the one edge case they check for.

---

## 6. Printing the Subarray Itself

Track a tentative start; commit the boundaries only when a new global best appears.

```cpp
void maxSubArrayWithIndices(vector<int>& nums) {
    int curr = nums[0], best = nums[0];
    int start = 0, ansStart = 0, ansEnd = 0;

    for (int i = 1; i < (int)nums.size(); i++) {
        if (nums[i] > curr + nums[i]) {   // restarting here
            curr = nums[i];
            start = i;                    // tentative new start
        } else {
            curr = curr + nums[i];        // extending
        }

        if (curr > best) {                // commit the boundaries
            best = curr;
            ansStart = start;
            ansEnd = i;
        }
    }
    cout << "Sum " << best << " from index " << ansStart << " to " << ansEnd << "\n";
}
```

The distinction between `start` (tentative) and `ansStart` (committed) is essential — a restart does not by itself mean a better answer was found.

---

## 7. Variants

### 7.1 Minimum subarray sum

Negate everything, or mirror the operators:

```cpp
curr = min(arr[i], curr + arr[i]);
best = min(best, curr);
```

### 7.2 Best time to buy and sell stock

`maxProfit` is Kadane over the **daily differences** `prices[i] - prices[i-1]`: the profit of buying on day `i` and selling on day `j` is the sum of the differences in between. The usual implementation states it directly as "track the minimum price so far":

```cpp
int maxProfit(vector<int>& prices) {
    int minPrice = prices[0], best = 0;
    for (int p : prices) {
        best = max(best, p - minPrice);   // sell today
        minPrice = min(minPrice, p);      // or buy today
    }
    return best;
}
```

Both are the same algorithm wearing different clothes. → [06-buy_and_sell_stock.cpp](../02-Medium/06-buy_and_sell_stock.cpp)

### 7.3 Circular maximum subarray sum

The best subarray either does not wrap, or it does — and if it wraps, its complement is a non-wrapping *minimum* subarray:

```
answer = max( kadaneMax(arr),  totalSum - kadaneMin(arr) )
```

Special case: if every element is negative, `totalSum - kadaneMin` equals `0` (the empty complement), so return `kadaneMax` alone.

### 7.4 Maximum sum with at most one deletion

Two running states — `keep` (nothing deleted yet) and `drop` (one element already deleted):

```cpp
int keep = arr[0], drop = 0, best = arr[0];
for (int i = 1; i < n; i++) {
    drop = max(drop + arr[i], keep);        // delete arr[i], or carry an earlier deletion
    keep = max(keep + arr[i], arr[i]);      // plain Kadane
    best = max({best, keep, drop});
}
```

Note `drop` is computed from the *old* `keep` — order matters.

### 7.5 Maximum sum rectangle in a 2D matrix

Fix a top row and a bottom row, collapse each column into a single sum, and run Kadane on the collapsed 1D array. O(m^2 · n).

### 7.6 Maximum length subarray with sum ≤ k

Kadane does **not** apply — this is a sliding-window (non-negative) or prefix-sum (with negatives) problem. Kadane optimises a *value*, not a *length*. → [04-Two-Pointers.md](04-Two-Pointers.md)

---

## 8. Maximum Product Subarray — Why Kadane Needs Modification

Naively swapping `+` for `*` fails, because with products the **most negative** running value is a valuable asset: one more negative element flips it into a large positive.

```
arr = [2, 3, -2, 4, -1]
```

A single-state Kadane sees `-2` and restarts, missing the answer `48` from the entire array (two negatives cancel).

### Two-state fix

Track the running maximum **and** the running minimum; swap them at every negative element:

```cpp
int maxProduct(vector<int>& nums) {
    int maxEnd = nums[0], minEnd = nums[0], best = nums[0];
    for (int i = 1; i < (int)nums.size(); i++) {
        int x = nums[i];
        if (x < 0) swap(maxEnd, minEnd);          // a negative flips the roles
        maxEnd = max(x, maxEnd * x);
        minEnd = min(x, minEnd * x);
        best   = max(best, maxEnd);
    }
    return best;
}
```

### Prefix / suffix fix

An equivalent, often cleaner formulation: sweep prefix products left to right and suffix products right to left, resetting to `1` at every zero, and take the largest value ever seen.

Why it works: an optimal block never contains a zero, and inside a zero-free block the number of negatives is either even (the whole block is positive — one of the two sweeps covers it entirely) or odd (the best is the block minus its prefix up to the first negative, or minus its suffix from the last negative — the left sweep catches one, the right sweep the other).

This is the approach used in [12-maximum_product_subarray.cpp](../03-Hard/12-maximum_product_subarray.cpp).

---

## 9. Worked Examples

### 9.1 Classic trace

`arr = [-2, 1, -3, 4, -1, 2, 1, -5, 4]` (the array in [02-Medium/04](../02-Medium/04-max_subarray_sum.cpp))

| i | arr[i] | curr + arr[i] | arr[i] | new curr | decision | best |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | −2 | — | — | −2 | init | **−2** |
| 1 | 1 | −1 | 1 | 1 | **restart** | **1** |
| 2 | −3 | −2 | −3 | −2 | extend | 1 |
| 3 | 4 | 2 | 4 | 4 | **restart** | **4** |
| 4 | −1 | 3 | −1 | 3 | extend | 4 |
| 5 | 2 | 5 | 2 | 5 | extend | **5** |
| 6 | 1 | 6 | 1 | 6 | extend | **6** |
| 7 | −5 | 1 | −5 | 1 | extend | 6 |
| 8 | 4 | 5 | 4 | 5 | extend | 6 |

**Answer: 6**, from `[4, -1, 2, 1]` (indices 3..6 — the window that was open when `best` last improved).

Notice index 2: `curr` went negative (`-2`) but we did not force a reset — the `max` at index 3 handled it automatically by choosing `arr[3] = 4` over `-2 + 4 = 2`.

### 9.2 All negative

`arr = [-3, -1, -4, -2]`

| i | arr[i] | curr + arr[i] | arr[i] | new curr | best |
| --- | --- | --- | --- | --- | --- |
| 0 | −3 | — | — | −3 | −3 |
| 1 | −1 | −4 | −1 | −1 | **−1** |
| 2 | −4 | −5 | −4 | −4 | −1 |
| 3 | −2 | −6 | −2 | −2 | −1 |

**Answer: −1.** Every step restarts, and the answer is the largest single element — as it must be.

### 9.3 Maximum product with the two-state form

`arr = [2, 3, -2, 4, -1]`

| i | x | swap? | maxEnd | minEnd | best |
| --- | --- | --- | --- | --- | --- |
| 0 | 2 | — | 2 | 2 | 2 |
| 1 | 3 | no | max(3, 6) = 6 | min(3, 6) = 3 | 6 |
| 2 | −2 | **yes** (max↔min → 3, 6) | max(−2, 3·−2 = −6) = −2 | min(−2, 6·−2 = −12) = −12 | 6 |
| 3 | 4 | no | max(4, −8) = 4 | min(4, −48) = −48 | 6 |
| 4 | −1 | **yes** (→ −48, 4) | max(−1, 48) = **48** | min(−1, −4) = −4 | **48** |

**Answer: 48** — the whole array. The `-48` that looked useless at step 3 is exactly what produced the answer at step 4. That is why the minimum must be tracked.

---

## 10. Common Pitfalls

1. **`best = 0` initialisation** — breaks all-negative arrays. Use `arr[0]` or `INT_MIN`.
2. **Resetting before updating `best`** in the reset form — same failure.
3. **Empty array** — `nums[0]` crashes. Guard with `if (nums.empty()) return 0;` when the constraints allow `n = 0`.
4. **Overflow** — sums over 10^5 elements of magnitude 10^9 need `long long`. Products overflow far sooner; the classic max-product constraints keep the answer within `int`, but do not assume it.
5. **Committing `ansStart` on every restart** rather than only when `best` improves — reports the wrong indices.
6. **Applying plain Kadane to products.** See §8.
7. **Using Kadane for a length question.** Kadane maximises value; for lengths use a window or prefix sums.
8. **Circular variant without the all-negative guard** — `total - kadaneMin` degenerates to 0 when every element is negative.

---

## 11. Related Problems in This Module

| Problem | Kadane connection | File |
| --- | --- | --- |
| Maximum subarray sum | Kadane, textbook form | [02-Medium/04](../02-Medium/04-max_subarray_sum.cpp) |
| Best time to buy and sell stock | Kadane on daily differences | [02-Medium/06](../02-Medium/06-buy_and_sell_stock.cpp) |
| Maximum product subarray | Two-state / prefix-suffix Kadane | [03-Hard/12](../03-Hard/12-maximum_product_subarray.cpp) |
| Max consecutive ones | Same "running state, reset on failure" shape | [01-Easy/07](../01-Easy/07-max_consecutive_ones.cpp) |
| Leaders in an array | Running-extremum scan (right to left) | [02-Medium/09](../02-Medium/09-leaders.cpp) |
| Second largest element | Running-extremum scan | [01-Easy/03](../01-Easy/03-second_largest.cpp) |

See also: [01-Subarrays.md](01-Subarrays.md), [02-Prefix-Sum.md](02-Prefix-Sum.md)

---

## Cheat Sheet

```cpp
// Maximum subarray sum - memorise this shape
int curr = a[0], best = a[0];
for (int i = 1; i < n; i++) {
    curr = max(a[i], curr + a[i]);
    best = max(best, curr);
}
```

| Variant | Change |
| --- | --- |
| Minimum subarray sum | `max` → `min` in both lines |
| Max product | Track `maxEnd` and `minEnd`; `swap` them when `a[i] < 0` |
| Circular max | `max(kadaneMax, total - kadaneMin)`, guard all-negative |
| Stock profit | Kadane on `a[i] - a[i-1]`, or track `minPrice` |
| At most one deletion | Two states: `keep` and `drop` |
| 2D max rectangle | Fix row pair, collapse columns, run Kadane — O(m^2 n) |
| Report indices | Tentative `start` on restart, commit only when `best` improves |
