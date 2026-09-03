# 26. Lower Bounds & Adversary Arguments

## 26.1 Comparison-Based Sorting Lower Bound

**Theorem.** Any comparison-based sorting algorithm requires `Ω(n log n)` comparisons in the worst case.

*Proof (decision tree).*
- A comparison-based algorithm corresponds to a binary decision tree; each internal node is a comparison, each leaf a permutation.
- There are `n!` possible input orderings, so the tree must have **at least `n!` leaves**.
- A binary tree of height `h` has at most `2^h` leaves. So `2^h ≥ n!`.
- `h ≥ log₂(n!) = Θ(n log n)` by Stirling. ∎

Consequences:
- Merge sort, heap sort, and randomized quicksort are **asymptotically optimal**.
- Average case is also `Ω(n log n)` (average leaf depth in a tree with `n!` leaves is `≥ log(n!)`).
- Non-comparison sorts (counting, radix, bucket) beat this by **not comparing** — they exploit key structure.

## 26.2 Other Information-Theoretic Lower Bounds

| Problem | Lower bound | Argument |
| --- | --- | --- |
| Searching a sorted array (comparisons) | `Ω(log n)` | `n` possible answers, binary decisions |
| Sorting | `Ω(n log n)` | `n!` outcomes |
| Finding min | `n − 1` comparisons | each comparison eliminates one candidate |
| Finding min **and** max | `⌈3n/2⌉ − 2` | pair-then-compare |
| Finding 2nd largest | `n + ⌈log n⌉ − 2` | tournament argument |
| Median (comparisons) | `Ω(n)`, exactly `≈ 2n` known | adversary |
| Element uniqueness | `Ω(n log n)` (algebraic decision tree) |  |
| Convex hull | `Ω(n log n)` | reduction from sorting |
| Any problem reading all input | `Ω(n)` | must read input |
| Matrix multiplication | `Ω(n²)` (trivial); true bound open | output size |

## 26.3 Adversary Arguments

The adversary answers queries in the way that keeps the most possibilities alive, forcing the algorithm to work.

> **Finding max requires `n−1` comparisons.** Think of each element as a candidate. Each comparison eliminates at most one candidate. To go from `n` candidates to 1, you need `n−1` eliminations. ∎
> 

> **Finding min and max in `⌈3n/2⌉−2`:** Compare elements in pairs (`⌊n/2⌋` comparisons), then find max among winners (`⌈n/2⌉−1`) and min among losers (`⌈n/2⌉−1`). Total `≈ 3n/2 − 2`. Naive is `2n−3`.
> 

## 26.4 Reductions

If problem `A` reduces to `B` in time `O(f(n))` and `B` is solvable in `O(g(n))`, then `A` is solvable in `O(f(n) + g(n))`.

Contrapositive (lower bounds): if `A` requires `Ω(h(n))` and `A ≤_{O(f)} B`, then `B` requires `Ω(h(n) − f(n))`.

> **Example.** Element Uniqueness `≤` Sorting: sort (`O(n log n)`), then scan for adjacent duplicates (`O(n)`).
Since Element Uniqueness requires `Ω(n log n)` in the algebraic decision tree model, so does sorting.
> 

---

# 27. Complexity Classes (P, NP, NP-Complete)

## 27.1 Definitions

| Class | Definition |
| --- | --- |
| **P** | Decision problems solvable by a deterministic TM in polynomial time `O(n^k)` |
| **NP** | Solvable by a non-deterministic TM in poly time — equivalently, a proposed solution is **verifiable** in poly time |
| **co-NP** | Complement is in NP (the “no” answer is verifiable) |
| **NP-hard** | Every problem in NP reduces to it in poly time. *Need not be in NP; need not be a decision problem.* |
| **NP-complete** | In NP **and** NP-hard |
| **PSPACE** | Solvable in polynomial *space* |
| **EXPTIME** | Solvable in `O(2^{n^k})` time |
| **BPP** | Solvable by a randomized algorithm with error `< 1/3` |
| **L / NL** | Logarithmic space (deterministic / non-deterministic) |

**Known:** `L ⊆ NL ⊆ P ⊆ NP ⊆ PSPACE ⊆ EXPTIME`, with `P ⊊ EXPTIME` proven.
**Open:** `P = NP?` (the biggest open question in CS).

## 27.2 Proving NP-Completeness

To show problem `X` is NP-complete:
1. Show `X ∈ NP` (give a poly-time verifier for a certificate).
2. Pick a known NP-complete problem `Y`.
3. Give a **polynomial-time reduction** `Y ≤_p X` — transform any instance of `Y` into an instance of `X` such that the answers match.

> ⚠️ **Direction matters.** You reduce a *known hard* problem **to** your problem. Reducing your problem to a known hard one proves nothing about hardness.
> 

## 27.3 Canonical NP-Complete Problems

- **SAT** (Cook–Levin theorem, 1971 — the first)
- **3-SAT**
- Clique, Vertex Cover, Independent Set
- Hamiltonian Path / Cycle, Traveling Salesman (decision version)
- Subset Sum, Partition, Bin Packing
- Graph Coloring (`k ≥ 3`)
- Set Cover, Knapsack (decision version)
- Integer Programming
- Sudoku (generalized `n×n`), Minesweeper consistency

