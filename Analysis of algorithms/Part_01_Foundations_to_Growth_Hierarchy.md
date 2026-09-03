# Analysis of Algorithms — The Complete Reference

> A single go-to document covering asymptotic notation, the mathematics behind it, recurrence relations, summations, logarithms, and step-by-step time & space complexity analysis of real algorithms.
> 

---

## Table of Contents

1. [Foundations](about:blank#1-foundations)
2. [Asymptotic Notations — Formal Definitions](about:blank#2-asymptotic-notations--formal-definitions)
3. [Properties & Algebra of Asymptotic Notation](about:blank#3-properties--algebra-of-asymptotic-notation)
4. [The Limit Method](about:blank#4-the-limit-method-comparing-two-functions)
5. [Ordering of Functions (Growth Hierarchy)](about:blank#5-ordering-of-functions-growth-hierarchy)
6. [Mathematical Toolkit — Logarithms & Exponents](about:blank#6-mathematical-toolkit--logarithms--exponents)
7. [Mathematical Toolkit — Floors, Ceilings, Factorials](about:blank#7-mathematical-toolkit--floors-ceilings-factorials)
8. [Summation Series — The Complete Catalogue](about:blank#8-summation-series--the-complete-catalogue)
9. [Bounding Summations](about:blank#9-bounding-summations-techniques)
10. [Recurrence Relations — Setting Them Up](about:blank#10-recurrence-relations--setting-them-up)
11. [Solving Recurrences: Substitution Method](about:blank#11-solving-recurrences-i-substitution-method)
12. [Solving Recurrences: Recursion Tree](about:blank#12-solving-recurrences-ii-recursion-tree-method)
13. [Solving Recurrences: Master Theorem](about:blank#13-solving-recurrences-iii-master-theorem)
14. [Solving Recurrences: Extended & Alternative Theorems](about:blank#14-solving-recurrences-iv-extended--alternative-theorems)
15. [Solving Recurrences: Characteristic Equations](about:blank#15-solving-recurrences-v-linear-recurrences--characteristic-equations)
16. [Solving Recurrences: Substitution of Variables & Generating Functions](about:blank#16-solving-recurrences-vi-change-of-variable--generating-functions)
17. [Computing Time Complexity from Code](about:blank#17-computing-time-complexity-from-code)
18. [Loop Analysis Patterns — Exhaustive Catalogue](about:blank#18-loop-analysis-patterns--exhaustive-catalogue)
19. [Space Complexity](about:blank#19-space-complexity)
20. [Worked Analyses — Iterative Algorithms](about:blank#20-worked-analyses--iterative-algorithms)
21. [Worked Analyses — Divide & Conquer](about:blank#21-worked-analyses--divide--conquer)
22. [Worked Analyses — Trees, Graphs, Heaps](about:blank#22-worked-analyses--trees-graphs-heaps)
23. [Worked Analyses — Dynamic Programming & Backtracking](about:blank#23-worked-analyses--dynamic-programming--backtracking)
24. [Amortized Analysis](about:blank#24-amortized-analysis)
25. [Probabilistic / Average-Case Analysis](about:blank#25-probabilistic--average-case-analysis)
26. [Lower Bounds & Adversary Arguments](about:blank#26-lower-bounds--adversary-arguments)
27. [Complexity Classes](about:blank#27-complexity-classes-p-np-np-complete)
28. [Master Cheat Sheets](about:blank#28-master-cheat-sheets)
29. [Common Pitfalls](about:blank#29-common-pitfalls-and-misconceptions)
30. [Practice Problems with Solutions](about:blank#30-practice-problems-with-solutions)

---

# 1. Foundations

## 1.1 What “Analysis of Algorithms” Means

Analysis of an algorithm is the process of **predicting the resources the algorithm requires**, as a function of the size of its input, *without running it*.

The two resources we care about:

| Resource | Measure | Name |
| --- | --- | --- |
| Processor time | Number of primitive operations executed | **Time complexity** |
| Memory | Number of memory cells occupied | **Space complexity** |

Other resources sometimes analysed: number of comparisons, number of swaps, number of disk I/Os (external memory model), number of cache misses, communication bits, number of processors (parallel).

## 1.2 The Model of Computation — RAM Model

All standard analysis assumes the **Random Access Machine (RAM)** model:

1. **Sequential execution** — one instruction at a time, no concurrency.
2. **Constant-time primitive operations**:
    - arithmetic: `+ - * / %`, bit operations `& | ^ << >>`
    - comparison: `< > <= >= == !=`
    - assignment / data movement: `x = y`
    - control flow: function call/return, branch, indexing `A[i]`
3. **Uniform cost criterion** — each primitive operation costs 1 unit, regardless of operand values.
4. **Word size** = `w` bits, where `w ≥ log₂ n`. A machine word can hold any array index. Integers fit in O(1) words.
5. **Memory access is O(1)** regardless of address (no cache hierarchy modelled).

> ⚠️ **Where the model breaks:** multiplying two `n`-bit numbers is *not* O(1) if `n` is huge (bignum arithmetic). Exponentiation `2^n` is not O(1). In such cases use the **logarithmic cost criterion**, where an operation on a `k`-bit number costs O(k).
> 

## 1.3 Input Size — `n`

`n` must be precisely defined per problem. This is the single most common source of confusion.

| Problem | Input size `n` |
| --- | --- |
| Sorting an array | number of elements |
| Graph algorithms | usually **two** parameters: `V` (vertices), `E` (edges) |
| Matrix operations on `n×n` | `n` (side), so input has `n²` numbers |
| Multiplying two integers | number of **bits/digits**, i.e. `n = ⌊log₂ x⌋ + 1` |
| Primality of integer `x` | `n = log₂ x` bits — *not* `x` |
| String algorithms | length of string(s) |
| Polynomial of degree d | `d + 1` coefficients |

### Pseudo-polynomial time

An algorithm is **pseudo-polynomial** if its running time is polynomial in the *numeric value* of the input but exponential in the *number of bits*.

Example: 0/1 Knapsack DP runs in `O(nW)`. `W` needs `log W` bits, so in terms of true input size `b = log W`, the time is `O(n·2^b)` — exponential. Hence Knapsack is NP-hard yet has an `O(nW)` algorithm.

## 1.4 Basic / Elementary Operation

Choose one operation whose count is proportional to total running time:

| Algorithm | Basic operation |
| --- | --- |
| Searching / Sorting | key comparison |
| Matrix multiplication | scalar multiplication |
| Graph traversal | edge examination |
| Polynomial evaluation | multiplication |
| Euclid’s GCD | modulo operation |

Total time: `T(n) = c_op × C(n)` where `C(n)` is the number of basic operations and `c_op` is the constant time per operation.

## 1.5 Cases of Analysis

Let `D_n` = set of all inputs of size `n`, and `t(I)` = running time on input `I`.

| Case | Definition | Meaning |
| --- | --- | --- |
| **Worst case** | `W(n) = max_{I ∈ D_n} t(I)` | Guarantee — never slower than this |
| **Best case** | `B(n) = min_{I ∈ D_n} t(I)` | Rarely useful; shows lower limit |
| **Average case** | `A(n) = Σ_{I ∈ D_n} P(I) · t(I)` | Expected time over an input distribution |
| **Amortized** | `total cost of m ops / m` | Worst-case *average* over a sequence, no probability |

Key notes:
- **Worst case is the default** when unqualified. It gives a guarantee and is often as easy to compute as average case.
- Average case **requires an assumption about input distribution** (usually “all permutations equally likely”). Without stating the distribution, “average case” is meaningless.
- Average case is **not** the average of best and worst.
- **Amortized ≠ average.** Amortized analysis involves no randomness; it is a deterministic worst-case guarantee over a sequence of operations.
- **Expected time ≠ average case** for *randomized* algorithms: there the randomness is in the algorithm’s coin flips, not in the input. Randomized QuickSort has *expected* `O(n log n)` on **every** input.

### Example: Linear Search

```
LINEAR-SEARCH(A, n, key):
    for i = 0 to n-1:
        if A[i] == key:
            return i
    return -1
```

- Best case: key at index 0 → 1 comparison → `Θ(1)`
- Worst case: key at last index or absent → `n` comparisons → `Θ(n)`
- Average case (successful search, key equally likely at any of `n` positions):
    
    ```
    A(n) = Σ_{i=1}^{n} i · (1/n) = (1/n) · n(n+1)/2 = (n+1)/2 = Θ(n)
    ```
    
- Average case with probability `p` of success (key uniformly placed if present):
    
    ```
    A(n) = Σ_{i=1}^{n} i·(p/n) + n(1-p) = p(n+1)/2 + n(1-p)
    ```
    
    For `p = 1`: `(n+1)/2`. For `p = 0`: `n`. For `p = 1/2`: `(n+1)/4 + n/2 ≈ 3n/4`.
    

## 1.6 Why Asymptotics? The Justification

Exact operation counts are:
- machine-dependent (constants differ per CPU),
- compiler-dependent,
- tedious and error-prone,
- and **irrelevant for large `n`**.

Growth rate dominates. Consider `T₁(n) = 100n` vs `T₂(n) = n²`:

| n | 100n | n² | Winner |
| --- | --- | --- | --- |
| 10 | 1,000 | 100 | n² |
| 100 | 10,000 | 10,000 | tie |
| 1,000 | 100,000 | 1,000,000 | 100n |
| 10⁶ | 10⁸ | 10¹² | 100n by 10,000× |

The **breakeven point** `n₀ = 100`; beyond it the asymptotically better algorithm wins forever, and the gap widens without bound.

### Effect of a 10× faster machine

If an algorithm solves size `n` in time `T`, on a 10× faster machine in the same time it solves:

| T(n) | New size solvable |
| --- | --- |
| `log n` | `n¹⁰` |
| `n` | `10n` |
| `n log n` | slightly less than `10n` |
| `n²` | `3.16 n` |
| `n³` | `2.15 n` |
| `2ⁿ` | `n + 3.3` |
| `n!` | `n + 1` (approximately) |

**Conclusion:** for exponential algorithms, hardware improvements are nearly worthless. Better algorithms are the only path.

### Practical size limits (assuming 10⁸ ops/sec, 1 second budget)

| Complexity | Max feasible n |
| --- | --- |
| `O(log n)` | astronomically large |
| `O(√n)` | ~10¹⁶ |
| `O(n)` | ~10⁸ |
| `O(n log n)` | ~10⁶ – 10⁷ |
| `O(n √n)` | ~10⁵ |
| `O(n²)` | ~10⁴ |
| `O(n³)` | ~500 |
| `O(2ⁿ)` | ~25 |
| `O(n! )` | ~11 |
| `O(nⁿ)` | ~9 |

---

# 2. Asymptotic Notations — Formal Definitions

Throughout, `f, g : ℕ → ℝ⁺` are asymptotically non-negative functions.

## 2.1 Big-O — Asymptotic Upper Bound

> **Definition.**`O(g(n)) = { f(n) : ∃ positive constants c, n₀ such that 0 ≤ f(n) ≤ c·g(n) for all n ≥ n₀ }`
> 

We write `f(n) = O(g(n))` (an abuse of notation for `f(n) ∈ O(g(n))`).

**Meaning:** `f` grows *no faster than* `g`. It is an **upper bound**, not necessarily tight.

```
        c·g(n)
       /
      /   f(n)
     / __/
    /_/
   ------------------→ n
        n₀
```

**Proof template.** To show `f(n) = O(g(n))`, exhibit specific `c` and `n₀`.

> **Example 1.** Show `3n² + 5n + 7 = O(n²)`.
For `n ≥ 1`: `3n² + 5n + 7 ≤ 3n² + 5n² + 7n² = 15n²`.
Take `c = 15, n₀ = 1`. ∎
> 
> 
> Tighter: for `n ≥ 7`, `5n + 7 ≤ 5n + n = 6n ≤ 6n²`, so `f(n) ≤ 9n²`; `c = 9, n₀ = 7`.
> *(Any valid pair works — you don’t need the smallest.)*
> 

> **Example 2.** Show `n² + 3n = O(n³)`. For `n ≥ 1`, `n² + 3n ≤ n³ + 3n³ = 4n³`. `c = 4, n₀ = 1`. ✔
This is **correct but not tight** — Big-O permits loose bounds.
> 

> **Example 3 (disproof).** Show `n³ ≠ O(n²)`.
Suppose `n³ ≤ c·n²` for all `n ≥ n₀`. Dividing by `n²` gives `n ≤ c` for all `n ≥ n₀` — contradiction, since `n` is unbounded. ∎
> 

## 2.2 Big-Omega (Ω) — Asymptotic Lower Bound

> **Definition.**`Ω(g(n)) = { f(n) : ∃ c, n₀ > 0 such that 0 ≤ c·g(n) ≤ f(n) for all n ≥ n₀ }`
> 

**Meaning:** `f` grows *at least as fast as* `g`.

> **Example.** `3n² + 5n + 7 = Ω(n²)`: for `n ≥ 0`, `3n² + 5n + 7 ≥ 3n²`. `c = 3, n₀ = 1`. ∎
Also `3n² + 5n + 7 = Ω(n)` and `= Ω(1)` — loose but valid lower bounds.
> 

## 2.3 Big-Theta (Θ) — Asymptotic Tight Bound

> **Definition.**`Θ(g(n)) = { f(n) : ∃ c₁, c₂, n₀ > 0 such that 0 ≤ c₁·g(n) ≤ f(n) ≤ c₂·g(n) for all n ≥ n₀ }`
> 

```
        c₂·g(n)
       /
      /  f(n)
     /  /  c₁·g(n)
    / /  /
   -----------------→ n
       n₀
```

**Fundamental Theorem:**
> `f(n) = Θ(g(n))  ⟺  f(n) = O(g(n))  AND  f(n) = Ω(g(n))`

This is the standard proof strategy for Θ.

> **Example.** Prove `½n² − 3n = Θ(n²)`.
Need `c₁n² ≤ ½n² − 3n ≤ c₂n²` for `n ≥ n₀`. Divide by `n²`:
`c₁ ≤ ½ − 3/n ≤ c₂`.
- Right: `½ − 3/n ≤ ½`, so `c₂ = ½` works for all `n ≥ 1`.
- Left: need `½ − 3/n ≥ c₁`. At `n = 7`: `½ − 3/7 = 1/14`. So `c₁ = 1/14, n₀ = 7`. ∎
> 

**Θ of a polynomial:** For `p(n) = a_d n^d + a_{d−1} n^{d−1} + ⋯ + a₀` with `a_d > 0`, we have `p(n) = Θ(n^d)`.

*Proof sketch:* `p(n)/n^d → a_d` as `n → ∞`, a positive constant. Formally, for `n ≥ n₀`, `(a_d/2)n^d ≤ p(n) ≤ 2a_d n^d`.

## 2.4 Little-o — Strict Upper Bound

> **Definition.**`o(g(n)) = { f(n) : ∀ c > 0, ∃ n₀ > 0 such that 0 ≤ f(n) < c·g(n) for all n ≥ n₀ }`
> 

Note the quantifier flip: **for every** `c` (however small), eventually `f < c·g`.

**Equivalent limit form:** `f(n) = o(g(n)) ⟺ lim_{n→∞} f(n)/g(n) = 0`

**Meaning:** `f` is *dominated by* `g`; `g` grows strictly faster.

Examples: `2n = o(n²)` ✔, `n log n = o(n²)` ✔, `2n² = o(n²)` ✘ (limit is 2, not 0).

## 2.5 Little-omega (ω) — Strict Lower Bound

> **Definition.**`ω(g(n)) = { f(n) : ∀ c > 0, ∃ n₀ > 0 such that 0 ≤ c·g(n) < f(n) for all n ≥ n₀ }`
> 

**Limit form:** `f(n) = ω(g(n)) ⟺ lim_{n→∞} f(n)/g(n) = ∞`

**Duality:** `f(n) = ω(g(n)) ⟺ g(n) = o(f(n))`

## 2.6 The Analogy Table

| Asymptotic | Numeric analogue | Limit `f/g` | Meaning |
| --- | --- | --- | --- |
| `f = O(g)` | `a ≤ b` | `< ∞` (finite, incl. 0) | at most |
| `f = Ω(g)` | `a ≥ b` | `> 0` (incl. ∞) | at least |
| `f = Θ(g)` | `a = b` | `= c`, `0 < c < ∞` | exactly (order) |
| `f = o(g)` | `a < b` | `= 0` | strictly less |
| `f = ω(g)` | `a > b` | `= ∞` | strictly greater |

Also:
- `O = o ∪ Θ` (informally), `Ω = ω ∪ Θ`
- `o(g) ⊂ O(g)` and `ω(g) ⊂ Ω(g)` (strict subsets)
- `Θ(g) = O(g) ∩ Ω(g)`
- `o(g) ∩ ω(g) = ∅`

## 2.7 Soft-O and Other Notations

| Notation | Definition | Use |
| --- | --- | --- |
| `Õ(g(n))` (soft-O) | `O(g(n) · log^k n)` for some `k` | hides polylog factors |
| `O*(g(n))` | ignores polynomial factors, keeps exponential | e.g. `O*(2ⁿ)` |
| `f ~ g` (asymptotic equality) | `lim f/g = 1` | precise leading term, e.g. `n! ~ √(2πn)(n/e)ⁿ` |
| `f ≪ g` | same as `f = o(g)` | Vinogradov notation |
| `f ≍ g` | same as `f = Θ(g)` |  |

## 2.8 The Equals-Sign Abuse & One-Way Equalities

`f(n) = O(g(n))` really means `f(n) ∈ O(g(n))`. Consequences:

- **Not symmetric.** `n = O(n²)` is true; `O(n²) = n` is meaningless.
- In a formula, `O(·)` denotes *some anonymous function*:
`2n² + Θ(n)` means “`2n² + h(n)` for some `h(n) ∈ Θ(n)`”.
- On the **left** of `=`, it means “for any choice”:
`2n² + Θ(n) = Θ(n²)` means: for *any* `h ∈ Θ(n)`, there *exists* `k ∈ Θ(n²)` with `2n² + h(n) = k(n)`.
- Chains read left-to-right: `T(n) = 2T(n/2) + Θ(n) = Θ(n log n)`.

> ⚠️ **Never write `O(f) = O(g)` and treat it as an equation.** Never “cancel” O-terms: from `f + O(n) = g + O(n)` you cannot conclude `f = g`.
> 

## 2.9 Common Correct/Incorrect Statements

| Statement | Verdict | Reason |
| --- | --- | --- |
| `n = O(n²)` | ✔ | valid loose upper bound |
| `n² = O(n)` | ✘ | `n²/n = n → ∞` |
| `2ⁿ⁺¹ = O(2ⁿ)` | ✔ | `2ⁿ⁺¹ = 2·2ⁿ`, `c = 2` |
| `2²ⁿ = O(2ⁿ)` | ✘ | `2²ⁿ/2ⁿ = 2ⁿ → ∞` |
| `(n+1)! = O(n!)` | ✘ | ratio is `n+1 → ∞` |
| `log(n!) = Θ(n log n)` | ✔ | Stirling |
| `n! = O(nⁿ)` | ✔ | `n! ≤ nⁿ` |
| `n! = Ω(2ⁿ)` | ✔ | for `n ≥ 4` |
| `log² n = O(n)` | ✔ | polylog < poly |
| `√n = O(log n)` | ✘ | reverse is true |
| `2^{log n} = Θ(n)` | ✔ | if base 2, `= n` |
| `f(n) = O(f(n)²)` | ✘ in general | fails if `f(n) = 1/n` or `f < 1` |
| `max(f,g) = Θ(f+g)` | ✔ | see §3.7 |

---

# 3. Properties & Algebra of Asymptotic Notation

## 3.1 Reflexivity

For all `f`:

```
f(n) = O(f(n))       ✔  (c = 1)
f(n) = Ω(f(n))       ✔
f(n) = Θ(f(n))       ✔
f(n) = o(f(n))       ✘  (limit is 1, not 0)
f(n) = ω(f(n))       ✘
```

## 3.2 Symmetry

```
f = Θ(g)  ⟺  g = Θ(f)          ✔ Θ is symmetric
```

O, Ω, o, ω are **not** symmetric.

## 3.3 Transpose Symmetry (Duality)

```
f(n) = O(g(n))  ⟺  g(n) = Ω(f(n))
f(n) = o(g(n))  ⟺  g(n) = ω(f(n))
```

## 3.4 Transitivity — holds for **all five**

```
f = O(g)  and  g = O(h)   ⟹  f = O(h)
f = Ω(g)  and  g = Ω(h)   ⟹  f = Ω(h)
f = Θ(g)  and  g = Θ(h)   ⟹  f = Θ(h)
f = o(g)  and  g = o(h)   ⟹  f = o(h)
f = ω(g)  and  g = ω(h)   ⟹  f = ω(h)
```

**Mixed transitivity** (useful):

```
f = O(g)  and  g = o(h)   ⟹  f = o(h)
f = o(g)  and  g = O(h)   ⟹  f = o(h)
f = Θ(g)  and  g = O(h)   ⟹  f = O(h)
```

## 3.5 Θ is an Equivalence Relation

Since Θ is reflexive, symmetric, and transitive, it **partitions the set of functions into equivalence classes** — the “orders of growth”. `Θ(n²)` is one class; every function in it grows at the same rate up to constants.

`O` and `Ω` are **preorders** (reflexive + transitive, not antisymmetric). They induce a partial order on the Θ-classes.

## 3.6 Trichotomy Fails

For real numbers, exactly one of `a < b`, `a = b`, `a > b` holds. **For functions this fails** — two functions may be incomparable.

> **Counterexample:** `f(n) = n`, `g(n) = n^{1 + sin n}`.
The exponent `1 + sin n` oscillates in `[0, 2]`, so `g` is sometimes `n⁰ = 1` and sometimes `n²`.
Neither `f = O(g)` nor `f = Ω(g)` holds.
> 
> 
> Simpler: `f(n) = n`, `g(n) = n` if `n` even, `n²` if `n` odd.
> 

## 3.7 Arithmetic Rules

Let `f₁ = O(g₁)` and `f₂ = O(g₂)`.

### Sum Rule

```
f₁(n) + f₂(n) = O( max(g₁(n), g₂(n)) )  =  O(g₁(n) + g₂(n))
```

**Key identity:** `max(a,b) ≤ a + b ≤ 2·max(a,b)`, hence for non-negative functions

```
Θ( f + g ) = Θ( max(f, g) )
```

This justifies **dropping lower-order terms**.

*Application:* sequential code blocks → take the max.

```
Block A: O(n²)
Block B: O(n log n)
Total:   O(n² + n log n) = O(n²)
```

### Product Rule

```
f₁(n) · f₂(n) = O( g₁(n) · g₂(n) )
```

*Application:* nested loops multiply.

Also: `O(c · f(n)) = O(f(n))` for constant `c > 0` — **constants are dropped**.

### Division / Subtraction — **NO general rule**

```
O(g₁) − O(g₂) ≠ O(g₁ − g₂)     ✘ INVALID
```

Counterexample: `f₁ = n² = O(n²)`, `f₂ = n² − n = O(n²)`, but `f₁ − f₂ = n ≠ O(0)`.

### Transitive Composition

If `f = O(g)` then `c·f = O(g)` for constant `c > 0`, and `f^k = O(g^k)` for constant `k > 0`.

> ⚠️ **Exponentiation is NOT preserved:** `f = O(g)` does **not** imply `2^f = O(2^g)`.
Counterexample: `2n = O(n)` but `2^{2n} = 4ⁿ ≠ O(2ⁿ)`.
> 

> ⚠️ **Logarithm side:** `f = O(g)` does imply `log f = O(log g)` **only under conditions** (`f, g ≥ 1` and `log g = Ω(1)` — generally safe in practice, but note `log(2ⁿ) = n` and `log(3ⁿ) = n log 3` are Θ-equal even though `2ⁿ = o(3ⁿ)`). **Logs compress differences.**
> 

## 3.8 Summation Property

If `f(i) = O(g(i))` for each `i`, then

```
Σ_{i=1}^{n} f(i) = O( Σ_{i=1}^{n} g(i) )
```

provided the constants are uniform (a single `c` and `n₀` work for all `i`).

> ⚠️ **Common error:** `Σ_{i=1}^{n} O(1) = O(n)`, **not** `O(1)`. The number of terms depends on `n`, so the “constant” cannot be pulled out of a variable-length sum.
> 

## 3.9 Polynomial & Exponential Facts

For constants `a, b, ε > 0`:

```
n^a = o(n^b)              if a < b
log^a n = o(n^ε)          polylog is always beaten by any polynomial
n^a = o(bⁿ)               if b > 1: any polynomial beaten by any exponential
aⁿ = o(bⁿ)                if a < b
bⁿ = o(n!)                for any constant b
n! = o(nⁿ)
n^{log n} = o(2^n)?       NO — compare: n^{log₂n} = 2^{(log₂ n)²}, and (log n)² = o(n), so n^{log n} = o(2ⁿ) ✔
```

**Fundamental hierarchy limits:**

```
lim_{n→∞} n^k / aⁿ = 0        for a > 1, any constant k   (exponential beats polynomial)
lim_{n→∞} log^k n / n^ε = 0   for any ε > 0                (polynomial beats polylog)
lim_{n→∞} aⁿ / n! = 0         for any constant a           (factorial beats exponential)
```

## 3.10 Transformations to Remember

```
a^{log_b n} = n^{log_b a}          ← extremely useful identity
2^{log₂ n} = n
n^{1/log n} = 2                     (base-2 log)
(log n)^{log n} = n^{log log n}
```

---

# 4. The Limit Method (Comparing Two Functions)

This is the **fastest practical tool** for comparing growth rates.

## 4.1 The Rule

Compute `L = lim_{n→∞} f(n) / g(n)`.

| Value of L | Conclusion |
| --- | --- |
| `L = 0` | `f = o(g)`, hence `f = O(g)`, and `f ≠ Θ(g)`, `f ≠ Ω(g)` |
| `L = c`, `0 < c < ∞` | `f = Θ(g)` (also `O` and `Ω`) |
| `L = ∞` | `f = ω(g)`, hence `f = Ω(g)`, and `f ≠ O(g)` |
| limit does not exist | inconclusive — use definitions directly |

## 4.2 L’Hôpital’s Rule

If `lim f(n) = lim g(n) = ∞` (or both → 0), and `f, g` are differentiable, then

```
lim_{n→∞} f(n)/g(n) = lim_{n→∞} f'(n)/g'(n)
```

(treating `n` as a real variable). Apply repeatedly if needed.

> **Example 1.** Compare `f(n) = log₂ n` and `g(n) = √n`.
> 
> 
> ```
> lim (log₂ n)/√n  = lim ( (1/(n ln 2)) / (1/(2√n)) )
>                   = lim ( 2√n / (n ln 2) )
>                   = lim ( 2 / (√n · ln 2) ) = 0
> ```
> 
> So `log n = o(√n)`. ✔
> 

> **Example 2.** Compare `f(n) = n²` and `g(n) = 2ⁿ`.
> 
> 
> ```
> lim n²/2ⁿ →(L'H) lim 2n/(2ⁿ ln 2) →(L'H) lim 2/(2ⁿ ln²2) = 0
> ```
> 
> So `n² = o(2ⁿ)`. ✔
> 

> **Example 3.** Compare `f(n) = n^{1/2}` and `g(n) = n^{2/3}`.
`lim n^{1/2}/n^{2/3} = lim n^{-1/6} = 0` → `√n = o(n^{2/3})`. ✔
> 

> **Example 4.** Compare `f(n) = 2ⁿ` and `g(n) = 3ⁿ`.
`lim (2/3)ⁿ = 0` → `2ⁿ = o(3ⁿ)`. ✔
But `log(2ⁿ) = n` and `log(3ⁿ) = n log 3` are `Θ`-equal. Logs destroy this distinction.
> 

> **Example 5 (trap).** Compare `f(n) = n^{1.001}` and `g(n) = n log n`.
`lim n^{1.001}/(n log n) = lim n^{0.001}/log n`. Take logs: `0.001 ln n − ln ln n → ∞`. So limit is `∞`.
**`n^{1.001} = ω(n log n)`.** Any polynomial power above 1 eventually beats `n log n`.
> 

## 4.3 The Logarithm Trick

When functions are huge or ratios are messy, compare `log f(n)` vs `log g(n)`:

> If `log f(n) − log g(n) → ∞`, then `f/g → ∞`, so `f = ω(g)`.
If `log f(n) − log g(n) → −∞`, then `f = o(g)`.
> 

⚠️ **Caution:** If `log f − log g → c` (a finite constant), then `f/g → 2^c`, so `f = Θ(g)`. But if `log f = Θ(log g)` only, you can conclude *nothing* — e.g. `log(2ⁿ) = n` and `log(4ⁿ) = 2n` are Θ-equal but `2ⁿ = o(4ⁿ)`.

> **Example.** Compare `f(n) = n^{log n}` and `g(n) = 2^{√n}` (log base 2).
`log f = (log n)²`, `log g = √n`.
Since `(log n)² = o(√n)` (polylog < polynomial), `log f − log g → −∞`.
Therefore `n^{log n} = o(2^{√n})`. ✔
> 

## 4.4 The Ratio Test (for discrete comparison)

`lim f(n+1)/f(n)` characterizes the class:
- `= 1` → sub-exponential (polynomial, polylog)
- `= c > 1` → exponential
- `= ∞` → super-exponential (factorial, `nⁿ`)

---

# 5. Ordering of Functions (Growth Hierarchy)

## 5.1 The Master Ordering (slowest → fastest)

```
1  ≺  α(n)  ≺  log* n  ≺  log log n  ≺  √(log n)  ≺  log n  ≺  log² n  ≺  log^k n
   ≺  n^ε  ≺  √n  ≺  n^{2/3}  ≺  n  ≺  n log* n  ≺  n log log n  ≺  n log n
   ≺  n log² n  ≺  n^{1.5}  ≺  n²  ≺  n² log n  ≺  n³  ≺  n^k
   ≺  n^{log log n}  ≺  n^{log n} = 2^{log² n}  ≺  2^{√n}  (careful, see note)
   ≺  2ⁿ  ≺  3ⁿ  ≺  cⁿ  ≺  n!  ≺  (n+1)!  ≺  nⁿ  ≺  2^{n²}  ≺  2^{2ⁿ}
```

(`≺` means `o(·)`; `ε` is any constant with `0 < ε < 1`; `k ≥ 1` constant.)

> **Correction note on ordering of `2^{√n}` vs `n^{log n}`:**`log(n^{log n}) = (log n)²` and `log(2^{√n}) = √n`.
Since `(log n)² = o(√n)`, we get **`n^{log n} ≺ 2^{√n}`**. ✔ (as listed)
And `√n = o(n)` so `2^{√n} ≺ 2ⁿ`. ✔
> 

## 5.2 The Five Growth Tiers

| Tier | Forms | Character |
| --- | --- | --- |
| **Constant** | `O(1)` | independent of n |
| **Sub-linear** | `log* n`, `log log n`, `log n`, `log^k n`, `n^ε` (ε<1), `√n` | doesn’t read whole input |
| **Linear/quasi-linear** | `n`, `n log log n`, `n log n`, `n log^k n` | optimal for most problems |
| **Polynomial** | `n²`, `n³`, `n^k` | “tractable” |
| **Super-polynomial** | `n^{log n}`, `2^{√n}` | quasi-polynomial |
| **Exponential** | `2ⁿ`, `cⁿ`, `n!`, `nⁿ`, `2^{2ⁿ}` | intractable |

## 5.3 Rules for Ordering by Inspection

1. **Constant `<` log `<` polynomial `<` exponential `<` factorial** — the golden rule.
2. **Within polynomials:** compare exponents. `n^a ≺ n^b ⟺ a < b`.
3. **Within exponentials `aⁿ`:** compare bases. `aⁿ ≺ bⁿ ⟺ a < b`.
4. **Logs are indifferent to base:** `log₂ n = Θ(log₁₀ n) = Θ(ln n)`. Base only changes a constant.
5. **Logs are indifferent to polynomial arguments:** `log(n^k) = k log n = Θ(log n)`; `log(n!) = Θ(n log n)`.
6. **But exponents on logs matter:** `log n ≺ log² n ≺ log³ n`.
7. **To compare towers, take logs repeatedly** until comparable.
8. **`n^{f(n)}` vs `c^{g(n)}`:** write `n^{f(n)} = 2^{f(n) log n}` and compare exponents.

## 5.4 Worked Ordering Problems

> **Problem 1.** Order: `2^{log n}`, `n^{1/log n}`, `n!`, `(3/2)ⁿ`, `n³`, `log² n`, `log(n!)`, `2^{2ⁿ}`, `n^{1/2}`, `n·2ⁿ`, `nᵏ`, `log n`, `4^{log n}`, `(n+1)!`, `√(log n)`, `e^n`, `n log n`, `2^{√(2 log n)}`, `n`, `2ⁿ`, `n log n`, `2^{2^{n+1}}`
> 
> 
> **Key simplifications (log base 2):**
> - `n^{1/log n} = 2^{(1/log n)·log n} = 2` → **constant**
> - `2^{log n} = n`
> - `4^{log n} = (2²)^{log n} = 2^{2 log n} = n²`
> - `log(n!) = Θ(n log n)`
> - `2^{√(2 log n)}`: exponent `√(2 log n)` grows slower than `log n`, so this is `o(n)` but `ω(log^k n)` — it sits between polylog and any `n^ε`.
> 
> **Final order:**
> 
> ```
> n^{1/log n} = 2  ≺  √(log n)  ≺  log n  ≺  log² n  ≺  2^{√(2 log n)}
>   ≺  n  ≡  2^{log n}  ≺  n log n  ≡  log(n!)  ≺  n²  ≡  4^{log n}
>   ≺  n³  ≺  nᵏ  ≺  (3/2)ⁿ  ≺  2ⁿ  ≺  n·2ⁿ  ≺  eⁿ  ≺  n!  ≺  (n+1)!
>   ≺  2^{2ⁿ}  ≺  2^{2^{n+1}}
> ```
> 
> (`2^{2^{n+1}} = 2^{2·2ⁿ} = (2^{2ⁿ})²` — squaring a doubly-exponential still dominates.)
> 

> **Problem 2.** Which is bigger: `n^{√n}` or `2ⁿ`?
`log(n^{√n}) = √n · log n`. `log(2ⁿ) = n`.
Compare `√n log n` vs `n`, i.e. `log n` vs `√n`. Since `log n = o(√n)`, `√n log n = o(n)`.
**`n^{√n} = o(2ⁿ)`.** ✔
> 

> **Problem 3.** Compare `(log n)^{log n}` and `n / log n`.
`(log n)^{log n} = 2^{log n · log log n} = n^{log log n}`.
Since `log log n → ∞`, eventually the exponent exceeds 1. So `(log n)^{log n} = ω(n/log n)` — in fact `ω(n^k)` for every constant `k`.
> 

## 5.5 The Iterated Logarithm `log* n`

> **Definition.** `log* n = min{ i ≥ 0 : log^{(i)} n ≤ 1 }`, where `log^{(i)}` is the log applied `i` times.
> 

```
log^{(0)} n = n
log^{(i)} n = log(log^{(i−1)} n)   if log^{(i−1)} n > 0
```

| n | log* n |
| --- | --- |
| 1 | 0 |
| 2 | 1 |
| 3 – 4 | 2 |
| 5 – 16 | 3 |
| 17 – 65536 | 4 |
| 65537 – 2^65536 | 5 |

`log* n ≤ 5` for every `n` that fits in the observable universe. It is **effectively constant** but formally unbounded. It appears in union-find (with union by rank only) and Chazelle’s MST algorithm.

## 5.6 The Inverse Ackermann Function `α(n)`

Grows even slower than `log* n`: `α(n) ≤ 4` for all `n < 2^{2^{2^{2^{16}}}}`.

Appears in the amortized cost of **union-find with union by rank + path compression**: `O(m α(n))` for `m` operations.

```
α(n)  ≺  log* n  ≺  log log n  ≺  log n
```

---
