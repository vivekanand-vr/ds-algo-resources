# Sliding Window — The Complete Reference

> One contiguous window `[l, r]` over a sequence, where `r` only ever moves right and `l` only ever follows it.
> Total work `O(n)` even though the code looks nested — because neither pointer ever goes backwards.

---

## Table of Contents

1. [What a Window Is](#1-what-a-window-is)
2. [The Litmus Test — When a Window Is Legal](#2-the-litmus-test--when-a-window-is-legal)
3. [The Window State](#3-the-window-state)
4. [The Three Templates](#4-the-three-templates)
5. [Measuring vs Counting](#5-measuring-vs-counting)
6. [Distinct-Count Windows](#6-distinct-count-windows)
7. [The "Exactly K" Reduction](#7-the-exactly-k-reduction)
8. [Choosing the State Container](#8-choosing-the-state-container)
9. [Fixed-Size Windows](#9-fixed-size-windows)
10. [Monotonic Deque — Window Maximum](#10-monotonic-deque--window-maximum)
11. [Why It Is O(n), Not O(n²)](#11-why-it-is-on-not-on)
12. [When a Window Does NOT Apply](#12-when-a-window-does-not-apply)
13. [Worked Traces](#13-worked-traces)
14. [Common Pitfalls](#14-common-pitfalls)
15. [Cheat Sheet](#15-cheat-sheet)

---

## 1. What a Window Is

A window is a **contiguous** stretch of the input, tracked by two indices, plus some running summary of what is inside it (a sum, a count of distinct values, a frequency table…).

```
nums = [ 1, 2, 1, 2, 3 ]
              l     r
              └─────┘      window = nums[l..r], width = r - l + 1
```

The brute force for "find the best subarray/substring satisfying X" is to try all O(n²) start/end pairs and evaluate each in O(n) — O(n³), or O(n²) if the evaluation is incremental. A window collapses that to O(n) by exploiting one fact: **as `r` advances, the correct `l` never needs to move backwards.**

Everything else in this note is a consequence of that single property.

---

## 2. The Litmus Test — When a Window Is Legal

Before writing a single line, answer this:

> **If a window is invalid, is every window that contains it also invalid?**

If yes, the technique is sound. If no, `l` would sometimes need to back up, the pointers stop being monotonic, and the answer will be wrong.

Equivalently, phrased on the shrink side: **shrinking a window must never make it less valid.** That is why these all work:

| Constraint | Shrinking effect | Legal? |
| --- | --- | --- |
| sum ≤ T, all values ≥ 0 | sum can only drop | yes |
| number of distinct values ≤ k | distinct can only drop | yes |
| at most k zeros in the window | zero count can only drop | yes |
| no repeated character | duplicates can only disappear | yes |
| sum ≤ T with **negative** values | sum can go **up** when you drop a negative | **no** |
| sum is exactly T | dropping can overshoot in either direction | **no** |

The last two are the classic wrong applications — see §12 for what to use instead.

---

## 3. The Window State

Every window problem is the same skeleton with three problem-specific operations plugged in:

```cpp
add(x)      // fold x into the state as it enters on the right
remove(x)   // unfold x out of the state as it leaves on the left
valid()     // does the current state satisfy the constraint?
```

| Problem | State | `valid()` |
| --- | --- | --- |
| Longest subarray with sum ≤ T | running `sum` | `sum <= T` |
| Longest substring, no repeats | frequency map | `count[justAdded] == 1` |
| Longest substring, ≤ k distinct | frequency map | `map.size() <= k` |
| Max consecutive ones with ≤ k flips | count of zeros | `zeros <= k` |
| Shortest substring covering a pattern | frequency map + `matched` counter | `matched == required` |

The single most useful trick in this family: **keep a frequency map and erase keys the moment their count hits zero**, so that `map.size()` *is* the number of distinct values in the window. It turns "how many distinct?" into an O(1) read.

```cpp
mp[a[r]]++;                                  // add
mp[a[l]]--;  if (mp[a[l]] == 0) mp.erase(a[l]);   // remove  <- the erase matters
```

Forget the `erase` and `size()` silently counts values that have already left the window — the most common bug in distinct-count problems.

---

## 4. The Three Templates

### 4.1 Longest valid window

```cpp
int l = 0, best = 0;
for (int r = 0; r < n; r++) {
    add(a[r]);
    while (!valid()) remove(a[l++]);      // shrink until valid again
    best = max(best, r - l + 1);          // measure AFTER restoring validity
}
```

### 4.2 Shortest valid window

```cpp
int l = 0, best = INT_MAX;
for (int r = 0; r < n; r++) {
    add(a[r]);
    while (valid()) {                      // shrink while STILL valid
        best = min(best, r - l + 1);       // measure INSIDE the shrink loop
        remove(a[l++]);
    }
}
```

The difference is one word, and getting it backwards is the most common structural bug:

| | shrink while | measure |
| --- | --- | --- |
| **longest** | *invalid* | after the while loop |
| **shortest** | *valid* | inside the while loop |

Intuition: for the longest you want the window as wide as legality allows, so you only shrink under duress. For the shortest you want it as narrow as legality allows, so you keep squeezing as long as it survives.

### 4.3 Fixed-size window

No `while` at all — the window slides at constant width (§9).

---

## 5. Measuring vs Counting

The same skeleton answers two very different questions depending on what you harvest at the bottom of the loop:

```cpp
best = max(best, r - l + 1);   // MEASURE  -> "how wide can a valid window be?"
count += (r - l + 1);          // COUNT    -> "how many valid windows are there?"
```

**Why counting works.** After the shrink, `l` is the *smallest* index such that `[l, r]` is valid. Validity is monotone in the start index — pushing the start rightwards only ever shrinks the window, which can only preserve validity. So the valid start positions for right end `r` are exactly `l, l+1, …, r`: that is `r - l + 1` windows, and no others.

Every subarray has exactly one right endpoint, so summing that over all `r` counts each valid subarray exactly once — no double counting, no gaps.

```
r = 4, l = 3  ->  valid subarrays ending at 4: [3..4], [4..4]  = 4 - 3 + 1 = 2
```

Recognising which of the two you need is usually the whole difficulty of a "hard" window problem: [02-subarrays_with_k_different_integers.cpp](../02-Hard/02-subarrays_with_k_different_integers.cpp) counts, while [01-longest_substring_with_k_uniques.cpp](../02-Hard/01-longest_substring_with_k_uniques.cpp) measures — with an otherwise identical window.

---

## 6. Distinct-Count Windows

The three problems in this module are all built on "how many distinct values are in the window", so it is worth seeing them side by side.

### 6.1 No repeats at all

Every count must be 1. Only the character just added can have broken that, so the check is local:

```cpp
mp[s[r]]++;
while (mp[s[r]] > 1) { mp[s[l]]--; if (!mp[s[l]]) mp.erase(s[l]); l++; }
best = max(best, r - l + 1);
```

→ [01-longest_substring_with_uniques.cpp](../01-Medium/01-longest_substring_with_uniques.cpp)

Equivalently this is the "at most k distinct" window with `k = window width` — but the local check is cheaper and reads better.

### 6.2 At most k distinct

```cpp
mp[s[r]]++;
while ((int)mp.size() > k) { mp[s[l]]--; if (!mp[s[l]]) mp.erase(s[l]); l++; }
best = max(best, r - l + 1);
```

This is *the* canonical form. Both remaining variants are built on it.

### 6.3 Exactly k distinct — longest

Run the at-most-k window, but only record the width when the count is precisely `k`:

```cpp
// ... at-most-k shrink ...
if ((int)mp.size() == k) best = max(best, r - l + 1);
```

**Why that is sufficient**, and not just a heuristic: after the shrink, `l` is minimal with `distinct[l..r] <= k`.

- If `distinct[l..r] == k`, then any *wider* window ending at `r` starts before `l` and therefore — by minimality of `l` — has more than `k` distinct. So `[l, r]` is already the widest "exactly k" window ending at `r`. Recording it loses nothing.
- If `distinct[l..r] < k`, then `l == 0` (we never shrank), so every window ending at `r` is contained in `[0, r]` and has *at most* `distinct[0..r] < k` distinct. No window ending at `r` has exactly `k`, so recording nothing is correct.

Initialising `best = -1` then doubles as the "no such substring exists" answer. → [01-longest_substring_with_k_uniques.cpp](../02-Hard/01-longest_substring_with_k_uniques.cpp)

### 6.4 Exactly k distinct — count

Cannot be read off a single window; use the reduction in §7. → [02-subarrays_with_k_different_integers.cpp](../02-Hard/02-subarrays_with_k_different_integers.cpp)

---

## 7. The "Exactly K" Reduction

```
exactly(k) = atMost(k) - atMost(k - 1)
```

**Derivation.** Partition the subarrays counted by `atMost(k)` by their distinct count `d ≤ k`. Those with `d ≤ k - 1` are exactly the ones counted by `atMost(k - 1)`. What remains is precisely `d == k`. Subtracting removes them all, and nothing else, because both counts count the same objects under the same "unique right endpoint" scheme.

```cpp
int atMostK(vector<int>& a, int k) {
    int l = 0, c = 0; map<int,int> mp;
    for (int r = 0; r < (int)a.size(); r++) {
        mp[a[r]]++;
        while ((int)mp.size() > k) { mp[a[l]]--; if (!mp[a[l]]) mp.erase(a[l]); l++; }
        c += (r - l + 1);
    }
    return c;
}
int exactlyK(vector<int>& a, int k) { return atMostK(a, k) - atMostK(a, k - 1); }
```

Two O(n) passes, so still O(n) overall.

`k = 1` needs no special case: `atMostK(a, 0)` adds a value, immediately shrinks the window to empty (`l` ends at `r + 1`), and contributes `r - l + 1 == 0` each step — correctly reporting zero subarrays with at most zero distinct values.

> The reduction is the standard escape hatch whenever a constraint is an **equality** but only the **inequality** version is window-friendly. It applies to "exactly k odd numbers", "exactly k zeros", "sum exactly in [lo, hi]" (as `atMost(hi) - atMost(lo-1)`), and more.

---

## 8. Choosing the State Container

The window logic is identical; only the constant factor changes.

| State container | Per-op cost | Use when |
| --- | --- | --- |
| `int freq[26]` / `freq[256]` + a manual `distinct` counter | O(1) | lowercase letters or bytes — fastest, no allocation |
| `unordered_map<K,int>` | average O(1) | arbitrary `int` keys, order irrelevant |
| `map<K,int>` | O(log k) | you also need sorted keys or a hard worst-case bound |

The files in this module use `map`, which is why their complexity is stated as **O(n log k)**. Swapping in `unordered_map` — or a fixed array when the alphabet is small — makes them **O(n)** with no change to the algorithm.

With a fixed array you maintain the distinct count yourself, since there is no `size()` to read:

```cpp
int freq[256] = {0}, distinct = 0;
if (freq[(unsigned char)s[r]]++ == 0) distinct++;      // add
if (--freq[(unsigned char)s[l]] == 0) distinct--;      // remove
```

See [C++ Data Structures / map reference](../../C%2B%2B%20Data%20Structures/03-CPP-Map-Reference.md) for the trade-offs in detail.

---

## 9. Fixed-Size Windows

When the width `k` is given, there is nothing to decide — add the entering element, drop the leaving one:

```cpp
long long sum = 0;
for (int i = 0; i < n; i++) {
    sum += a[i];
    if (i >= k) sum -= a[i - k];             // width is now exactly k
    if (i >= k - 1) best = max(best, sum);   // first full window ends at k-1
}
```

The two guards are the only fiddly part: `i >= k` decides when something starts leaving, `i >= k - 1` decides when the window is first complete. Off-by-one here is the usual bug — sanity-check with `k = 1` and `k = n`.

---

## 10. Monotonic Deque — Window Maximum

Sums and counts fold and unfold cleanly, so a plain scalar tracks them. **Maximum does not**: when the current maximum leaves the window you have no idea what the new maximum is, and rescanning costs O(k) per step.

The fix is a deque of *indices* whose values are kept in decreasing order:

```cpp
deque<int> dq;                                  // indices, values decreasing
for (int i = 0; i < n; i++) {
    while (!dq.empty() && dq.front() <= i - k) dq.pop_front();   // drop out-of-window
    while (!dq.empty() && a[dq.back()] <= a[i]) dq.pop_back();   // drop dominated
    dq.push_back(i);
    if (i >= k - 1) res.push_back(a[dq.front()]);                // front is the max
}
```

**Why popping from the back is safe:** if `a[i] >= a[j]` for some earlier `j` still in the deque, then `j` can never be the answer again — every future window containing `j` also contains `i`, which is at least as large. So `j` is dominated and can be discarded forever.

Each index is pushed once and popped once ⇒ **O(n)** total, O(k) space. This is the tool for "max/min of every window of size k", and the same idea underpins monotonic-stack problems.

> No problem in this module uses it yet — it is here because it is the standard answer to "my window state is not foldable", the one gap the three templates cannot cover.

---

## 11. Why It Is O(n), Not O(n²)

The code has a `while` inside a `for`, which *looks* quadratic. Reason **amortised**, not per-iteration:

- `r` advances exactly `n` times over the whole run.
- `l` only ever increases, and never passes `r`, so it advances at most `n` times **in total, across all iterations combined**.
- Total pointer movement ≤ `2n` ⇒ **O(n)** (times the per-op cost of the state container, §8).

The trap is to reason "the inner loop could run `n` times, and the outer loop runs `n` times, so `n²`". The first half is true of a *single* iteration but cannot be true of every iteration — the inner loop draws from one shared global budget of `n` steps. Any iteration that shrinks a lot leaves less for the others.

---

## 12. When a Window Does NOT Apply

| Situation | Why it breaks | Use instead |
| --- | --- | --- |
| Sum constraint with **negative** values | dropping a negative *raises* the sum, so shrinking can break validity — `l` would need to back up | prefix sum + hash map ([Prefix-Sum](../../Module%2003-Arrays/notes/02-Prefix-Sum.md)) |
| Sum **exactly** T, negatives allowed | same loss of monotonicity | prefix sum + hash map |
| Non-contiguous selection (subsequence) | a window is contiguous by definition | DP |
| Maximum *value* rather than a constrained *length* | there is no validity to shrink toward | Kadane ([Kadane's](../../Module%2003-Arrays/notes/03-Kadanes-Algorithm.md)) |
| Window state that cannot be unfolded (max, median) | `remove()` is not implementable in O(1) | monotonic deque (§10), or two heaps for median |
| Constraint not monotone in width | shrinking may not restore validity | varies; often prefix sums |

Note the pattern: with **non-negative** values a window works, and with **negatives** you fall back on prefix sums. That single dividing line decides most "longest/count subarray with sum k" problems.

---

## 13. Worked Traces

### 13.1 Longest substring without repeats — `s = "pwwkew"`

| r | s[r] | after add | shrink | l | window | width | best |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 0 | p | {p:1} | — | 0 | `p` | 1 | 1 |
| 1 | w | {p:1,w:1} | — | 0 | `pw` | 2 | **2** |
| 2 | w | {p:1,w:2} | drop p, drop w | 2 | `w` | 1 | 2 |
| 3 | k | {w:1,k:1} | — | 2 | `wk` | 2 | 2 |
| 4 | e | {w:1,k:1,e:1} | — | 2 | `wke` | 3 | **3** |
| 5 | w | {w:2,k:1,e:1} | drop w | 3 | `kew` | 3 | 3 |

**Answer: 3.** `r` moved 6 times, `l` moved 3 times — 9 steps, not 36.

### 13.2 Longest substring with exactly k = 2 distinct — `s = "aabaaab"`

| r | s[r] | state | size | == k? | window | best |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | a | {a:1} | 1 | no | `a` | -1 |
| 1 | a | {a:2} | 1 | no | `aa` | -1 |
| 2 | b | {a:2,b:1} | 2 | yes | `aab` | 3 |
| 3 | a | {a:3,b:1} | 2 | yes | `aaba` | 4 |
| 4 | a | {a:4,b:1} | 2 | yes | `aabaa` | 5 |
| 5 | a | {a:5,b:1} | 2 | yes | `aabaaa` | 6 |
| 6 | b | {a:5,b:2} | 2 | yes | `aabaaab` | **7** |

**Answer: 7.** The window never exceeded 2 distinct, so `l` never moved at all.

### 13.3 Count subarrays with exactly k = 2 distinct — `nums = [1,2,1,2,3]`

`atMost(2)`:

| r | after add | size | shrink | l | width | running count |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | {1:1} | 1 | — | 0 | 1 | 1 |
| 1 | {1:1,2:1} | 2 | — | 0 | 2 | 3 |
| 2 | {1:2,2:1} | 2 | — | 0 | 3 | 6 |
| 3 | {1:2,2:2} | 2 | — | 0 | 4 | 10 |
| 4 | {1:2,2:2,3:1} | 3 | drop 1, 2, 1 | 3 | 2 | **12** |

`atMost(1)`: by the same walk the window is forced down to a single distinct value at every step, giving width 1 each time ⇒ **5**.

```
exactly(2) = atMost(2) - atMost(1) = 12 - 5 = 7
```

The seven: `[1,2] [2,1] [1,2] [1,2,1] [2,1,2] [1,2,1,2] [2,3]` ✓

---

## 14. Common Pitfalls

1. **Using a window when values can be negative.** The single biggest correctness error. See §12.
2. **Forgetting to `erase` a zero count** from the frequency map — then `map.size()` counts values that already left the window and the distinct count is silently wrong.
3. **Shrinking in the wrong direction.** Longest ⇒ shrink while *invalid*, measure after. Shortest ⇒ shrink while *valid*, measure inside.
4. **`mp.size() > k` without a cast.** `size()` is unsigned, `k` is `int` — the comparison is signed/unsigned and warns (and misbehaves for a negative `k`). Write `(int)mp.size() > k`.
5. **Off-by-one in the width.** It is `r - l + 1`, not `r - l`.
6. **Measuring while the window is still invalid** — put the measurement after the shrink loop, not before it.
7. **`best = 0` when the answer must distinguish "impossible".** Use `-1` (or `INT_MAX` for a minimising problem) and translate at the end.
8. **Counting with `count++` instead of `count += (r - l + 1)`.** One window is not one subarray — see §5.
9. **Resetting the state inside the loop.** The whole point is that the state carries over; rebuilding it per `r` is the O(n²) brute force wearing a window's clothes.
10. **Applying "exactly k" as a single window.** Only the *longest* variant can be read off an at-most-k window (§6.3); *counting* needs the subtraction (§7).

---

## 15. Cheat Sheet

```cpp
// Longest valid window
int l = 0, best = 0;
for (int r = 0; r < n; r++) { add(a[r]);
    while (!valid()) remove(a[l++]);
    best = max(best, r - l + 1); }

// Shortest valid window
int l = 0, best = INT_MAX;
for (int r = 0; r < n; r++) { add(a[r]);
    while (valid()) { best = min(best, r - l + 1); remove(a[l++]); } }

// Count windows (at most k distinct)
int l = 0, cnt = 0;
for (int r = 0; r < n; r++) { mp[a[r]]++;
    while ((int)mp.size() > k) { mp[a[l]]--; if (!mp[a[l]]) mp.erase(a[l]); l++; }
    cnt += (r - l + 1); }

// Exactly k
exactly(k) = atMost(k) - atMost(k - 1);

// Fixed width k
for (int i = 0; i < n; i++) { sum += a[i];
    if (i >= k)     sum -= a[i - k];
    if (i >= k - 1) best = max(best, sum); }

// Distinct count without a map (small alphabet)
if (freq[(unsigned char)s[r]]++ == 0) distinct++;
if (--freq[(unsigned char)s[l]] == 0) distinct--;

// Window maximum (monotonic deque of indices)
while (!dq.empty() && dq.front() <= i - k)   dq.pop_front();
while (!dq.empty() && a[dq.back()] <= a[i])  dq.pop_back();
dq.push_back(i);
```

---

## Related Notes

- [02-Two-Pointers.md](02-Two-Pointers.md) — the other pointer shapes: converging ends, fast & slow, expand-around-center
- [../../Module%2003-Arrays/notes/04-Two-Pointers.md](../../Module%2003-Arrays/notes/04-Two-Pointers.md) — the array-module treatment, with Dutch National Flag, backward merge and 3Sum/4Sum
- [../../Module%2003-Arrays/notes/02-Prefix-Sum.md](../../Module%2003-Arrays/notes/02-Prefix-Sum.md) — what to reach for when the values can be negative
- [../../C%2B%2B%20Data%20Structures/03-CPP-Map-Reference.md](../../C%2B%2B%20Data%20Structures/03-CPP-Map-Reference.md) — the frequency-map mechanics these windows depend on
