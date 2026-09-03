# 11. Solving Recurrences I: Substitution Method

Two distinct things share this name. Both are essential.

## 11.A Iteration / Back-Substitution (unrolling)

Repeatedly expand the recurrence, find the pattern, sum the series, apply the base case.

### Example 1: `T(n) = T(n−1) + n`, `T(1) = 1`

```
T(n) = T(n−1) + n
     = T(n−2) + (n−1) + n
     = T(n−3) + (n−2) + (n−1) + n
     ⋮
     = T(n−k) + Σ_{i=n−k+1}^{n} i
```

Stop when `n − k = 1 ⟹ k = n − 1`:

```
T(n) = T(1) + Σ_{i=2}^{n} i = 1 + [n(n+1)/2 − 1] = n(n+1)/2 = Θ(n²)
```

### Example 2: `T(n) = T(n/2) + 1`, `T(1) = 1`

```
T(n) = T(n/2) + 1
     = T(n/4) + 1 + 1
     = T(n/2^k) + k
```

Stop when `n/2^k = 1 ⟹ k = log₂ n`:

```
T(n) = T(1) + log₂ n = 1 + log₂ n = Θ(log n)
```

### Example 3: `T(n) = 2T(n/2) + n`, `T(1) = 1`

```
T(n)   = 2T(n/2) + n
       = 2[2T(n/4) + n/2] + n = 4T(n/4) + 2n
       = 8T(n/8) + 3n
       ⋮
       = 2^k T(n/2^k) + k·n
```

At `k = log₂ n`: `2^{log n} T(1) + n log n = n + n log n = Θ(n log n)`

### Example 4: `T(n) = 2T(n/2) + n²`

```
T(n) = 2T(n/2) + n²
     = 4T(n/4) + 2(n/2)² + n² = 4T(n/4) + n²/2 + n²
     = 8T(n/8) + n²/4 + n²/2 + n²
     = 2^k T(n/2^k) + n² Σ_{i=0}^{k−1} (1/2)^i
```

The geometric sum `Σ (1/2)^i < 2`, so `T(n) = n·T(1) + n²·Θ(1) = Θ(n²)`**Root dominates** — a decreasing geometric series.

### Example 5: `T(n) = 3T(n/2) + n`

```
T(n) = 3^k T(n/2^k) + n Σ_{i=0}^{k−1} (3/2)^i
```

At `k = log₂ n`: `3^{log₂ n} = n^{log₂ 3}`, and `Σ(3/2)^i = Θ((3/2)^{log n}) = Θ(n^{log₂ 3}/n)`

```
T(n) = Θ(n^{log₂ 3}) + n·Θ(n^{log₂3 − 1}) = Θ(n^{log₂ 3}) = Θ(n^{1.585})
```

**Leaves dominate** — an increasing geometric series.

### Example 6: `T(n) = T(n−1) + log n`

```
T(n) = Σ_{i=1}^{n} log i = log(n!) = Θ(n log n)
```

### Example 7: `T(n) = 2T(n−1) + 1`, `T(0) = 0` (Hanoi)

```
T(n) = 2T(n−1) + 1
     = 4T(n−2) + 2 + 1
     = 2^k T(n−k) + (2^k − 1)
```

At `k = n`: `T(n) = 2ⁿ·0 + 2ⁿ − 1 = 2ⁿ − 1 = Θ(2ⁿ)`

### Example 8: `T(n) = T(√n) + 1`

Let `n = 2^m`, so `m = log n`. Define `S(m) = T(2^m)`.

```
T(2^m) = T(2^{m/2}) + 1  ⟹  S(m) = S(m/2) + 1  ⟹  S(m) = Θ(log m)
T(n) = Θ(log log n)
```

### Example 9: `T(n) = 2T(√n) + log n`

Let `n = 2^m`, `S(m) = T(2^m)`:

```
S(m) = 2S(m/2) + m  ⟹  S(m) = Θ(m log m)
T(n) = Θ(log n · log log n)
```

### Example 10: `T(n) = T(n/2) + n`

```
T(n) = n + n/2 + n/4 + ⋯ + 1 = n(2 − 1/n) = Θ(n)
```

## 11.B Guess-and-Verify (Induction)

**Steps:**
1. **Guess** the form of the solution (from a recursion tree, similar recurrence, or intuition).
2. **Verify** by mathematical induction, determining the constants.

