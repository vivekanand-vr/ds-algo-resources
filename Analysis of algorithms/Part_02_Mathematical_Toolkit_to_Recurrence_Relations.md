# 6. Mathematical Toolkit — Logarithms & Exponents

## 6.1 Definition and Notation Conventions

`log_b x = y  ⟺  b^y = x`, for `b > 0, b ≠ 1, x > 0`.

| Notation | Base | Field |
| --- | --- | --- |
| `lg n` or `log n` | 2 | Computer Science (default!) |
| `ln n` | `e ≈ 2.71828` | Mathematics / calculus |
| `log n` | 10 | Engineering |
| `log^k n` | means `(log n)^k` | **not** `log(log(...))` |
| `log log n` | `log(log n)` | double log |
| `log^{(k)} n` | log applied k times | iterated |

> ⚠️ **Precedence:** `log n^k = log(n^k) = k log n`, but `log^k n = (log n)^k`. These are very different!
`log n + k = (log n) + k`, and `log n·k` is ambiguous — write `(log n)·k` or `log(nk)`.
> 

## 6.2 Core Identities (memorize all)

```
(1)   log_b(xy)      = log_b x + log_b y
(2)   log_b(x/y)     = log_b x − log_b y
(3)   log_b(x^a)     = a · log_b x
(4)   log_b(1)       = 0
(5)   log_b(b)       = 1
(6)   log_b(b^x)     = x
(7)   b^{log_b x}    = x
(8)   log_b(1/x)     = − log_b x
(9)   log_b(x)       = 1 / log_x(b)
(10)  log_b(ⁿ√x)     = (1/n) log_b x
```

## 6.3 Change of Base — the most important formula

```
log_b x = log_a x / log_a b = log_a x · log_b a
```

**Consequence:** `log_b n = (1/log₂ b) · log₂ n = Θ(log₂ n)`.

> **Therefore the base of a logarithm is irrelevant inside Θ, O, Ω.**`Θ(log₂ n) = Θ(log₁₀ n) = Θ(ln n) = Θ(log n)`
> 

Common constants:

```
log₂ e ≈ 1.4427       ln 2 ≈ 0.6931
log₂ 10 ≈ 3.3219      ln 10 ≈ 2.3026
log₁₀ 2 ≈ 0.3010      log₂ 3 ≈ 1.585
```

> ⚠️ **But base matters in the EXPONENT:** `2^{log₂ n} = n` while `2^{log₃ n} = n^{log₃ 2} = n^{0.631}`. Never drop bases inside exponents.
> 

## 6.4 Base-Swapping Identity (very useful)

```
a^{log_b n} = n^{log_b a}
```

*Proof:* Take `log_b` of both sides. LHS: `log_b n · log_b a`. RHS: `log_b a · log_b n`. Equal. ∎

Applications:

```
3^{log₂ n}  = n^{log₂ 3} = n^{1.585}
2^{log₃ n}  = n^{log₃ 2} = n^{0.631}
n^{log₂ 3}  = 3^{log₂ n}
2^{log₂ n}  = n
```

## 6.5 Exponent Rules

```
b^0 = 1                 b^1 = b               b^{−a} = 1/b^a
b^a · b^c = b^{a+c}     b^a / b^c = b^{a−c}   (b^a)^c = b^{ac}
(bc)^a = b^a c^a        b^{1/n} = ⁿ√b
```

## 6.6 Useful Analytic Facts

```
e^x = Σ_{i=0}^{∞} x^i/i! = 1 + x + x²/2! + x³/3! + ⋯

e^x ≥ 1 + x                for all real x    (equality iff x=0)
1 + x ≤ e^x ≤ 1 + x + x²   for |x| ≤ 1
lim_{n→∞} (1 + x/n)^n = e^x
(1 − 1/n)^n → 1/e ≈ 0.3679
(1 + 1/n)^n → e

ln(1+x) ≤ x                for x > −1
x/(1+x) ≤ ln(1+x) ≤ x      for x > −1
ln(1+x) ≈ x − x²/2 + x³/3 − ⋯   for |x| < 1
```

**Approximation used constantly:** `(1 − 1/n)^n ≈ 1/e`, so an event with probability `1/n` fails `n` times with probability `≈ 1/e ≈ 0.37`.

## 6.7 Logs of Special Quantities

