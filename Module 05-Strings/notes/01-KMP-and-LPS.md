# KMP and the LPS Array — The Complete Reference

> Substring search in O(n + m) time instead of O(n·m), by never re-reading a character of the text.
> More importantly: the *thinking pattern* — "what I have already matched is a property of the pattern, not of the text, so precompute how far I may safely slide" — which turns a whole family of string problems into a single array lookup.

---

## Table of Contents

1. [The Problem](#1-the-problem)
2. [The Key Insight](#2-the-key-insight)
3. [What LPS Actually Is](#3-what-lps-actually-is)
4. [Building the LPS Array](#4-building-the-lps-array)
5. [Correctness Argument](#5-correctness-argument)
6. [The Search Loop](#6-the-search-loop)
7. [Why It Is O(n + m)](#7-why-it-is-on--m)
8. [Variants and Applications](#8-variants-and-applications)
9. [Worked Examples](#9-worked-examples)
10. [Common Pitfalls](#10-common-pitfalls)
11. [Related Problems in This Module](#11-related-problems-in-this-module)

---

## 1. The Problem

> Given a text `t` of length `n` and a pattern `p` of length `m`, find the index of the first occurrence of `p` in `t` (or all occurrences), or report that there is none.

```
t = "aabaacaabaaba",  p = "aabaaba"
answer = 6            t[6..12] == "aabaaba"
```

The naive approach tries every start position and compares from scratch. It is fine most of the time and catastrophic exactly when the pattern is self-similar:

```
t = "aaaaaaaab",  p = "aaab"
```

Every one of the `n - m + 1` start positions matches `m - 1` characters before failing on the last one. That is the worst case the whole field exists to remove.

### Complexity ladder

| Approach | Time | Space | Idea |
| --- | --- | --- | --- |
| Naive / brute force | O(n·m) | O(1) | try every start, compare from scratch |
| Rabin–Karp | O(n + m) average, O(n·m) worst | O(1) | rolling hash; verify on hash hit |
| Z-algorithm | O(n + m) | O(n + m) | Z-array of `p + '#' + t`; a sibling of KMP |
| **KMP** | **O(n + m)** | **O(m)** | precompute the LPS array, never rewind the text |

Rabin–Karp and Z are worth knowing exist; KMP is the one to own, because its LPS array answers several problems that have nothing to do with searching (see §8).

---

## 2. The Key Insight

Look at what the naive algorithm throws away.

```
t:  a a b a a c ...
p:  a a b a a b
              ^ mismatch at pattern index 5
```

Five characters matched. The naive algorithm now shifts the pattern by one and starts comparing at `t[1]` again — but it *already knows* what `t[0..4]` is. It is `p[0..4]` = `"aabaa"`. That is the whole point:

> At the moment of a mismatch after `j` matched characters, the last `j` characters of the consumed text are **exactly `p[0..j-1]`**. They contain no information about the text that the pattern does not already contain about itself.

So the question "how far may I slide the pattern without skipping a possible occurrence?" depends **only on `j` and on the pattern**. It does not depend on the text at all. Therefore it can be answered once, in advance, for every `j` — and stored in an array.

What is the answer? After sliding, the pattern's prefix must line up with the tail of what we just matched. That means we need the longest prefix of `p` that is also a suffix of `p[0..j-1]`. Anything longer was already ruled out by the mismatch; anything shorter is a wasted attempt that a later fallback would reach anyway.

Two consequences, and they are the reasons KMP is fast:

1. **The text pointer `i` never moves backwards.** It only ever advances.
2. **The precomputation is O(m)**, done once, using the same trick on the pattern against itself.

---

## 3. What LPS Actually Is

> `lps[i]` = the length of the longest **proper** prefix of `p[0..i]` that is also a **suffix** of `p[0..i]`.

The word *proper* means "not the whole string". Without it the definition is useless: `p[0..i]` is trivially both a prefix and a suffix of itself, so every entry would be `i + 1`.

The array is also called the **prefix function**, written `π`. Same object, different book.

### Hand-computed tables

`"abab"` — `"ab"` opens the string and also closes it.

| i | p[0..i] | longest proper prefix = suffix | lps[i] |
| --- | --- | --- | --- |
| 0 | `a` | — | 0 |
| 1 | `ab` | — | 0 |
| 2 | `aba` | `a` | 1 |
| 3 | `abab` | `ab` | 2 |

→ `[0, 0, 1, 2]`

`"aabaaba"` — the running example for the rest of this note.

| i | p[0..i] | longest proper prefix = suffix | lps[i] |
| --- | --- | --- | --- |
| 0 | `a` | — | 0 |
| 1 | `aa` | `a` | 1 |
| 2 | `aab` | — | 0 |
| 3 | `aaba` | `a` | 1 |
| 4 | `aabaa` | `aa` | 2 |
| 5 | `aabaab` | `aab` | 3 |
| 6 | `aabaaba` | `aaba` | 4 |

→ `[0, 1, 0, 1, 2, 3, 4]`

Three more, two of them at the extremes:

| Pattern | LPS | Why |
| --- | --- | --- |
| `"aaaa"` | `[0, 1, 2, 3]` | maximal self-overlap; every entry is `i` |
| `"abcd"` | `[0, 0, 0, 0]` | all characters distinct — no overlap is possible |
| `"abcabcd"` | `[0, 0, 0, 1, 2, 3, 0]` | builds up to `"abc"`, then the `d` destroys it |

> `lps[0]` is **always** 0. A one-character string has exactly one proper prefix — the empty one.

---

## 4. Building the LPS Array

Build it with two pointers: `i`, the position being filled, and `len`, the length of the prefix–suffix currently matched. The invariant is that `p[0..len-1] == p[i-len..i-1]` — the candidate is always live.

### Algorithm Steps

1. `lps[0] = 0`. Set `len = 0`, `i = 1`.
2. While `i < m`:
   a. If `p[i] == p[len]` — the candidate extends. Set `lps[i] = ++len`, then `i++`.
   b. Else if `len > 0` — the candidate died. Fall back with `len = lps[len-1]`, and **do not advance `i`**.
   c. Else (`len == 0`) — no candidate at all. Set `lps[i] = 0`, then `i++`.

```cpp
// lps[i] = length of the longest proper prefix of p[0..i]
// that is also a suffix of p[0..i].
vector<int> buildLPS(const string& p) {
    int m = p.size();
    vector<int> lps(m, 0);
    int len = 0;   // length of the prefix-suffix matched so far
    int i = 1;     // lps[0] is always 0, so start at 1

    while (i < m) {
        if (p[i] == p[len]) {
            lps[i] = ++len;       // extend the current prefix-suffix by one
            i++;
        } else if (len > 0) {
            len = lps[len - 1];   // fall back; deliberately do NOT advance i
        } else {
            lps[i] = 0;           // no prefix-suffix at all at this position
            i++;
        }
    }
    return lps;
}
```

This is exactly the implementation in [03-index_of_first_occuring_string.cpp](../03-Hard/03-index_of_first_occuring_string.cpp).

### Why the fallback is `len = lps[len-1]`

This is the line everybody's mental model breaks on, so take it slowly.

We are trying to show that `p[0..len]` is a suffix of `p[0..i]`, and `p[i] != p[len]` has just killed that. The next candidate must be **shorter than `len`** — but not just any shorter length. It must still be a prefix of `p` *and* still a suffix of `p[0..i-1]`, because the `i-1` end has not changed.

The strings that are both a prefix of `p` and a suffix of `p[0..i-1]` of length `< len` are exactly the strings that are both a prefix and a suffix of `p[0..len-1]`. Why? Because `p[0..len-1]` **is** the suffix of `p[0..i-1]` of length `len` (that is the invariant). Any shorter suffix of `p[0..i-1]` is therefore a suffix of `p[0..len-1]`.

And the longest proper prefix of `p[0..len-1]` that is also a suffix of it is, by definition, `lps[len-1]`.

> The fallback chain `len → lps[len-1] → lps[lps[len-1]-1] → … → 0` enumerates **all** the prefix-suffixes of `p[0..i-1]`, in strictly decreasing order of length. No candidate is skipped.

### Why `i` must not advance on a fallback

Two separate reasons, and both matter.

**Correctness.** `lps[i]` has not been written yet. Advancing `i` would leave it at its initialised `0`, silently claiming that `p[0..i]` has no prefix-suffix — when a shorter candidate may well succeed. Trace `"aabaaba"` at `i = 2`: the candidate `len = 1` fails, we fall back to `len = 0`, and only *then* write `lps[2] = 0` and move on. Here the answer happened to be 0 anyway. In `"aabaabaaa"` the same fallback at a later index lands on a *non-zero* candidate, and advancing `i` would have lost it.

**Complexity.** The fallback is the only step that decreases `len`. If it also advanced `i`, the loop would no longer be doing "one unit of progress per iteration" and the amortised argument in §7 would collapse. The fallback is paid for by the increments that built `len` up — see §7.

> Writing `i++` in the `else if` branch is the single most common KMP bug. It compiles, it terminates, and it produces wrong arrays on exactly the self-similar patterns KMP exists for.

---

## 5. Correctness Argument

**Claim.** When the loop writes `lps[i]`, the value written is the length of the longest proper prefix of `p[0..i]` that is also a suffix of `p[0..i]`. The loop invariant is that on entry to each iteration, `len` is the length of the longest prefix of `p` that is a suffix of `p[0..i-1]`, and all of `lps[0..i-1]` are already correct.

*Base case.* `lps[0] = 0`, correct by definition (the only proper prefix of a single character is the empty string). We enter the loop with `i = 1` and `len = 0`, and the longest prefix of `p` that is a suffix of `p[0..0]` is indeed the empty string, of length 0. ✓

*Inductive step.* Assume the invariant on entry to an iteration.

- **Match (`p[i] == p[len]`).** Any prefix-suffix of `p[0..i]` of length `L > 0` has, after dropping its last character, a prefix-suffix of `p[0..i-1]` of length `L-1`, and requires `p[i] == p[L-1]`. By hypothesis the longest prefix-suffix of `p[0..i-1]` has length `len`, so `L - 1 <= len`, so `L <= len + 1`. The value `len + 1` is achieved, since `p[0..len-1]` is a suffix of `p[0..i-1]` and `p[i] == p[len]` extends it. Hence `lps[i] = len + 1` is correct, and after `len++` the invariant holds for `i+1`. ✓
- **Fallback (`p[i] != p[len]`, `len > 0`).** No prefix-suffix of `p[0..i]` of length `len + 1` exists. The next candidate length must be the largest `L < len` such that `p[0..L-1]` is a suffix of `p[0..i-1]`. As argued in §4, those `L` are exactly the prefix-suffix lengths of `p[0..len-1]`, whose maximum is `lps[len-1]` — correct by hypothesis, since `len - 1 < i`. So setting `len = lps[len-1]` re-establishes the invariant for the *same* `i` with a strictly smaller `len`, and the iteration repeats. Termination is guaranteed because `len` strictly decreases and is bounded below by 0. ✓
- **No candidate (`p[i] != p[len]`, `len == 0`).** The fallback chain has been exhausted, so no `L >= 1` works: every candidate length was tried and rejected. Hence `lps[i] = 0` is correct, and the invariant holds for `i+1` with `len = 0`. ✓

*Conclusion.* By induction every entry `lps[0..m-1]` is correct when the loop exits at `i = m`. ∎

The search loop inherits this: at a mismatch with `j` matched, the only alignments not yet disproved start at a suffix of the matched block that is also a prefix of `p`, and the longest such has length `lps[j-1]`. Jumping there skips only alignments that provably could not match, so no occurrence is missed.

---

## 6. The Search Loop

### Algorithm Steps

1. If `p` is empty return 0 (convention). If `m > n` return −1.
2. Build `lps` for `p`.
3. Walk `i` over the text and `j` over the pattern, both from 0:
   a. If `t[i] == p[j]`, advance both. If `j == m`, the match starts at `i - m`.
   b. Else if `j > 0`, slide the pattern: `j = lps[j-1]`, and **leave `i` where it is**.
   c. Else advance `i` — nothing was matched, so move the text along.
4. If the scan finishes with no match, return −1.

```cpp
int strStr(const string& t, const string& p) {
    int n = t.size(), m = p.size();
    if (m == 0) return 0;     // empty pattern matches at 0 by convention
    if (m > n)  return -1;    // cannot possibly fit

    vector<int> lps = buildLPS(p);

    int i = 0, j = 0;         // i indexes the text, j indexes the pattern
    while (i < n) {
        if (t[i] == p[j]) {
            i++; j++;
            if (j == m) return i - m;   // consumed the whole pattern
        } else if (j > 0) {
            j = lps[j - 1];             // slide the pattern, i stays put
        } else {
            i++;                        // nothing matched, advance the text
        }
    }
    return -1;
}
```

Note the symmetry: the search loop is the *same shape* as the LPS builder. The builder matches the pattern against itself; the search matches the pattern against the text. `len` and `j` play the same role, and `len = lps[len-1]` and `j = lps[j-1]` are the same move.

---

## 7. Why It Is O(n + m)

The loop body contains no inner loop, so the cost is the number of iterations. Count them with an amortised (potential) argument on `j`.

- **`i` only ever increases**, by 1, and stops at `n`. So the branches that advance `i` (match, and the `j == 0` mismatch) run at most `n` times in total.
- **`j` increases by exactly 1** on a match — and every `j++` is paired with an `i++`. So `j` is incremented at most `n` times over the whole run.
- **`j` strictly decreases on every fallback**, since `lps[j-1] <= j-1 < j`. And `j >= 0` always.

> A quantity cannot be decreased more times than it was increased, if it never goes below its starting value.

`j` starts at 0, is incremented at most `n` times, and is never negative. Therefore the number of fallback iterations is at most `n`. Total iterations ≤ 2n = O(n).

The exact same argument applied to `len` and `i` inside `buildLPS` gives O(m). Together: **O(n + m) time, O(m) space** for the LPS array.

This is why the "do not advance `i` on a fallback" rule is not merely a correctness detail. The argument above works precisely because a fallback consumes stored potential (`j`) rather than making forward progress; every iteration does exactly one of the two.

---

## 8. Variants and Applications

### 8.1 First occurrence (LeetCode 28)

The plain search of §6. → [03-index_of_first_occuring_string.cpp](../03-Hard/03-index_of_first_occuring_string.cpp)

### 8.2 Counting all occurrences (including overlapping)

Change one line: on a full match, do not return — fall back as if it were a mismatch.

```cpp
int countOccurrences(const string& t, const string& p) {
    int n = t.size(), m = p.size();
    if (m == 0 || m > n) return 0;
    vector<int> lps = buildLPS(p);

    int i = 0, j = 0, count = 0;
    while (i < n) {
        if (t[i] == p[j]) {
            i++; j++;
            if (j == m) {
                count++;
                j = lps[j - 1];   // NOT return - keep scanning, allow overlap
            }
        } else if (j > 0) j = lps[j - 1];
        else i++;
    }
    return count;
}
```

`j = lps[m-1]` is exactly right: it keeps the longest suffix of the just-found match that is also a prefix of `p`, which is what allows overlapping occurrences to be found. `p = "aba"`, `t = "ababa"` gives 2 (at 0 and 2), not 1. Setting `j = 0` instead would count only non-overlapping occurrences — a different, also-useful, question.

### 8.3 Longest happy prefix (LeetCode 1392)

> Find the longest prefix of `s` that is also a suffix of `s`, excluding `s` itself.

That is the *definition* of `lps[n-1]`. There is no search at all:

```cpp
string longestPrefix(string s) {
    vector<int> lps = buildLPS(s);
    return s.substr(0, lps[s.size() - 1]);
}
```

For `s = "aabaaba"`, `lps[6] = 4`, so the answer is `"aaba"` — and indeed `"aaba"` both opens and closes the string. → [05-longest_happy_prefix.cpp](../03-Hard/05-longest_happy_prefix.cpp)

### 8.4 The smallest period of a string

> `period = n - lps[n-1]`. The string is a whole number of repetitions of that period **iff** `n % period == 0`.

The intuition: if `p[0..n-1-k]` (a prefix of length `n-k`) equals `p[k..n-1]` (the suffix of the same length), then shifting the string by `k` maps it onto itself, so `k` is a period. Taking the *longest* border `k = lps[n-1]` gives the *smallest* period.

| String | n | lps[n-1] | period | n % period | Repetition? |
| --- | --- | --- | --- | --- | --- |
| `"ababab"` | 6 | 4 | 2 | 0 | ✓ `"ab"` × 3 |
| `"aabaaba"` | 7 | 4 | 3 | 1 | ✗ — it is `"aab"+"aab"+"a"` |
| `"aaaa"` | 4 | 3 | 1 | 0 | ✓ `"a"` × 4 |
| `"abcd"` | 4 | 0 | 4 | 0 | trivially one copy of itself |

`"aabaaba"` is the instructive one: 3 *is* a genuine period (the string is a prefix of `"aab"` repeated forever) but it does not divide 7, so the string is not a whole number of copies.

### 8.5 Repeated substring pattern (LeetCode 459)

A direct corollary of §8.4:

```cpp
bool repeatedSubstringPattern(string s) {
    int n = s.size();
    vector<int> lps = buildLPS(s);
    int period = n - lps[n - 1];
    return lps[n - 1] > 0 && n % period == 0;
}
```

> The `lps[n-1] > 0` guard is not optional. `"abcd"` has `lps[3] = 0`, period 4, and `4 % 4 == 0` — which would wrongly report that a string is a repetition of its whole self. The problem asks for **two or more** copies.

### 8.6 Shortest palindrome (LeetCode 214)

> Prepend the fewest characters to `s` to make it a palindrome.

You must find the longest **palindromic prefix** of `s`; everything after it gets mirrored onto the front. The trick: a prefix of `s` is a palindrome exactly when it is also a suffix of `reverse(s)`. So build the LPS of `s + '#' + reverse(s)` and read its last entry.

```cpp
string shortestPalindrome(string s) {
    int n = s.size();
    if (n == 0) return s;
    string rev(s.rbegin(), s.rend());
    string comb = s + '#' + rev;        // '#' must not occur in s
    vector<int> lps = buildLPS(comb);
    int k = lps.back();                 // length of the longest palindromic prefix
    return rev.substr(0, n - k) + s;
}
```

`s = "abcd"` → `comb = "abcd#dcba"`, whose LPS is `[0,0,0,0,0,0,0,0,1]`. So `k = 1` (`"a"`), and the answer is `"dcb" + "abcd" = "dcbabcd"`. ✓

> **Why the separator is essential.** Take `s = "aaa"`. Without it, `comb = "aaaaaa"` and `lps[5] = 5` — a claimed palindromic prefix of length 5 in a string of length 3. The matched border has run *across the join*. The separator `#`, chosen to appear in neither half, makes any border longer than `n` impossible, because it would have to contain the `#` at a position where there is no `#`. With it, `comb = "aaa#aaa"` gives `lps[6] = 3` — correct, `"aaa"` is already a palindrome.

### 8.7 Repeated string match

Concatenate `a` enough times to cover `b`, then run KMP once. Bounding the number of copies is the real content of the problem. → [04-repeated_string_match.cpp](../03-Hard/04-repeated_string_match.cpp)

---

## 9. Worked Examples

### 9.1 Building the LPS of `"aabaaba"`

`p = a a b a a b a`, `m = 7`. Start `lps = [0,0,0,0,0,0,0]`, `len = 0`, `i = 1`.

| i | len | p[i] | p[len] | Result | Action | lps after | i, len after |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | 0 | `a` | `a` | match | `lps[1] = ++len` | `[0,1,0,0,0,0,0]` | 2, 1 |
| 2 | 1 | `b` | `a` | mismatch, `len > 0` | **`len = lps[0] = 0`** (i stays) | unchanged | 2, 0 |
| 2 | 0 | `b` | `a` | mismatch, `len == 0` | `lps[2] = 0` | `[0,1,0,0,0,0,0]` | 3, 0 |
| 3 | 0 | `a` | `a` | match | `lps[3] = ++len` | `[0,1,0,1,0,0,0]` | 4, 1 |
| 4 | 1 | `a` | `a` | match | `lps[4] = ++len` | `[0,1,0,1,2,0,0]` | 5, 2 |
| 5 | 2 | `b` | `b` | match | `lps[5] = ++len` | `[0,1,0,1,2,3,0]` | 6, 3 |
| 6 | 3 | `a` | `a` | match | `lps[6] = ++len` | `[0,1,0,1,2,3,4]` | 7, 4 |

**Final LPS = `[0, 1, 0, 1, 2, 3, 4]`**

```
p:    a  a  b  a  a  b  a
lps:  0  1  0  1  2  3  4
```

Row 2 is the whole reason this pattern was chosen. Index 2 is visited **twice**: once to fall back from `len = 1` to `len = 0`, once to write `lps[2] = 0`. Had `i` advanced on that fallback, `lps[2]` would never have been considered and the builder would have gone out of step for the rest of the array.

### 9.2 Searching `t = "aabaacaabaaba"` for `p = "aabaaba"`

```
t index:  0  1  2  3  4  5  6  7  8  9 10 11 12
t char:   a  a  b  a  a  c  a  a  b  a  a  b  a
p:        a  a  b  a  a  b  a
lps:      0  1  0  1  2  3  4
```

| i | j | t[i] | p[j] | Result | Action | i, j after |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | 0 | `a` | `a` | match | `i++, j++` | 1, 1 |
| 1 | 1 | `a` | `a` | match | `i++, j++` | 2, 2 |
| 2 | 2 | `b` | `b` | match | `i++, j++` | 3, 3 |
| 3 | 3 | `a` | `a` | match | `i++, j++` | 4, 4 |
| 4 | 4 | `a` | `a` | match | `i++, j++` | 5, 5 |
| 5 | 5 | `c` | `b` | mismatch | `j = lps[4] = 2` | **5**, 2 |
| 5 | 2 | `c` | `b` | mismatch | `j = lps[1] = 1` | **5**, 1 |
| 5 | 1 | `c` | `a` | mismatch | `j = lps[0] = 0` | **5**, 0 |
| 5 | 0 | `c` | `a` | mismatch, `j == 0` | `i++` | 6, 0 |
| 6 | 0 | `a` | `a` | match | `i++, j++` | 7, 1 |
| 7 | 1 | `a` | `a` | match | `i++, j++` | 8, 2 |
| 8 | 2 | `b` | `b` | match | `i++, j++` | 9, 3 |
| 9 | 3 | `a` | `a` | match | `i++, j++` | 10, 4 |
| 10 | 4 | `a` | `a` | match | `i++, j++` | 11, 5 |
| 11 | 5 | `b` | `b` | match | `i++, j++` | 12, 6 |
| 12 | 6 | `a` | `a` | match, `j == m` | **return `13 - 7 = 6`** | — |

`i` is **never** decremented anywhere in the table. Index 5 is examined four times in a row, but only because `j` is shrinking each time — and by the amortised argument those four visits were paid for by the five increments that preceded them.

Read the first fallback out loud: we had matched `"aabaa"`; its longest border is `"aa"`; so the pattern slides right by 3 and its `p[0..1] = "aa"` now sits over `t[3..4] = "aa"`, which we know already matches. Comparison resumes at `p[2]` versus the *same* `t[5]`.

### 9.3 Counting overlapping occurrences: `p = "aba"`, `t = "ababa"`

`lps("aba") = [0, 0, 1]`.

| i | j | t[i] | p[j] | Result | Action | i, j after |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | 0 | `a` | `a` | match | `i++, j++` | 1, 1 |
| 1 | 1 | `b` | `b` | match | `i++, j++` | 2, 2 |
| 2 | 2 | `a` | `a` | match, `j == m` | **count at 0**; `j = lps[2] = 1` | 3, 1 |
| 3 | 1 | `b` | `b` | match | `i++, j++` | 4, 2 |
| 4 | 2 | `a` | `a` | match, `j == m` | **count at 2**; `j = lps[2] = 1` | 5, 1 |

**Answer: 2**, at indices 0 and 2. Resetting `j = 0` instead would have produced 1 — the non-overlapping count.

---

## 10. Common Pitfalls

1. **`lps[j]` instead of `lps[j-1]`.** The fallback uses the border of the *matched* block `p[0..j-1]`, whose last index is `j-1`. Writing `lps[j]` reads the entry for a character that has not matched — and at `j == m` it reads out of bounds.
2. **Advancing `i` on a fallback.** Breaks correctness (the entry at `i` is never written) *and* the O(n + m) bound. See §4 and §7.
3. **Forgetting that `lps[0] = 0` always.** Start the build loop at `i = 1`, never 0. Starting at 0 compares `p[0]` with `p[0]`, always matches, and produces `lps[0] = 1` — a whole-string border, which the "proper" requirement forbids.
4. **Confusing prefix-suffix with palindrome.** A border is a prefix that *equals* a suffix; it has nothing to do with reading the same backwards. `"abab"` has border `"ab"` and is not a palindrome. Shortest palindrome (§8.6) only becomes an LPS problem *after* reversing one copy.
5. **Returning instead of continuing when counting all occurrences.** §8.2. And within that, `j = 0` versus `j = lps[m-1]` is the overlapping/non-overlapping distinction — decide which the problem wants.
6. **Empty pattern.** Guard it. `buildLPS("")` returns an empty vector and `lps[n-1]` then indexes `lps[-1]`. LeetCode 28 wants 0 for an empty needle; the period formulas are undefined for `n = 0`.
7. **Missing the `lps[n-1] > 0` guard in LC 459.** §8.5.
8. **Omitting the separator in the shortest-palindrome trick.** §8.6 — the border runs across the join and exceeds `n`.
9. **A separator that can occur in the input.** `'#'` is fine for lowercase-letter constraints; for arbitrary bytes, pick a character outside the alphabet or track the length cap explicitly.
10. **Reaching for KMP when `string::find` would do.** In production, `find` is usually the right call; KMP is for when the worst case actually bites, when the LPS array itself is the answer (§8.3–8.5), or when the interviewer forbids built-ins.

---

## 11. Related Problems in This Module

| Problem | KMP / LPS connection | File |
| --- | --- | --- |
| Index of first occurrence (LC 28) | KMP search, textbook form | [03-Hard/03](../03-Hard/03-index_of_first_occuring_string.cpp) |
| Longest happy prefix (LC 1392) | `s.substr(0, lps[n-1])` — no search at all | [03-Hard/05](../03-Hard/05-longest_happy_prefix.cpp) |
| Repeated string match | Concatenate, then one KMP scan | [03-Hard/04](../03-Hard/04-repeated_string_match.cpp) |
| Rotate string | `s` is a rotation of `g` iff `s` is a substring of `g + g` | [01-Basic/06](../01-Basic/06-rotate_string.cpp) |
| Longest common prefix | Prefix reasoning, but across strings rather than within one | [01-Basic/04](../01-Basic/04-longest_common_prefix.cpp) |
| Longest palindromic substring | Expand-around-centre; the Manacher upgrade uses the same "reuse what you know" idea | [02-Medium/07](../02-Medium/07-longest_palindromic_substring.cpp) |

See also: [../../Module%2003-Arrays/notes/03-Kadanes-Algorithm.md](../../Module%2003-Arrays/notes/03-Kadanes-Algorithm.md) for the sibling "precompute a running state" pattern, and [../../C%2B%2B%20Data%20Structures/02-CPP-String-Reference.md](../../C%2B%2B%20Data%20Structures/02-CPP-String-Reference.md) for `substr`, `rbegin`, and the rest of the `std::string` surface used above.

---

## Cheat Sheet

```cpp
// Build the LPS / prefix function - memorise this shape
vector<int> lps(m, 0);
for (int i = 1, len = 0; i < m; ) {
    if      (p[i] == p[len]) lps[i++] = ++len;
    else if (len > 0)        len = lps[len - 1];   // i does NOT move
    else                     lps[i++] = 0;
}

// Search - the same shape, pattern against text
for (int i = 0, j = 0; i < n; ) {
    if      (t[i] == p[j]) { i++; if (++j == m) { /* hit at i-m */ j = lps[j-1]; } }
    else if (j > 0)        j = lps[j - 1];         // i does NOT move
    else                   i++;
}
```

| Question | Answer from the LPS array |
| --- | --- |
| Longest prefix that is also a suffix | `lps[n-1]` |
| Smallest period | `n - lps[n-1]` |
| Is the string `k >= 2` repetitions? | `lps[n-1] > 0 && n % (n - lps[n-1]) == 0` |
| First occurrence | search; return `i - m` on `j == m` |
| Count all (overlapping) | search; on `j == m` do `count++, j = lps[m-1]` |
| Count all (non-overlapping) | search; on `j == m` do `count++, j = 0` |
| Longest palindromic prefix of `s` | `lps` of `s + '#' + reverse(s)`, take `.back()` |
| Shortest palindrome | `reverse(s).substr(0, n - k) + s`, `k` as above |

| Complexity | Value |
| --- | --- |
| Build LPS | O(m) time, O(m) space |
| Search | O(n) time, O(1) extra |
| Total | **O(n + m) time, O(m) space** |