### Example: `T(n) = 2T(⌊n/2⌋) + n`. Prove `T(n) = O(n log n)`.

**Guess:** `T(n) ≤ c·n log n` for some `c > 0` and all `n ≥ n₀`.

**Inductive step.** Assume it holds for all `m < n`, in particular `m = ⌊n/2⌋`:

```
T(n) ≤ 2 · c⌊n/2⌋ log⌊n/2⌋ + n
     ≤ 2 · c(n/2) log(n/2) + n
     = c·n (log n − 1) + n
     = c·n log n − c·n + n
     ≤ c·n log n            provided  c ≥ 1
```

**Base case.** `T(1) = 1`, but `c·1·log 1 = 0` — fails! **Fix:** start induction at `n = 2`.
`T(2) = 2T(1) + 2 = 4 ≤ c·2·log 2 = 2c` requires `c ≥ 2`.
`T(3) = 2T(1) + 3 = 5 ≤ c·3·log 3 ≈ 4.75c` requires `c ≥ 1.06`.
Take `c = 2, n₀ = 2`. ∎

### The Subtraction Trick (strengthening the hypothesis)

Sometimes the obvious guess fails by a lower-order term. Strengthen it by *subtracting*.

> **Problem.** `T(n) = 2T(⌊n/2⌋ + 17) + n`. Prove `T(n) = O(n log n)`.
> 
> 
> **Problem.** `T(n) = T(⌊n/2⌋) + T(⌈n/2⌉) + 1`. Prove `T(n) = O(n)`.
> 
> Naive attempt with `T(n) ≤ cn`:
> 
> ```
> T(n) ≤ c⌊n/2⌋ + c⌈n/2⌉ + 1 = cn + 1    ✘  (cn + 1 ≰ cn)
> ```
> 
> **Fix:** guess `T(n) ≤ cn − d` for constants `c, d > 0`:
> 
> ```
> T(n) ≤ (c⌊n/2⌋ − d) + (c⌈n/2⌉ − d) + 1 = cn − 2d + 1 ≤ cn − d   iff  d ≥ 1
> ```
> 
> Take `d = 1`, and choose `c` large enough for the base case. ∎
> 

### The “Avoid the Trap” Warning

> **Wrong proof.** `T(n) = 2T(⌊n/2⌋) + n`. “Prove” `T(n) = O(n)`:
> 
> 
> ```
> T(n) ≤ 2·c⌊n/2⌋ + n ≤ cn + n = O(n)     ✘ WRONG
> ```
> 
> The error: we must prove `T(n) ≤ cn` **with the same `c`**. We got `cn + n = (c+1)n`, which is *not* `≤ cn`. Absorbing into `O(n)` is invalid because the constant grows with each level of the induction.
> 

### Change of Variables in Induction

For `T(n) = 2T(⌊√n⌋) + log n`, substitute `m = log n`:

```
T(2^m) = 2T(2^{m/2}) + m
S(m) = 2S(m/2) + m  →  S(m) = O(m log m)
T(n) = O(log n log log n)
```

---

# 12. Solving Recurrences II: Recursion Tree Method

## 12.1 The Method

Draw the tree of recursive calls. At each node write the **non-recursive cost** `f(·)` of that call.

```
Total cost = Σ over all levels of (cost per level)
           = Σ_{i=0}^{depth} (number of nodes at level i) × (cost per node at level i)
```

**Three quantities to compute:**
1. **Depth of the tree** — how many levels until the base case.
2. **Cost per level** — nodes at level `i` × cost of each.
3. **Cost of the leaf level** — number of leaves × `T(1)`.

For `T(n) = aT(n/b) + f(n)`:

```
Level i:  a^i nodes, each of size n/b^i, each costing f(n/b^i)
Cost at level i:  a^i · f(n/b^i)
Depth:  log_b n     (since n/b^i = 1 ⟹ i = log_b n)
Number of leaves:  a^{log_b n} = n^{log_b a}

T(n) = Σ_{i=0}^{log_b n − 1} a^i f(n/b^i)  +  Θ(n^{log_b a})
       └────────── internal node cost ───────┘   └── leaf cost ──┘
```

## 12.2 Example: `T(n) = 2T(n/2) + n`

```
Level 0:                     n                        cost = n
                           /   \
Level 1:              n/2       n/2                   cost = 2·(n/2) = n
                     /   \     /   \
Level 2:          n/4   n/4  n/4   n/4                cost = 4·(n/4) = n
                   ⋮                                       ⋮
Level log n:   1  1  1 ... 1  (n leaves)              cost = n
```

