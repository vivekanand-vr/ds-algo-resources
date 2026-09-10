# Sliding Window & Two Pointers — Concept Notes

Theory notes for the techniques behind the problems in [Module 10](../). Each note gives the idea, the templates, the reasoning behind why the pruning is valid, worked traces, and pitfalls.

| # | Note | Covers |
| --- | --- | --- |
| 01 | [Sliding Window](01-Sliding-Window.md) | the three templates, window state, measuring vs counting, distinct-count windows, `exactly(k) = atMost(k) - atMost(k-1)`, fixed-size windows, monotonic deque, amortised O(n) |
| 02 | [Two Pointers](02-Two-Pointers.md) | the five pointer shapes, monotonic pruning, converging ends, fast & slow, two sequences, expand around center, shape selection |

Two closely related notes live in other modules and are not duplicated here:

- [Module 03-Arrays / Two Pointers](../../Module%2003-Arrays/notes/04-Two-Pointers.md) — the array-flavoured instances: Dutch National Flag, in-place backward merge, 3Sum/4Sum, plus worked traces
- [Module 03-Arrays / Prefix Sum](../../Module%2003-Arrays/notes/02-Prefix-Sum.md) — the fallback whenever the values can be negative and a window is therefore illegal

---

## Which technique for which problem?

```
CONTIGUOUS range chosen by a constraint?
|
+-- longest / shortest / count of valid ranges          -> Sliding window   [01]
|      +-- constraint on distinct values                 -> frequency map, size() = distinct  [01 §6]
|      +-- constraint is "EXACTLY k", and you must COUNT -> atMost(k) - atMost(k-1)           [01 §7]
|      +-- constraint is "EXACTLY k", and you must MEASURE-> at-most-k window, record at == k  [01 §6.3]
|      +-- width is fixed                                -> fixed-size window                 [01 §9]
|      +-- state is max/min (cannot be un-folded)        -> window + monotonic deque          [01 §10]
+-- sums with NEGATIVE values                            -> not a window; prefix sum + hash map
|
PAIR / TRIPLET with a target sum?
+-- sorted, or sorting is allowed                        -> Converging ends  [02 §3]
+-- unsorted and original indices needed                 -> hash map
|
IN-PLACE rewrite (remove / partition / compact)?         -> Fast & slow      [02 §4]
|
TWO sorted inputs to combine?                            -> Two sequences    [02 §6]
|
SYMMETRIC structure inside a string?
+-- verify one string is a palindrome                    -> converge inward, O(n)     [02 §3]
+-- find the longest palindromic substring               -> expand outward, O(n^2)    [02 §7]
```

**Litmus test before writing any window:** *if a window is invalid, is every window containing it also invalid?* If no, do not use a window — see [01 §2](01-Sliding-Window.md#2-the-litmus-test--when-a-window-is-legal).

---

## Technique index by problem

### 01-Medium

| Problem | Technique |
| --- | --- |
| [01 Longest substring with unique characters](../01-Medium/01-longest_substring_with_uniques.cpp) | Longest-window template, local duplicate check — [01 §4.1](01-Sliding-Window.md#41-longest-valid-window), [§6.1](01-Sliding-Window.md#61-no-repeats-at-all) |

### 02-Hard

| Problem | Technique |
| --- | --- |
| [01 Longest substring with K unique characters](../02-Hard/01-longest_substring_with_k_uniques.cpp) | At-most-k window, measured only at exactly k — [01 §6.3](01-Sliding-Window.md#63-exactly-k-distinct--longest) |
| [02 Subarrays with K different integers](../02-Hard/02-subarrays_with_k_different_integers.cpp) | Counting window + the exactly-k reduction — [01 §5](01-Sliding-Window.md#5-measuring-vs-counting), [§7](01-Sliding-Window.md#7-the-exactly-k-reduction) |

### Related problems in other modules

| Problem | Technique |
| --- | --- |
| [Longest palindromic substring](../../Module%2005-Strings/02-Medium/07-longest_palindromic_substring.cpp) | Expand around center — [02 §7](02-Two-Pointers.md#7-shape-e--expand-around-center) |
| [Longest subarray with sum k](../../Module%2003-Arrays/01-Easy/09-longest_subarray_with_sum_k.cpp) | Window if all-positive, prefix sum + map if negatives are possible |
| [Max consecutive ones](../../Module%2003-Arrays/01-Easy/07-max_consecutive_ones.cpp) | Degenerate window — the run resets on a zero |
| [Remove duplicates from sorted array](../../Module%2003-Arrays/01-Easy/10-remove_duplicates.cpp) | Fast & slow compaction — [02 §4](02-Two-Pointers.md#4-shape-b--fast--slow) |
| [Move zeroes](../../Module%2003-Arrays/01-Easy/04-move_zeros.cpp) | Fast & slow with `swap` |
| [Union of two sorted arrays](../../Module%2003-Arrays/01-Easy/05-find_union.cpp) | Two-sequence merge walk — [02 §6](02-Two-Pointers.md#6-shape-d--two-sequences) |
| [3Sum](../../Module%2003-Arrays/03-Hard/03-three_sum.cpp) / [4Sum](../../Module%2003-Arrays/03-Hard/04-four_sum.cpp) | Anchor + converging ends |
| [Merge sorted arrays in place](../../Module%2003-Arrays/03-Hard/08-merge_sorted_arrays.cpp) | Backward merge |