**In P (easy to confuse with the above):**
- 2-SAT, 2-Coloring (bipartiteness)
- Shortest path (non-negative weights), Euler circuit
- Bipartite matching, Max flow
- Linear programming, Primality (AKS, 2002)
- Minimum spanning tree

## 27.4 Approximation & Parameterized Complexity

| Approach | Idea | Example |
| --- | --- | --- |
| Approximation algorithm | guaranteed ratio `ρ` | Vertex Cover 2-approx; TSP (metric) 1.5-approx (Christofides) |
| PTAS | `(1+ε)`-approx in `O(n^{f(1/ε)})` | Euclidean TSP |
| FPTAS | `(1+ε)` in time poly in `n` and `1/ε` | Knapsack |
| Fixed-parameter tractable (FPT) | `O(f(k)·n^c)` | Vertex Cover `O(2^k · n)` |
| Heuristics | no guarantee | genetic, simulated annealing |
| Exact exponential | better than brute force | TSP `O(2ⁿ n²)` vs `O(n!)` |

**Inapproximability:** unless `P=NP`, no constant-factor approximation exists for general TSP; Set Cover cannot be approximated better than `ln n`.

---

# 28. Master Cheat Sheets

## 28.1 Growth Rate Values

| n | log n | n | n log n | n² | n³ | 2ⁿ | n! |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 10 | 3.3 | 10 | 33 | 100 | 1,000 | 1,024 | 3.6×10⁶ |
| 20 | 4.3 | 20 | 86 | 400 | 8,000 | 10⁶ | 2.4×10¹⁸ |
| 50 | 5.6 | 50 | 282 | 2,500 | 125,000 | 10¹⁵ | 3×10⁶⁴ |
| 100 | 6.6 | 100 | 664 | 10⁴ | 10⁶ | 10³⁰ | 9×10¹⁵⁷ |
| 1,000 | 10 | 10³ | 10⁴ | 10⁶ | 10⁹ | 10³⁰¹ | — |
| 10⁶ | 20 | 10⁶ | 2×10⁷ | 10¹² | 10¹⁸ | — | — |
| 10⁹ | 30 | 10⁹ | 3×10¹⁰ | 10¹⁸ | — | — | — |

## 28.2 Sorting Algorithms — Complete Comparison

| Algorithm | Best | Average | Worst | Space | Stable | In-place | Adaptive | Method |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Bubble | `Ω(n)`* | `Θ(n²)` | `O(n²)` | `O(1)` | ✔ | ✔ | ✔* | exchange |
| Cocktail | `Ω(n)` | `Θ(n²)` | `O(n²)` | `O(1)` | ✔ | ✔ | ✔ | exchange |
| Selection | `Ω(n²)` | `Θ(n²)` | `O(n²)` | `O(1)` | ✘ | ✔ | ✘ | selection |
| Insertion | `Ω(n)` | `Θ(n²)` | `O(n²)` | `O(1)` | ✔ | ✔ | ✔ | insertion |
| Binary insertion | `Ω(n log n)` cmp | `Θ(n²)` moves | `O(n²)` | `O(1)` | ✔ | ✔ | partly | insertion |
| Shell (Ciura gaps) | `Ω(n log n)` | `~Θ(n^{1.25})` | `O(n^{1.5})`–`O(n log²n)` | `O(1)` | ✘ | ✔ | ✔ | insertion |
| Merge | `Ω(n log n)` | `Θ(n log n)` | `O(n log n)` | `O(n)` | ✔ | ✘ | ✘ | merging |
| In-place merge | — | `Θ(n log²n)` | `O(n log² n)` | `O(1)` | ✔ | ✔ | ✘ | merging |
| Quick | `Ω(n log n)` | `Θ(n log n)` | `O(n²)` | `O(log n)` | ✘ | ✔ | ✘ | partitioning |
| Randomized Quick | `Ω(n log n)` | `Θ(n log n)` exp. | `O(n²)` (unlikely) | `O(log n)` | ✘ | ✔ | ✘ | partitioning |
| Heap | `Ω(n log n)` | `Θ(n log n)` | `O(n log n)` | `O(1)` | ✘ | ✔ | ✘ | selection |
| Intro | `Ω(n log n)` | `Θ(n log n)` | `O(n log n)` | `O(log n)` | ✘ | ✔ | ✘ | hybrid |
| Tim | `Ω(n)` | `Θ(n log n)` | `O(n log n)` | `O(n)` | ✔ | ✘ | ✔ | hybrid |
| Counting | `Ω(n+k)` | `Θ(n+k)` | `O(n+k)` | `O(n+k)` | ✔ | ✘ | ✘ | non-comparison |
| Radix (LSD) | `Ω(d(n+k))` | `Θ(d(n+k))` | `O(d(n+k))` | `O(n+k)` | ✔ | ✘ | ✘ | non-comparison |
| Bucket | `Ω(n+k)` | `Θ(n+k)` | `O(n²)` | `O(n+k)` | ✔ | ✘ | ✘ | non-comparison |
| Cycle | `Ω(n²)` | `Θ(n²)` | `O(n²)` | `O(1)` | ✘ | ✔ | ✘ | minimum writes |
| Bogo | `Ω(n)` | `Θ(n·n!)` | `∞` | `O(1)` | ✘ | ✔ | ✘ | joke |
- with the swapped-flag optimization.