- Depth = `log₂ n`, so `log₂ n + 1` levels
- Cost per level = `n` (constant across levels)
- **Total = `n · (log₂ n + 1) = Θ(n log n)`** ✔

## 12.3 Example: `T(n) = 2T(n/2) + n²` (decreasing)

| Level | Nodes | Size | Cost/node | Level cost |
| --- | --- | --- | --- | --- |
| 0 | 1 | n | n² | n² |
| 1 | 2 | n/2 | n²/4 | n²/2 |
| 2 | 4 | n/4 | n²/16 | n²/4 |
| i | 2^i | n/2^i | n²/4^i | n²/2^i |

```
Total = n² Σ_{i=0}^{log n} (1/2)^i < n² · 2 = Θ(n²)
```

**Root dominates.** Geometric with ratio `< 1` → sum is `Θ(first term)`.

## 12.4 Example: `T(n) = 4T(n/2) + n` (increasing)

| Level | Nodes | Cost/node | Level cost |
| --- | --- | --- | --- |
| 0 | 1 | n | n |
| 1 | 4 | n/2 | 2n |
| 2 | 16 | n/4 | 4n |
| i | 4^i | n/2^i | 2^i n |

Depth `log₂ n`, leaves `= 4^{log₂ n} = n²`.

```
Total = n Σ_{i=0}^{log n −1} 2^i + Θ(n²) = n(n − 1) + Θ(n²) = Θ(n²)
```

**Leaves dominate.** Geometric with ratio `> 1` → sum is `Θ(last term)`.

## 12.5 Example: Unbalanced — `T(n) = T(n/3) + T(2n/3) + n`

```
Level 0:                    n                        cost = n
                          /   \
Level 1:              n/3      2n/3                  cost = n/3 + 2n/3 = n
                     /  \      /   \
Level 2:          n/9  2n/9  2n/9  4n/9              cost = n
```

- Every **full** level costs exactly `n`.
- **Shortest** path (always ÷3): depth `log₃ n`
- **Longest** path (always ×2/3): depth `log_{3/2} n`
- Levels beyond `log₃ n` are partially filled, so cost per level ≤ n.

```
Lower bound: n · log₃ n = Ω(n log n)
Upper bound: n · log_{3/2} n = O(n log n)
T(n) = Θ(n log n)   ✔
```

## 12.6 Example: `T(n) = T(n/3) + T(2n/3) + 1`

Every level costs (number of nodes) × 1. This is like counting nodes in the tree.
- Total = number of nodes ≈ number of leaves.
- The tree is unbalanced; the answer is `Θ(n)`.
- *Verify by Akra–Bazzi:* `(1/3)^p + (2/3)^p = 1 ⟹ p = 1`, `T(n) = Θ(n(1 + ∫_1^n 1/x² dx)) = Θ(n)`. ✔

## 12.7 Example: `T(n) = T(n/5) + T(7n/10) + n` (Median of Medians)

Sum of fractions: `1/5 + 7/10 = 9/10 < 1`.

```
Level 0: n
Level 1: n/5 + 7n/10 = 9n/10
Level 2: (9/10)² n
Level i: (9/10)^i n
```

```
Total = n Σ_{i=0}^{∞} (9/10)^i = n · 1/(1 − 9/10) = 10n = Θ(n)
```

**This is why deterministic selection is linear.** The key: fractions sum to **< 1**.

> **General principle for `T(n) = Σ T(α_i n) + n`:**
- `Σα_i < 1` → `Θ(n)` (root dominates)
- `Σα_i = 1` → `Θ(n log n)` (all levels equal)
- `Σα_i > 1` → `Θ(n^c)` for some `c > 1` (leaves dominate)
> 

## 12.8 Example: `T(n) = T(n−1) + T(n−2) + 1` (Fibonacci tree)

Tree has depth `n`, branching ~2 but unbalanced. Number of nodes = `Θ(F_n) = Θ(φⁿ)` where `φ ≈ 1.618`.

```
T(n) = Θ(φⁿ) ≈ Θ(1.618ⁿ)
```

Bounds by simpler trees: `2T(n/2)`-like reasoning gives `Ω(2^{n/2}) = Ω(1.414ⁿ)` and `O(2ⁿ)`.

## 12.9 Example: `T(n) = 3T(n/4) + n²`

