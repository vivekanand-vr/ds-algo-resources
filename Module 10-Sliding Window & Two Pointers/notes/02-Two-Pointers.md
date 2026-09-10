# Two Pointers — Shapes and Selection

> Two indices walking a sequence under a rule, each moving in one direction only.
> A sliding window is one of these shapes; this note covers the other three and how to tell them apart.

> **Scope note.** The array-flavoured instances — Dutch National Flag, in-place merges, 3Sum/4Sum — are worked through in detail in
> [../../Module%2003-Arrays/notes/04-Two-Pointers.md](../../Module%2003-Arrays/notes/04-Two-Pointers.md).
> This note is the technique-level view: what the shapes are, why the pruning is valid, and which one a given problem wants.
> For windows specifically, see [01-Sliding-Window.md](01-Sliding-Window.md).

---

## Table of Contents

1. [The Core Idea — Monotonic Pruning](#1-the-core-idea--monotonic-pruning)
2. [The Shapes](#2-the-shapes)
3. [Shape A — Converging Ends](#3-shape-a--converging-ends)
4. [Shape B — Fast & Slow](#4-shape-b--fast--slow)
5. [Shape C — Sliding Window](#5-shape-c--sliding-window)
6. [Shape D — Two Sequences](#6-shape-d--two-sequences)
7. [Shape E — Expand Around Center](#7-shape-e--expand-around-center)
8. [Picking a Shape](#8-picking-a-shape)
9. [Why It Is O(n)](#9-why-it-is-on)
10. [Common Pitfalls](#10-common-pitfalls)
11. [Cheat Sheet](#11-cheat-sheet)

---

## 1. The Core Idea — Monotonic Pruning

The brute force for pair and range problems is a nested loop over O(n²) index pairs. Two pointers beats it only when a **single comparison lets you discard a whole set of candidates without examining them**.

That licence comes from one of three places:

- **Sortedness** — in a sorted array, `a[l] + a[r] > target` implies `a[l] + a[anything ≥ r]` is also too large, so every pair using that `r` dies with one `r--`.
- **Non-negativity** — with non-negative values, widening a window can only increase the sum and narrowing can only decrease it, so neither pointer needs to reverse.
- **Symmetry** — for palindromes and reversals, position `i` from the left is fundamentally paired with position `i` from the right; the pairing itself is the structure.

Without one of these, the pruning is unjustified and the answer will be wrong on some input. Every shape below is that same idea wearing different starting positions.

---

## 2. The Shapes

| Shape | Start | Movement | Answers |
| --- | --- | --- | --- |
| **A. Converging ends** | `0` and `n-1` | toward each other | pair with target sum, palindrome check, reverse, container with most water |
| **B. Fast & slow** | both at `0` | forward at different rates | in-place removal / partition / compaction |
| **C. Sliding window** | both at `0` | `r` expands, `l` follows | longest/shortest/count of ranges under a constraint |
| **D. Two sequences** | `0` in each input | whichever is behind advances | merge, union, intersection |
| **E. Expand around center** | both at a center | **outward**, together | longest palindromic substring |

A–D all move pointers *inward or forward*. E is the odd one out: it starts in the middle and grows outward, which is why it is O(n²) rather than O(n) — there are O(n) centers to try.

---

## 3. Shape A — Converging Ends

**Precondition: sorted input, or an inherently symmetric problem.**

```cpp
int l = 0, r = n - 1;
while (l < r) {
    int sum = a[l] + a[r];
    if (sum == target)      { /* found */ l++; r--; }
    else if (sum < target)  l++;      // need bigger
    else                    r--;      // need smaller
}
```

Each comparison retires an entire row or column of the O(n²) pair table, so the walk is O(n) after sorting.

### Palindrome check — symmetry, no sorting needed

```cpp
bool isPalindrome(const string& s) {
    int l = 0, r = (int)s.size() - 1;
    while (l < r) if (s[l++] != s[r--]) return false;
    return true;
}
```

Only `n/2` comparisons, O(1) space. The "ignore non-alphanumerics / case" variant just adds skip loops on both ends before each comparison:

```cpp
while (l < r && !isalnum((unsigned char)s[l])) l++;
while (l < r && !isalnum((unsigned char)s[r])) r--;
if (tolower((unsigned char)s[l]) != tolower((unsigned char)s[r])) return false;
```

### Reversal

```cpp
while (l < r) swap(a[l++], a[r--]);
```

The building block of array rotation (reverse whole, reverse the two parts) and of "build forwards, reverse once" string construction — see the note on avoiding O(n²) prepends in [../../C%2B%2B%20Data%20Structures/02-CPP-String-Reference.md](../../C%2B%2B%20Data%20Structures/02-CPP-String-Reference.md) §5.

`while (l < r)`, not `l <= r`: when the two pointers meet on the same element there is nothing left to compare or swap.

---

## 4. Shape B — Fast & Slow

Both start left. **Fast** reads every element; **slow** marks where the next kept element belongs. Everything before `slow` is final.

```
         slow                fast
          |                   |
[ kept kept | ...garbage... | unread... ]
```

```cpp
int slow = 0;
for (int fast = 0; fast < n; fast++) {
    if (keep(a[fast])) a[slow++] = a[fast];
}
// a[0..slow-1] is the answer; slow is the new length
```

**Invariant:** `slow <= fast` always, so a write never clobbers an element that has not been read yet. If your variant can break that invariant, the logic is wrong.

Use `swap(a[slow], a[fast])` instead of assignment when the discarded values still need to end up somewhere (move-zeroes pushes the zeros to the back for free that way).

Worked instances: [../../Module%2003-Arrays/notes/04-Two-Pointers.md](../../Module%2003-Arrays/notes/04-Two-Pointers.md) §4.

---

## 5. Shape C — Sliding Window

The subject of this module. Two indices moving forward, maintaining a contiguous range plus a folded summary of its contents, answering "longest / shortest / how many ranges satisfy X".

Full treatment — templates, the validity litmus test, distinct-count windows, the `exactly(k) = atMost(k) - atMost(k-1)` reduction, monotonic deques — in **[01-Sliding-Window.md](01-Sliding-Window.md)**.

---

## 6. Shape D — Two Sequences

One pointer per input; advance whichever is behind.

```cpp
int i = 0, j = 0;
while (i < n && j < m) {
    if      (a[i] < b[j]) { take(a[i]); i++; }
    else if (a[i] > b[j]) { take(b[j]); j++; }
    else                  { take(a[i]); i++; j++; }   // equal — union takes one
}
while (i < n) take(a[i++]);                            // drain the remainder
while (j < m) take(b[j++]);
```

O(n + m). The two drain loops after the main loop are the part people forget.

Worked instances, including the backward in-place merge (fill from the back so unread elements are never overwritten): [../../Module%2003-Arrays/notes/04-Two-Pointers.md](../../Module%2003-Arrays/notes/04-Two-Pointers.md) §6.

---

## 7. Shape E — Expand Around Center

The shape that only shows up on strings. Instead of walking inward, plant both pointers at a candidate center and step them **outward** while the pair still matches.

```cpp
void expand(const string& s, int l, int r, int& bestStart, int& bestLen) {
    int n = s.size();
    while (l >= 0 && r < n && s[l] == s[r]) {
        if (r - l + 1 > bestLen) { bestLen = r - l + 1; bestStart = l; }
        l--; r++;
    }
}
```

**Why it terminates early and correctly:** a palindrome cannot contain a mismatched symmetric pair, so the first mismatch means no wider palindrome shares this center — stop and move on.

**Both center shapes must be tried**, which is the detail most people miss:

```cpp
for (int i = 0; i < n; i++) {
    expand(s, i, i,     bestStart, bestLen);   // odd length,  center = one char
    expand(s, i, i + 1, bestStart, bestLen);   // even length, center = a gap
}
```

That is `2n - 1` centers, each expanding up to O(n) ⇒ **O(n²) time, O(1) space**. Manacher's algorithm does it in O(n) but is rarely expected.

→ [../../Module%2005-Strings/02-Medium/07-longest_palindromic_substring.cpp](../../Module%2005-Strings/02-Medium/07-longest_palindromic_substring.cpp)

> Note the contrast with Shape A: checking *whether* a given string is a palindrome converges inward in O(n); *finding* the longest palindromic substring expands outward from every center in O(n²).

---

## 8. Picking a Shape

```
Is the answer a CONTIGUOUS range chosen by a constraint?
|
+-- longest / shortest / count of such ranges     -> C  Sliding window  [01]
|      (values non-negative, or a count/distinct constraint)
+-- constraint involves negative sums              -> not a window; prefix sum + hash map
|
Is the input SORTED (or sortable) and you need a pair/triplet summing to a target?
                                                   -> A  Converging ends
|
Are you rewriting the input in place (remove / partition / compact)?
                                                   -> B  Fast & slow
|
Are there TWO sorted inputs to combine?
                                                   -> D  Two sequences
|
Are you looking for a symmetric structure (palindrome) inside a string?
+-- verify one string is symmetric                 -> A  converge inward, O(n)
+-- find the longest symmetric substring           -> E  expand outward, O(n^2)
|
Is the window state impossible to un-fold (max, median)?
                                                   -> window + monotonic deque / heaps  [01 §10]
```

---

## 9. Why It Is O(n)

Shapes A–D are linear for the same amortised reason, phrased two ways:

- **Converging (A, D):** the gap between the pointers starts at `n` and only ever narrows. Each iteration closes it by at least 1, so there are at most `n` iterations.
- **Forward (B, C):** each pointer only increases and is bounded by `n`, so their combined movement is ≤ `2n` — regardless of how the loops are nested.

The trap in shape C is reading a `while` inside a `for` as O(n²). The inner loop's total budget is `n` steps **shared across the whole run**, not `n` steps per iteration. Shape E genuinely is O(n²), because the outer loop over centers is not a pointer walk — it restarts the expansion each time.

---

## 10. Common Pitfalls

1. **Forgetting to sort** before a converging walk — the entire pruning argument depends on order.
2. **`while (l < r)` vs `while (l <= r)`.** Use `<` when the pointers must select *different* elements (pair sums, palindromes, swaps); use `<=` when the element they meet on still needs processing (binary search, DNF's `mid <= high`).
3. **Trying only odd centers** in expand-around-center — `"cbbd"` then returns `"b"` instead of `"bb"`.
4. **Breaking `slow <= fast`** in shape B, which overwrites unread input.
5. **Merging forward** into an array with trailing space — overwrites unread data; merge backwards.
6. **Skipping the drain loops** in shape D, silently truncating the longer input.
7. **Not skipping duplicates** in 3Sum/4Sum, which emits repeated tuples — skip at *every* level, anchor included.
8. **`int` overflow** in 3Sum/4Sum sums — cast to `long long` before adding.
9. **Using a window where values may be negative.** See [01-Sliding-Window.md](01-Sliding-Window.md) §12.
10. **Passing signed/unsigned into the same comparison** (`mp.size() > k`) — cast the `size()` to `int`.

---

## 11. Cheat Sheet

```cpp
// A. Converging ends (sorted)
int l = 0, r = n - 1;
while (l < r) { if (a[l]+a[r] < t) l++; else if (a[l]+a[r] > t) r--; else { /*hit*/ l++; r--; } }

// A'. Palindrome check
int l = 0, r = n - 1;
while (l < r) if (s[l++] != s[r--]) return false;

// B. Fast & slow (in-place keep)
int slow = 0;
for (int fast = 0; fast < n; fast++) if (keep(a[fast])) a[slow++] = a[fast];

// C. Sliding window  -> see 01-Sliding-Window.md
int l = 0;
for (int r = 0; r < n; r++) { add(a[r]); while (!valid()) remove(a[l++]); harvest(); }

// D. Merge two sorted
int i = 0, j = 0;
while (i < n && j < m) (a[i] <= b[j]) ? take(a[i++]) : take(b[j++]);
while (i < n) take(a[i++]);  while (j < m) take(b[j++]);

// E. Expand around center (try both shapes)
for (int i = 0; i < n; i++) { expand(s, i, i); expand(s, i, i + 1); }
```

---

## Related Notes

- [01-Sliding-Window.md](01-Sliding-Window.md) — shape C in full: templates, distinct-count windows, exactly-K, monotonic deque
- [../../Module%2003-Arrays/notes/04-Two-Pointers.md](../../Module%2003-Arrays/notes/04-Two-Pointers.md) — array instances: Dutch National Flag, backward merge, 3Sum/4Sum, worked traces
- [../../C%2B%2B%20Data%20Structures/02-CPP-String-Reference.md](../../C%2B%2B%20Data%20Structures/02-CPP-String-Reference.md) — the string mechanics behind the palindrome and reversal idioms