```
log(n!) = Σ_{i=1}^{n} log i = Θ(n log n)          ← Stirling
log(2ⁿ) = n
log(nⁿ) = n log n
log(n^{log n}) = (log n)²
log(C(n,k)) ≤ k log(en/k)
```

**Number of bits to represent `n`:** `⌊log₂ n⌋ + 1`**Number of digits (base 10) of `n`:** `⌊log₁₀ n⌋ + 1`**Height of a complete binary tree with `n` nodes:** `⌊log₂ n⌋`**Number of levels when repeatedly halving `n` until 1:** `⌈log₂ n⌉`**Number of leaves in a binary tree of height `h`:** at most `2^h`**Number of times you can double from 1 to reach `n`:** `⌈log₂ n⌉`

## 6.8 The Harmonic Number (log’s constant companion)

```
H_n = Σ_{k=1}^{n} 1/k = 1 + 1/2 + 1/3 + ⋯ + 1/n
```

**Asymptotics:**

```
H_n = ln n + γ + 1/(2n) − 1/(12n²) + O(1/n⁴)
γ ≈ 0.5772156649   (Euler–Mascheroni constant)
H_n = Θ(log n)
ln(n+1) ≤ H_n ≤ 1 + ln n
```

*Integral proof of bounds:*

```
∫_1^{n+1} dx/x  ≤  Σ_{k=1}^{n} 1/k  ≤  1 + ∫_1^{n} dx/x
     ln(n+1)     ≤       H_n        ≤  1 + ln n
```

`H_n` appears in: QuickSort average case, coupon collector, randomized algorithms, BST random insertion depth.

**Generalized harmonic:** `H_n^{(k)} = Σ_{i=1}^{n} 1/i^k`.
- `k = 1` → `Θ(log n)`
- `k > 1` → `Θ(1)` (converges; `H_∞^{(2)} = π²/6`)
- `k < 1` → `Θ(n^{1−k})`

---

# 7. Mathematical Toolkit — Floors, Ceilings, Factorials

## 7.1 Floor and Ceiling

```
⌊x⌋ = greatest integer ≤ x
⌈x⌉ = least integer ≥ x
```

**Properties:**

```
x − 1 < ⌊x⌋ ≤ x ≤ ⌈x⌉ < x + 1
⌈x⌉ − ⌊x⌋ = 1 if x ∉ ℤ, else 0
⌊x⌋ + ⌈x⌉ = 2x if x ∈ ℤ
⌊−x⌋ = −⌈x⌉,   ⌈−x⌉ = −⌊x⌋
⌊n/2⌋ + ⌈n/2⌉ = n                      ← used in mergesort
⌈⌈n/a⌉/b⌉ = ⌈n/(ab)⌉                   for positive integers a,b
⌊⌊n/a⌋/b⌋ = ⌊n/(ab)⌋
⌈n/m⌉ = ⌊(n + m − 1)/m⌋                 ← ceiling via floor
0 ≤ ⌈n/2⌉ − n/2 < 1
```

> **In asymptotic analysis, floors and ceilings can almost always be ignored.**`T(n) = 2T(⌈n/2⌉) + n` and `T(n) = 2T(n/2) + n` have the same Θ solution.
This is justified by the “smoothness” condition: `T` is *eventually non-decreasing* and *b-smooth* (`T(bn) = Θ(T(n))`). All standard complexity functions (`n^k`, `n^k log n`, etc.) are smooth. Exponentials are **not** smooth — be careful there.
> 

## 7.2 Factorials

```
n! = n × (n−1) × ⋯ × 2 × 1,      0! = 1
n! = n · (n−1)!
```

### Stirling’s Approximation

```
n! = √(2πn) · (n/e)^n · (1 + Θ(1/n))
```

More precisely:

```
n! = √(2πn) (n/e)^n e^{α_n},   where  1/(12n+1) < α_n < 1/(12n)
```

**Bounds (easier to use):**

```
(n/e)^n ≤ n! ≤ n^n
(n/e)^n ≤ n! ≤ e·n·(n/e)^n
2^{n−1} ≤ n! ≤ n^n          for n ≥ 1
```

**Log of factorial (crucial for sorting lower bound):**

```
log(n!) = n log n − n log e + Θ(log n) = Θ(n log n)
ln(n!) = n ln n − n + O(log n)
```