| Level | Nodes | Cost/node | Level cost |
| --- | --- | --- | --- |
| 0 | 1 | n² | n² |
| 1 | 3 | (n/4)² | (3/16)n² |
| i | 3^i | (n/4^i)² | (3/16)^i n² |

```
Total = n² Σ (3/16)^i + Θ(n^{log₄ 3}) ≤ n²·(16/13) + O(n^{0.79}) = Θ(n²)
```

---

# 13. Solving Recurrences III: Master Theorem

## 13.1 Statement

For the recurrence

```
T(n) = a·T(n/b) + f(n)
```

where `a ≥ 1`, `b > 1` are constants and `f(n)` is asymptotically positive:

Let the **critical exponent** be `c* = log_b a`, and compare `f(n)` with `n^{log_b a}`.

| Case | Condition | Solution |
| --- | --- | --- |
| **1** | `f(n) = O(n^{log_b a − ε})` for some `ε > 0` | `T(n) = Θ(n^{log_b a})` |
| **2** | `f(n) = Θ(n^{log_b a})` | `T(n) = Θ(n^{log_b a} · log n)` |
| **3** | `f(n) = Ω(n^{log_b a + ε})` for some `ε > 0` **and** regularity: `a·f(n/b) ≤ k·f(n)` for some `k < 1` and large `n` | `T(n) = Θ(f(n))` |

### Intuition

`n^{log_b a}` is the **cost of the leaves** (there are `a^{log_b n} = n^{log_b a}` of them).
`f(n)` is the **cost of the root**.

- **Case 1:** leaves dominate → the cost is the leaf cost.
- **Case 2:** every level costs the same → multiply by the number of levels.
- **Case 3:** root dominates → the cost is the root cost.

### The Regularity Condition

`a·f(n/b) ≤ k·f(n)` with `k < 1` ensures the cost really does decrease geometrically going down. It holds for all polynomially-bounded `f` (i.e. `f(n) = n^k` or `n^k log^j n`) and is essentially automatic in practice. It **fails** for pathological `f` such as `f(n) = n²·(2 + sin n)` or `f(n) = n^{log n}`… — see §13.4.

## 13.2 Worked Examples

| # | Recurrence | `a` | `b` | `log_b a` | `f(n)` | Case | Result |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | `T(n)=2T(n/2)+n` | 2 | 2 | 1 | `n` | 2 | `Θ(n log n)` |
| 2 | `T(n)=2T(n/2)+n²` | 2 | 2 | 1 | `n²` | 3 | `Θ(n²)` |
| 3 | `T(n)=2T(n/2)+1` | 2 | 2 | 1 | `1` | 1 | `Θ(n)` |
| 4 | `T(n)=T(n/2)+1` | 1 | 2 | 0 | `1` | 2 | `Θ(log n)` |
| 5 | `T(n)=T(n/2)+n` | 1 | 2 | 0 | `n` | 3 | `Θ(n)` |
| 6 | `T(n)=4T(n/2)+n` | 4 | 2 | 2 | `n` | 1 | `Θ(n²)` |
| 7 | `T(n)=4T(n/2)+n²` | 4 | 2 | 2 | `n²` | 2 | `Θ(n² log n)` |
| 8 | `T(n)=4T(n/2)+n³` | 4 | 2 | 2 | `n³` | 3 | `Θ(n³)` |
| 9 | `T(n)=3T(n/2)+n` | 3 | 2 | 1.585 | `n` | 1 | `Θ(n^{log₂3})` |
| 10 | `T(n)=7T(n/2)+n²` (Strassen) | 7 | 2 | 2.807 | `n²` | 1 | `Θ(n^{log₂7})=Θ(n^{2.81})` |
| 11 | `T(n)=8T(n/2)+n²` (naive matmul) | 8 | 2 | 3 | `n²` | 1 | `Θ(n³)` |
| 12 | `T(n)=9T(n/3)+n` | 9 | 3 | 2 | `n` | 1 | `Θ(n²)` |
| 13 | `T(n)=3T(n/4)+n log n` | 3 | 4 | 0.793 | `n log n` | 3 | `Θ(n log n)` |
| 14 | `T(n)=2T(n/4)+√n` | 2 | 4 | 0.5 | `√n` | 2 | `Θ(√n log n)` |
| 15 | `T(n)=16T(n/4)+n²` | 16 | 4 | 2 | `n²` | 2 | `Θ(n² log n)` |
| 16 | `T(n)=2ᵏT(n/2)+n^k`? | — | — | — | — | — | `*a` must be constant — MT does not apply* |