**When to use what:**
- Small `n` (< 20–50): insertion sort (low constants, adaptive).
- Guaranteed `n log n` + stability + external data: merge sort.
- General in-memory, speed-critical: quicksort/introsort.
- Guaranteed `n log n` + `O(1)` space: heap sort.
- Nearly sorted data: insertion sort or Tim sort.
- Small integer keys in a known range: counting/radix sort.
- Uniformly distributed floats: bucket sort.

## 28.3 Data Structure Operations

| Structure | Access | Search | Insert | Delete | Space |
| --- | --- | --- | --- | --- | --- |
| Array | `O(1)` | `O(n)` | `O(n)` | `O(n)` | `O(n)` |
| Sorted array | `O(1)` | `O(log n)` | `O(n)` | `O(n)` | `O(n)` |
| Dynamic array | `O(1)` | `O(n)` | `O(1)` amort. append | `O(n)` | `O(n)` |
| Singly linked list | `O(n)` | `O(n)` | `O(1)`† | `O(1)`† | `O(n)` |
| Doubly linked list | `O(n)` | `O(n)` | `O(1)`† | `O(1)`† | `O(n)` |
| Stack | `O(n)` | `O(n)` | `O(1)` | `O(1)` | `O(n)` |
| Queue / Deque | `O(n)` | `O(n)` | `O(1)` | `O(1)` | `O(n)` |
| Hash table | — | `O(1)` avg / `O(n)` worst | `O(1)` avg | `O(1)` avg | `O(n)` |
| BST (average) | `O(log n)` | `O(log n)` | `O(log n)` | `O(log n)` | `O(n)` |
| BST (worst) | `O(n)` | `O(n)` | `O(n)` | `O(n)` | `O(n)` |
| AVL / Red-Black | `O(log n)` | `O(log n)` | `O(log n)` | `O(log n)` | `O(n)` |
| Splay tree | `O(log n)` amort. | `O(log n)` amort. | `O(log n)` amort. | `O(log n)` amort. | `O(n)` |
| B-tree / B+ tree | `O(log n)` | `O(log n)` | `O(log n)` | `O(log n)` | `O(n)` |
| Binary heap | `O(1)` min | `O(n)` | `O(log n)` | `O(log n)` | `O(n)` |
| Fibonacci heap | `O(1)` min | `O(n)` | `O(1)` amort. | `O(log n)` amort. | `O(n)` |
| Trie (`σ` alphabet) | — | `O(L)` | `O(L)` | `O(L)` | `O(N·σ)` |
| Skip list | `O(log n)` exp. | `O(log n)` exp. | `O(log n)` exp. | `O(log n)` exp. | `O(n)` exp. |
| Segment tree | — | `O(log n)` query | `O(log n)` update | — | `O(n)` |
| Fenwick (BIT) | — | `O(log n)` prefix | `O(log n)` update | — | `O(n)` |
| Sparse table | `O(1)` query | `O(1)` RMQ | build `O(n log n)` | — | `O(n log n)` |
| Union-Find | — | `O(α(n))` | `O(α(n))` | — | `O(n)` |
| Bloom filter | — | `O(k)` | `O(k)` | ✘ | `O(m)` bits |

† given a pointer to the node/position.

## 28.4 Master Theorem Quick Card

```
T(n) = a·T(n/b) + f(n)          Compare f(n) with n^{log_b a}

  f smaller polynomially  →  Θ(n^{log_b a})              [leaves win]
  f equal (×log^p n)      →  Θ(n^{log_b a} · log^{p+1} n) [tie]
  f bigger polynomially   →  Θ(f(n))                     [root wins]
     (+ regularity a·f(n/b) ≤ k·f(n), k<1)
```

## 28.5 Formula Sheet (one page)

```
── Sums ──────────────────────────────────────────
Σ_{i=1}^{n} 1       = n
Σ_{i=1}^{n} i       = n(n+1)/2
Σ_{i=1}^{n} i²      = n(n+1)(2n+1)/6
Σ_{i=1}^{n} i³      = [n(n+1)/2]²
Σ_{i=1}^{n} 1/i     = H_n ≈ ln n + 0.5772
Σ_{i=0}^{n} x^i     = (x^{n+1}−1)/(x−1)
Σ_{i=0}^{∞} x^i     = 1/(1−x)          |x|<1
Σ_{i=0}^{n} 2^i     = 2^{n+1} − 1
Σ_{i=1}^{∞} i/2^i   = 2
Σ_{i=0}^{∞} i·x^i   = x/(1−x)²
Σ_{i=1}^{n} log i   = log(n!) = Θ(n log n)
Σ_{k=0}^{n} C(n,k)  = 2^n

── Logs ──────────────────────────────────────────
log(xy)=log x+log y      log(x/y)=log x−log y
log(x^a)=a log x         log_b x = log_a x / log_a b
b^{log_b x} = x          a^{log_b n} = n^{log_b a}
2^{log₂ n} = n           n^{1/log₂ n} = 2

── Stirling & Factorials ─────────────────────────
n! ≈ √(2πn)(n/e)^n       log(n!) = Θ(n log n)
(n/e)^n ≤ n! ≤ n^n       C(n,k) ≤ (en/k)^k

── e and limits ──────────────────────────────────
e^x ≥ 1+x                (1+1/n)^n → e
(1−1/n)^n → 1/e          Σ x^i/i! = e^x

── Recurrences ───────────────────────────────────
T(n)=T(n−1)+1     → Θ(n)         T(n)=T(n/2)+1    → Θ(log n)
T(n)=T(n−1)+n     → Θ(n²)        T(n)=T(n/2)+n    → Θ(n)
T(n)=2T(n−1)+1    → Θ(2ⁿ)        T(n)=2T(n/2)+1   → Θ(n)
T(n)=T(n−1)+log n → Θ(n log n)   T(n)=2T(n/2)+n   → Θ(n log n)
T(n)=nT(n−1)      → Θ(n!)        T(n)=T(√n)+1     → Θ(log log n)

── Growth Order ──────────────────────────────────
1 ≺ α(n) ≺ log*n ≺ log log n ≺ log n ≺ log^k n ≺ n^ε
  ≺ n ≺ n log n ≺ n² ≺ n³ ≺ n^{log n} ≺ 2^{√n} ≺ 2ⁿ ≺ n! ≺ nⁿ
```

