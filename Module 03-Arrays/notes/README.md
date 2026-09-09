# Arrays — Concept Notes

Theory notes for the recurring techniques behind the problems in [Module 03-Arrays](../). Each note gives the idea, the templates, worked traces, pitfalls, and links to the problem files that use it.

| # | Note | Covers |
| --- | --- | --- |
| 01 | [Subarrays](01-Subarrays.md) | definition, counting formulas, brute force, choosing between the three O(n) patterns |
| 02 | [Prefix Sum](02-Prefix-Sum.md) | range queries, prefix + hash map, XOR variant, 2D prefix, difference array |
| 03 | [Kadane's Algorithm](03-Kadanes-Algorithm.md) | maximum subarray, correctness, variants, max product |
| 04 | [Two Pointers & Sliding Window](04-Two-Pointers.md) | the four pointer shapes, window templates, Dutch National Flag |
| 05 | [Boyer–Moore Voting](05-Boyer-Moore-Voting.md) | majority > n/2, the n/3 variant, the general n/k case |
| — | [C++ `std::vector` Reference](../../C%2B%2B%20Data%20Structures/01-CPP-Vector-Reference.md) | member functions, STL algorithms, comparators, complexities, pitfalls (moved to [C++ Data Structures/](../../C%2B%2B%20Data%20Structures/)) |

---

## Which technique for which problem?

```
CONTIGUOUS block asked for?
|
+-- max/min VALUE (sum, product, profit)        -> Kadane            [03]
+-- COUNT / LENGTH with an exact target
|      +-- negatives, or XOR / mod target       -> Prefix sum + map  [02]
|      +-- all values positive                  -> Sliding window    [04]
+-- "at most K" constraint                      -> Sliding window    [04]

PAIR / TRIPLET with a target sum?
+-- sorted, or sorting is allowed               -> Opposite pointers [04]
+-- unsorted and indices needed                 -> Hash map

IN-PLACE rearrangement (remove, partition, move)?
                                                -> Fast & slow       [04]

Element occurring more than n/2 or n/3 times, O(1) space?
                                                -> Boyer-Moore       [05]
```

---

## Technique index by problem

### 01-Easy

| Problem | Technique |
| --- | --- |
| [01 Is rotated and sorted](../01-Easy/01-is_rotated_and_sorted.cpp) | Single scan with a wrap check |
| [02 Rotate array](../01-Easy/02-rotate_array.cpp) | Reversal — [Two Pointers](04-Two-Pointers.md) |
| [03 Second largest](../01-Easy/03-second_largest.cpp) | Running extremum — [Kadane](03-Kadanes-Algorithm.md) family |
| [04 Move zeroes](../01-Easy/04-move_zeros.cpp) | Fast & slow — [Two Pointers](04-Two-Pointers.md) |
| [05 Union of sorted arrays](../01-Easy/05-find_union.cpp) | Merge walk — [Two Pointers](04-Two-Pointers.md) |
| [06 Missing number](../01-Easy/06-missing_number.cpp) | Sum formula — [Prefix Sum](02-Prefix-Sum.md) |
| [07 Max consecutive ones](../01-Easy/07-max_consecutive_ones.cpp) | Window / running counter — [Two Pointers](04-Two-Pointers.md) |
| [08 Single number](../01-Easy/08-single_number.cpp) | XOR cancellation — see [Boyer–Moore](05-Boyer-Moore-Voting.md) §11 |
| [09 Longest subarray with sum k](../01-Easy/09-longest_subarray_with_sum_k.cpp) | [Prefix Sum](02-Prefix-Sum.md) + hash map |
| [10 Remove duplicates](../01-Easy/10-remove_duplicates.cpp) | Fast & slow — [Two Pointers](04-Two-Pointers.md) |
| [11 Move zeroes](../01-Easy/11-move_zeros.cpp) | Fast & slow — [Two Pointers](04-Two-Pointers.md) |

### 02-Medium

| Problem | Technique |
| --- | --- |
| [01 Two sum](../02-Medium/01-two_sum.cpp) | Hash map (two pointers if sorted) |
| [02 Sort colors](../02-Medium/02-sort_colors.cpp) | Counting; DNF in [Two Pointers](04-Two-Pointers.md) §7 |
| [03 Majority element](../02-Medium/03-majority_element.cpp) | [Boyer–Moore](05-Boyer-Moore-Voting.md) |
| [04 Max subarray sum](../02-Medium/04-max_subarray_sum.cpp) | [Kadane](03-Kadanes-Algorithm.md) |
| [05 Next permutation](../02-Medium/05-next_permutation.cpp) | Dip + swap + reverse suffix |
| [06 Buy and sell stock](../02-Medium/06-buy_and_sell_stock.cpp) | [Kadane](03-Kadanes-Algorithm.md) variant |
| [07 Rearrange array by sign](../02-Medium/07-rearrange_array.cpp) | Two write cursors |
| [08 Max sum of 2 min elements](../02-Medium/08-max_sum_2_min_elements.cpp) | Adjacent pair scan |
| [09 Leaders in an array](../02-Medium/09-leaders.cpp) | Suffix maximum — [Prefix Sum](02-Prefix-Sum.md) §7 |
| [10 Longest consecutive sequence](../02-Medium/10-longest_consecutive.cpp) | Hash set |
| [11 Set matrix zeroes](../02-Medium/11-set_zeros.cpp) | In-place markers — [Vectors](../../C%2B%2B%20Data%20Structures/01-CPP-Vector-Reference.md) §9 |
| [12 Rotate matrix](../02-Medium/12-rotate.cpp) | Transpose + reverse rows |
| [13 Spiral order](../02-Medium/13-spiral_order.cpp) | Four boundary pointers |
| [14 Count subarrays with sum k](../02-Medium/14-subarray_sum.cpp) | [Prefix Sum](02-Prefix-Sum.md) + hash map |

### 03-Hard

| Problem | Technique |
| --- | --- |
| [01 Pascal's triangle](../03-Hard/01-pascals_triangle.cpp) | Row-by-row build — [Vectors](../../C%2B%2B%20Data%20Structures/01-CPP-Vector-Reference.md) §9 |
| [02 Majority element II](../03-Hard/02-majority_element_II.cpp) | [Boyer–Moore](05-Boyer-Moore-Voting.md) with 2 candidates |
| [03 Three sum](../03-Hard/03-three_sum.cpp) | Sort + [Two Pointers](04-Two-Pointers.md) |
| [04 Four sum](../03-Hard/04-four_sum.cpp) | Sort + [Two Pointers](04-Two-Pointers.md) |
| [05 Largest subarray sum zero](../03-Hard/05-largest_subarray_sum_zero.cpp) | [Prefix Sum](02-Prefix-Sum.md), `k = 0` |
| [06 Subarrays with XOR k](../03-Hard/06-subarrays_with_xor_k.cpp) | [Prefix XOR](02-Prefix-Sum.md) §6.1 |
| [07 Merge intervals](../03-Hard/07-merge_intervals.cpp) | Sort + linear sweep |
| [08 Merge sorted arrays](../03-Hard/08-merge_sorted_arrays.cpp) | Backward merge — [Two Pointers](04-Two-Pointers.md) §6 |
| [09 Missing and repeating](../03-Hard/09-missing_and_repeating_number.cpp) | Sum equations — [Prefix Sum](02-Prefix-Sum.md) |
| [10 Inversion count](../03-Hard/10-inversion_count.cpp) | Merge sort + two-pointer counting |
| [11 Reverse pairs](../03-Hard/11-reverse_pairs.cpp) | Merge sort + two-pointer counting |
| [12 Maximum product subarray](../03-Hard/12-maximum_product_subarray.cpp) | [Prefix/suffix](02-Prefix-Sum.md) §7, [Kadane](03-Kadanes-Algorithm.md) §8 |