### Detailed Case 1: `T(n) = 9T(n/3) + n`

`a=9, b=3, log₃9 = 2`. `f(n) = n = O(n^{2−ε})` with `ε = 1`. ✔ Case 1.
**`T(n) = Θ(n²)`**

### Detailed Case 2: `T(n) = T(2n/3) + 1`

`a=1, b=3/2, log_{3/2}1 = 0`, so `n^{log_b a} = n⁰ = 1`. `f(n) = 1 = Θ(1)`. ✔ Case 2.
**`T(n) = Θ(log n)`**

### Detailed Case 3: `T(n) = 3T(n/4) + n log n`

`a=3, b=4, log₄3 ≈ 0.793`. Is `n log n = Ω(n^{0.793+ε})`? Yes with `ε = 0.2` (since `n log n = ω(n^{0.993})`). ✔
Regularity: `3·(n/4)log(n/4) ≤ (3/4)·n log n`. ✔ with `k = 3/4`.
**`T(n) = Θ(n log n)`**

### Detailed Case 3: `T(n) = 2T(n/2) + n log n`

`log₂2 = 1`. Is `n log n = Ω(n^{1+ε})`? **NO** — `n log n / n^{1+ε} = log n / n^ε → 0`.
Is `n log n = Θ(n)`? No.
**Master Theorem does not apply** — this falls in the *gap* between cases 2 and 3.
Use the **Extended Master Theorem** (§14.1) → `T(n) = Θ(n log² n)`.

## 13.3 The Three Gaps Where MT Fails

The Master Theorem is **not exhaustive**. It fails when:

**Gap 1 (between Case 1 and 2):** `f(n)` is smaller than `n^{log_b a}` but not *polynomially* smaller.
> Example: `T(n) = 2T(n/2) + n/log n`. Here `n^{log_b a} = n`, and `f = n/log n`. Ratio `= log n`, which is not `n^ε` for any `ε > 0`. MT fails.
> Actual answer (recursion tree): `T(n) = Θ(n log log n)`.

**Gap 2 (between Case 2 and 3):** `f(n)` is larger than `n^{log_b a}` but not *polynomially* larger.
> Example: `T(n) = 2T(n/2) + n log n` → `Θ(n log² n)` (use extended MT).

**Gap 3 (regularity fails):** `f(n) = Ω(n^{log_b a+ε})` but `a f(n/b) ≤ k f(n)` fails.
> Example: `T(n) = 2T(n/2) + n²(2 + sin n)`. Answer is still `Θ(n²)` here but MT can’t be applied directly.

**Also fails when:**
- `a` is not a constant: `T(n) = 2ⁿT(n/2) + n`
- `a < 1`: `T(n) = 0.5T(n/2) + n` (fewer than one subproblem is meaningless)
- `b ≤ 1`: `T(n) = T(n) + n`
- `f(n)` is not asymptotically positive: `T(n) = 2T(n/2) − n`
- Subproblems have different sizes: `T(n) = T(n/3) + T(2n/3) + n` → use Akra–Bazzi

## 13.4 The Master Theorem Decision Flowchart

```
Given T(n) = aT(n/b) + f(n)
       │
       ├─ Is a ≥ 1 constant, b > 1 constant, f(n) > 0? ──NO──→ MT does not apply
       │                                                        (use recursion tree / Akra-Bazzi)
      YES
       ↓
  Compute c* = log_b a  and  compare f(n) with n^{c*}
       │
       ├─ f(n) = O(n^{c*−ε}) ────────────────────→ Θ(n^{c*})            [Case 1]
       │
       ├─ f(n) = Θ(n^{c*} log^k n), k ≥ 0 ───────→ Θ(n^{c*} log^{k+1} n) [Case 2 extended]
       │
       ├─ f(n) = Θ(n^{c*} log^k n), k < −1 ──────→ Θ(n^{c*})            [Case 2 extended]
       │
       ├─ f(n) = Θ(n^{c*} log^{−1} n) ───────────→ Θ(n^{c*} log log n)  [Case 2 extended]
       │
       └─ f(n) = Ω(n^{c*+ε}) AND regularity ─────→ Θ(f(n))              [Case 3]
```

---

