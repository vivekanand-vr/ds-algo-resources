# Greedy Scheduling & Sequencing — Picking the Right Sort Key

> Every problem in this family reduces to one decision: **what do you sort by?** After that the code is a single scan that commits to whatever still fits.
> Cost is almost always `O(n log n)` — the sort dominates, and the scan that follows is linear.

---

## Table of Contents

1. [What Makes It a Scheduling Problem](#1-what-makes-it-a-scheduling-problem)
2. [The Shared Skeleton](#2-the-shared-skeleton)
3. [The Sort-Key Decision Table](#3-the-sort-key-decision-table)
4. [Wrong Keys, and Why They Fail](#4-wrong-keys-and-why-they-fail)
5. [The Exchange Argument](#5-the-exchange-argument)
6. [Slot Assignment with Deadlines](#6-slot-assignment-with-deadlines)
7. [The DSU Speedup](#7-the-dsu-speedup)
8. [Resource Counting — The Event Sweep](#8-resource-counting--the-event-sweep)
9. [Frequency and Cooldown — Task Scheduler](#9-frequency-and-cooldown--task-scheduler)
10. [When Greedy Fails](#10-when-greedy-fails)
11. [Worked Traces](#11-worked-traces)
12. [Common Pitfalls](#12-common-pitfalls)
13. [Cheat Sheet](#13-cheat-sheet)

---

## 1. What Makes It a Scheduling Problem

Three signals, any one of which puts you in this family:

- **You choose an order.** The items are the same whatever you do; only the sequence is free. (Fractional knapsack, job sequencing, assign cookies.)
- **You assign items to slots or resources.** Time slots, platforms, machines, CPU intervals. (Job sequencing, minimum platforms, task scheduler.)
- **You accept or reject** items under a "one at a time" constraint. (Activity selection, remove overlapping intervals.)

What they share is that the *set* of candidates is fixed and the only lever is arrangement. That is exactly the situation where a total order on the items — a sort key — can encode the whole decision policy.

```
   unsorted candidates          sorted by the RIGHT key            scan & commit
   ┌───┬───┬───┬───┬───┐        ┌───┬───┬───┬───┬───┐           ┌───┬───┬───┐
   │ D │ A │ E │ B │ C │  ───►  │ A │ B │ C │ D │ E │  ───►     │ A │ C │ E │
   └───┴───┴───┴───┴───┘        └───┴───┴───┴───┴───┘           └───┴───┴───┘
                                   key = f(item)                  feasible ones
```

The hard part is never the scan. It is proving that *this* key makes the first item safe to take — which is what §5 is for.

---

## 2. The Shared Skeleton

Every problem here is this, with three blanks filled in:

```cpp
sort(v.begin(), v.end(), byKey);      // 1. impose the order
State st;                             // 2. what has been committed so far
for (auto& item : v)
    if (feasible(st, item))           // 3. does it still fit?
        commit(st, item);             // 4. take it, irrevocably
```

| Problem | Order (`byKey`) | `State` | `feasible` |
| --- | --- | --- | --- |
| Activity selection | finish time ↑ | `lastFinish` | `lastFinish < item.start` |
| Remove overlapping | end ↑ | `last` | `item.start >= last` |
| Job sequencing | profit ↓ | `slot[1..maxD]` | a free slot `≤ deadline` exists |
| Fractional knapsack | value/weight ↓ | remaining `cap` | always (take a fraction) |
| Assign cookies | both arrays ↑ | pointer `i` into `g` | `g[i] <= s[j]` |

The word **irrevocably** is the whole bet. A greedy never un-commits, so the sort key must guarantee that no future item could ever have justified a different choice. When that guarantee does not hold, you are in §10 territory and need DP.

Two problems in this note do *not* fit the skeleton and are worth naming as exceptions:

- **Minimum platforms** sorts two streams and counts a peak rather than accepting/rejecting anything (§8).
- **Task scheduler** does not sort at all — it counts frequencies and evaluates a closed form (§9).

---

## 3. The Sort-Key Decision Table

The centrepiece. Read it as: *what am I optimising → therefore what do I sort by*.

| Problem | Maximise / minimise | Sort key | Feasibility test | Why this key |
| --- | --- | --- | --- | --- |
| [01-activity_selection.cpp](../02-Medium/01-activity_selection.cpp) | max **count** of activities | **finish time ↑** | `lastFinish < start` | the earliest finisher frees the resource soonest, leaving the widest remaining timeline |
| [10-remove_overlapping_intervals.cpp](../02-Medium/10-remove_overlapping_intervals.cpp) | min **removals** | **end ↑** | `start >= last` | mirror image of the above — maximise kept ⇔ minimise removed |
| [05-job_sequencing.cpp](../02-Medium/05-job_sequencing.cpp) | max **profit** | **profit ↓** | a free slot in `1..deadline` | slots are interchangeable, so only the value ranking matters; deadlines are handled by *where* you place, not *when* you consider |
| [02-fractional_knapsack.cpp](../01-Easy/02-fractional_knapsack.cpp) | max **value** | **value/weight ↓** | always feasible | the binding resource is weight, so the right currency is value *per unit weight* |
| [01-assign_cookies.cpp](../01-Easy/01-assign_cookies.cpp) | max **matches** | both arrays **↑** | `g[i] <= s[j]` | spend the smallest adequate cookie on the least demanding child; anything bigger is wasted on them |
| [04-minimum_platforms.cpp](../02-Medium/04-minimum_platforms.cpp) | min **resources** | arrivals ↑, departures ↑ **separately** | n/a — count, don't select | the answer is peak occupancy, which depends only on the two multisets of timestamps |
| [07-task_scheduler.cpp](../02-Medium/07-task_scheduler.cpp) | min **makespan** | — (frequency count) | n/a | the bottleneck is the most frequent task, not any ordering of the rest |

Two patterns fall out of the table:

1. **Maximising a count under a resource constraint ⇒ sort by when the resource is released** (finish/end time). Never by when it is requested.
2. **Maximising a value ⇒ sort by value, or by value per unit of the binding resource.** Plain value is right when every item consumes the same amount (job sequencing: one unit of time each); the ratio is right when consumption varies (fractional knapsack: weight varies).

> The single most common way to get one of these wrong is to sort by the quantity mentioned first in the problem statement. "Trains arrive at…" does not mean sort by arrival; "jobs have deadlines…" does not mean sort by deadline.

---

## 4. Wrong Keys, and Why They Fail

Counterexamples below are all worked through by hand. The activity-selection ones use this module's **strict** convention (`lastFinish < start`, so touching activities conflict — see §6 of the file's own notes), but each is chosen so that it also breaks under the looser `<=` convention.

### 4.1 Activity selection sorted by **start time**

```
A = (1, 10)      ├───────────────────┤
B = (2, 3)          ├──┤
C = (4, 5)                 ├──┤
```

Sorted by start: `A, B, C`. Greedy takes `A` (finish 10); `B` starts at 2 < 10 → reject; `C` starts at 4 < 10 → reject. **Result: 1.**
Optimal is `{B, C}`: `B` finishes at 3, `C` starts at 4, and `3 < 4` — compatible under either convention. **Optimal: 2.**

One long activity that starts early poisons the entire schedule. Start time says nothing about when the resource comes back.

### 4.2 Activity selection sorted by **shortest duration**

```
A = (1, 5)       ├────────┤            duration 4
B = (4, 7)             ├─────┤         duration 3
C = (6, 10)                ├──────┤    duration 4
```

Sorted by duration: `B, A, C`. Greedy takes `B = (4,7)`. Then `A = (1,5)` overlaps `B` (they share `[4,5]`) → reject; `C = (6,10)` overlaps `B` (they share `[6,7]`) → reject. **Result: 1.**
Optimal is `{A, C}`: `A` finishes at 5, `C` starts at 6, `5 < 6` ✓. **Optimal: 2.**

The short one sits in the middle and straddles two others. Duration is a property of the activity alone; the constraint is about *position*.

### 4.3 Activity selection sorted by **fewest conflicts**

Intuitive and still wrong — it is merely harder to break. The standard counterexample needs ~11 intervals arranged so the globally least-conflicting interval is the one that must be dropped. Not worth memorising; the takeaway is that "fewest conflicts" is a local statistic and greedy needs a key whose optimality can be *exchanged* forward (§5).

### 4.4 Job sequencing sorted by **deadline**

Earliest-deadline-first is the right key for *feasibility* problems ("can everything be done on time?"), and the wrong key here, because it says nothing about value. With

```
dead = [1, 1],  prof = [1, 100]
```

both jobs need slot 1 and the deadline key is a tie — the sort has no opinion about which survives, so a stable sort takes the profit-1 job and returns **1** instead of **100**. Deadline determines *where a job may go*, not *which job is worth keeping*.

> There *is* a correct deadline-ordered algorithm — see §6.3 — but it needs a min-heap to undo earlier choices, which means it is no longer a pure commit-once greedy.

### 4.5 Fractional knapsack sorted by **value**

`val = [100, 10], wt = [50, 1], cap = 1`. By value: take item 0, but only `1/50` of it → `2.0`. By ratio (`2.0` vs `10.0`): take item 1 whole → `10.0`. A high-value item can be terrible value *per unit of the thing you are short of*.

Sorting by **lightest weight** fails symmetrically: `val = [1, 100], wt = [1, 2], cap = 2` gives `1 + 100·(1/2) = 51` instead of `100`.

---

## 5. The Exchange Argument

This is the proof technique that separates "I tried a key and it passed the tests" from "I know this key is right".

### 5.1 The template

Let `G` be the greedy solution and `O` any optimal one.

1. **Find the first disagreement.** Order both by the greedy's decision sequence and let `x` be the first item where `G` and `O` differ.
2. **Exchange.** Modify `O` into `O'` by making it agree with `G` at `x` — swapping in `G`'s item, swapping out whatever `O` used.
3. **Show `O'` is still feasible** (the exchange breaks no constraint).
4. **Show `O'` is no worse** (`value(O') >= value(O)`), hence `O'` is also optimal.
5. **Induct.** `O'` agrees with `G` one step further. Repeat until `O'` *is* `G`, so `G` is optimal.

The whole art is step 3. The sort key is correct exactly when the greedy's pick is the one that makes step 3 go through — which is why "earliest finish" and not "earliest start".

### 5.2 Instantiated: activity selection

Greedy picks `a₁`, the globally earliest-finishing activity. Let `O = {b₁, b₂, …, b_k}` be optimal, listed in increasing finish time.

- By choice of `a₁`, `f(a₁) <= f(b₁)`.
- Form `O' = {a₁, b₂, …, b_k}`. Is it feasible? `O` was, so `s(b₂) > f(b₁)`. Combined with `f(a₁) <= f(b₁)` this gives `s(b₂) > f(a₁)` — so `a₁` conflicts with nothing in `b₂..b_k`. ✓ *(The chain works with `>` throughout, so it holds under the strict convention this module uses; replacing both with `>=` proves the loose convention identically.)*
- `|O'| = |O| = k`. ✓ No worse.
- The remaining problem is "select from activities starting after `f(a₁)`" — the same problem on a smaller input. Induct.

**What the argument actually uses:** only that `f(a₁)` is minimal. That is the entire justification for the sort key, and it is exactly what breaks if you sort by start or by duration — there is then no guarantee `f(a₁) <= f(b₁)`, so step 3 collapses.

### 5.3 Instantiated: job sequencing

Let `J` be the highest-profit job on which greedy and optimal `S` first disagree: greedy schedules it, `S` does not.

- Greedy fit `J`, so some slot `t <= deadline(J)` was free in greedy at that moment.
- **Case A — slot `t` is empty in `S`.** Add `J` there. Total profit strictly rises, contradicting optimality of `S`.
- **Case B — slot `t` holds some job `K` in `S`.** Everything greedy scheduled before `J` has profit `>= profit(J)`, and `S` agrees with greedy on all of those, so `K` is a job greedy did *not* take by this point ⇒ `profit(K) <= profit(J)`. Swap `K` out for `J`. Slot `t` is within `J`'s deadline by construction, no other job moved, so every deadline still holds. Profit does not fall.

Either way `S` can be made to agree with greedy on `J` without losing profit. Induct. ∎

### 5.4 Instantiated: fractional knapsack

Suppose an optimal solution takes a positive amount of item `B` while leaving room for more of item `A`, where `ratio(A) > ratio(B)`. Move `ε` units of weight from `B` to `A`. Total weight is unchanged, so feasibility is preserved, and total value changes by `ε·(ratio(A) − ratio(B)) > 0` — strictly better, contradiction.

> **The exchange only exists because fractions are legal.** In 0/1 knapsack you cannot move `ε` units; you must move a whole item, which changes total weight and can break feasibility. Same key, same intuition, and the proof dies — see §10.2.

---

## 6. Slot Assignment with Deadlines

Job sequencing is the archetype: `n` jobs, each taking exactly one unit of time, each with a deadline `d` (usable slots are `1..d`) and a profit.

### 6.1 Two independent decisions

| Decision | Answer | Governed by |
| --- | --- | --- |
| Which job to consider next? | highest profit first | the exchange argument of §5.3 |
| Where to put an accepted job? | the **latest** free slot `<= d` | the argument below |

### 6.2 Why "as late as possible"

Slots are interchangeable for a job except for the deadline cap, so placement affects nothing except **which slots remain available**. A job with deadline `d` can use `1..d`; a job with a smaller deadline can use strictly fewer slots. So early slots are the contested ones.

```
deadline 1  → can only use  [1]
deadline 2  → can use       [1][2]
deadline 4  → can use       [1][2][3][4]
                             ▲
                        everyone wants slot 1
```

Taking slot `d` instead of slot 1 keeps the scarce early slots free for jobs that have no alternative. Formally: any schedule that places a job early can be rewritten to place it as late as possible without evicting anything — if the later slot is occupied by a job `K`, swap the two placements; `K` moved earlier, which is legal since `K`'s deadline was at least the later slot. So the late placement **dominates** and never costs a job.

**Concrete failure of early placement:** `A(d=2, p=100)`, `B(d=1, p=50)`. Profit order is `A, B`.
- Earliest-slot: `A` → slot 1. `B` needs slot 1, taken, no alternative → skipped. **Total 100.**
- Latest-slot: `A` → slot 2. `B` → slot 1. **Total 150.** ✓

### 6.3 Aside: the heap variant

There is a second correct algorithm for the same problem, ordered by **deadline ascending** with a min-heap of accepted profits:

```cpp
// not in this repo's files - the standard alternative, O(n log n), no maxD term
sort by deadline ascending;
priority_queue<int, vector<int>, greater<int>> pq;   // profits of accepted jobs
for (auto& [d, p] : v) {
    pq.push(p);
    if ((int)pq.size() > d) pq.pop();                // drop the weakest accepted job
}
```

The invariant is that after processing all jobs with deadline `<= d`, the heap holds the most profitable feasible subset of them, of size `<= d`. It is *not* a commit-once greedy — it retracts earlier acceptances — which is precisely why deadline order is usable here. Its advantage is that the complexity has no `maxD` term at all, so it stays `O(n log n)` even with deadlines up to `1e9` (the slot array would be unallocatable).

---

## 7. The DSU Speedup

The plain implementation's inner loop is the bottleneck:

```cpp
for (int j = deadline - 1; j > 0; j--)        // O(maxD) per job
    if (!slot[j]) { slot[j] = true; /* take it */ break; }
```

Every scan re-walks slots already known to be full — `O(n · maxD)` overall. A disjoint-set union removes the repetition by changing the question from *"is slot t free?"* to *"what is the latest free slot `<= t`?"*.

**The invariant:** `parent[t]` = the latest free slot at or before `t`, with `parent[t] == t` meaning `t` is itself free.

```cpp
int findFreeSlot(vector<int>& parent, int t) {
    if (parent[t] == t) return t;                         // t is free
    return parent[t] = findFreeSlot(parent, parent[t]);   // path compression
}
// init: parent[t] = t for t in 0..maxD   (slot 0 is the sentinel "no room")
int s = findFreeSlot(parent, deadline);
if (s > 0) { parent[s] = s - 1; /* take slot s */ }       // fall through to s-1
```

Three things make it work:

- `parent[s] = s - 1` on taking slot `s` means every later search landing on `s` immediately falls through to `s - 1`.
- Path compression flattens whole runs of occupied slots into one hop, so each lookup is amortised `α(maxD)` — under 5 for any realistic input.
- `find(d) == 0` means slots `1..d` are all full, i.e. the job is unschedulable — the same conclusion the backward scan reaches, without walking.

The greedy order and the resulting schedule are **identical**; only the slot search is faster.

| Variant | Time | Space |
| --- | --- | --- |
| Backward scan | `O(n log n + n·maxD)` | `O(n + maxD)` |
| DSU | `O(n log n + n·α(maxD))` | `O(n + maxD)` |
| Deadline + min-heap (§6.3) | `O(n log n)` | `O(n)` |

Both slot-array versions allocate `maxD + 1` entries, so both are unusable when deadlines are huge and sparse.

---

## 8. Resource Counting — The Event Sweep

[04-minimum_platforms.cpp](../02-Medium/04-minimum_platforms.cpp) flips the question. You are not choosing *which* trains to run — all of them run — you are asking **how many resources are needed at once**.

### 8.1 Peak occupancy is the answer

Two halves, both needed:

- **Lower bound.** At the instant when `k` trains are simultaneously present, each needs its own platform, so at least `k` platforms are required.
- **Upper bound.** With exactly `peak` platforms, assign every arriving train any free platform. One always exists, because the number in use at any instant equals the occupancy at that instant, which never exceeds `peak`.

Lower bound = upper bound ⇒ **minimum platforms = maximum simultaneous occupancy.**

### 8.2 The sweep

Treat each arrival as `+1` and each departure as `−1`, walk all events in chronological order, and track the running counter's maximum.

```
time →   900   910   940   950  1100  1120  1130  1200
event     +1    -1    +1    +1    +1    -1    -1    -1
count      1     0     1     2     3     2     1     0
                                   ▲ peak = 3
```

```cpp
sort(begin(arr), end(arr));                  // arrival events, chronological
sort(begin(dep), end(dep));                  // departure events, chronological
int res = 0, p = 0, i = 0, j = 0;
while (i < n && j < m) {
    if (arr[i] <= dep[j]) { p++; i++; }      // tie => arrival first (closed intervals)
    else                  { p--; j++; }
    res = max(res, p);
}
```

### 8.3 Why sorting the two arrays **independently** is legal

This looks alarming — after sorting, `arr[k]` and `dep[k]` no longer describe the same train. But the algorithm never uses the pairing. Occupancy at any instant `t` is

```
occupancy(t) = (# arrivals <= t) − (# departures < t)
```

and both terms depend only on the *multiset* of timestamps, not on which arrival belongs to which departure. Destroying the pairing destroys nothing the sweep reads. Every train's departure is at or after its arrival, so the running counter is a genuine occupancy count at every step and never dips below the true value in a way that could hide a peak.

### 8.4 The tie-break encodes the interval convention

`arr[i] <= dep[j]` processes an arrival *before* a same-timestamp departure: the counter goes up before it comes down, so both trains are counted — **closed intervals** `[arr, dep]`. Switching to `<` treats a departure at `t` as freeing the platform in time for an arrival at `t` — **half-open** `[arr, dep)`. Neither is more correct; it is a property of the problem statement. This file uses closed.

> Same structural choice as the strict `<` in [01-activity_selection.cpp](../02-Medium/01-activity_selection.cpp), which likewise treats touching activities as conflicting. One character, one convention, two different answers on touching inputs.

**Alternative when timestamps are bounded and dense** (e.g. a 24-hour clock, `0..2359`): skip sorting entirely with a difference array — `delta[arr]++`, `delta[dep + 1]--`, then take the maximum prefix sum. That is `O(n + T)`, better when `T` is small and `n` is large, worse when timestamps are sparse or unbounded.

---

## 9. Frequency and Cooldown — Task Scheduler

[07-task_scheduler.cpp](../02-Medium/07-task_scheduler.cpp) asks for the minimum makespan when two occurrences of the same task must be separated by at least `n` intervals. No sort, no scan — a closed form.

### 9.1 The skeleton derivation

Let `max_f` be the highest frequency and `c` the number of distinct tasks tied at `max_f`. Lay the most frequent task out in rows of width `n + 1`:

```
tasks = [A,A,A,B,B,B], n = 2        max_f = 3, c = 2

   ┌─ n+1 = 3 ─┐
   │ A  B  _   │   row 1   (full width: A, then n cooldown slots)
   │ A  B  _   │   row 2
   │ A  B      │   last row: no trailing cooldown needed
   └───────────┘
   length = (max_f − 1)·(n + 1) + c = 2·3 + 2 = 8
```

- The `max_f` occurrences of the bottleneck task are forced into `max_f` rows, since consecutive occurrences are at least `n + 1` apart.
- The first `max_f − 1` rows must each be a full `n + 1` wide — the cooldown after each occurrence is mandatory.
- The **last** row needs no trailing cooldown (nothing follows), so it contributes only as many slots as tasks placed in it. All `c` tied-for-max tasks land there, one column each ⇒ exactly `c`.

Total: `(max_f − 1)·(n + 1) + c`.

Every less-frequent task fits into the idle gaps of the first `max_f − 1` rows without lengthening the skeleton, because it occurs at most `max_f` times and can be dealt down the columns.

### 9.2 Why the `max(m, …)` floor is needed

The skeleton assumes the gaps can absorb the remaining tasks. With many distinct low-frequency tasks they overflow — and when they do, the cooldown stops binding entirely, because there are always enough *other* tasks to fill every gap. The schedule is then simply back-to-back with zero idle time: length `m`, the total task count, which is an unconditional lower bound.

```cpp
res = max(m, (max_f - 1) * (n + 1) + c);
```

| Input | `m` | `max_f` | `c` | formula | answer | binding constraint |
| --- | --- | --- | --- | --- | --- | --- |
| `AAABBB`, n=2 | 6 | 3 | 2 | `2·3 + 2 = 8` | **8** | cooldown |
| `AAABBB`, n=0 | 6 | 3 | 2 | `2·1 + 2 = 4` | **6** | task count |
| `AAAAAA BCDEFG`, n=2 | 12 | 6 | 1 | `5·3 + 1 = 16` | **16** | cooldown |
| `AAABBBCDEF`, n=2 | 10 | 3 | 2 | `2·3 + 2 = 8` | **10** | task count |

The last row is the one that justifies the `max`: the formula says 8, but 10 tasks cannot fit in 8 slots. A valid 10-slot schedule exists — `A B C | A B D | A B E | F` — with `A` at indices 0, 3, 6 (gaps of 3 ≥ `n + 1`) ✓.

Complexity: `O(m)` to count frequencies over a 26-letter alphabet, `O(1)` arithmetic.

---

## 10. When Greedy Fails

The most valuable section, because this family's failure modes look *exactly* like its successes.

| Problem | Looks like | Why greedy breaks | Use instead |
| --- | --- | --- | --- |
| **Weighted interval scheduling** (arbitrary durations **and** profits) | activity selection | earliest-finish maximises *count*, and count ≠ profit | DP over intervals sorted by end + binary search for the last compatible one |
| **0/1 knapsack** | fractional knapsack | no `ε`-exchange exists; you must take items whole | DP on capacity, `O(n·cap)` |
| **Jobs with durations > 1 unit** | job sequencing | slots stop being interchangeable; a long job blocks a whole range | DP / flow, often NP-hard |
| **Minimise total completion time with precedences** | sequencing by key | the order is constrained by a DAG, not free | topological order + DP |
| **Bin packing** | minimum platforms | resources are not interchangeable once items are assigned | NP-hard; greedy (first-fit-decreasing) is only an approximation |

### 10.1 Weighted interval scheduling

```
A = (1, 10)  profit 100      ├──────────────────┤
B = (1, 3)   profit   1      ├───┤
C = (4, 6)   profit   1               ├──┤
```

Earliest-finish greedy sorts to `B, C, A` and takes `B` then `C` — two intervals, total profit **2**. The optimum is `{A}` alone: profit **100**. No sort key rescues this: the choice at `B` depends on the *total value* of everything downstream, which is not a property of any single item.

The fix is DP. Sort by end time, let `p(i)` be the last interval ending at or before `start(i)` (found by binary search), and take

```
dp[i] = max(dp[i - 1], profit(i) + dp[p(i)])
```

`O(n log n)` — same cost as the greedy, and actually correct.

> The tell: **greedy handles "how many", DP handles "how much"** — unless every item is worth the same, in which case the two coincide and the greedy is the right tool.

### 10.2 0/1 vs fractional knapsack

The cleanest illustration that a greedy can be optimal *only* because of a modelling detail.

```
val = [60, 100, 120],  wt = [10, 20, 30],  cap = 50
ratios:  6.0    5.0     4.0
```

| | Choice | Value |
| --- | --- | --- |
| Fractional, ratio greedy | item0 whole + item1 whole + 2/3 of item2 | **240** ✓ optimal |
| 0/1, same ratio greedy | item0 (wt 10) + item1 (wt 20); item2 needs 30 > 20 left | **160** ✗ |
| 0/1, optimal | item1 + item2 (wt 20 + 30 = 50) | **220** |

The greedy leaves 20 units of capacity stranded. In the fractional version that space is filled with a slice of item 2; in 0/1 there is no slice to take, and the ratio ranking loses to a combination it never considers. Nothing about the sort key changed — only whether the exchange step of §5.4 is executable.

---

## 11. Worked Traces

### 11.1 Activity selection — `s = [5,1,8,0,3]`, `f = [7,2,9,6,4]`

Sorted by finish: `(1,2), (3,4), (0,6), (5,7), (8,9)`.

| i | activity | `lastFinish` before | `lastFinish < start`? | action | `lastFinish` after | count |
| --- | --- | --- | --- | --- | --- | --- |
| — | (1,2) | — | — | init (always take first) | 2 | 1 |
| 1 | (3,4) | 2 | `2 < 3` ✓ | accept | 4 | 2 |
| 2 | (0,6) | 4 | `4 < 0` ✗ | reject | 4 | 2 |
| 3 | (5,7) | 4 | `4 < 5` ✓ | accept | 7 | 3 |
| 4 | (8,9) | 7 | `7 < 8` ✓ | accept | 9 | **4** |

**Answer: 4.** Note `(0,6)` — the longest activity, and the earliest-starting — is rejected. Sorting by start would have taken it first and lost two activities.

### 11.2 The strict-`<` nuance — `s = [1,2,3]`, `f = [2,3,4]`

| i | activity | `lastFinish` | test | this file (`<`) | usual variant (`<=`) |
| --- | --- | --- | --- | --- | --- |
| — | (1,2) | — | — | accept, count 1 | accept, count 1 |
| 1 | (2,3) | 2 | `2 < 2` / `2 <= 2` | **reject** | accept, count 2 |
| 2 | (3,4) | 2 / 3 | `2 < 3` / `3 <= 3` | accept, count **2** | accept, count **3** |

Same input, answers **2** and **3**. Back-to-back activities are a conflict here by design. Always check which convention a problem statement wants before trusting a solution.

### 11.3 Job sequencing — `dead = [3,1,2,2]`, `prof = [50,10,20,30]`

Sorted by profit descending: `(3,50), (2,30), (2,20), (1,10)`. Slots `1..3` all free.

| job (d, p) | `slot[d]` free? | backward scan | slot taken | count | profit |
| --- | --- | --- | --- | --- | --- |
| (3, 50) | yes | — | 3 | 1 | 50 |
| (2, 30) | yes | — | 2 | 2 | 80 |
| (2, 20) | no | `j=1` → free | 1 | 3 | **100** |
| (1, 10) | no | `j>0` fails immediately | none | 3 | 100 |

**Answer: `{3, 100}`.** Final schedule: slot 1 → profit 20, slot 2 → profit 30, slot 3 → profit 50.

Same input through the DSU, `parent = [0,1,2,3]`:

| job | `find(d)` | result | `parent` after |
| --- | --- | --- | --- |
| (3, 50) | `parent[3]==3` | 3 → take | `[0,1,2,2]` |
| (2, 30) | `parent[2]==2` | 2 → take | `[0,1,1,2]` |
| (2, 20) | `parent[2]=1`, `parent[1]==1` | 1 → take | `[0,0,1,2]` |
| (1, 10) | `parent[1]=0`, `parent[0]==0` | 0 → skip | `[0,0,1,2]` |

Identical schedule, identical profit — each lookup a compressed pointer hop instead of a backward walk.

### 11.4 Minimum platforms — `arr = [900,940,950,1100,1500,1800]`, `dep = [910,1200,1120,1130,1900,2000]`

After independent sorts: `arr = [900,940,950,1100,1500,1800]`, `dep = [910,1120,1130,1200,1900,2000]`.

| i | j | `arr[i]` | `dep[j]` | `arr <= dep`? | event | `p` | `res` |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 0 | 0 | 900 | 910 | yes | arrival | 1 | 1 |
| 1 | 0 | 940 | 910 | no | departure | 0 | 1 |
| 1 | 1 | 940 | 1120 | yes | arrival | 1 | 1 |
| 2 | 1 | 950 | 1120 | yes | arrival | 2 | 2 |
| 3 | 1 | 1100 | 1120 | yes | arrival | 3 | **3** |
| 4 | 1 | 1500 | 1120 | no | departure | 2 | 3 |
| 4 | 2 | 1500 | 1130 | no | departure | 1 | 3 |
| 4 | 3 | 1500 | 1200 | no | departure | 0 | 3 |
| 4 | 4 | 1500 | 1900 | yes | arrival | 1 | 3 |
| 5 | 4 | 1800 | 1900 | yes | arrival | 2 | 3 |
| 6 | — | — | — | `i == n` | stop | 2 | **3** |

**Answer: 3.** At 11:00 the 9:40 (leaves 11:20), 9:50 (leaves 11:30) and 11:00 (leaves 12:00) trains are all present. The loop exits when arrivals run out — from there only departures remain, so `p` can only fall and the peak is already decided.

---

## 12. Common Pitfalls

1. **Sorting by start time in activity selection.** The single most common error in this family. Sort by *finish*. §4.1.
2. **Sorting by duration** because "short jobs fit more". §4.2.
3. **Placing deadline-jobs as early as possible.** Correct order, wrong placement — it burns the contested early slots. §6.2.
4. **Sorting job sequencing by deadline** and committing once. Deadline order requires the retracting heap variant, not a plain greedy. §4.4, §6.3.
5. **Using value instead of value-per-weight** in fractional knapsack, or weight instead of ratio. §4.5.
6. **Applying the ratio greedy to 0/1 knapsack.** It looks identical and is wrong. §10.2.
7. **Applying earliest-finish to weighted intervals.** Greedy counts; it does not sum profits. §10.1.
8. **Getting the touching-endpoint convention wrong.** `<` vs `<=` in activity selection, `<` vs `<=` in the platform tie-break — one character, different answers on touching inputs. §8.4, §11.2.
9. **Forgetting `slot[0]` is a sentinel.** The backward scan's condition is `j > 0` and the DSU treats `find() == 0` as failure; using index 0 as a real slot silently schedules a job before time begins.
10. **Allocating `slot[maxD + 1]` when deadlines are large.** `maxD = 1e9` is unallocatable — switch to the heap variant. §6.3.
11. **Taking the max in the platform sweep only on arrivals.** Harmless here since `p` only rises on arrivals, but the habit breaks the moment you add a third event type.
12. **Omitting `max(m, …)` in task scheduler.** The skeleton formula alone under-reports whenever the low-frequency tasks overflow the gaps. §9.2.
13. **Assuming a passing test proves the key.** Wrong keys agree with the right one on most random inputs; the counterexamples in §4 are all 3 intervals wide. Prove it with §5 or reach for DP.

---

## 13. Cheat Sheet

```cpp
// ---- Activity selection: max non-overlapping (sort by FINISH) -------------
sort(v.begin(), v.end(), [](auto a, auto b){ return a.second < b.second; });
int lastFinish = v[0].second, count = 1;
for (int i = 1; i < n; i++)
    if (lastFinish < v[i].first) { lastFinish = v[i].second; count++; }
// use  <=  instead of  <  if touching activities are allowed back-to-back

// ---- Job sequencing: max profit, unit jobs, deadlines --------------------
sort(v.begin(), v.end(), [](auto a, auto b){ return a.second > b.second; });
for (auto& [d, p] : v)
    for (int j = d; j > 0; j--)              // LATEST free slot <= d
        if (!slot[j]) { slot[j] = true; count++; profit += p; break; }

// ---- Same greedy, DSU slot lookup: O(n * alpha) --------------------------
int find(vector<int>& par, int t) {
    if (par[t] == t) return t;
    return par[t] = find(par, par[t]);
}
// init par[t] = t for 0..maxD
int s = find(par, d);
if (s > 0) { par[s] = s - 1; count++; profit += p; }

// ---- Minimum platforms: peak occupancy -----------------------------------
sort(begin(arr), end(arr)); sort(begin(dep), end(dep));   // independently!
int res = 0, p = 0, i = 0, j = 0;
while (i < n && j < m) {
    if (arr[i] <= dep[j]) { p++; i++; } else { p--; j++; }
    res = max(res, p);
}

// ---- Task scheduler: closed form -----------------------------------------
res = max(m, (max_f - 1) * (n + 1) + c);     // c = #tasks tied at max_f

// ---- Fractional knapsack: ratio descending -------------------------------
sort(v.begin(), v.end(), [](Item a, Item b){ return a.ratio > b.ratio; });
for (auto& it : v) {
    if (cap >= it.wt) { cap -= it.wt; res += it.val; }
    else { res += it.val * ((double)cap / it.wt); cap = 0; break; }
}

// ---- Weighted intervals: NOT greedy, DP ----------------------------------
// sort by end; p(i) = last interval ending <= start(i), via binary search
// dp[i] = max(dp[i-1], profit(i) + dp[p(i)])
```

**Key selection in one line each:**

| Goal | Key |
| --- | --- |
| max count of compatible items | earliest **finish** |
| max profit, equal-size items | **profit** descending |
| max value, varying item size | **value / size** descending |
| min resources for fixed items | none — **sweep** `+1/−1` events, take the peak |
| min makespan under cooldown | none — **frequency** closed form |
| max profit, varying-size items | none — **DP** |

---

## Related Notes

- [02-Interval-Problems.md](02-Interval-Problems.md) — interval geometry: merging, inserting, and overlap removal. Activity selection and minimum platforms appear there too, from the interval side; this note treats them as ordering and resource-counting problems.
- [../../Module%2010-Sliding%20Window%20%26%20Two%20Pointers/notes/01-Sliding-Window.md](../../Module%2010-Sliding%20Window%20%26%20Two%20Pointers/notes/01-Sliding-Window.md) — the other "commit and never back up" technique; §8's two-pointer merge is a two-sequence cousin of it.
- [../../Module%2002-Sorting%20Algorithms/SORTING_ALGORITHMS.md](../../Module%2002-Sorting%20Algorithms/SORTING_ALGORITHMS.md) — the `O(n log n)` that every algorithm in this note is paying for.
- [../../C%2B%2B%20Data%20Structures/04-CPP-Pair-Reference.md](../../C%2B%2B%20Data%20Structures/04-CPP-Pair-Reference.md) — `pair` and custom comparators, the mechanics behind every sort key here.
- [../../C%2B%2B%20Data%20Structures/01-CPP-Vector-Reference.md](../../C%2B%2B%20Data%20Structures/01-CPP-Vector-Reference.md) — `sort`, `max_element`, and the slot-table idioms.
- [../../Analysis%20of%20algorithms/Part_05_Advanced_Analysis_Techniques.md](../../Analysis%20of%20algorithms/Part_05_Advanced_Analysis_Techniques.md) — amortised analysis, which is what makes the DSU's `α(n)` claim in §7 meaningful.