## 28.6 Recognizing Complexity from Problem Constraints

Competitive-programming heuristic (≈10⁸ operations/sec):

| Constraint on `n` | Intended complexity |
| --- | --- |
| `n ≤ 10` | `O(n!)`, `O(nⁿ)` |
| `n ≤ 20` | `O(2ⁿ)`, `O(2ⁿ·n)` |
| `n ≤ 100` | `O(n⁴)` |
| `n ≤ 500` | `O(n³)` |
| `n ≤ 5,000` | `O(n²)` |
| `n ≤ 10⁵` | `O(n log n)`, `O(n √n)` |
| `n ≤ 10⁶` | `O(n)`, `O(n log n)` |
| `n ≤ 10⁸` | `O(n)`, `O(√n)` |
| `n > 10⁸` | `O(log n)`, `O(1)` |

---

# 29. Common Pitfalls and Misconceptions

| # | Misconception | Reality |
| --- | --- | --- |
| 1 | “Big-O means worst case” | Big-O is a bound on a *function*. You can say “best case is `O(n)`”. Worst/best/average are *cases*; O/Ω/Θ are *bounds*. They are orthogonal. |
| 2 | “`O` and `Θ` are the same” | `O` is an upper bound (possibly loose). `n = O(n²)` is true; `n = Θ(n²)` is false. |
| 3 | “Dropping the constant is always safe” | Asymptotically yes; practically no. `1000n` vs `n²` — for `n < 1000` the quadratic wins. |
| 4 | “`Θ(n²/2)`” | Constants inside Θ are meaningless: `Θ(n²/2) = Θ(n²)`. Never write the constant. |
| 5 | “`O(2n) = O(n)`, so `O(2ⁿ) = O(n)`” | Constants multiply, not exponents. `2ⁿ` is exponential. |
| 6 | “`log` base matters” | Not inside O/Θ (change of base is a constant factor). It **does** matter in exponents: `2^{log₂n}=n` vs `2^{log₃n}=n^{0.63}`. |
| 7 | Nested loops always multiply | Only if the bounds are independent. `for i=1..n; for j=1..i` sums to `n²/2 = Θ(n²)` — multiplying happens to give the right order here. But `for i*=2; for j=1..i` sums to `2n−1 = Θ(n)`, **not** `Θ(n log n)`. Always write the summation. |
| 8 | “Recursion depth = time complexity” | Depth gives *space*. Time is the *number of nodes* in the recursion tree. Naive Fibonacci: depth `n`, time `Θ(φⁿ)`. |
| 9 | “Amortized = average” | Amortized is a worst-case guarantee over a sequence; average is probabilistic over inputs. |
| 10 | “Build-heap is `O(n log n)`” | It is `Θ(n)`. `n` inserts is `O(n log n)`, but bottom-up heapify is linear. |
| 11 | “`O(n)` beats `O(n log n)` always” | Only asymptotically. Constants and cache behaviour often decide real performance. |
| 12 | “Space complexity ignores the input” | Total space includes the input; *auxiliary* space excludes it. Always say which. |
| 13 | “Recursion is always slower” | Same asymptotic complexity; only constant-factor and stack overhead differ. |
| 14 | “Memoization always turns exponential into polynomial” | Only if the number of *distinct states* is polynomial. Memoized subset-sum over subsets is still `Θ(2ⁿ)` states. |
| 15 | “`O(nW)` knapsack is polynomial” | Pseudo-polynomial — exponential in the *bit length* of `W`. |
| 16 | “Sum of two `O(n)` loops is `O(n²)`” | Sequential blocks **add** (→ `O(n)`), only nesting multiplies. |
| 17 | “`f = O(g)` implies `2^f = O(2^g)`” | False. `2n = O(n)` but `4ⁿ ≠ O(2ⁿ)`. |
| 18 | “Hash tables are `O(1)`” | `O(1)` *expected*. Worst case is `O(n)` (all collisions). Adversarial input can force it. |
| 19 | “Graph algorithms are `O(V+E)`, so linear” | Linear **in the graph size**. For dense graphs `E = Θ(V²)`, so it’s quadratic in `V`. |
| 20 | “Master Theorem always applies” | It fails in the three gaps (§13.3) and for non-constant `a, b`, or unequal subproblem sizes. |
| 21 | “`Ω` means best case” | `Ω` is a lower bound on a function. You can say “worst case is `Ω(n²)`”. |
| 22 | Slicing/copying is free | `a[i:j]` costs `O(j−i)`. A slice inside a loop silently adds a factor of `n`. |
| 23 | “String concatenation in a loop is `O(n)`” | With immutable strings it’s `O(n²)`. Use a list + join, or a StringBuilder. |
| 24 | “`n` in the exponent is the same as `n` in the base” | `n^c` (polynomial) vs `cⁿ` (exponential) — vastly different. |
| 25 | “Two `Θ` functions can always be compared” | Trichotomy fails; oscillating functions may be incomparable (§3.6). |
| 26 | “The best case of quicksort is `O(n)`” | Best case is `Θ(n log n)` — you still must touch each element at each of the `log n` levels. |
| 27 | “Binary search is always `O(log n)`” | Only on a **random-access sorted** structure. On a linked list it degrades to `O(n)`. |
| 28 | “In-place means `O(1)` space” | Often defined as `O(1)` *or* `O(log n)` auxiliary, to allow recursion stacks (quicksort is called in-place). |

