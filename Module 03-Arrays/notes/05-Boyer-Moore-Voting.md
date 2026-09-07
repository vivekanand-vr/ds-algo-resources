# Boyer–Moore Majority Voting — The Complete Reference

> Find the element occurring more than `n/2` times in **O(n) time and O(1) space** — no hash map, no sorting.
> Then generalise: more than `n/k` times with only `k-1` counters.

---

## Table of Contents

1. [The Problem](#1-the-problem)
2. [The Cancellation Idea](#2-the-cancellation-idea)
3. [The Algorithm — n/2 Majority](#3-the-algorithm--n2-majority)
4. [Correctness Proof](#4-correctness-proof)
5. [The Verification Pass — When It Is Mandatory](#5-the-verification-pass--when-it-is-mandatory)
6. [Generalisation — Elements Appearing More Than n/3 Times](#6-generalisation--elements-appearing-more-than-n3-times)
7. [The General n/k Case](#7-the-general-nk-case)
8. [Worked Examples](#8-worked-examples)
9. [Common Pitfalls](#9-common-pitfalls)
10. [Comparison with Other Approaches](#10-comparison-with-other-approaches)
11. [Related Problems in This Module](#11-related-problems-in-this-module)

---

## 1. The Problem

> Given an array of size `n`, find the **majority element** — the one that appears **more than `n/2`** times.

```
arr = [2, 2, 1, 1, 1, 2, 2]        n = 7,  need count > 3
2 appears 4 times  ->  answer = 2
```

Note the strictness: "more than `n/2`", not "at least". For `n = 4` the element must appear at least 3 times; 2 occurrences is not a majority. This strictness is exactly what makes the algorithm work.

### Why the obvious solutions are unsatisfying

| Approach | Time | Space |
| --- | --- | --- |
| Count every element with a nested loop | O(n²) | O(1) |
| Hash map of frequencies | O(n) | O(n) |
| Sort and take `a[n/2]` | O(n log n) | O(1) |
| **Boyer–Moore voting** | **O(n)** | **O(1)** |

Boyer–Moore is the only one that is optimal on both axes.

---

## 2. The Cancellation Idea

Picture the array as an election where each element is a vote. Repeatedly take **any two votes for different candidates and destroy them both**.

Each such cancellation removes one majority vote at most, and one non-majority vote at least. If the majority holds strictly more than half the ballots, it holds more than all other candidates *combined* — so no matter how the pairings go, the majority can never be fully cancelled out. Whatever survives at the end must be the majority element (if a majority exists at all).

```
[2, 2, 1, 1, 1, 2, 2]

pair (2,1) -> both gone      [2, 1, 1, 2, 2]
pair (2,1) -> both gone      [1, 2, 2]
pair (1,2) -> both gone      [2]              survivor = 2  ✓
```

The algorithm performs exactly this cancellation in a single left-to-right pass, using one candidate variable and one counter instead of physically removing elements. The counter is "how many unmatched votes the current candidate is holding".

---

## 3. The Algorithm — n/2 Majority

### Algorithm Steps

1. Set `count = 0` and `candidate` to anything.
2. For each element `x`:
   a. If `count == 0`, adopt `x` as the new `candidate` and set `count = 1`.
   b. Else if `x == candidate`, `count++` (a supporting vote).
   c. Else `count--` (an opposing vote cancels one supporting vote).
3. `candidate` now holds the only possible majority element.
4. **If a majority is not guaranteed to exist**, make a second pass to count its occurrences and confirm `count > n/2`.

```cpp
int majorityElement(vector<int>& nums) {
    int candidate = nums[0], count = 0;

    // Pass 1: cancellation - find the only possible majority
    for (int x : nums) {
        if (count == 0) { candidate = x; count = 1; }
        else if (x == candidate) count++;
        else count--;
    }

    // Pass 2: verify (skip only if the problem guarantees a majority exists)
    count = 0;
    for (int x : nums) if (x == candidate) count++;
    return (count > (int)nums.size() / 2) ? candidate : -1;
}
```

→ [03-majority_element.cpp](../02-Medium/03-majority_element.cpp)

**Time O(n)** — two linear passes. **Space O(1)** — two variables.

### What `count` actually means

Not "the frequency of the candidate". It is the number of votes for the candidate that have **not yet been cancelled** by an opposing vote in the segment processed so far. When it hits 0, the segment just processed is perfectly balanced and can be discarded entirely — the remaining suffix has the same majority as the whole array (if one exists), which is why restarting from scratch is safe.

---

## 4. Correctness Proof

**Claim.** If some value `m` occurs more than `n/2` times, the algorithm ends with `candidate == m`.

*Setup.* The pass splits the array into consecutive segments, each ending exactly when `count` returns to 0 (with a possible final segment where `count > 0`).

*Property of a completed segment.* Inside a segment starting with candidate `c`, the count rises by 1 for each `c` and falls by 1 for each non-`c`; ending at 0 means the segment contains exactly as many `c` values as non-`c` values. So a completed segment is **at most half `c`, and at most half any other single value** — in particular, at most half of it can be `m`.

*Suppose the algorithm ends with `candidate != m`.* Then every occurrence of `m` lies inside some segment, and in each segment `m` accounts for at most half the elements — because a completed segment is half `c` / half others, and `m` is among the "others", so `m` occupies at most half of it. The final incomplete segment (candidate `c != m`, `count > 0`) has strictly more `c` than everything else, so `m` again holds less than half of it.

Summing over all segments, `m` occupies at most `n/2` positions — contradicting `count(m) > n/2`. ∎

**The converse is false**, which is the whole reason step 4 exists: the algorithm always outputs *something*, and if no majority exists that something is arbitrary.

```
arr = [1, 2, 3]   ->  candidate ends as 3, which is not a majority of anything
```

---

## 5. The Verification Pass — When It Is Mandatory

| Problem statement | Verification needed? |
| --- | --- |
| "A majority element **always exists**" (LeetCode 169) | No — pass 1 alone is correct |
| "Return the majority element **or −1** if none" | **Yes** |
| n/3 variant (LeetCode 229) | **Always yes** — 0, 1 or 2 valid answers |

Skipping verification when the guarantee is absent is the classic Boyer–Moore bug. It costs one extra O(n) pass and zero extra memory, so when in doubt, verify.

---

## 6. Generalisation — Elements Appearing More Than n/3 Times

At most **two** elements can exceed `n/3` (three such elements would need more than `n` positions). So track **two** candidates and **two** counters.

### Algorithm Steps

1. `cnt1 = cnt2 = 0`, candidates unset (use sentinels that cannot occur, or guard with the counter).
2. For each `x`:
   a. If `x == cand1` → `cnt1++`
   b. Else if `x == cand2` → `cnt2++`
   c. Else if `cnt1 == 0` → `cand1 = x, cnt1 = 1`
   d. Else if `cnt2 == 0` → `cand2 = x, cnt2 = 1`
   e. Else → `cnt1--, cnt2--` (a three-way cancellation)
3. Verify both candidates with a second pass, keeping those with count `> n/3`.

```cpp
vector<int> majorityElementII(vector<int>& nums) {
    int cand1 = INT_MIN, cand2 = INT_MIN, cnt1 = 0, cnt2 = 0;

    for (int x : nums) {
        if (cnt1 > 0 && x == cand1)      cnt1++;
        else if (cnt2 > 0 && x == cand2) cnt2++;
        else if (cnt1 == 0)              { cand1 = x; cnt1 = 1; }
        else if (cnt2 == 0)              { cand2 = x; cnt2 = 1; }
        else                             { cnt1--; cnt2--; }
    }

    // Verification is NOT optional here
    int c1 = 0, c2 = 0;
    for (int x : nums) {
        if (cnt1 > 0 && x == cand1) c1++;
        else if (cnt2 > 0 && x == cand2) c2++;
    }

    vector<int> res;
    int n = nums.size();
    if (cnt1 > 0 && c1 > n / 3) res.push_back(cand1);
    if (cnt2 > 0 && c2 > n / 3) res.push_back(cand2);
    return res;
}
```

→ [02-majority_element_II.cpp](../03-Hard/02-majority_element_II.cpp)

### The two ordering rules that matter

1. **Check "matches an existing candidate" before "a counter is zero."** Reversed, a value equal to `cand1` could be installed as `cand2`, and the array would end up with two identical candidates while a genuine third answer is lost.
2. **Decrement both counters together** in the final branch. That is the three-way cancellation — one vote each for `cand1`, `cand2` and `x` annihilate.

---

## 7. The General n/k Case

To find all elements occurring more than `n/k` times, keep **`k-1` candidates** with counters (at most `k-1` such elements can exist):

- If `x` matches a tracked candidate → increment it.
- Else if fewer than `k-1` candidates are live → install `x` with count 1.
- Else → decrement **all** `k-1` counters, dropping any that reach 0.

Then verify all `k-1` survivors in a second pass.

**Time O(nk)** (each element may touch all `k-1` counters), **Space O(k)**. For `k = 2` and `k = 3` this reduces to the two algorithms above. This is the *Misra–Gries* summary — the same structure used in streaming systems to find heavy hitters over data too large to store.

---

## 8. Worked Examples

### 8.1 n/2 majority

`arr = [2, 2, 1, 1, 1, 2, 2]` (the array in [02-Medium/03](../02-Medium/03-majority_element.cpp))

| i | x | count before | action | candidate | count after |
| --- | --- | --- | --- | --- | --- |
| 0 | 2 | 0 | adopt | 2 | 1 |
| 1 | 2 | 1 | match → ++ | 2 | 2 |
| 2 | 1 | 2 | differ → −− | 2 | 1 |
| 3 | 1 | 1 | differ → −− | 2 | **0** |
| 4 | 1 | 0 | adopt | **1** | 1 |
| 5 | 2 | 1 | differ → −− | 1 | **0** |
| 6 | 2 | 0 | adopt | **2** | 1 |

Candidate = `2`. Verification: 2 appears 4 times, and `4 > 7/2 = 3` ✓ → **answer 2**.

Watch index 4: the candidate switched to `1`, which is *not* the answer. Intermediate candidates mean nothing; only the final one does.

### 8.2 No majority exists — why verification matters

`arr = [1, 2, 3, 4]`

| i | x | count before | action | candidate | count after |
| --- | --- | --- | --- | --- | --- |
| 0 | 1 | 0 | adopt | 1 | 1 |
| 1 | 2 | 1 | differ → −− | 1 | 0 |
| 2 | 3 | 0 | adopt | 3 | 1 |
| 3 | 4 | 1 | differ → −− | 3 | 0 |

Candidate = `3`, appearing once. `1 > 4/2` is false → correctly report "none". Without pass 2 this returns a confident, wrong `3`.

### 8.3 n/3 variant

`arr = [1, 1, 1, 3, 3, 2, 2, 2]`, `n = 8`, threshold `> 2`

| i | x | branch | cand1/cnt1 | cand2/cnt2 |
| --- | --- | --- | --- | --- |
| 0 | 1 | cnt1 == 0 → install | 1 / 1 | — / 0 |
| 1 | 1 | match cand1 | 1 / 2 | — / 0 |
| 2 | 1 | match cand1 | 1 / 3 | — / 0 |
| 3 | 3 | cnt2 == 0 → install | 1 / 3 | 3 / 1 |
| 4 | 3 | match cand2 | 1 / 3 | 3 / 2 |
| 5 | 2 | neither, both counts > 0 → decrement both | 1 / 2 | 3 / 1 |
| 6 | 2 | neither, both counts > 0 → decrement both | 1 / 1 | 3 / 0 |
| 7 | 2 | cnt2 == 0 → install | 1 / 1 | **2 / 1** |

Candidates `1` and `2`. Verification: `1` appears 3 times (`3 > 2` ✓), `2` appears 3 times (`3 > 2` ✓) → **answer `[1, 2]`**. The value `3` appears twice, which is not `> 8/3`, and it was correctly evicted.

---

## 9. Common Pitfalls

1. **Skipping the verification pass** when the problem does not guarantee a majority. The algorithm never fails to output — it just outputs garbage.
2. **Reading meaning into intermediate candidates.** Only the final candidate is significant (see §8.1, index 4).
3. **Checking `cnt == 0` before checking `x == cand`** in the n/3 version — duplicates the candidate and loses an answer. Order the branches as in §6.
4. **Using `>=` instead of `>`** in the threshold. "More than `n/2`" is strict; `[1,1,2,2]` has no majority.
5. **Using a real value like `0` or `-1` as an "unset" sentinel** — it collides with legitimate data. Guard on the counter (`cnt1 > 0 && x == cand1`) instead.
6. **Decrementing only one counter** in the n/3 fall-through branch. Both must drop for the cancellation to be sound.
7. **Comparing against `n/3` with integer division carelessly.** For `n = 8`, `n/3 == 2`, so `count > 2` is right. If you prefer to avoid the truncation entirely, test `3 * count > n`.
8. **Assuming the array is non-empty.** `nums[0]` on an empty vector is undefined behaviour.

---

## 10. Comparison with Other Approaches

| Approach | Time | Space | Notes |
| --- | --- | --- | --- |
| Nested-loop count | O(n²) | O(1) | fine for tiny `n` only |
| Hash map | O(n) | O(n) | simplest to write; also gives every frequency |
| Sorting + middle element | O(n log n) | O(1)* | `a[n/2]` must be the majority if one exists |
| Randomised sampling | O(n) expected | O(1) | pick at random, verify, repeat — succeeds with probability > 1/2 per try |
| **Boyer–Moore** | **O(n)** | **O(1)** | optimal; needs the verify pass |
| Bitwise counting | O(32n) | O(1) | count each bit position, rebuild the majority bit by bit — works for the n/2 case |

\* if sorting in place; also destroys the input order.

**When to prefer the hash map:** when you also need frequencies of other elements, or when the streaming/constant-space constraint is absent — it is easier to write correctly under interview pressure. Mention Boyer–Moore as the O(1)-space follow-up.

---

## 11. Related Problems in This Module

| Problem | Variant | File |
| --- | --- | --- |
| Majority element (> n/2) | Single candidate + counter | [02-Medium/03](../02-Medium/03-majority_element.cpp) |
| Majority element II (> n/3) | Two candidates + two counters | [03-Hard/02](../03-Hard/02-majority_element_II.cpp) |
| Single number (all others twice) | XOR cancellation — the same "annihilate pairs" idea in bit form | [01-Easy/08](../01-Easy/08-single_number.cpp) |
| Missing number | Sum / XOR cancellation | [01-Easy/06](../01-Easy/06-missing_number.cpp) |

See also: [01-Subarrays.md](01-Subarrays.md), [04-Two-Pointers.md](04-Two-Pointers.md)

---

## Cheat Sheet

```cpp
// n/2 majority
int cand = nums[0], cnt = 0;
for (int x : nums) {
    if (cnt == 0)          { cand = x; cnt = 1; }
    else if (x == cand)      cnt++;
    else                     cnt--;
}
// then VERIFY: count(cand) > n/2 ?
```

| Threshold | Candidates to track | Verification |
| --- | --- | --- |
| > n/2 | 1 | needed unless the problem guarantees existence |
| > n/3 | 2 | always |
| > n/k | k − 1 | always; O(nk) time, O(k) space (Misra–Gries) |

**One-line summary:** cancel unequal pairs; whatever survives is the only possible majority — then prove it.