# 14. Solving Recurrences IV: Extended & Alternative Theorems

## 14.1 Extended (Generalized) Master Theorem

For `T(n) = aT(n/b) + Θ(n^k log^p n)` with `a ≥ 1, b > 1, k ≥ 0, p ∈ ℝ`:

Let `c* = log_b a`.

| Condition | Solution |
| --- | --- |
| `k < c*` | `T(n) = Θ(n^{c*})` |
| `k = c*` and `p > −1` | `T(n) = Θ(n^{c*} log^{p+1} n)` |
| `k = c*` and `p = −1` | `T(n) = Θ(n^{c*} log log n)` |
| `k = c*` and `p < −1` | `T(n) = Θ(n^{c*})` |
| `k > c*` and `p ≥ 0` | `T(n) = Θ(n^k log^p n)` |
| `k > c*` and `p < 0` | `T(n) = Θ(n^k)` |

### Examples

```
T(n) = 2T(n/2) + n log n        a=2,b=2,c*=1,k=1,p=1  → k=c*, p>−1  → Θ(n log² n)
T(n) = 2T(n/2) + n/log n        c*=1, k=1, p=−1        → Θ(n log log n)
T(n) = 2T(n/2) + n/log² n       c*=1, k=1, p=−2 < −1   → Θ(n)
T(n) = 4T(n/2) + n² log n       c*=2, k=2, p=1         → Θ(n² log² n)
T(n) = 2T(n/4) + n^0.51         c*=0.5, k=0.51 > c*    → Θ(n^0.51)
T(n) = 8T(n/2) + n³ log n       c*=3, k=3, p=1         → Θ(n³ log² n)
T(n) = 3T(n/3) + √n             c*=1, k=0.5 < c*       → Θ(n)
T(n) = 2T(n/2) + n log log n    c*=1, k=1, "p=log-log" → Θ(n log n log log n)
```

## 14.2 Master Theorem for Subtract-and-Conquer

For

```
T(n) = a·T(n − b) + f(n),   where f(n) = O(n^k),  a > 0, b > 0, k ≥ 0
```

| Condition | Solution |
| --- | --- |
| `a < 1` | `T(n) = O(n^k)` |
| `a = 1` | `T(n) = O(n^{k+1})` |
| `a > 1` | `T(n) = O(n^k · a^{n/b})` |

### Examples

```
T(n) = T(n−1) + 1          a=1,b=1,k=0  → O(n)
T(n) = T(n−1) + n          a=1,b=1,k=1  → O(n²)
T(n) = T(n−1) + n²         a=1,b=1,k=2  → O(n³)
T(n) = T(n−1) + log n      a=1,b=1      → O(n log n)
T(n) = 2T(n−1) + 1         a=2,b=1,k=0  → O(2ⁿ)
T(n) = 2T(n−1) + n         a=2,b=1,k=1  → O(n·2ⁿ)
T(n) = 3T(n−1) + 1         a=3,b=1,k=0  → O(3ⁿ)
T(n) = 2T(n−2) + 1         a=2,b=2,k=0  → O(2^{n/2}) = O(1.414ⁿ)
T(n) = T(n−1) + T(n−2) + 1                → Θ(φⁿ), not covered by this form
```

## 14.3 Akra–Bazzi Method (the most general)

For recurrences with **unequal subproblem sizes**:

```
T(n) = Σ_{i=1}^{k} a_i · T(n/b_i) + f(n),     a_i > 0, b_i > 1
```

**Step 1.** Find the unique real `p` satisfying

```
Σ_{i=1}^{k} a_i / b_i^p = 1
```

**Step 2.** Then

```
T(n) = Θ( n^p ( 1 + ∫_1^n f(u)/u^{p+1} du ) )
```

### Example A: `T(n) = T(n/2) + T(n/3) + n`

Solve `(1/2)^p + (1/3)^p = 1`. By inspection `p = 1`: `1/2 + 1/3 = 5/6 ≠ 1`. Try `p < 1`.
At `p = 0`: `1 + 1 = 2 > 1`. At `p = 1`: `0.833 < 1`. So `p ∈ (0,1)`, numerically `p ≈ 0.7878`.

```
∫_1^n u/u^{p+1} du = ∫_1^n u^{−p} du = (n^{1−p} − 1)/(1−p) = Θ(n^{1−p})
T(n) = Θ(n^p · n^{1−p}) = Θ(n)
```