---

# 30. Practice Problems with Solutions

## Section A — Asymptotic Notation

**A1.**

Prove

```
2n² + 3n + 1 = Θ(n²)
```

.

- Solution
    
    ```
    n ≥ 1
    ```
    
    ```
    2n²+3n+1 ≤ 2n²+3n²+n² = 6n²
    ```
    
    ```
    c₂=6
    ```
    
    ```
    n ≥ 0
    ```
    
    ```
    2n²+3n+1 ≥ 2n²
    ```
    
    ```
    c₁=2
    ```
    
    ```
    n₀=1
    ```
    
    ```
    2n² ≤ 2n²+3n+1 ≤ 6n²
    ```
    

**A2.**

Is

```
2^{n+1} = O(2ⁿ)
```

? Is

```
2^{2n} = O(2ⁿ)
```

?

- Solution
    
    ```
    2^{n+1} = 2·2ⁿ ≤ 2·2ⁿ
    ```
    
    ```
    c = 2
    ```
    
    ```
    2^{2n} = (2ⁿ)²
    ```
    
    ```
    2^{2n}/2ⁿ = 2ⁿ → ∞
    ```
    

**A3.**

Prove or disprove:

```
f(n) = O(g(n)) ⟹ g(n) = Ω(f(n))
```

.

- Solution
    
    ```
    f ≤ c·g
    ```
    
    ```
    n ≥ n₀
    ```
    
    ```
    g ≥ (1/c)·f
    ```
    
    ```
    n ≥ n₀
    ```
    
    ```
    1/c > 0
    ```
    

**A4.**

Is

```
max(f(n), g(n)) = Θ(f(n) + g(n))
```

for non-negative

```
f, g
```

?

- Solution
    
    ```
    max(f,g) ≤ f+g ≤ 2·max(f,g)
    ```
    
    ```
    c₁ = 1/2
    ```
    
    ```
    c₂ = 1
    ```
    

**A5.**

Order by growth:

```
n^{1/log n}
```

,

```
log(log n)
```

,

```
2^{log n}
```

,

```
(√2)^{log n}
```

,

```
n²
```

,

```
n!
```

,

```
(log n)!
```

,

```
(3/2)ⁿ
```

,

```
n³
```

,

```
log²n
```

,

```
log(n!)
```

,

```
2^{2ⁿ}
```

,

```
n^{1/2}
```

,

```
n·2ⁿ
```

,

```
nᵏ
```

,

```
4^{log n}
```

.

- Solution (log base 2)
    
    Simplify: `n^{1/log n} = 2`; `2^{log n} = n`; `(√2)^{log n} = 2^{(log n)/2} = √n`; `4^{log n} = n²`; `log(n!) = Θ(n log n)`; `(log n)! = 2^{Θ(log n · log log n)} = n^{Θ(log log n)}` — super-polynomial but sub-exponential.
    
    ```
    2  ≺  log log n  ≺  log² n  ≺  √n ≡ (√2)^{log n}  ≺  n ≡ 2^{log n}
      ≺  log(n!) = Θ(n log n)  ≺  n² ≡ 4^{log n}  ≺  n³  ≺  nᵏ
      ≺  (log n)!  ≺  (3/2)ⁿ  ≺  n·2ⁿ  ≺  n!  ≺  2^{2ⁿ}
    ```
    

**A6.**

Which is bigger:

```
2^{√(log n)}
```

or

```
log² n
```

?

- Solution
    
    ```
    √(log n)
    ```
    
    ```
    2 log log n
    ```
    
    ```
    m = log n
    ```
    
    ```
    √m
    ```
    
    ```
    2 log m
    ```
    
    ```
    log m = o(√m)
    ```
    
    ```
    √m
    ```
    
    ```
    2^{√(log n)} = ω(log² n)
    ```
    