*Simple proof that `log(n!) = Θ(n log n)`:*
- Upper: `n! ≤ nⁿ ⟹ log n! ≤ n log n`.
- Lower: `n! ≥ (n/2)^{n/2}` (the top half of factors each ≥ n/2)
`⟹ log n! ≥ (n/2) log(n/2) = (n/2)(log n − 1) = Ω(n log n)`. ∎

**Growth:** `n! = ω(cⁿ)` for every constant `c`, and `n! = o(nⁿ)`.

## 7.3 Binomial Coefficients

```
C(n,k) = "n choose k" = n! / (k!(n−k)!)
```

**Identities:**

```
C(n,k) = C(n, n−k)                          symmetry
C(n,k) = C(n−1,k−1) + C(n−1,k)              Pascal's rule
C(n,0) = C(n,n) = 1
Σ_{k=0}^{n} C(n,k) = 2^n                     ← number of subsets
Σ_{k=0}^{n} (−1)^k C(n,k) = 0
Σ_{k=0}^{n} k·C(n,k) = n·2^{n−1}
Σ_{k=0}^{n} C(n,k)² = C(2n,n)
C(n,k) = (n/k)·C(n−1,k−1)                    absorption
```

**Bounds:**

```
(n/k)^k ≤ C(n,k) ≤ (en/k)^k
C(n,k) ≤ n^k / k!
C(n, n/2) = Θ(2^n / √n)                      the central binomial coefficient
C(2n,n) ≈ 4^n / √(πn)
```

**Binomial theorem:** `(x+y)^n = Σ_{k=0}^{n} C(n,k) x^k y^{n−k}`

## 7.4 Catalan Numbers

Count of: binary trees with `n` nodes, balanced parenthesizations, triangulations, monotone lattice paths.

```
C_n = (1/(n+1)) · C(2n, n) = (2n)! / ((n+1)! n!)
C_0 = 1, C_1 = 1, C_2 = 2, C_3 = 5, C_4 = 14, C_5 = 42, C_6 = 132
Recurrence:  C_{n+1} = Σ_{i=0}^{n} C_i · C_{n−i}
Asymptotic:  C_n ~ 4^n / (n^{3/2} √π)  =  Θ(4^n / n^{1.5})
```

## 7.5 Fibonacci Numbers

```
F_0 = 0, F_1 = 1, F_n = F_{n−1} + F_{n−2}
```

Closed form (Binet):

```
F_n = (φ^n − ψ^n)/√5,   φ = (1+√5)/2 ≈ 1.618,  ψ = (1−√5)/2 ≈ −0.618
F_n = round(φ^n / √5)
F_n = Θ(φ^n)                    ← exponential growth
```

`Σ_{i=0}^{n} F_i = F_{n+2} − 1`

This is why **naive recursive Fibonacci is `Θ(φⁿ)`**, not `Θ(2ⁿ)`.

---

# 8. Summation Series — The Complete Catalogue

## 8.1 Linearity (the fundamental manipulation rules)

```
Σ (a·f(k) + b·g(k)) = a·Σ f(k) + b·Σ g(k)
Σ_{k=1}^{n} c = c·n                          ← constant summand
Σ_{k=m}^{n} f(k) = Σ_{k=1}^{n} f(k) − Σ_{k=1}^{m−1} f(k)
Σ_{k=0}^{n} f(k) = Σ_{k=0}^{n} f(n−k)        ← reversal
```