### Example B: Median of Medians `T(n) = T(n/5) + T(7n/10) + n`

`(1/5)^p + (7/10)^p = 1`. At `p=1`: `0.2 + 0.7 = 0.9 < 1`, so `p < 1`. Numerically `p ≈ 0.8397`.

```
∫_1^n u^{−p} du = Θ(n^{1−p})
T(n) = Θ(n^p · n^{1−p}) = Θ(n)   ✔
```

### Example C: `T(n) = 2T(n/2) + n log n`

`2·(1/2)^p = 1 ⟹ p = 1`.

```
∫_1^n (u log u)/u² du = ∫_1^n (log u)/u du = (log² n)/2 = Θ(log² n)
T(n) = Θ(n(1 + log² n)) = Θ(n log² n)   ✔  (matches extended MT)
```

### Example D: `T(n) = 3T(n/4) + T(n/2) + n²`

`3/4^p + 1/2^p = 1`. At `p=2`: `3/16 + 1/4 = 0.4375 < 1`. At `p=1`: `0.75+0.5=1.25>1`. So `1<p<2`, `p ≈ 1.36`.

```
∫_1^n u²/u^{p+1} du = ∫ u^{1−p} du = Θ(n^{2−p})
T(n) = Θ(n^p · n^{2−p}) = Θ(n²)
```

### Akra–Bazzi Sanity Check

For `T(n) = aT(n/b) + f(n)`, Akra–Bazzi gives `a/b^p = 1 ⟹ p = log_b a`, recovering the Master Theorem exactly.

## 14.4 Muster Theorem (Decreasing Function Recurrences)

For `T(n) = aT(n − b) + f(n)` where `f(n) = Θ(n^k)`, `a ≥ 1`, `b > 0`:
same as §14.2 but with `Θ`.

## 14.5 Summary Decision Table

| Recurrence shape | Method |
| --- | --- |
| `aT(n/b) + n^k log^p n` | Extended Master Theorem |
| `aT(n−b) + n^k` | Subtract-and-conquer MT |
| `Σ a_i T(n/b_i) + f(n)` | Akra–Bazzi |
| `T(n/2)+T(n/3)+…` unbalanced | Akra–Bazzi or recursion tree |
| `T(√n) + f(n)` | Change of variable `n = 2^m` |
| `c₁T(n−1)+c₂T(n−2)` (linear, const coeff.) | Characteristic equation |
| Full-history `(1/n)ΣT(i)` | Subtract shifted recurrences |
| Anything weird | Recursion tree + guess & verify |

---

# 15. Solving Recurrences V: Linear Recurrences & Characteristic Equations

## 15.1 Homogeneous Linear Recurrence with Constant Coefficients

```
a_n = c₁ a_{n−1} + c₂ a_{n−2} + ⋯ + c_k a_{n−k}
```

**Step 1.** Write the **characteristic equation**:

```
x^k − c₁x^{k−1} − c₂x^{k−2} − ⋯ − c_k = 0
```

**Step 2.** Find roots `r₁, r₂, …, r_k`.

**Step 3.** General solution:
- **Distinct roots:** `a_n = A₁r₁ⁿ + A₂r₂ⁿ + ⋯ + A_k r_kⁿ`
- **Root `r` with multiplicity `m`:** contributes `(A₀ + A₁n + A₂n² + ⋯ + A_{m−1}n^{m−1}) rⁿ`
- **Complex roots `α ± βi`:** write as `ρⁿ(A cos nθ + B sin nθ)` where `ρ = √(α²+β²)`, `θ = atan2(β,α)`

**Step 4.** Solve for constants using initial conditions.

**Asymptotics:** `a_n = Θ(|r_max|ⁿ · n^{m−1})` where `r_max` is the largest-magnitude root with multiplicity `m`.

### Example 1: Fibonacci

```
F_n = F_{n−1} + F_{n−2},  F_0=0, F_1=1
Characteristic:  x² − x − 1 = 0
Roots: x = (1 ± √5)/2 → φ = 1.618…, ψ = −0.618…
F_n = Aφⁿ + Bψⁿ
F_0 = A + B = 0     → B = −A
F_1 = Aφ + Bψ = A(φ − ψ) = A√5 = 1  → A = 1/√5
F_n = (φⁿ − ψⁿ)/√5 = Θ(φⁿ)
```

### Example 2: `a_n = 6a_{n−1} − 9a_{n−2}`, `a_0=1, a_1=6`