**A7.**

True or false: if

```
f(n) = Θ(g(n))
```

then

```
2^{f(n)} = Θ(2^{g(n)})
```

.

- Solution
    
    **False.**
    
    ```
    f(n) = 2n
    ```
    
    ```
    g(n) = n
    ```
    
    ```
    f = Θ(g)
    ```
    
    ```
    2^{2n}/2ⁿ = 2ⁿ → ∞
    ```
    
    **is**
    
    ```
    f(n) − g(n) = O(1)
    ```
    

**A8.**

Show

```
log(n!) = Θ(n log n)
```

without Stirling.

- Solution
    
    ```
    n! ≤ nⁿ
    ```
    
    ```
    log n! ≤ n log n
    ```
    
    ```
    n! ≥ (n/2)^{n/2}
    ```
    
    ```
    n/2
    ```
    
    ```
    n/2
    ```
    
    ```
    log n! ≥ (n/2)(log n − 1) = Ω(n log n)
    ```
    

## Section B — Summations

**B1.**

Evaluate

```
Σ_{i=1}^{n} Σ_{j=1}^{i} 1
```

.

- Solution
    
    ```
    = Σ_{i=1}^{n} i = n(n+1)/2 = Θ(n²)
    ```
    

**B2.**

Evaluate

```
Σ_{i=1}^{n} Σ_{j=i}^{n} Σ_{k=1}^{j} 1
```

asymptotically.

- Solution
    
    ```
    = j
    ```
    
    ```
    Σ_{i=1}^{n} Σ_{j=i}^{n} j = Σ_{i=1}^{n} [n(n+1)/2 − (i−1)i/2] = Θ(n³)
    ```
    

**B3.**

Find

```
Σ_{i=1}^{n} i·2^i
```

.

- Solution
    
    ```
    S = Σ i·2^i
    ```
    
    ```
    2S = Σ i·2^{i+1} = Σ (i−1)2^i
    ```
    
    ```
    i=2..n+1
    ```
    
    ```
    S = 2S − S = n·2^{n+1} − Σ_{i=1}^{n}2^i = n·2^{n+1} − (2^{n+1}−2) = (n−1)2^{n+1} + 2 = Θ(n2ⁿ)
    ```
    

**B4.**

Show

```
Σ_{i=1}^{n} 1/i² = O(1)
```

.

- Solution
    
    ```
    Σ_{i=1}^{n}1/i² ≤ 1 + Σ_{i=2}^{n}1/(i(i−1)) = 1 + Σ(1/(i−1) − 1/i) = 1 + (1 − 1/n) < 2
    ```
    
    ```
    π²/6 ≈ 1.6449
    ```
    

**B5.**

```
Σ_{i=1}^{n} log i
```

— order?

- Solution
    
    ```
    = log(n!) = Θ(n log n)
    ```
    
    ```
    ∫₁ⁿ ln x dx = n ln n − n + 1
    ```
    

## Section C — Recurrences

**C1.**

```
T(n) = 3T(n/2) + n²
```

- Solution
    
    ```
    log₂3 ≈ 1.585 < 2
    ```
    
    ```
    f(n)=n² = Ω(n^{1.585+ε})
    ```
    
    ```
    3(n/2)² = 0.75n² ≤ 0.75 f(n)
    ```
    
    **`Θ(n²)`**
    

**C2.**

```
T(n) = 4T(n/2) + n²log n
```

- Solution
    
    ```
    log₂4 = 2
    ```
    
    ```
    f = n² log n = Θ(n² log¹ n)
    ```
    
    ```
    k=c*=2, p=1
    ```
    
    **`Θ(n² log² n)`**
    

**C3.**

```
T(n) = 2T(n/2) + n/log n
```

- Solution
    
    ```
    c* = 1
    ```
    
    ```
    f = n log^{−1} n
    ```
    
    ```
    k = 1 = c*
    ```
    
    ```
    p = −1
    ```
    
    **`Θ(n log log n)`**
    
    ```
    i
    ```
    
    ```
    2^i · (n/2^i)/log(n/2^i) = n/(log n − i)
    ```
    
    ```
    Σ_{i=0}^{log n −1} n/(log n − i) = n·H_{log n} = Θ(n log log n)
    ```
    

**C4.**

```
T(n) = T(n−1) + 1/n
```

- Solution
    
    ```
    T(n) = Σ_{i=1}^{n}1/i = H_n = Θ(log n)
    ```
    

**C5.**

```
T(n) = 2T(√n) + log n
```

- Solution
    
    ```
    n = 2^m
    ```
    
    ```
    S(m) = T(2^m) = 2S(m/2) + m
    ```
    
    ```
    Θ(m log m)
    ```
    
    **`Θ(log n · log log n)`**
    

**C6.**

```
T(n) = T(n/2) + T(n/3) + T(n/6) + n
```

- Solution
    
    ```
    1/2+1/3+1/6 = 1
    ```
    
    ```
    n
    ```
    
    ```
    Θ(log n)
    ```
    
    **`Θ(n log n)`**
    
    ```
    (1/2)^p+(1/3)^p+(1/6)^p = 1
    ```
    
    ```
    p=1
    ```
    
    ```
    ∫₁ⁿ u/u² du = ln n
    ```
    
    ```
    Θ(n log n)
    ```
    

