# Interval Problems — Merge, Insert, Select, Count

> The input is a bag of `[start, end]` ranges; the answer is almost always "sort, then make one left-to-right pass".
> The whole art is picking the sort key: **sort by START to merge, sort by END to select** — and everything is `O(n log n)`, dominated by that sort.

---

## Table of Contents

1. [What an Interval Problem Is](#1-what-an-interval-problem-is)
2. [Boundary Conventions — Closed vs Half-Open](#2-boundary-conventions--closed-vs-half-open)
3. [The Sort-Key Decision](#3-the-sort-key-decision)
4. [Merging Overlapping Intervals](#4-merging-overlapping-intervals)
5. [Inserting Into a Sorted List](#5-inserting-into-a-sorted-list)
6. [Selecting and Removing — The Exchange Argument](#6-selecting-and-removing--the-exchange-argument)
7. [Counting Maximum Overlap — Sweep Line](#7-counting-maximum-overlap--sweep-line)
8. [The Difference-Array Alternative](#8-the-difference-array-alternative)
9. [Other Shapes in the Family](#9-other-shapes-in-the-family)
10. [Worked Traces](#10-worked-traces)
11. [Common Pitfalls](#11-common-pitfalls)
12. [Cheat Sheet](#12-cheat-sheet)

---

## 1. What an Interval Problem Is

An interval is a pair `(start, end)` with `start <= end`, drawn on a number line as a bar:

```
        0   1   2   3   4   5   6   7   8   9  10
        |   |   |   |   |   |   |   |   |   |   |
A           [-----------]                            A = [1,4]
B                   [-----------]                    B = [3,6]
C                                   [-------]        C = [7,9]
```

`A` and `B` overlap, `C` is disjoint from both. Every problem in this family is one of four questions about a picture like that:

| Question | Name | Answer shape |
| --- | --- | --- |
| Which bars fuse into one? | **merge** | a shorter list of intervals |
| Where does a new bar go? | **insert** | a shorter list of intervals |
| What is the biggest disjoint subset? | **select / remove** | a count |
| How many bars stack at the busiest instant? | **count overlap** | a count |

The representation in this repo is `vector<vector<int>>` (LeetCode's shape, `iv[0]` = start, `iv[1]` = end) or `vector<pair<int,int>>` (`.first` / `.second`). The pair version is cheaper — one allocation instead of `n` — and sorts lexicographically by start for free; the vector-of-vector version also sorts lexicographically, which is why `sort(arr.begin(), arr.end())` with no comparator is already "sort by start, tie-break by end". See [../../C%2B%2B%20Data%20Structures/04-CPP-Pair-Reference.md](../../C%2B%2B%20Data%20Structures/04-CPP-Pair-Reference.md).

**Why sorting is always step one.** Unsorted, deciding whether interval `i` interacts with interval `j` is an all-pairs question — `O(n²)`. Sorting imposes an order in which *locality* holds: after the right sort, the only thing you ever need to compare the current interval against is **one** remembered value (the last emitted end, or the last kept end, or a running counter). That collapses `O(n²)` to `O(n)` for the pass, leaving the sort as the dominant cost.

> Every algorithm below is `O(n log n)` time. If the input is *already* sorted — as it is in [09-insert_interval.cpp](../02-Medium/09-insert_interval.cpp) — the pass alone is `O(n)`, and that is the whole reason that problem is worth stating separately.

---

## 2. Boundary Conventions — Closed vs Half-Open

**The single most common source of wrong answers in this family.** Before writing a comparison, answer: *do `[1,2]` and `[2,3]` overlap?*

```
        1       2       3
        |       |       |
        [-------]                 [1,2]
                [-------]         [2,3]
                ^
            they share exactly the point 2
```

| Convention | Written | Do `[1,2]`,`[2,3]` overlap? | Mental model |
| --- | --- | --- | --- |
| **Closed** `[a, b]` | both ends included | **yes** — they share the point `2` | a train *occupies* the platform at its departure minute |
| **Half-open** `[a, b)` | end excluded | **no** — `[1,2)` stops just short of `2` | a meeting *frees* the room at its end time |

### 2.1 The overlap predicate

Two intervals `A`, `B` overlap iff the later start comes before the earlier end:

```cpp
int lo = max(A[0], B[0]), hi = min(A[1], B[1]);
bool overlapHalfOpen = lo <  hi;   // [a,b): touching is NOT overlap
bool overlapClosed   = lo <= hi;   // [a,b]: touching IS overlap
// when they do overlap, [lo, hi] is the intersection itself
```

Check it on the picture: `A=[1,2]`, `B=[2,3]` ⇒ `lo = 2`, `hi = 2`. Half-open says `2 < 2` false; closed says `2 <= 2` true. One character.

### 2.2 The same code, one character apart

Every algorithm in this note has exactly one comparison that encodes the convention:

| Algorithm | Half-open (touching is fine) | Closed (touching conflicts) |
| --- | --- | --- |
| merge (§4) | `if (back[1] > cur[0]) merge` | `if (back[1] >= cur[0]) merge` |
| select / remove (§6) | `if (cur[0] >= last) keep` | `if (cur[0] > last) keep` |
| sweep (§7) | departure before arrival at ties | arrival before departure at ties |

### 2.3 What this repo's files actually use

| File | Convention | The line |
| --- | --- | --- |
| [../../Module%2003-Arrays/03-Hard/07-merge_intervals.cpp](../../Module%2003-Arrays/03-Hard/07-merge_intervals.cpp) | **closed** — touching merges | `list.back()[1] >= arr[i][0]` |
| [09-insert_interval.cpp](../02-Medium/09-insert_interval.cpp) | **closed** — touching merges | `arr[i][1] < interval[0]` / `arr[i][0] > interval[1]` |
| [10-remove_overlapping_intervals.cpp](../02-Medium/10-remove_overlapping_intervals.cpp) | **half-open** — touching is fine | `if (arr[i][0] >= last) keep` |
| [01-activity_selection.cpp](../02-Medium/01-activity_selection.cpp) | **closed** — touching conflicts | `if (lastFinish < v[i].first) accept` |
| [04-minimum_platforms.cpp](../02-Medium/04-minimum_platforms.cpp) | **closed** — touching conflicts | `if (arr[i] <= dep[j]) p++` |

> Note the trap sitting in plain sight: [01-activity_selection.cpp](../02-Medium/01-activity_selection.cpp) and [10-remove_overlapping_intervals.cpp](../02-Medium/10-remove_overlapping_intervals.cpp) are the *same greedy* (see §6) but disagree on touching. On `[1,2],[2,3],[3,4]` the activity-selection code keeps **2** activities, while LeetCode 435 keeps all **3** and removes 0. Neither is buggy — they answer different problem statements. Always re-read the statement's wording on endpoints before trusting a remembered template.

---

## 3. The Sort-Key Decision

This is the spine of the note. Three tasks, three keys, three reasons.

| Task | Sort by | Why that key | Cost |
| --- | --- | --- | --- |
| **Merge** overlapping intervals | **start**, ascending | makes overlapping intervals *adjacent*, so the current one can only ever interact with the last one emitted | `O(n log n)` |
| **Select** max disjoint / **remove** min | **end**, ascending | the earliest finisher leaves the most room for the future — the greedy choice is always safe (§6) | `O(n log n)` |
| **Count** max simultaneous overlap | neither — **decouple the endpoints into `+1` / `-1` events** | occupancy at an instant depends only on *how many* starts and ends precede it, never on which start pairs with which end | `O(n log n)` |

### 3.1 Sorting by start fails for selection

Intervals `[0,10]`, `[1,2]`, `[3,4]` — already in start order.

```
        0   1   2   3   4   5   6   7   8   9  10
        [-----------------------------------------]   [0,10]
            [---]                                      [1,2]
                    [---]                              [3,4]
```

Greedy on start order takes `[0,10]` first (nothing kept yet, so it is always accepted), then rejects `[1,2]` (`1 >= 10` false) and `[3,4]` (`3 >= 10` false) ⇒ **1 kept**. The optimum is `[1,2]` and `[3,4]` ⇒ **2**. Sorting by end gives `[1,2](2), [3,4](4), [0,10](10)`: keep `[1,2]` (`last=2`), keep `[3,4]` (`3 >= 2`, `last=4`), reject `[0,10]` (`0 >= 4` false) ⇒ **2**. ✓

The lesson: an early *start* tells you nothing about how much of the future an interval devours. An early *end* does.

### 3.2 Sorting by end fails for merging

Intervals `[1,10]`, `[2,3]`, `[4,5]`. Sorted by end: `[2,3], [4,5], [1,10]`.

The merge pass compares each interval only against the last one emitted:

- emit `[2,3]`
- `[4,5]`: `4 > 3`, no overlap with the last ⇒ emit `[4,5]` as new
- `[1,10]`: `1 <= 5` ⇒ merge into the last ⇒ `[4,10]`

Result `[[2,3],[4,10]]` — wrong twice over: the true answer is the single interval `[1,10]`, and `[2,3]` was stranded as a separate output even though `[1,10]` swallows it. The comparison against `back()` is only sound when starts are non-decreasing, because only then does "does not overlap `back`" imply "does not overlap anything already emitted".

> The two halves of the rule are not interchangeable folklore — each key is *required* by the locality argument of its own algorithm, and each fails concretely under the other's task.

---

## 4. Merging Overlapping Intervals

Sort by start; keep a result list; each interval either extends the last entry or starts a new one.

```cpp
sort(arr.begin(), arr.end());                   // lexicographic == by start
vector<vector<int>> out;
for (auto& iv : arr) {
    if (!out.empty() && out.back()[1] >= iv[0])          // closed: touching merges
        out.back()[1] = max(out.back()[1], iv[1]);       // <- max, not assignment
    else
        out.push_back(iv);
}
```

**Why comparing against only `out.back()` is enough.** After sorting, starts are non-decreasing. `out.back()` is the most recently emitted interval, and every *earlier* emitted interval ends no later than it does (otherwise they would have been merged). So if `iv` does not reach `out.back()`'s end, it reaches none of the earlier ends either. One comparison per interval ⇒ `O(n)` pass.

**Why `max` and not assignment — the nesting case.** Take `[1,10]` and `[2,3]`:

```
        1   2   3   4   5   6   7   8   9  10
        [-----------------------------------]      back = [1,10]
            [-------]                              iv   = [2,3]
        [-----------------------------------]      merged = [1, max(10,3)] = [1,10]
                                                            NOT [1,3]
```

`[2,3]` is fully **contained** in `[1,10]`. Writing `out.back()[1] = iv[1]` would *shrink* the result to `[1,3]` and silently lose the tail. Sorting guarantees the start is already minimal (`iv[0] >= back[0]`), so only the end can move — but it can only move **outward**. This is the single most common merge bug.

→ [../../Module%2003-Arrays/03-Hard/07-merge_intervals.cpp](../../Module%2003-Arrays/03-Hard/07-merge_intervals.cpp)

**Complexity:** `O(n log n)` time (the sort), `O(n)` for the output, `O(1)` auxiliary.

---

## 5. Inserting Into a Sorted List

Now the input is *already* sorted **and already non-overlapping**, and one new interval must go in. Two extra guarantees change what is achievable.

The naive route — `push_back` then run §4 — is correct but `O(n log n)`, and throws away both guarantees. The single pass is `O(n)`. With the list sorted and disjoint, every existing interval falls into exactly one of three buckets relative to the new one:

```
                          new = [4,8]
                          [---------------]
    [---]                                        1. BEFORE   end < new.start
          [-------]                              3. OVERLAP  (absorb)
                                [-------]        3. OVERLAP  (absorb)
                                                 2. AFTER    start > new.end
                                           [--]
```

```cpp
vector<vector<int>> res;
for (int i = 0; i < (int)arr.size(); i++) {
    if (arr[i][1] < nw[0]) {                 // 1. entirely before
        res.push_back(arr[i]);
    } else if (arr[i][0] > nw[1]) {          // 2. entirely after -> done
        res.push_back(nw);
        while (i < (int)arr.size()) res.push_back(arr[i++]);
        return res;
    } else {                                 // 3. overlaps -> absorb in place
        nw[0] = min(nw[0], arr[i][0]);
        nw[1] = max(nw[1], arr[i][1]);
    }
}
res.push_back(nw);                           // new interval belongs at the end
return res;
```

Three things carry the correctness:

1. **The three cases are exhaustive and mutually exclusive** — an interval either ends before the new one starts, starts after the new one ends, or intersects it. There is no fourth possibility.
2. **The new interval is mutated in place as it absorbs.** By the time a later interval is tested, `nw` already reflects everything swallowed so far — so a chain of absorptions cascades correctly (`[4,8]` grows to `[3,8]` then `[3,10]` in §10.2).
3. **Case 2 can `return` immediately.** Once some interval starts after the *finalised* `nw` ends, sorted order means every later interval starts later still, so none can overlap `nw`; and they were already pairwise disjoint among themselves, so they are copied verbatim.

The outer `for` and the inner `while` between them visit each index **once** — the `while` only ever runs on indices the `for` has not reached, and the function returns immediately after. So it is `O(n)`, not `O(n²)`.

→ [09-insert_interval.cpp](../02-Medium/09-insert_interval.cpp)

> `nw` is taken by non-const reference in that file and is genuinely modified. If you need the caller's interval intact, copy it in first.

---

## 6. Selecting and Removing — The Exchange Argument

Two problems that are the same problem:

| Problem | Asks for | Repo file |
| --- | --- | --- |
| Activity selection | **maximise** the number of pairwise-disjoint intervals kept | [01-activity_selection.cpp](../02-Medium/01-activity_selection.cpp) |
| LeetCode 435, non-overlapping intervals | **minimise** the number removed | [10-remove_overlapping_intervals.cpp](../02-Medium/10-remove_overlapping_intervals.cpp) |

```
minRemovals = n - maxKept
```

Removing an interval and not keeping it are the same act, and every interval is either kept or removed — so the two counts sum to `n`. Solve either and you have both. **The identity only holds under a shared boundary convention**: on `[1,2],[2,3],[3,4]` LeetCode 435 (half-open) gets `maxKept = 3, removals = 0`, while the strict-`<` activity-selection code gets `maxKept = 2` — which under *its own* convention would mean 2 removals. Fix the convention first, then the arithmetic is exact.

### 6.1 The greedy

```cpp
sort(arr.begin(), arr.end(),
     [](const vector<int>& a, const vector<int>& b){ return a[1] < b[1]; });  // by END
int last = arr[0][1], kept = 1, removed = 0;
for (int i = 1; i < n; i++) {
    if (arr[i][0] >= last) { last = arr[i][1]; kept++; }   // compatible -> keep
    else                     removed++;                     // conflicts  -> drop
}
```

Note what is *not* here: no backtracking, no reconsidering. `last` moves forward only, so the pass is `O(n)` and the sort dominates. Space is `O(1)`.

### 6.2 The exchange argument (why earliest-finish-first is optimal)

Work in the half-open convention with non-empty intervals (`start < end`), so "compatible" means `end(P) <= start(Q)` or `end(Q) <= start(P)`.

Sort by end and call the intervals `I₁, I₂, …, Iₙ` with `end(I₁) <= end(I₂) <= …`.

**Claim.** Some maximum-size compatible subset contains `I₁`.

**Proof.** Let `OPT` be any maximum-size compatible subset, and let `X ∈ OPT` be its earliest-*ending* member.

*Step 1 — every other member of `OPT` starts at or after `end(X)`.* Take any `Y ∈ OPT`, `Y ≠ X`. They are compatible, so either `end(X) <= start(Y)` (what we want) or `end(Y) <= start(X)`. Suppose the second. Since `X` is the earliest finisher in `OPT`, `end(X) <= end(Y)`. Chaining: `end(X) <= end(Y) <= start(X) < end(X)` — a contradiction. So `end(X) <= start(Y)` for every `Y`.

*Step 2 — swap `I₁` in for `X`.* `I₁` has the globally smallest end, so `end(I₁) <= end(X)`. Combined with Step 1, `end(I₁) <= end(X) <= start(Y)` for every other `Y ∈ OPT`, so `I₁` is compatible with all of them. Therefore

```
OPT' = (OPT \ {X}) ∪ {I₁}
```

is compatible and `|OPT'| = |OPT|` — still optimal, and it contains `I₁`. ∎

*Step 3 — induct.* Having committed to `I₁`, every remaining interval that starts before `end(I₁)` is now unusable, and the residual problem is the same problem on the intervals that start at or after `end(I₁)`. The claim applies again to that subproblem, and so on. The greedy makes exactly this sequence of choices, so it produces an optimal solution. ∎

**The intuition in one line:** among the candidates still available, the one that finishes earliest never blocks more of the future than any alternative — it leaves a superset of the room. Nothing about *how long* an interval is, or *when it started*, carries that property, which is precisely why §3.1's start-sorted greedy fails.

> The same exchange-argument skeleton — "assume an optimal solution, swap in the greedy choice, show the swap costs nothing, induct" — is the standard tool for every greedy proof. The general treatment of picking a sort key and proving it lives in [01-Scheduling-And-Sequencing.md](01-Scheduling-And-Sequencing.md); this section is the interval instance.

---

## 7. Counting Maximum Overlap — Sweep Line

"How many platforms / meeting rooms / CPUs do I need?" is asking for the **peak simultaneous overlap**.

**Why peak occupancy is exactly the answer.** One resource serves one interval at a time, so at the instant when `k` intervals are live you need at least `k` resources — the peak is a lower bound. It is also achievable: hand each arriving interval any free resource, which must exist if you provisioned the peak, since the number in use never exceeds it. Lower bound met ⇒ equality.

### 7.1 The decoupling insight

Do **not** sort intervals. Sort the two endpoint streams independently:

```
arr sorted:   900   940   950  1100  1500  1800      each is a  +1  event
dep sorted:   910  1120  1130  1200  1900  2000      each is a  -1  event
```

This looks alarming — after sorting, `arr[k]` and `dep[k]` no longer describe the same train. **It does not matter.** Occupancy at instant `t` is

```
live(t) = (# starts <= t)  -  (# ends < t)
```

and both terms depend only on the *multisets* of start times and end times, never on the pairing. Since every interval's end is at or after its own start, the running counter is a genuine occupancy count at every step.

```cpp
sort(arr.begin(), arr.end());
sort(dep.begin(), dep.end());
int res = 0, live = 0, i = 0, j = 0;
while (i < n && j < m) {
    if (arr[i] <= dep[j]) { live++; i++; }   // closed: arrival wins the tie
    else                  { live--; j++; }
    res = max(res, live);
}
return res;
```

The loop stops when arrivals run out: after the last `+1`, only `-1`s remain, so `live` can only fall and the peak is already decided.

**The tie-break is the boundary convention** (§2). `arr[i] <= dep[j]` processes an arrival *before* a same-timestamp departure — the counter goes up before it comes down, so both intervals are counted and both need a resource. That is the **closed** convention, used by [04-minimum_platforms.cpp](../02-Medium/04-minimum_platforms.cpp). Flipping to `<` would let a departure free the resource in time, i.e. half-open `[arr, dep)`.

### 7.2 The generic event form

When the intervals arrive as pairs rather than two parallel arrays:

```cpp
vector<pair<int,int>> ev;                       // (time, delta)
for (auto& iv : a) { ev.push_back({iv[0], +1}); ev.push_back({iv[1], -1}); }
sort(ev.begin(), ev.end());                     // ties: -1 sorts before +1
int live = 0, best = 0;
for (auto& e : ev) best = max(best, live += e.second);
```

> Careful: the default `pair` sort compares `.second` on a tie, and `-1 < +1`, so **a plain `sort` gives you half-open semantics** (departures first). For closed semantics — where touching intervals both count — sort by `(time, -delta)` or push `+1` as the smaller tag. This is the sweep-line version of the exact same one-character decision as §2.2.

**Complexity:** `O(n log n)` time from the sorts, `O(n)` space for the event list (`O(1)` for the two-pointer form on two parallel arrays).

→ [04-minimum_platforms.cpp](../02-Medium/04-minimum_platforms.cpp)

---

## 8. The Difference-Array Alternative

When times come from a **small, bounded, integer** range, skip sorting entirely. Mark `+1` at each start and `-1` just past each end, then take a running prefix sum:

```cpp
vector<int> delta(T + 2, 0);                    // T = largest possible timestamp
for (auto& iv : a) { delta[iv[0]]++; delta[iv[1] + 1]--; }   // closed [start, end]
int live = 0, best = 0;
for (int t = 0; t <= T + 1; t++) best = max(best, live += delta[t]);
```

The `+1` on the decrement index is the closed convention again: the interval is still live *at* `iv[1]`, and only stops being live at `iv[1] + 1`. Drop the `+1` and you get half-open.

| | two-pointer sweep (§7) | difference array (§8) |
| --- | --- | --- |
| Time | `O(n log n)` | `O(n + T)` |
| Space | `O(1)` / `O(n)` | `O(T)` |
| Timestamps may be sparse / huge / 64-bit | yes | no — needs an array per tick |
| Also answers "occupancy at time `t`" for every `t` | no | yes, for free |

For a 24-hour clock (`T = 2359`) the difference array wins outright when `n` is large. For arbitrary timestamps it is unusable, which is why the sweep is the default. If the range is huge but `n` is small, **coordinate-compress** the endpoints first and you are back to §7.2.

---

## 9. Other Shapes in the Family

Brief, because each is one of the above wearing a costume.

**Meeting Rooms I — "can one person attend all meetings?"** Sort by start, then check adjacent pairs only. If any meeting starts before its predecessor ends, the answer is no; and if none does, no *non-adjacent* pair can conflict either, since starts are non-decreasing.

```cpp
sort(a.begin(), a.end());
for (int i = 1; i < n; i++) if (a[i][0] < a[i-1][1]) return false;
return true;
```

**Meeting Rooms II — "how many rooms?"** Exactly §7 with the half-open convention (a room frees at the end time). The min-heap formulation you often see — push each end time, pop every end `<= cur.start`, answer is the max heap size — is the same sweep with the events discovered lazily; `O(n log n)` either way.

**Interval list intersections** (two lists, each already sorted and disjoint). Two pointers, and the overlap predicate of §2.1 verbatim:

```cpp
int i = 0, j = 0;
while (i < A.size() && j < B.size()) {
    int lo = max(A[i][0], B[j][0]), hi = min(A[i][1], B[j][1]);
    if (lo <= hi) res.push_back({lo, hi});              // closed
    if (A[i][1] < B[j][1]) i++; else j++;               // retire the earlier finisher
}
```

`O(n + m)`, no sort needed. Advancing whichever interval ends first is safe: it can never intersect anything further along the other list, because everything further along starts at or after the current one's start and the retired interval already ends before the current one's end.

**Employee free time / gaps** — merge everything (§4), then the gaps between consecutive merged intervals are the free time.

---

## 10. Worked Traces

### 10.1 Merge — `[[1,3],[2,6],[8,10],[15,18]]`

Already in start order.

```
        1   2   3   4   5   6   7   8   9  10 ... 15 ... 18
        [-------]                                              [1,3]
            [-------------------]                              [2,6]
                                    [-----------]              [8,10]
                                                   [---------] [15,18]
```

| i | `arr[i]` | `out.back()` | test `back[1] >= iv[0]` | action | `out` after |
| --- | --- | --- | --- | --- | --- |
| 0 | `[1,3]` | — (empty) | not evaluated | push | `[1,3]` |
| 1 | `[2,6]` | `[1,3]` | `3 >= 2` true | `end = max(3,6) = 6` | `[1,6]` |
| 2 | `[8,10]` | `[1,6]` | `6 >= 8` false | push | `[1,6],[8,10]` |
| 3 | `[15,18]` | `[8,10]` | `10 >= 15` false | push | `[1,6],[8,10],[15,18]` |

**Answer: `[[1,6],[8,10],[15,18]]`.** Note step 0: `out.empty()` short-circuits the `&&`, so `out.back()` is never called on an empty vector — that guard is the only thing preventing undefined behaviour.

### 10.2 Insert — `arr = [[1,2],[3,5],[6,7],[8,10],[12,16]]`, `nw = [4,8]`

```
        1 2 3 4 5 6 7 8 9 10  12    16
        [-]                                  [1,2]    before
            [---]                            [3,5]    overlap
                [-]                          [6,7]    overlap
                  [---]                      [8,10]   overlap (touches at 8)
                            [-------]        [12,16]  after
            [-----------]                    nw = [4,8]
```

| i | `arr[i]` | `end < nw[0]`? | `start > nw[1]`? | case | `nw` after | `res` after |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | `[1,2]` | `2 < 4` yes | — | before | `[4,8]` | `[1,2]` |
| 1 | `[3,5]` | `5 < 4` no | `3 > 8` no | overlap | `[3,8]` | `[1,2]` |
| 2 | `[6,7]` | `7 < 3` no | `6 > 8` no | overlap | `[3,8]` | `[1,2]` |
| 3 | `[8,10]` | `10 < 3` no | `8 > 8` no | overlap | `[3,10]` | `[1,2]` |
| 4 | `[12,16]` | `16 < 3` no | `12 > 10` **yes** | after | `[3,10]` | `[1,2],[3,10],[12,16]` → return |

**Answer: `[[1,2],[3,10],[12,16]]`.** Watch row 3: `8 > 8` is false, so the touching `[8,10]` is absorbed — the closed convention. Watch row 2: `[6,7]` is contained in the growing `[3,8]` and correctly changes nothing (`min(3,6)=3`, `max(8,7)=8`).

### 10.3 Remove overlapping — `[[1,2],[2,3],[3,4],[1,3]]`

Sorted by end (`2, 3, 3, 4`; the two `end = 3` intervals tie and keep input order):

```
index:     0      1      2      3
         [1,2]  [2,3]  [1,3]  [3,4]
 end:      2      3      3      4

        1   2   3   4
        [-------]                    [1,2]
            [-------]                [2,3]
        [---------------]            [1,3]   <- the troublemaker
                [-------]            [3,4]
```

| i | `arr[i]` | `last` before | `start >= last`? | action | `last` after | `removed` |
| --- | --- | --- | --- | --- | --- | --- |
| — | `[1,2]` | — | — | kept (init) | 2 | 0 |
| 1 | `[2,3]` | 2 | `2 >= 2` yes | keep | 3 | 0 |
| 2 | `[1,3]` | 3 | `1 >= 3` no | **drop** | 3 | 1 |
| 3 | `[3,4]` | 3 | `3 >= 3` yes | keep | 4 | 1 |

**Answer: 1.** Kept chain `[1,2] → [2,3] → [3,4]`; `maxKept = 3`, `n = 4`, and `4 - 3 = 1` ✓ (§6). Under the *closed* convention (`>` instead of `>=`) the same input would keep only `[1,2]` and `[3,4]` and report 2 removals — the boundary convention changes the answer, not just the code.

### 10.4 Peak overlap — `arr = [900,940,950,1100,1500,1800]`, `dep = [910,1200,1120,1130,1900,2000]`

Sorted independently: `arr = [900,940,950,1100,1500,1800]`, `dep = [910,1120,1130,1200,1900,2000]`.

| `i` | `j` | `arr[i]` | `dep[j]` | `arr <= dep`? | event | `live` | `best` |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 0 | 0 | 900 | 910 | yes | arrive | 1 | 1 |
| 1 | 0 | 940 | 910 | no | depart | 0 | 1 |
| 1 | 1 | 940 | 1120 | yes | arrive | 1 | 1 |
| 2 | 1 | 950 | 1120 | yes | arrive | 2 | 2 |
| 3 | 1 | 1100 | 1120 | yes | arrive | **3** | **3** |
| 4 | 1 | 1500 | 1120 | no | depart | 2 | 3 |
| 4 | 2 | 1500 | 1130 | no | depart | 1 | 3 |
| 4 | 3 | 1500 | 1200 | no | depart | 0 | 3 |
| 4 | 4 | 1500 | 1900 | yes | arrive | 1 | 3 |
| 5 | 4 | 1800 | 1900 | yes | arrive | 2 | 3 |
| 6 | — | — | — | `i == n` | stop | 2 | **3** |

**Answer: 3.** Just after 11:00 the station holds the 9:40 train (leaves 11:20), the 9:50 train (leaves 11:30) and the 11:00 train (leaves 12:00).

---

## 11. Common Pitfalls

1. **Not settling the boundary convention first.** Half the wrong answers in this family are a `<` that should be `<=`. Decide "do `[1,2]` and `[2,3]` overlap?" *before* writing the comparison (§2), and re-check it against the problem statement rather than a remembered template.
2. **Assignment instead of `max` when merging.** `back[1] = iv[1]` silently shrinks `[1,10]` to `[1,3]` when `[2,3]` is nested inside it. Always `back[1] = max(back[1], iv[1])` (§4).
3. **Sorting by the wrong key.** Start-sorted greedy gives 1 instead of 2 on `[0,10],[1,2],[3,4]`; end-sorted merging strands `[2,3]` on `[1,10],[2,3],[4,5]`. Both checked in §3.
4. **Calling `back()` on an empty result.** `out.back()[1] >= iv[0]` is undefined behaviour on the first iteration. The `!out.empty() &&` guard must come first, and `&&` short-circuits precisely so it can.
5. **Comparing against *all* previously emitted intervals.** Unnecessary after sorting by start, and it turns an `O(n)` pass into `O(n²)`. Only `back()` can still grow.
6. **Sorting by end with a comparator that reads `a[0]`.** `return a[1] < b[1];` — a stray `a[0] < b[0]` fallback is fine as a tie-break but a swapped index silently gives you the §3.1 failure.
7. **Assuming the input is sorted.** [09-insert_interval.cpp](../02-Medium/09-insert_interval.cpp) is `O(n)` *only* because the problem guarantees a sorted, disjoint list. Apply that template to unsorted input and it produces garbage, not just a slow answer.
8. **Forgetting `n == 0`.** `arr[0][1]` in the selection greedy and `arr.back()` elsewhere both blow up on an empty list. Guard it.
9. **Wrong tie-break in the sweep.** A default `sort` on `pair<int,int>` events puts `-1` before `+1`, which is half-open. If the problem says touching conflicts, you must flip it (§7.2).
10. **Difference array sized to the data instead of the range.** `delta` must be indexable at `maxEnd + 1`; a one-off on the size is the classic out-of-bounds here.
11. **Mistaking "max overlap" for "min removals".** They are different questions: `[1,10],[2,3],[4,5]` has peak overlap **2** but needs only **1** removal to become disjoint (drop `[1,10]`, leaving the already-disjoint `[2,3]` and `[4,5]`). Peak overlap counts how many stack up at the busiest instant; min removals counts how many must go to flatten *every* instant to one. §6 and §7 are not interchangeable.
12. **Mutating the caller's interval.** The insert template modifies `nw` in place; if the caller still needs it, pass by value or copy.

---

## 12. Cheat Sheet

```cpp
// ---- overlap predicate -------------------------------------------------
int lo = max(A[0], B[0]), hi = min(A[1], B[1]);
bool overlap = (lo <  hi);   // half-open [a,b)  : touching is NOT overlap
bool overlap = (lo <= hi);   // closed    [a,b]  : touching IS overlap
// intersection, when it exists, is exactly [lo, hi]

// ---- MERGE: sort by START ---------------------------------------------
sort(a.begin(), a.end());
vector<vector<int>> out;
for (auto& iv : a) {
    if (!out.empty() && out.back()[1] >= iv[0])
        out.back()[1] = max(out.back()[1], iv[1]);   // max! nesting case
    else out.push_back(iv);
}

// ---- INSERT into a sorted, disjoint list: O(n), no sort ----------------
for (int i = 0; i < n; i++) {
    if (a[i][1] < nw[0])      res.push_back(a[i]);                    // before
    else if (a[i][0] > nw[1]) { res.push_back(nw);                    // after
                                while (i < n) res.push_back(a[i++]);
                                return res; }
    else { nw[0] = min(nw[0], a[i][0]);                               // overlap
           nw[1] = max(nw[1], a[i][1]); }
}
res.push_back(nw);

// ---- SELECT / REMOVE: sort by END -------------------------------------
sort(a.begin(), a.end(), [](auto& x, auto& y){ return x[1] < y[1]; });
int last = a[0][1], kept = 1;
for (int i = 1; i < n; i++)
    if (a[i][0] >= last) { last = a[i][1]; kept++; }   // '>' if touching conflicts
int removed = n - kept;

// ---- PEAK OVERLAP: two parallel arrays, two pointers -------------------
sort(s.begin(), s.end()); sort(e.begin(), e.end());
int live = 0, best = 0, i = 0, j = 0;
while (i < n && j < m) {
    if (s[i] <= e[j]) { live++; i++; }   // '<=' closed, '<' half-open
    else              { live--; j++; }
    best = max(best, live);
}

// ---- PEAK OVERLAP: generic events -------------------------------------
for (auto& iv : a) { ev.push_back({iv[0], +1}); ev.push_back({iv[1], -1}); }
sort(ev.begin(), ev.end());                 // ties: -1 first  => HALF-OPEN
for (auto& e : ev) best = max(best, live += e.second);

// ---- PEAK OVERLAP: difference array, bounded range T -------------------
for (auto& iv : a) { d[iv[0]]++; d[iv[1] + 1]--; }        // closed
for (int t = 0; t <= T + 1; t++) best = max(best, live += d[t]);

// ---- INTERSECT two sorted disjoint lists: O(n + m) ---------------------
while (i < A.size() && j < B.size()) {
    int lo = max(A[i][0], B[j][0]), hi = min(A[i][1], B[j][1]);
    if (lo <= hi) res.push_back({lo, hi});
    if (A[i][1] < B[j][1]) i++; else j++;
}
```

**The one line to remember:** *sort by **start** to **merge**, sort by **end** to **select**, and **decouple** the endpoints to **count**.*

---

## Related Notes

- [01-Scheduling-And-Sequencing.md](01-Scheduling-And-Sequencing.md) — the general greedy question of *which* sort key and how to prove it, job sequencing with deadlines, task-scheduler cooldown, and when greedy fails and DP takes over
- [../../Module%2003-Arrays/notes/04-Two-Pointers.md](../../Module%2003-Arrays/notes/04-Two-Pointers.md) — the two-pointer merge that the sweep in §7 and the intersection in §9 both are
- [../../Module%2003-Arrays/notes/02-Prefix-Sum.md](../../Module%2003-Arrays/notes/02-Prefix-Sum.md) — the difference-array technique of §8 in its general form
- [../../Module%2010-Sliding%20Window%20%26%20Two%20Pointers/notes/01-Sliding-Window.md](../../Module%2010-Sliding%20Window%20%26%20Two%20Pointers/notes/01-Sliding-Window.md) — the other "one monotone pass over sorted data" family
- [../../C%2B%2B%20Data%20Structures/01-CPP-Vector-Reference.md](../../C%2B%2B%20Data%20Structures/01-CPP-Vector-Reference.md) — `sort` with a custom comparator, `back()`, and the `vector<vector<int>>` mechanics these templates lean on
- [../../C%2B%2B%20Data%20Structures/04-CPP-Pair-Reference.md](../../C%2B%2B%20Data%20Structures/04-CPP-Pair-Reference.md) — why `pair` sorts lexicographically, and the tie-break trap in §7.2

### Problem files covered

- [../../Module%2003-Arrays/03-Hard/07-merge_intervals.cpp](../../Module%2003-Arrays/03-Hard/07-merge_intervals.cpp) — merge overlapping intervals (§4)
- [09-insert_interval.cpp](../02-Medium/09-insert_interval.cpp) — insert into a sorted disjoint list (§5)
- [10-remove_overlapping_intervals.cpp](../02-Medium/10-remove_overlapping_intervals.cpp) — LeetCode 435, minimum removals (§6)
- [01-activity_selection.cpp](../02-Medium/01-activity_selection.cpp) — maximum compatible activities (§6)
- [04-minimum_platforms.cpp](../02-Medium/04-minimum_platforms.cpp) — peak simultaneous overlap (§7)