```
x² − 6x + 9 = 0 → (x−3)² = 0 → r = 3 with multiplicity 2
a_n = (A + Bn)3ⁿ
a_0 = A = 1
a_1 = (1 + B)·3 = 6 → B = 1
a_n = (1 + n)3ⁿ = Θ(n·3ⁿ)
```

### Example 3: `T(n) = 2T(n−1) + 3T(n−2)`, `T(0)=1, T(1)=2`

```
x² − 2x − 3 = 0 → (x−3)(x+1)=0 → r = 3, −1
T(n) = A·3ⁿ + B(−1)ⁿ
T(0)=A+B=1;  T(1)=3A−B=2  → A=3/4, B=1/4
T(n) = (3^{n+1} + (−1)ⁿ)/4 = Θ(3ⁿ)
```

## 15.2 Non-Homogeneous Linear Recurrences

```
a_n = c₁a_{n−1} + ⋯ + c_k a_{n−k} + F(n)
```

**Solution = homogeneous solution + particular solution:** `a_n = a_n^{(h)} + a_n^{(p)}`

**Guessing the particular solution** when `F(n) = (b_t n^t + ⋯ + b_0)·sⁿ`:

| Is `s` a root of the characteristic equation? | Guess `a_n^{(p)}` |
| --- | --- |
| No | `(p_t n^t + ⋯ + p_0)·sⁿ` |
| Yes, with multiplicity `m` | `n^m (p_t n^t + ⋯ + p_0)·sⁿ` |

### Example: `a_n = 3a_{n−1} + 2n`, `a_1 = 3`

- Homogeneous: `x − 3 = 0 → a^{(h)} = A·3ⁿ`
- `F(n) = 2n` is a degree-1 polynomial times `1ⁿ`; `s=1` is not a root → guess `a^{(p)} = pn + q`.

```
pn + q = 3(p(n−1) + q) + 2n
pn + q = 3pn − 3p + 3q + 2n
Compare n:  p = 3p + 2 → p = −1
Compare 1:  q = −3p + 3q → 0 = −3p + 2q → q = 3p/2 = −3/2
a^{(p)} = −n − 3/2
a_n = A·3ⁿ − n − 3/2;   a_1 = 3A − 1 − 1.5 = 3 → A = 11/6
a_n = (11/6)3ⁿ − n − 3/2 = Θ(3ⁿ)
```

### Example: `T(n) = 2T(n−1) + 1`, `T(0)=0` (Hanoi)

- Homogeneous: `A·2ⁿ`
- `F(n) = 1 = 1·1ⁿ`, `s=1` not a root → guess constant `p`: `p = 2p + 1 → p = −1`
- `T(n) = A·2ⁿ − 1`; `T(0)=A−1=0 → A=1`
- **`T(n) = 2ⁿ − 1`** ✔

### Example: `a_n = 2a_{n−1} + 2ⁿ`

`s = 2` **is** a root (multiplicity 1) → guess `a^{(p)} = pn·2ⁿ`.

```
pn2ⁿ = 2p(n−1)2^{n−1} + 2ⁿ = p(n−1)2ⁿ + 2ⁿ
pn = pn − p + 1 → p = 1
a_n = A·2ⁿ + n·2ⁿ = Θ(n·2ⁿ)
```

## 15.3 Full-History Recurrences

```
T(n) = c·n + (2/n) Σ_{i=0}^{n−1} T(i)      ← QuickSort average case
```

**Technique:** multiply by `n`, then subtract the same equation for `n−1`.

```
n·T(n)     = c n² + 2 Σ_{i=0}^{n−1} T(i)
(n−1)T(n−1) = c(n−1)² + 2 Σ_{i=0}^{n−2} T(i)
Subtract:
n T(n) − (n−1)T(n−1) = c(2n − 1) + 2T(n−1)
n T(n) = (n+1)T(n−1) + c(2n−1)
```

Divide by `n(n+1)`:

```
T(n)/(n+1) = T(n−1)/n + c(2n−1)/(n(n+1))
```

Let `S(n) = T(n)/(n+1)`. Then `S(n) = S(n−1) + Θ(1/n)`, so `S(n) = Θ(H_n) = Θ(log n)`.

```
T(n) = (n+1)·Θ(log n) = Θ(n log n)   ✔
```

(Exact: `T(n) ≈ 2n ln n ≈ 1.39 n log₂ n`.)

---