**C7.**

```
T(n) = T(n/2) + T(n/4) + T(n/8) + n
```

- Solution
    
    ```
    7/8 < 1
    ```
    
    **`Θ(n)`**
    

**C8.**

```
T(n) = √n · T(√n) + n
```

- Solution
    
    ```
    n
    ```
    
    ```
    U(n) = T(n)/n
    ```
    
    ```
    U(n) = U(√n) + 1
    ```
    
    ```
    Θ(log log n)
    ```
    
    **`T(n) = Θ(n log log n)`**
    

**C9.**

Use substitution to prove

```
T(n) = T(⌈n/2⌉) + 1
```

is

```
O(log n)
```

.

- Solution
    
    ```
    T(n) ≤ c·log(n) − b
    ```
    
    ```
    n ≥ 2
    ```
    
    ```
    T(n) ≤ c log⌈n/2⌉ − b + 1 ≤ c log((n+1)/2) − b + 1 = c log(n+1) − c − b + 1 ≤ c log n − b
    ```
    
    ```
    c ≥ 2
    ```
    
    ```
    n ≥ 2
    ```
    
    ```
    log(n+1) ≤ log n + 1
    ```
    
    ```
    n ≥ 1
    ```
    
    ```
    n ≥ 2
    ```
    
    ```
    b
    ```
    

**C10.**

```
T(n) = 2T(n−1) − 1
```

,

```
T(1) = 1
```

. (Note the minus.)

- Solution
    
    ```
    A·2ⁿ
    ```
    
    ```
    p = 2p − 1 → p = 1
    ```
    
    ```
    T(n) = A2ⁿ + 1
    ```
    
    ```
    T(1) = 2A+1 = 1 → A = 0
    ```
    
    **`T(n) = 1`**
    
    ```
    n
    ```
    
    ```
    f(n)
    ```
    

## Section D — Code Analysis

**D1.**

```
int count = 0;
for (int i = n; i > 0; i /= 2)
    for (int j = 0; j < i; j++)
        count++;
```

- Solution
    
    ```
    i
    ```
    
    ```
    n, n/2, n/4, …, 1
    ```
    
    ```
    = n + n/2 + ⋯ + 1 = 2n − 1
    ```
    
    **`Θ(n)`**
    

**D2.**

```
int count = 0;
for (int i = 0; i < n; i++)
    for (int j = i; j < n; j++)
        for (int k = j+1; k < n; k++)
            count++;
```

- Solution
    
    ```
    i ≤ j < k
    ```
    
    ```
    Θ(n³)
    ```
    
    ```
    Σ_{i}Σ_{j≥i}(n−j−1) = Θ(n³)
    ```
    

**D3.**

```
void f(int n) {
    if (n <= 1) return;
    f(n/2); f(n/2); f(n/2); f(n/2);
    for (int i = 0; i < n*n; i++) g();     // g is O(1)
}
```

- Solution
    
    ```
    T(n) = 4T(n/2) + Θ(n²)
    ```
    
    ```
    log₂4 = 2 = k
    ```
    
    ```
    p = 0
    ```
    
    **`Θ(n² log n)`**
    

**D4.**

```
int count = 0;
for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n; j += i)
        count++;
```

- Solution
    
    ```
    ⌈n/i⌉
    ```
    
    ```
    Σ_{i=1}^{n} n/i = n·H_n = Θ(n log n)
    ```
    

**D5.**

```
int i = 1;
while (i < n) {
    int j = n;
    while (j > 1) j = j / 2;
    i = i * 2;
}
```

- Solution
    
    ```
    Θ(log n)
    ```
    
    ```
    Θ(log n)
    ```
    
    **`Θ(log² n)`**
    

**D6.**

```
def foo(n):
    if n <= 1: return 1
    return foo(n-1) + foo(n-1)
```

- Solution
    
    ```
    T(n) = 2T(n−1) + Θ(1)
    ```
    
    **time `Θ(2ⁿ)`**
    
    **space `Θ(n)`**
    
    ```
    n
    ```
    

**D7.** Time and space of:

```
def rec(n):
    if n <= 0: return
    arr = [0] * n          # allocates n
    rec(n - 1)
```

- Solution
    
    ```
    Σ_{i=1}^{n} Θ(i) = Θ(n²)
    ```
    
    ```
    n
    ```
    
    ```
    Σ i = Θ(n²)
    ```
    

**D8.**

```
for (int i = 2; i <= n; i = i * i)
    print(i);
```

- Solution
    
    ```
    i
    ```
    
    ```
    2, 4, 16, 256, …
    ```
    
    ```
    2^{2^t}
    ```
    
    ```
    2^{2^t} > n
    ```
    
    ```
    t > log log n
    ```
    
    **`Θ(log log n)`**
    

**D9.** What is the time complexity?

```
def isPrime(n):
    if n < 2: return False
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0: return False
    return True
```

- Solution
    
    ```
    Θ(√n)
    ```
    
    ```
    n
    ```
    
    **But**
    
    ```
    b = log n
    ```
    
    ```
    Θ(2^{b/2})
    ```
    
    **exponential**
    
    ```
    Õ(b⁶)
    ```
    