**Number of terms** in `Σ_{i=a}^{b}` is `b − a + 1`. (Off-by-one errors here are the #1 cause of wrong complexity.)

## 8.2 Arithmetic Series

```
Σ_{k=1}^{n} k = 1 + 2 + ⋯ + n = n(n+1)/2 = Θ(n²)

Σ_{k=0}^{n} k = n(n+1)/2                     (same, k=0 adds nothing)
Σ_{k=1}^{n} (a + (k−1)d) = n/2 · [2a + (n−1)d] = n(first + last)/2
Σ_{k=1}^{n} (2k−1) = n²                      sum of first n odd numbers
Σ_{k=1}^{n} 2k = n(n+1)                      sum of first n even numbers
```

## 8.3 Sums of Powers

```
Σ_{k=1}^{n} k    = n(n+1)/2                          = Θ(n²)
Σ_{k=1}^{n} k²   = n(n+1)(2n+1)/6                    = Θ(n³)
Σ_{k=1}^{n} k³   = [n(n+1)/2]² = (Σk)²               = Θ(n⁴)
Σ_{k=1}^{n} k⁴   = n(n+1)(2n+1)(3n²+3n−1)/30         = Θ(n⁵)

General:  Σ_{k=1}^{n} k^d = n^{d+1}/(d+1) + Θ(n^d) = Θ(n^{d+1})   for d ≥ 0 constant
```

(Faulhaber’s formula gives exact coefficients via Bernoulli numbers.)

**Negative powers:**

```
Σ_{k=1}^{n} 1/k   = H_n = Θ(log n)
Σ_{k=1}^{n} 1/k²  = Θ(1)      → π²/6 ≈ 1.6449 as n→∞
Σ_{k=1}^{n} 1/k^d = Θ(1)      for d > 1
Σ_{k=1}^{n} 1/√k  = Θ(√n)
Σ_{k=1}^{n} k^{−d} = Θ(n^{1−d})  for 0 ≤ d < 1
```

## 8.4 Geometric Series — the most important family

**Finite:**

```
Σ_{k=0}^{n} x^k = 1 + x + x² + ⋯ + x^n = (x^{n+1} − 1)/(x − 1),   x ≠ 1
Σ_{k=0}^{n} x^k = n + 1,                                          x = 1
Σ_{k=1}^{n} x^k = x(x^n − 1)/(x − 1)
```

**Infinite (converges iff `|x| < 1`):**

```
Σ_{k=0}^{∞} x^k = 1/(1 − x)
Σ_{k=1}^{∞} x^k = x/(1 − x)
```

**Asymptotic behaviour — memorize this trichotomy:**

| Ratio | `Σ_{k=0}^{n} x^k` | Dominated by |
| --- | --- | --- |
| `x > 1` | `Θ(x^n)` | **last** term |
| `x = 1` | `Θ(n)` | all equal |
| `x < 1` | `Θ(1)` | **first** term |

This is the core of the Master Theorem’s three cases.

**Powers of 2 (most common):**

```
Σ_{k=0}^{n} 2^k = 2^{n+1} − 1
Σ_{k=0}^{n−1} 2^k = 2^n − 1
Σ_{k=0}^{∞} 1/2^k = 2
Σ_{k=1}^{∞} 1/2^k = 1
Σ_{k=0}^{n} 1/2^k = 2 − 1/2^n < 2
```

## 8.5 Arithmetico-Geometric Series

```
Σ_{k=0}^{∞} k·x^k = x/(1−x)²                     for |x| < 1
Σ_{k=1}^{∞} k/2^k = 2                            ← very common!
Σ_{k=0}^{∞} k²x^k = x(1+x)/(1−x)³
Σ_{k=1}^{n} k·2^k = (n−1)2^{n+1} + 2
Σ_{k=1}^{n} k/2^k = 2 − (n+2)/2^n  < 2
Σ_{k=0}^{n} k x^k = x(1 − (n+1)x^n + n x^{n+1})/(1−x)²
```

**Derivation trick:** differentiate the geometric series.

```
Σ x^k = 1/(1−x)
d/dx: Σ k x^{k−1} = 1/(1−x)²
×x:   Σ k x^k = x/(1−x)²
```

`Σ k/2^k = 2` is exactly the sum that gives **build-heap = O(n)**.

## 8.6 Telescoping Series

```
Σ_{k=1}^{n} (a_k − a_{k−1}) = a_n − a_0
Σ_{k=0}^{n−1} (a_k − a_{k+1}) = a_0 − a_n
```

> **Example.** `Σ_{k=1}^{n} 1/(k(k+1))`. Since `1/(k(k+1)) = 1/k − 1/(k+1)`:
`= (1 − 1/2) + (1/2 − 1/3) + ⋯ + (1/n − 1/(n+1)) = 1 − 1/(n+1) = n/(n+1) = Θ(1)`
> 

Telescoping is the backbone of the **substitution method for recurrences**.

## 8.7 Other Frequently Needed Sums

```
Σ_{k=1}^{n} log k = log(n!) = Θ(n log n)
Σ_{k=1}^{n} k log k = Θ(n² log n)
Σ_{k=1}^{n} log(n/k) = n log n − log(n!) = Θ(n)
Σ_{k=1}^{n} 2^k · k = Θ(n 2^n)
Σ_{k=0}^{log n} n/2^k = 2n − 1 = Θ(n)
Σ_{k=0}^{log n} 2^k = 2n − 1 = Θ(n)
Σ_{k=1}^{n} √k = Θ(n^{3/2})           more precisely ≈ (2/3)n^{3/2}
Σ_{k=1}^{n} k! = Θ(n!)
Σ_{i=1}^{n} Σ_{j=1}^{i} 1 = n(n+1)/2
Σ_{i=1}^{n} Σ_{j=i}^{n} 1 = n(n+1)/2
Σ_{i=1}^{n} Σ_{j=1}^{n} 1 = n²
Σ_{i=1}^{n} Σ_{j=1}^{i} Σ_{k=1}^{j} 1 = C(n+2,3) = Θ(n³)
```

**Double-sum pattern (triangular loops):**

```
Σ_{i=1}^{n} Σ_{j=i}^{n} (j − i + 1) = Σ_{i=1}^{n} (n−i+1)(n−i+2)/2 = Θ(n³)
```

## 8.8 Products

```
Π_{k=1}^{n} k = n!
log(Π f(k)) = Σ log f(k)             ← convert products to sums
Π_{k=1}^{n} a = a^n
Π_{k=1}^{n} 2^k = 2^{n(n+1)/2}
```

---

# 9. Bounding Summations (Techniques)

When there is no closed form, bound the sum.

## 9.1 Bounding by the Largest Term

```
Σ_{k=1}^{n} a_k ≤ n · max_k a_k
```

Gives `O`; often loose but sometimes tight.

> **Example.** `Σ_{k=1}^{n} k ≤ n · n = n²` → `O(n²)`. ✔ (tight up to constant)
**Example.** `Σ_{k=1}^{n} 1/k ≤ n · 1 = n` → `O(n)`. ✘ Very loose (truth: `Θ(log n)`).
> 

## 9.2 Bounding by a Geometric Series

If `a_{k+1}/a_k ≤ r < 1` for all `k ≥ k₀`, then `Σ_{k=k₀}^{∞} a_k ≤ a_{k₀}/(1−r) = O(a_{k₀})`.

> **Example.** `Σ_{k=1}^{∞} k/3^k`. Ratio `= (k+1)/(3k) ≤ 2/3` for `k ≥ 1`.
So sum `≤ (1/3)/(1 − 2/3) = 1 = O(1)`. ✔
> 

## 9.3 Splitting the Sum

Break at a well-chosen point.

> **Example.** Show `Σ_{k=1}^{n} 1/k = O(log n)`.
Group into blocks of size `2^i`:
> 
> 
> ```
> Σ_{k=1}^{n} 1/k ≤ Σ_{i=0}^{⌊log n⌋} Σ_{j=0}^{2^i − 1} 1/(2^i + j)
>                 ≤ Σ_{i=0}^{⌊log n⌋} Σ_{j=0}^{2^i − 1} 1/2^i
>                 = Σ_{i=0}^{⌊log n⌋} 1
>                 = ⌊log n⌋ + 1 = O(log n)  ∎
> ```
> 

## 9.4 Integral Approximation (very powerful)

If `f` is **monotonically increasing**:

```
∫_{m−1}^{n} f(x) dx  ≤  Σ_{k=m}^{n} f(k)  ≤  ∫_{m}^{n+1} f(x) dx
```

If `f` is **monotonically decreasing**:

```
∫_{m}^{n+1} f(x) dx  ≤  Σ_{k=m}^{n} f(k)  ≤  ∫_{m−1}^{n} f(x) dx
```

Practical rule: `Σ_{k=1}^{n} f(k) ≈ ∫_1^n f(x)dx` with error `O(f(n))`.

> **Example 1.** `Σ_{k=1}^{n} k² ≈ ∫_1^n x² dx = n³/3` → `Θ(n³)` ✔
**Example 2.** `Σ_{k=1}^{n} 1/k ≈ ∫_1^n dx/x = ln n` → `Θ(log n)` ✔
**Example 3.** `Σ_{k=1}^{n} √k ≈ ∫_1^n √x dx = (2/3)n^{3/2}` → `Θ(n^{1.5})` ✔
**Example 4.** `Σ_{k=2}^{n} 1/(k log k) ≈ ∫_2^n dx/(x ln x) = ln ln n` → `Θ(log log n)` ✔
**Example 5.** `Σ_{k=1}^{n} log k ≈ ∫_1^n ln x dx = n ln n − n + 1` → `Θ(n log n)` ✔
> 

## 9.5 Quick Reference: `Σ_{k=1}^{n} f(k)` for common `f`

| `f(k)` | `Σ` | Order |
| --- | --- | --- |
| `1` | `n` | `Θ(n)` |
| `1/k` | `H_n` | `Θ(log n)` |
| `1/k²` | `< π²/6` | `Θ(1)` |
| `1/√k` | `≈ 2√n` | `Θ(√n)` |
| `√k` | `≈ (2/3)n^{1.5}` | `Θ(n^{1.5})` |
| `k` | `n(n+1)/2` | `Θ(n²)` |
| `k²` | `n(n+1)(2n+1)/6` | `Θ(n³)` |
| `k^d`, `d>−1` | `≈ n^{d+1}/(d+1)` | `Θ(n^{d+1})` |
| `log k` | `log n!` | `Θ(n log n)` |
| `k log k` | — | `Θ(n² log n)` |
| `1/(k log k)` | — | `Θ(log log n)` |
| `2^k` | `2^{n+1}−1` | `Θ(2^n)` |
| `k·2^k` | `(n−1)2^{n+1}+2` | `Θ(n2^n)` |
| `1/2^k` | `< 2` | `Θ(1)` |
| `k/2^k` | `< 2` | `Θ(1)` |
| `c^k`, `c>1` | `≈ c^{n+1}/(c−1)` | `Θ(c^n)` |
| `k!` | — | `Θ(n!)` |

---

# 10. Recurrence Relations — Setting Them Up

## 10.1 What a Recurrence Is

A **recurrence** expresses `T(n)` in terms of `T` at smaller arguments, plus a base case.

```
T(n) = a·T(n/b) + f(n),        T(1) = c
       └─────┬──┘   └┬┘
   recursive calls   work done outside recursion (divide + combine)
```

- `a` = number of subproblems
- `n/b` = size of each subproblem
- `f(n)` = cost of splitting + merging + non-recursive work

## 10.2 Deriving a Recurrence from Code

**Recipe:**
1. Identify the parameter that shrinks.
2. Count recursive calls and the size of each.
3. Sum the non-recursive work in the function body (loops, calls to other functions).
4. Write the base case.

> **Example: Merge Sort**
> 
> 
> ```
> MERGE-SORT(A, p, r):
>     if p < r:
>         q = (p + r)/2               # Θ(1)
>         MERGE-SORT(A, p, q)         # T(n/2)
>         MERGE-SORT(A, q+1, r)       # T(n/2)
>         MERGE(A, p, q, r)           # Θ(n)
> ```
> 
> → `T(n) = 2T(n/2) + Θ(n)`, `T(1) = Θ(1)`
> 

> **Example: Binary Search**
One recursive call on half the array, `Θ(1)` work → `T(n) = T(n/2) + Θ(1)`
> 

> **Example: Tower of Hanoi**`T(n) = 2T(n−1) + 1`, `T(1) = 1` → `T(n) = 2ⁿ − 1`
> 

> **Example: Naive Fibonacci**`T(n) = T(n−1) + T(n−2) + Θ(1)` → `Θ(φⁿ)`
> 

> **Example: QuickSort worst case**
Pivot always extreme: `T(n) = T(n−1) + Θ(n)` → `Θ(n²)`
> 

> **Example: QuickSort best case**`T(n) = 2T(n/2) + Θ(n)` → `Θ(n log n)`
> 

## 10.3 Standard Recurrence Forms

| Form | Name | Typical source |
| --- | --- | --- |
| `T(n) = T(n−1) + f(n)` | subtract-and-conquer, 1 branch | linear recursion, loops |
| `T(n) = aT(n−b) + f(n)` | subtract-and-conquer, `a` branches | Hanoi, subsets |
| `T(n) = aT(n/b) + f(n)` | divide-and-conquer | mergesort, Strassen |
| `T(n) = T(αn) + T(βn) + f(n)`, `α+β<1` or `=1` | unbalanced split | median-of-medians |
| `T(n) = T(√n) + f(n)` | root recursion | van Emde Boas |
| `T(n) = (1/n)Σ T(i) + f(n)` | full-history / average case | QuickSort average |

## 10.4 Technical Conventions

- **Base case is usually omitted** and assumed `T(n) = Θ(1)` for `n ≤ n₀`. This is valid because changing the base case changes `T` by only a constant factor for well-behaved recurrences.
- **Floors/ceilings are usually dropped** (see §7.1).
- **Boundary conditions matter for exact solutions** but not for Θ.

---
