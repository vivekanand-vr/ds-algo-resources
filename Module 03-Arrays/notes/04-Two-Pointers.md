# Two Pointers & Sliding Window — The Complete Reference

> Two indices walking an array under a rule, each moving forward only.
> Total work `O(n)` even though the code looks nested — because each pointer traverses the array at most once.

---

## Table of Contents

1. [The Core Idea](#1-the-core-idea)
2. [The Four Shapes](#2-the-four-shapes)
3. [Shape A — Opposite Ends (Converging)](#3-shape-a--opposite-ends-converging)
4. [Shape B — Fast & Slow (In-Place Compaction)](#4-shape-b--fast--slow-in-place-compaction)
5. [Shape C — Sliding Window](#5-shape-c--sliding-window)
6. [Shape D — Two Sequences (Merge)](#6-shape-d--two-sequences-merge)
7. [Three Pointers — Dutch National Flag](#7-three-pointers--dutch-national-flag)
8. [Why It Is O(n), Not O(n²)](#8-why-it-is-on-not-on)
9. [When Two Pointers Do NOT Apply](#9-when-two-pointers-do-not-apply)
10. [Worked Examples](#10-worked-examples)
11. [Common Pitfalls](#11-common-pitfalls)
12. [Related Problems in This Module](#12-related-problems-in-this-module)

---

## 1. The Core Idea

The brute force for pair/range problems is a nested loop: O(n²). Two pointers replaces it whenever the data has a **monotonic property** that lets you conclude, from one comparison, that a whole set of candidates can be discarded without checking them.

That monotonicity comes from one of two places:

- **Sortedness** — in a sorted array, if `a[l] + a[r] > target`, then `a[l] + a[anything >= r]` is also too big, so the entire right side of `r` dies with a single `r--`.
- **Non-negativity** — with non-negative values, extending a window can only grow the sum and shrinking can only reduce it, so a pointer never needs to back up.

Without one of these, the pruning is unjustified and the technique gives wrong answers.

---

## 2. The Four Shapes

| Shape | Pointers start at | Movement | Typical use |
| --- | --- | --- | --- |
| **A. Opposite ends** | `0` and `n-1` | move toward each other | pair with target sum, reverse, palindrome, container with most water |
| **B. Fast & slow** | both at `0` | both forward, at different speeds | in-place removal / partition / compaction |
| **C. Sliding window** | both at `0` | right expands, left catches up | longest/shortest subarray under a constraint |
| **D. Two sequences** | `0` in each array | whichever is behind advances | merge sorted arrays, union, intersection |

All four are one loop with two indices. What differs is *where they start* and *what moves them*.

---

## 3. Shape A — Opposite Ends (Converging)

**Precondition: the array is sorted** (or the problem is symmetric, e.g. reversal).

```cpp
int l = 0, r = n - 1;
while (l < r) {
    int sum = a[l] + a[r];
    if (sum == target) { /* found */ l++; r--; }
    else if (sum < target) l++;      // need a bigger sum
    else r--;                        // need a smaller sum
}
```

**Why the discard is safe.** If `a[l] + a[r] < target`, then `a[l]` paired with *any* index `<= r` is even smaller, so `a[l]` can never be part of a solution within `[l, r]` — retire it. One comparison eliminates an entire row of the O(n²) table.

### 3.1 Two Sum on a sorted array

O(n) time, O(1) space — beats the hash-map solution's O(n) space, but requires sorting first (O(n log n)) and loses the original indices. When indices are required and the array is unsorted, the hash map wins. → [01-two_sum.cpp](../02-Medium/01-two_sum.cpp) uses the hash map for exactly that reason.

### 3.2 3Sum = fix one + Two Pointers

```cpp
sort(a.begin(), a.end());
for (int i = 0; i < n - 2; i++) {
    if (i > 0 && a[i] == a[i-1]) continue;        // skip duplicate anchors
    int l = i + 1, r = n - 1;
    while (l < r) {
        long long s = (long long)a[i] + a[l] + a[r];
        if (s == 0) {
            res.push_back({a[i], a[l], a[r]});
            l++; r--;
            while (l < r && a[l] == a[l-1]) l++;   // skip duplicate lefts
            while (l < r && a[r] == a[r+1]) r--;   // skip duplicate rights
        }
        else if (s < 0) l++;
        else r--;
    }
}
```

O(n²) overall: an O(n) anchor loop × an O(n) two-pointer sweep. The three duplicate-skips are what make the output set unique without a hash set. → [03-three_sum.cpp](../03-Hard/03-three_sum.cpp)

**4Sum** is the same idea one level deeper — two nested anchors plus a two-pointer sweep, O(n³), with a duplicate-skip at each level and `long long` for the sum. → [04-four_sum.cpp](../03-Hard/04-four_sum.cpp)

### 3.3 Reversal

Reversing a range is the degenerate opposite-ends walk, and it is the building block of the rotation algorithm:

```cpp
void reverse(vector<int>& a, int l, int r) {
    while (l < r) swap(a[l++], a[r--]);
}
```

**Rotate by k** = reverse whole, reverse first `k`, reverse rest (or the mirror order for left rotation). O(n) time, O(1) space. → [02-rotate_array.cpp](../01-Easy/02-rotate_array.cpp)

---

## 4. Shape B — Fast & Slow (In-Place Compaction)

Both pointers start at the left. The **fast** pointer reads every element; the **slow** pointer marks where the next kept element goes. Everything before `slow` is finished and correct.

```
         slow                 fast
          |                    |
[ kept kept | ... garbage ... | unread ... ]
```

```cpp
int slow = 0;
for (int fast = 0; fast < n; fast++) {
    if (keep(a[fast])) {
        a[slow] = a[fast];   // or swap(a[slow], a[fast]) to preserve discarded values
        slow++;
    }
}
// a[0..slow-1] is the result; slow is the new length
```

**Invariant:** `a[0 .. slow-1]` always holds the kept elements in their original relative order, and `slow <= fast` always, so a write never clobbers an unread element.

### Instances in this module

| Problem | `keep()` predicate | File |
| --- | --- | --- |
| Remove duplicates from sorted array | `a[fast] != a[slow]` | [10-remove_duplicates.cpp](../01-Easy/10-remove_duplicates.cpp) |
| Move zeroes to the end | `a[fast] != 0` (then zero-fill, or `swap`) | [04-move_zeros.cpp](../01-Easy/04-move_zeros.cpp), [11-move_zeros.cpp](../01-Easy/11-move_zeros.cpp) |
| Remove all occurrences of a value | `a[fast] != val` | — |

`swap` vs `assign`: use `swap` for move-zeroes so the zeros are pushed to the back automatically in one pass; use assign-then-fill when the discarded values are irrelevant.

---

## 5. Shape C — Sliding Window

A contiguous window `[left, right]` that expands on the right and contracts on the left. Two templates cover almost everything.

### 5.1 Longest window satisfying a constraint

```cpp
int left = 0, best = 0;
for (int right = 0; right < n; right++) {
    add(a[right]);                         // include the new element
    while (!valid()) remove(a[left++]);    // shrink until the window is valid again
    best = max(best, right - left + 1);    // every valid window is a candidate
}
```

### 5.2 Shortest window satisfying a constraint

```cpp
int left = 0, best = INT_MAX;
for (int right = 0; right < n; right++) {
    add(a[right]);
    while (valid()) {                       // shrink while STILL valid
        best = min(best, right - left + 1);
        remove(a[left++]);
    }
}
```

The difference is one word: **longest** shrinks while *invalid* and measures after; **shortest** shrinks while *valid* and measures inside. Getting these backwards is the most common sliding-window bug.

### 5.3 Fixed-size window

```cpp
long long sum = 0;
for (int i = 0; i < n; i++) {
    sum += a[i];
    if (i >= k) sum -= a[i - k];      // window is now exactly k wide
    if (i >= k - 1) best = max(best, sum);
}
```

### 5.4 The "exactly K" reduction

```
exactly(K) = atMost(K) - atMost(K - 1)
```

`atMost` is directly expressible as a longest-window scan (§5.1); "exactly" usually is not. Two O(n) passes.

### 5.5 Max consecutive ones

The simplest window of all — the window is the run of ones, and a zero resets it:

```cpp
int count = 0, best = 0;
for (int x : nums) {
    if (x == 1) best = max(best, ++count);
    else count = 0;
}
```

→ [07-max_consecutive_ones.cpp](../01-Easy/07-max_consecutive_ones.cpp)

The generalisation "max consecutive ones with at most `k` flips" is the §5.1 template with `valid() = (zerosInWindow <= k)`.

---

## 6. Shape D — Two Sequences (Merge)

One pointer per array; advance whichever one is behind.

```cpp
int i = 0, j = 0;
while (i < n && j < m) {
    if (a[i] < b[j])      { take(a[i]); i++; }
    else if (a[i] > b[j]) { take(b[j]); j++; }
    else                  { take(a[i]); i++; j++; }   // equal - union takes one
}
while (i < n) { take(a[i++]); }
while (j < m) { take(b[j++]); }
```

O(n + m). Skip duplicates by comparing with the last taken value. → [05-find_union.cpp](../01-Easy/05-find_union.cpp)

### The backward merge

When merging `b` into `a`, which has trailing space, filling **from the front** would overwrite unread elements of `a`. Filling **from the back** cannot — the largest remaining element always lands in a slot that is either free space or an already-copied position.

```cpp
int i = m - 1, j = n - 1, k = m + n - 1;
while (j >= 0) {
    if (i >= 0 && a[i] > b[j]) a[k--] = a[i--];
    else                        a[k--] = b[j--];
}
// no need to drain 'a' - its remaining elements are already in place
```

→ [08-merge_sorted_arrays.cpp](../03-Hard/08-merge_sorted_arrays.cpp)

The condition is `while (j >= 0)`, not `while (i >= 0 && j >= 0)`: if `b` runs out first the rest of `a` is already correctly positioned, but if `a` runs out first the remainder of `b` still has to be copied.

---

## 7. Three Pointers — Dutch National Flag

Sorting an array of only `0`, `1`, `2` in **one pass**, in place. Three pointers carve the array into four regions:

```
[ 0 0 0 | 1 1 1 | ? ? ? ? | 2 2 2 ]
        ^       ^        ^
       low     mid      high
```

**Invariant:** `a[0..low-1]` are all 0, `a[low..mid-1]` are all 1, `a[high+1..n-1]` are all 2, and `a[mid..high]` is unexplored.

```cpp
int low = 0, mid = 0, high = n - 1;
while (mid <= high) {
    if (a[mid] == 0)      swap(a[low++], a[mid++]);
    else if (a[mid] == 1) mid++;
    else                  swap(a[mid], a[high--]);   // do NOT advance mid
}
```

**Why `mid` does not advance in the `2` case:** the value swapped in from `high` has never been examined, so it must be processed on the next iteration. In the `0` case, the value coming from `low` is already known to be a `1` (or `low == mid`), so advancing is safe.

The repo's [02-sort_colors.cpp](../02-Medium/02-sort_colors.cpp) solves this by counting occurrences and overwriting — a clean two-pass O(n) solution. DNF is the one-pass alternative; know both, since interviewers usually ask for the single pass after you give the counting one.

### Related: rearrange by sign

Placing positives at even indices and negatives at odd indices is a two-pointer *placement* (two write cursors stepping by 2 over a result array) rather than a partition. → [07-rearrange_array.cpp](../02-Medium/07-rearrange_array.cpp)

---

## 8. Why It Is O(n), Not O(n²)

The sliding window has a `while` inside a `for`, which *looks* quadratic. It is not — use **amortised** reasoning:

- `right` advances exactly `n` times over the whole run.
- `left` only ever increases, and never exceeds `right`, so it advances at most `n` times **in total across all iterations**.
- Total pointer movement `<= 2n` ⇒ **O(n)**.

The trap is to reason "inner loop could run n times, outer loop runs n times, so n²". That bound is real for a single iteration but cannot happen on every iteration — the inner loop's budget is shared globally. The same argument covers the opposite-ends shape: `l` and `r` together close a gap of `n` and never reopen it.

---

## 9. When Two Pointers Do NOT Apply

| Situation | Why it breaks | Use instead |
| --- | --- | --- |
| Sum window on an array with **negatives** | growing the window can shrink the sum — no monotonicity | prefix sum + hash map ([02-Prefix-Sum.md](02-Prefix-Sum.md)) |
| Pair with target sum, **unsorted**, indices required | sorting destroys the indices | hash map ([01-two_sum.cpp](../02-Medium/01-two_sum.cpp)) |
| Non-contiguous selection (subsequence) | a window is contiguous by definition | DP |
| Maximum *value* rather than a constrained *length* | nothing to shrink toward | Kadane ([03-Kadanes-Algorithm.md](03-Kadanes-Algorithm.md)) |
| Constraint that is not monotone in window size | shrinking may not restore validity | varies; often prefix sums or a monotonic deque |

**Litmus test before writing a window:** *"If a window is invalid, is every larger window containing it also invalid?"* If yes, the window is valid; if no, do not use it.

---

## 10. Worked Examples

### 10.1 Opposite ends — pair with sum 10

`a = [1, 3, 4, 5, 7, 11]` (sorted), target `10`

| step | l | r | a[l] | a[r] | sum | action |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | 0 | 5 | 1 | 11 | 12 > 10 | `r--` |
| 2 | 0 | 4 | 1 | 7 | 8 < 10 | `l++` |
| 3 | 1 | 4 | 3 | 7 | **10** | found `(3, 7)` |

Three comparisons instead of 15 pairs.

### 10.2 Fast & slow — remove duplicates

`a = [1, 1, 2, 2, 2, 3, 4, 4]`

| fast | a[fast] | a[slow] | keep? | array after | slow |
| --- | --- | --- | --- | --- | --- |
| — | — | — | init | `[1,...]` | 0 |
| 1 | 1 | 1 | no | `[1,1,2,2,2,3,4,4]` | 0 |
| 2 | 2 | 1 | yes | `[1,2,2,2,2,3,4,4]` | 1 |
| 3 | 2 | 2 | no | unchanged | 1 |
| 4 | 2 | 2 | no | unchanged | 1 |
| 5 | 3 | 2 | yes | `[1,2,3,2,2,3,4,4]` | 2 |
| 6 | 4 | 3 | yes | `[1,2,3,4,2,3,4,4]` | 3 |
| 7 | 4 | 4 | no | unchanged | 3 |

New length `slow + 1 = 4`, and `a[0..3] = [1, 2, 3, 4]`. Elements past index 3 are junk by design — the problem only guarantees the prefix.

### 10.3 Sliding window — longest subarray with sum ≤ 7 (positives)

`a = [3, 1, 2, 4, 1, 1]`

| right | a[right] | sum after add | shrink? | window | length | best |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | 3 | 3 | no | [3] | 1 | 1 |
| 1 | 1 | 4 | no | [3,1] | 2 | 2 |
| 2 | 2 | 6 | no | [3,1,2] | 3 | **3** |
| 3 | 4 | 10 | drop 3 → 7 | [1,2,4] | 3 | 3 |
| 4 | 1 | 8 | drop 1 → 7 | [2,4,1] | 3 | 3 |
| 5 | 1 | 8 | drop 2 → 6 | [4,1,1] | 3 | 3 |

**Answer: 3.** `right` moved 6 times, `left` moved 3 times — 9 steps total, not 36.

### 10.4 Dutch National Flag

`a = [2, 0, 2, 1, 1, 0]`

| low | mid | high | a[mid] | action | array |
| --- | --- | --- | --- | --- | --- |
| 0 | 0 | 5 | 2 | swap mid↔high, `high--` | `[0,0,2,1,1,2]` |
| 0 | 0 | 4 | 0 | swap low↔mid, both++ | `[0,0,2,1,1,2]` |
| 1 | 1 | 4 | 0 | swap low↔mid, both++ | `[0,0,2,1,1,2]` |
| 2 | 2 | 4 | 2 | swap mid↔high, `high--` | `[0,0,1,1,2,2]` |
| 2 | 2 | 3 | 1 | `mid++` | unchanged |
| 2 | 3 | 3 | 1 | `mid++` | unchanged |
| 2 | 4 | 3 | — | `mid > high`, stop | `[0,0,1,1,2,2]` |

Sorted in a single pass with zero extra memory.

---

## 11. Common Pitfalls

1. **Using a window on data with negatives.** The single biggest correctness error. See §9.
2. **Forgetting to sort** before an opposite-ends walk — the pruning argument depends entirely on order.
3. **`while (l < r)` vs `while (l <= r)`.** Use `<` when the two pointers must pick *different* elements (pair sums); use `<=` when a single element still needs processing (binary search, DNF's `mid <= high`).
4. **Advancing `mid` after the `2`-swap in DNF** — the incoming value was never examined.
5. **Shrinking in the wrong direction.** Longest ⇒ shrink while invalid; shortest ⇒ shrink while valid.
6. **Not skipping duplicates in 3Sum/4Sum** — produces repeated triplets. Skip at *every* level, anchor included.
7. **Forward merge into an array with trailing space** — overwrites unread data. Merge backwards.
8. **Off-by-one in window length.** It is `right - left + 1`, not `right - left`.
9. **`int` overflow in 3Sum/4Sum sums** — cast to `long long` before adding.
10. **Assuming `slow` and `fast` can cross.** In shape B, `slow <= fast` is an invariant; if your code can break it, the logic is wrong.

---

## 12. Related Problems in This Module

| Problem | Shape | File |
| --- | --- | --- |
| Rotate array by k | A (reversal) | [01-Easy/02](../01-Easy/02-rotate_array.cpp) |
| Move zeroes | B (fast & slow) | [01-Easy/04](../01-Easy/04-move_zeros.cpp), [01-Easy/11](../01-Easy/11-move_zeros.cpp) |
| Union of two sorted arrays | D (merge) | [01-Easy/05](../01-Easy/05-find_union.cpp) |
| Max consecutive ones | C (window) | [01-Easy/07](../01-Easy/07-max_consecutive_ones.cpp) |
| Longest subarray with sum k | C (positives) / prefix (negatives) | [01-Easy/09](../01-Easy/09-longest_subarray_with_sum_k.cpp) |
| Remove duplicates from sorted array | B (fast & slow) | [01-Easy/10](../01-Easy/10-remove_duplicates.cpp) |
| Sort colors (0s, 1s, 2s) | 3-pointer DNF (file uses counting) | [02-Medium/02](../02-Medium/02-sort_colors.cpp) |
| Next permutation | A (reverse the suffix) | [02-Medium/05](../02-Medium/05-next_permutation.cpp) |
| Rearrange array by sign | Two write cursors | [02-Medium/07](../02-Medium/07-rearrange_array.cpp) |
| Rotate matrix 90° | A (reverse each row) | [02-Medium/12](../02-Medium/12-rotate.cpp) |
| Spiral order traversal | 4 boundary pointers | [02-Medium/13](../02-Medium/13-spiral_order.cpp) |
| 3Sum | A + anchor | [03-Hard/03](../03-Hard/03-three_sum.cpp) |
| 4Sum | A + 2 anchors | [03-Hard/04](../03-Hard/04-four_sum.cpp) |
| Merge intervals | D-like linear sweep after sorting | [03-Hard/07](../03-Hard/07-merge_intervals.cpp) |
| Merge sorted arrays in place | D (backward merge) | [03-Hard/08](../03-Hard/08-merge_sorted_arrays.cpp) |
| Count inversions | D (merge step of merge sort) | [03-Hard/10](../03-Hard/10-inversion_count.cpp) |
| Reverse pairs | D (extra two-pointer count pass) | [03-Hard/11](../03-Hard/11-reverse_pairs.cpp) |

See also: [01-Subarrays.md](01-Subarrays.md), [02-Prefix-Sum.md](02-Prefix-Sum.md), [03-Kadanes-Algorithm.md](03-Kadanes-Algorithm.md)

---

## Cheat Sheet

```cpp
// A. Opposite ends (sorted)
int l = 0, r = n - 1;
while (l < r) { if (a[l]+a[r] < t) l++; else if (a[l]+a[r] > t) r--; else {/*hit*/ l++, r--;} }

// B. Fast & slow (in-place keep)
int slow = 0;
for (int fast = 0; fast < n; fast++) if (keep(a[fast])) a[slow++] = a[fast];

// C. Longest valid window
int left = 0, best = 0;
for (int right = 0; right < n; right++) { add(a[right]);
    while (!valid()) remove(a[left++]);
    best = max(best, right - left + 1); }

// C'. Shortest valid window
int left = 0, best = INT_MAX;
for (int right = 0; right < n; right++) { add(a[right]);
    while (valid()) { best = min(best, right-left+1); remove(a[left++]); } }

// D. Merge two sorted
int i = 0, j = 0;
while (i < n && j < m) (a[i] <= b[j]) ? take(a[i++]) : take(b[j++]);

// DNF three-way partition
int low = 0, mid = 0, high = n - 1;
while (mid <= high) {
    if (a[mid] == 0) swap(a[low++], a[mid++]);
    else if (a[mid] == 1) mid++;
    else swap(a[mid], a[high--]);
}
```