**D10.**

```
def f(arr):
    result = []
    for x in arr:
        if x not in result:      # list membership
            result.append(x)
    return result
```

- Solution
    
    ```
    x not in result
    ```
    
    ```
    O(len(result)) = O(n)
    ```
    
    **`O(n²)` time**
    
    ```
    O(n)
    ```
    
    ```
    set
    ```
    
    ```
    O(n)
    ```
    
    ```
    O(n)
    ```
    

## Section E — Mixed / Conceptual

**E1.**

An algorithm has

```
T(n) = 3T(n/3) + n/log n
```

. Solve it.

- Solution
    
    ```
    c* = log₃3 = 1
    ```
    
    ```
    f = n·log^{−1} n
    ```
    
    ```
    k = 1 = c*
    ```
    
    ```
    p = −1
    ```
    
    **`Θ(n log log n)`**
    

**E2.**

You have two algorithms: A runs in

```
Θ(n^{2.81})
```

(Strassen), B in

```
Θ(n³)
```

with a 10× smaller constant. For what

```
n
```

is A better?

- Solution
    
    ```
    c·n^{2.81} < 10c·n³
    ```
    
    ```
    n^{0.19} > 1/10
    ```
    
    ```
    n^{2.81} < 10 n³ ⟺ 1 < 10 n^{0.19}
    ```
    
    ```
    10c
    ```
    
    ```
    10c n^{2.81} < c n³ ⟺ 10 < n^{0.19} ⟺ n > 10^{1/0.19} ≈ 10^{5.26} ≈ 180,000
    ```
    
    ```
    n ≈ 500–1000
    ```
    

**E3.**

Why is build-heap

```
Θ(n)
```

but heap-sort

```
Θ(n log n)
```

?

- Solution
    
    *down*
    
    ```
    Σ n/2^{h+1}·h = O(n)
    ```
    
    ```
    n
    ```
    
    *leaf value*
    
    ```
    Θ(log n)
    ```
    
    ```
    Θ(n log n)
    ```
    
    ```
    n log n
    ```
    
    ```
    n
    ```
    

**E4.**

An algorithm processes a list; it removes half of the elements at each step but does

```
Θ(n²)
```

work at each step on the current list size. Complexity?

- Solution
    
    ```
    T(n) = T(n/2) + n²
    ```
    
    ```
    n² + n²/4 + n²/16 + ⋯ = n²·(4/3) = Θ(n²)
    ```
    
    ```
    c* = log₂1 = 0
    ```
    
    ```
    f = n² = Ω(n^{0+ε})
    ```
    
    ```
    Θ(n²)
    ```
    

**E5.**

Prove that any algorithm that must output all

```
n!
```

permutations takes

```
Ω(n·n!)
```

time.

- Solution
    
    ```
    n
    ```
    
    ```
    Ω(n)
    ```
    
    ```
    n!
    ```
    
    ```
    Ω(n·n!)
    ```
    
    **output-size lower bound**
    

**E6.**

Merge sort on a linked list — what are the time and space complexities, and why does the space differ from the array version?

- Solution
    
    ```
    Θ(n log n)
    ```
    
    ```
    Θ(log n)
    ```
    
    ```
    Θ(n)
    ```
    

**E7.**

Is

```
Θ(n) ⊆ O(n²)
```

? Is

```
O(n²) ⊆ Θ(n)
```

?

- Solution
    
    ```
    Θ(n)
    ```
    
    ```
    n²  ∈ O(n²)
    ```
    
    ```
    n² ∉ Θ(n)
    ```
    

**E8.**

A recursive function has recursion tree with branching factor 3, depth

```
log₂ n
```

, and

```
Θ(1)
```

work per node. Complexity?

- Solution
    
    ```
    = Σ_{i=0}^{log₂ n} 3^i = Θ(3^{log₂ n}) = Θ(n^{log₂3}) = Θ(n^{1.585})
    ```
    
    ```
    T(n) = 3T(n/2) + Θ(1)
    ```
    

---

## Final Checklist for Analysing Any Algorithm

- [ ]  **Define `n`** precisely. Is it elements, bits, vertices+edges, or a value?
- [ ]  **Identify the model** — is every operation really `O(1)`? (bignums, string ops, slicing)
- [ ]  **Iterative?** Write the summation for each loop. Check dependence between loop bounds.
- [ ]  **Recursive?** Write the recurrence: number of calls × subproblem size + non-recursive work.
- [ ]  **Solve** via Master Theorem → extended MT → Akra–Bazzi → recursion tree → substitution.
- [ ]  **Check hidden costs** in library calls and data-structure operations.
- [ ]  **State the case** — best / average / worst / amortized / expected.
- [ ]  **Space:** auxiliary data structures + max recursion depth × frame size.
- [ ]  **Simplify:** drop constants, drop lower-order terms, take the max of sequential parts.
- [ ]  **Sanity-check** against known bounds (e.g. must be `Ω(n)` if all input is read; must be `Ω(output size)`).
- [ ]  **Verify empirically** if possible: double `n` and see if the runtime doubles (`Θ(n)`), quadruples (`Θ(n²)`), or barely changes (`Θ(log n)`).

---

*End of document.*