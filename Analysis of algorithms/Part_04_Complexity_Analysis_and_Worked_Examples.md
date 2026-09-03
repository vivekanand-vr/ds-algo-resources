# 16. Solving Recurrences VI: Change of Variable & Generating Functions

## 16.1 Change of Variable (Domain Transformation)

Used when the argument shrinks by a **root** or the recurrence isn’t in standard form.

### `T(n) = 2T(√n) + log n`

Substitute `n = 2^m` (so `m = log₂ n`, `√n = 2^{m/2}`). Let `S(m) = T(2^m)`.

```
S(m) = 2S(m/2) + m
Master Theorem: a=2,b=2,log₂2=1, f=m=Θ(m) → Case 2 → S(m) = Θ(m log m)
T(n) = Θ(log n · log log n)
```

### `T(n) = 2T(√n) + 1`

```
S(m) = 2S(m/2) + 1 → Θ(m) → T(n) = Θ(log n)
```

### `T(n) = T(√n) + Θ(1)`

```
S(m) = S(m/2) + Θ(1) → Θ(log m) → T(n) = Θ(log log n)
```

(This is the van Emde Boas / interpolation-search style recurrence.)

### `T(n) = T(√n) + Θ(log log n)`

Let `n = 2^m`: `S(m) = S(m/2) + Θ(log m)`. Unroll: `Σ_{i=0}^{log m} log(m/2^i) = Θ(log² m)`.

```
T(n) = Θ(log² log n)
```

### `T(n) = √n · T(√n) + n`

Divide both sides by `n`: let `U(n) = T(n)/n`.

```
T(n)/n = (√n T(√n))/n + 1 = T(√n)/√n + 1
U(n) = U(√n) + 1  →  U(n) = Θ(log log n)
T(n) = Θ(n log log n)
```

## 16.2 Range Transformation

For `T(n) = n·T(n−1)`, `T(1)=1`: take logs.

```
log T(n) = log n + log T(n−1)  →  log T(n) = Σ log i = log n!  →  T(n) = n!
```

## 16.3 Generating Functions (brief)

Define `G(x) = Σ_{n≥0} a_n x^n`. Translate the recurrence into an equation in `G(x)`, solve algebraically, then extract coefficients.

### Fibonacci via generating functions

```
G(x) = Σ F_n x^n
F_n = F_{n−1} + F_{n−2} for n ≥ 2, F_0=0, F_1=1
G(x) = x + xG(x) + x²G(x)
G(x)(1 − x − x²) = x
G(x) = x/(1 − x − x²)
```

Partial fractions with roots of `1 − x − x²`, giving Binet’s formula.

**Useful generating functions:**

```
1/(1−x)        = Σ x^n
1/(1−x)²       = Σ (n+1)x^n
1/(1−ax)       = Σ aⁿx^n
x/(1−x)²       = Σ n x^n
e^x            = Σ x^n/n!
1/(1−x)^{k}    = Σ C(n+k−1, k−1) x^n
(1+x)^n        = Σ C(n,k)x^k
(1−√(1−4x))/(2x) = Σ C_n x^n      (Catalan)
```

## 16.4 Recurrence Solutions — Master Reference Table

| Recurrence | Solution | Where it appears |
| --- | --- | --- |
| `T(n)=T(n−1)+1` | `Θ(n)` | linear scan recursion |
| `T(n)=T(n−1)+n` | `Θ(n²)` | selection/insertion sort, quicksort worst |
| `T(n)=T(n−1)+log n` | `Θ(n log n)` |  |
| `T(n)=T(n−1)+n²` | `Θ(n³)` |  |
| `T(n)=2T(n−1)+1` | `Θ(2ⁿ)` | Tower of Hanoi, subsets |
| `T(n)=2T(n−1)+n` | `Θ(n2ⁿ)` |  |
| `T(n)=3T(n−1)+1` | `Θ(3ⁿ)` | ternary enumeration |
| `T(n)=T(n−1)+T(n−2)+1` | `Θ(φⁿ)≈Θ(1.618ⁿ)` | naive Fibonacci |
| `T(n)=nT(n−1)` | `Θ(n!)` | permutation generation |
| `T(n)=T(n/2)+1` | `Θ(log n)` | binary search |
| `T(n)=T(n/2)+n` | `Θ(n)` |  |
| `T(n)=T(n/2)+log n` | `Θ(log² n)` |  |
| `T(n)=2T(n/2)+1` | `Θ(n)` | tree traversal |
| `T(n)=2T(n/2)+log n` | `Θ(n)` |  |
| `T(n)=2T(n/2)+n` | `Θ(n log n)` | merge sort |
| `T(n)=2T(n/2)+n log n` | `Θ(n log² n)` |  |
| `T(n)=2T(n/2)+n²` | `Θ(n²)` |  |
| `T(n)=2T(n/2)+n/log n` | `Θ(n log log n)` |  |
| `T(n)=3T(n/2)+n` | `Θ(n^{1.585})` | Karatsuba-style |
| `T(n)=4T(n/2)+n` | `Θ(n²)` |  |
| `T(n)=4T(n/2)+n²` | `Θ(n² log n)` |  |
| `T(n)=7T(n/2)+n²` | `Θ(n^{2.807})` | Strassen |
| `T(n)=8T(n/2)+n²` | `Θ(n³)` | naive matrix mult |
| `T(n)=3T(n/3)+n` | `Θ(n log n)` | 3-way merge sort |
| `T(n)=T(n/3)+T(2n/3)+n` | `Θ(n log n)` | unbalanced quicksort |
| `T(n)=T(n/5)+T(7n/10)+n` | `Θ(n)` | median of medians |
| `T(n)=T(√n)+1` | `Θ(log log n)` | van Emde Boas |
| `T(n)=2T(√n)+log n` | `Θ(log n log log n)` |  |
| `T(n)=√n·T(√n)+n` | `Θ(n log log n)` |  |
| `T(n)=T(n−1)+1/n` | `Θ(log n)` | harmonic |

---

# 17. Computing Time Complexity from Code

## 17.1 The Systematic Procedure

1. **Identify `n`** — the input size parameter.
2. **Assign a cost and a frequency to every statement.**
3. **Sum:** `T(n) = Σ (cost_i × frequency_i)`.
4. **Simplify** — drop constants and lower-order terms.

### Full Frequency-Count Example: Bubble Sort

```
                                        cost    times
BUBBLE-SORT(A, n):
1   for i = 0 to n−2:                    c₁      n
2       for j = 0 to n−2−i:              c₂      Σ_{i=0}^{n−2}(n−i)
3           if A[j] > A[j+1]:            c₃      Σ_{i=0}^{n−2}(n−1−i)
4               swap(A[j], A[j+1])       c₄      ≤ Σ_{i=0}^{n−2}(n−1−i)
```

Inner loop runs `(n−1) + (n−2) + ⋯ + 1 = n(n−1)/2` times.

```
T(n) = c₁n + c₂[n(n−1)/2 + n−1] + c₃·n(n−1)/2 + c₄·(≤ n(n−1)/2)
     = An² + Bn + C
     = Θ(n²)
```

- Best case (already sorted, with early-exit flag): `Θ(n)` — one pass, no swaps.
- Without the flag: `Θ(n²)` in all cases.
- Swaps: worst `n(n−1)/2`, best `0`, average `n(n−1)/4`.

## 17.2 The Structural Rules

| Construct | Rule |
| --- | --- |
| Simple statement (assignment, arithmetic, comparison, array index) | `Θ(1)` |
| Sequence `S₁; S₂` | `T₁ + T₂ = Θ(max(T₁, T₂))` |
| `if C then S₁ else S₂` | `T_C + max(T₁, T₂)` (worst case) |
| Loop | `Σ over iterations of (body cost)`; if body is constant, `Θ(#iterations)` |
| Nested loops (independent) | product of iteration counts |
| Nested loops (dependent) | must sum — see §18 |
| Function call | cost of the function body |
| Recursion | write and solve a recurrence |

### Sequential blocks

```
for i in 1..n:        # Θ(n)
    print(i)
for i in 1..n:
    for j in 1..n:    # Θ(n²)
        print(i,j)
```

`T(n) = Θ(n) + Θ(n²) = Θ(n²)` ✔ (**add, then take the max**)

### Nested blocks

```
for i in 1..n:            # n times
    for j in 1..n:        # n times
        for k in 1..n:    # n times
            x++           # Θ(1)
```

`T(n) = Θ(n³)` ✔ (**multiply**)

### Conditional inside loop

```
for i in 1..n:
    if isPrime(i):        # Θ(√i)
        doSomething()     # Θ(1)
```

`T(n) = Σ_{i=1}^{n} Θ(√i) = Θ(n^{1.5})`

## 17.3 Hidden Costs (extremely common source of error)

| Operation | Naive assumption | Reality |
| --- | --- | --- |
| `s = s + "x"` (immutable strings) | `O(1)` | `O(len(s))` → loop becomes `O(n²)` |
| `list.insert(0, x)` (Python) | `O(1)` | `O(n)` |
| `list.pop(0)` | `O(1)` | `O(n)` |
| `x in list` | `O(1)` | `O(n)` |
| `x in set/dict` | — | `O(1)` average, `O(n)` worst |
| `list.append(x)` | — | `O(1)` **amortized** |
| Slicing `a[i:j]` | `O(1)` | `O(j−i)` — copies! |
| `len(list)` | — | `O(1)` (length is stored) |
| `min(list)` / `max(list)` / `sum(list)` | `O(1)` | `O(n)` |
| `sorted(list)` | — | `O(n log n)` |
| Copying an array | `O(1)` | `O(n)` |
| Passing array by value | `O(1)` | `O(n)` |
| `dict.keys()` iteration | — | `O(n)` |
| Concatenating two lists | — | `O(n+m)` |
| Set union / intersection | — | `O(min(n,m))` to `O(n+m)` |
| Java `substring` (post-JDK7) | `O(1)` | `O(n)` |

> **Classic trap:**
> 
> 
> ```python
> s = ""
> for i in range(n):
>     s += "a"          # each += copies the whole string
> ```
> 
> `T(n) = Σ_{i=1}^{n} O(i) = O(n²)`, **not** `O(n)`.
> Fix: `"".join(["a"] * n)` → `O(n)`.
> 

> **Classic trap 2:**
> 
> 
> ```python
> for i in range(n):
>     if arr[i] in seen_list:    # O(n) membership test
>         ...
> ```
> 
> `O(n²)`. Fix: use a `set` → `O(n)`.
> 

## 17.4 Analysing Recursive Code

**Procedure:**
1. Count recursive calls and their argument sizes.
2. Count non-recursive work per call.
3. Write `T(n) = (calls) + (work)`.
4. Solve with the appropriate method.

```
FACTORIAL(n):
    if n <= 1: return 1
    return n * FACTORIAL(n−1)
```

`T(n) = T(n−1) + Θ(1)` → `Θ(n)`. Space: `Θ(n)` stack.

```
POWER(x, n):                       # naive
    if n == 0: return 1
    return x * POWER(x, n−1)
```

`T(n) = T(n−1) + Θ(1) = Θ(n)`

```
FAST-POWER(x, n):                  # exponentiation by squaring
    if n == 0: return 1
    h = FAST-POWER(x, n/2)
    if n even: return h*h
    else:      return h*h*x
```

`T(n) = T(n/2) + Θ(1) = Θ(log n)`

```
BAD-POWER(x, n):                   # NOTE: two recursive calls!
    if n == 0: return 1
    if n even: return BAD-POWER(x,n/2) * BAD-POWER(x,n/2)
    else:      return x * BAD-POWER(x,n−1)
```

`T(n) = 2T(n/2) + Θ(1) = Θ(n)` — the “optimization” is destroyed by making two calls.

---

# 18. Loop Analysis Patterns — Exhaustive Catalogue

## 18.1 Additive (Arithmetic) Increment

```
for (i = 0; i < n; i = i + 1)      →  n iterations           → Θ(n)
for (i = 0; i < n; i = i + 2)      →  n/2 iterations         → Θ(n)
for (i = 0; i < n; i = i + c)      →  n/c iterations         → Θ(n)
for (i = n; i > 0; i = i − 1)      →  n iterations           → Θ(n)
for (i = 0; i < n; i = i + k)      →  ⌈n/k⌉                  → Θ(n/k)
```

**Rule:** constant additive step → `Θ(n)`. Constants like `/2` never change the order.

## 18.2 Multiplicative (Geometric) Increment

```
for (i = 1; i <= n; i = i * 2)     →  ⌊log₂ n⌋ + 1           → Θ(log n)
for (i = 1; i <= n; i = i * 3)     →  ⌊log₃ n⌋ + 1           → Θ(log n)
for (i = 1; i <= n; i = i * k)     →  Θ(log_k n) = Θ(log n)
for (i = n; i >= 1; i = i / 2)     →  Θ(log n)
for (i = n; i >= 1; i = i / k)     →  Θ(log n)
```

**Derivation:** after `t` iterations `i = 2^t`. Loop stops when `2^t > n ⟹ t > log₂ n`.

## 18.3 Double-Exponential Increment

```
for (i = 2; i < n; i = i * i)      →  Θ(log log n)
```

**Derivation:** `i` takes values `2, 4, 16, 256, 65536, …` i.e. `2^{2^t}`.
Stop when `2^{2^t} ≥ n ⟹ 2^t ≥ log n ⟹ t ≥ log log n`. → `Θ(log log n)`

```
for (i = 2; i < n; i = i^3)        →  Θ(log log n)   (base of inner log changes only constants)
for (i = n; i > 1; i = sqrt(i))    →  Θ(log log n)
```

## 18.4 Increment by a Growing Amount

```
for (i = 1, j = 0; i < n; j++, i = i + j)
```

`i` takes values `1, 2, 4, 7, 11, …` = `1 + j(j+1)/2`. Loop stops when `j²/2 ≈ n ⟹ j = Θ(√n)`.
→ **`Θ(√n)`**

```
for (i = 0; i*i < n; i++)          →  Θ(√n)
for (i = 1; i*i*i <= n; i++)       →  Θ(n^{1/3})
for (i = 0; i < n; i += i)         →  INFINITE LOOP if i starts at 0!
```

## 18.5 Loop Controlled by a Sum

```
sum = 0
for (i = 1; sum < n; i++)
    sum = sum + i
```

Stops when `i(i+1)/2 ≥ n ⟹ i = Θ(√n)`. → **`Θ(√n)`**

## 18.6 Nested Loops — Independent

```
for i in 1..n:
    for j in 1..n:
        stmt                       →  n × n = Θ(n²)

for i in 1..n:
    for j in 1..m:
        stmt                       →  Θ(nm)

for i in 1..n:
    for j in 1..n:
        for k in 1..n:
            stmt                   →  Θ(n³)
```

## 18.7 Nested Loops — Dependent (Triangular)

```
for i = 1 to n:
    for j = 1 to i:
        stmt
```

```
T(n) = Σ_{i=1}^{n} i = n(n+1)/2 = Θ(n²)
```

```
for i = 1 to n:
    for j = i to n:
        stmt
```

```
T(n) = Σ_{i=1}^{n} (n − i + 1) = n + (n−1) + ⋯ + 1 = n(n+1)/2 = Θ(n²)
```

```
for i = 1 to n:
    for j = 1 to i:
        for k = 1 to j:
            stmt
```

```
T(n) = Σ_{i=1}^{n} Σ_{j=1}^{i} j = Σ_{i=1}^{n} i(i+1)/2 = Θ(n³)
```

> **Key insight:** a triangular double loop is `n²/2` — **still `Θ(n²)`**. The constant `1/2` disappears.
> 

## 18.8 Nested Loops — Mixed Increment Types

```
for (i = 1; i <= n; i++)              # n times
    for (j = 1; j <= n; j = j * 2)    # log n times
        stmt
```

→ **`Θ(n log n)`**

```
for (i = 1; i <= n; i = i * 2)        # log n times
    for (j = 1; j <= i; j++)          # i times
        stmt
```

`i` takes `1, 2, 4, …, 2^{log n} = n`:

```
T(n) = Σ_{k=0}^{log n} 2^k = 2^{log n + 1} − 1 = 2n − 1 = Θ(n)
```

> ⚠️ **Not `Θ(n log n)`!** The inner loop’s cost varies. This is the single most common analysis mistake.
> 

```
for (i = 1; i <= n; i = i * 2)        # log n times
    for (j = 1; j <= n; j = j * 2)    # log n times
        stmt
```

→ **`Θ(log² n)`**

```
for (i = n; i >= 1; i = i / 2)        # log n times
    for (j = 1; j <= i; j++)          # i times
        stmt
```

`i` takes `n, n/2, n/4, …, 1`:

```
T(n) = n + n/2 + n/4 + ⋯ + 1 = 2n − 1 = Θ(n)
```

```
for (i = 1; i <= n; i++)              # n times
    for (j = 1; j <= n; j += i)       # n/i times
        stmt
```

```
T(n) = Σ_{i=1}^{n} n/i = n Σ_{i=1}^{n} 1/i = n·H_n = Θ(n log n)
```

> This is the **Sieve of Eratosthenes** pattern (without the prime restriction).
> 

```
for (i = 1; i <= n; i++)
    for (j = 1; j <= n; j *= i)       # careful: i=1 → infinite loop
```

```
for (i = 1; i*i <= n; i++)            # √n times
    for (j = 1; j <= n; j++)          # n times
        stmt
```

→ **`Θ(n^{1.5})`**

```
for (i = 1; i <= n; i++)              # n times
    for (j = 1; j <= i*i; j++)        # i² times
        stmt
```

```
T(n) = Σ_{i=1}^{n} i² = Θ(n³)
```

```
for (i = 1; i <= n; i++)
    for (j = 1; j <= i; j++)
        for (k = 1; k <= 100; k++)    # constant
            stmt
```

→ **`Θ(n²)`** (the constant inner loop contributes only a factor of 100)

## 18.9 The Sieve of Eratosthenes

```
for (i = 2; i <= n; i++)
    if (isPrime[i])
        for (j = i*i; j <= n; j += i)
            isPrime[j] = false
```

```
T(n) = Σ_{p prime, p ≤ n} n/p = n Σ_{p ≤ n} 1/p = n · Θ(log log n) = Θ(n log log n)
```

(Uses Mertens’ theorem: `Σ_{p ≤ n} 1/p = ln ln n + M + o(1)`.)

## 18.10 While Loops

```
i = n
while (i > 1):
    i = i / 2                        →  Θ(log n)

i = 1
while (i < n):
    i = i * 2                        →  Θ(log n)

i = n
while (i > 0):
    i = i − k                        →  Θ(n/k) = Θ(n)

while (n > 0):
    n = n / 10                       →  Θ(log₁₀ n) = Θ(log n)   ← digit extraction
```

**Euclid’s GCD:**

```
GCD(a, b):
    while (b != 0):
        (a, b) = (b, a mod b)
    return a
```

Each two iterations at least halve the larger value (Lamé’s theorem: the worst case is consecutive Fibonacci numbers).
→ **`O(log(min(a,b)))`**. In bit-terms `O(log a)` iterations, each a `Θ(log² a)` division for bignums.

## 18.11 Loops Modifying the Loop Variable Inside

```
for (i = 0; i < n; i++):
    if (condition):
        i = i * 2                     # complexity depends on how often condition fires
```

Must analyse case by case. Best case `Θ(log n)`, worst `Θ(n)`.

## 18.12 Two-Pointer / Sliding Window

```
left = 0
for right in 0..n−1:
    while (condition):
        left++
```

Even though there is a nested `while`, **`left` moves at most `n` times total across the entire run**.
→ **`Θ(n)`**, not `Θ(n²)`. This is an *amortized* argument (aggregate method).

## 18.13 Recursive Loops Over Subsets

```
for (S = mask; S > 0; S = (S−1) & mask)     # iterate submasks of mask
```

For all masks of `n` bits, the total is `Σ_{k} C(n,k)2^k = 3ⁿ`.
→ **`Θ(3ⁿ)`** — the classic subset-sum DP over subsets bound.

## 18.14 Summary Table of Loop Patterns

| Loop | Iterations |
| --- | --- |
| `i += 1` up to `n` | `Θ(n)` |
| `i += c` up to `n` | `Θ(n)` |
| `i *= 2` up to `n` | `Θ(log n)` |
| `i /= 2` down to 1 | `Θ(log n)` |
| `i = i*i` up to `n` | `Θ(log log n)` |
| `i = √i` down to 2 | `Θ(log log n)` |
| `i` up to `√n` | `Θ(√n)` |
| `i += j; j++` | `Θ(√n)` |
| `sum += i` until `sum ≥ n` | `Θ(√n)` |
| nested `i: 1..n`, `j: 1..i` | `Θ(n²)` |
| nested `i: 1..n`, `j: 1..n step i` | `Θ(n log n)` |
| nested `i *= 2`, `j: 1..i` | `Θ(n)` |
| nested `i *= 2`, `j *= 2` | `Θ(log² n)` |
| nested `i: 1..n`, `j *= 2` up to n | `Θ(n log n)` |
| nested `i: 1..n`, `j: 1..i²` | `Θ(n³)` |

---

# 19. Space Complexity

## 19.1 Definitions

**Space complexity** `S(n)` = total memory an algorithm needs as a function of input size.

```
S(n) = Input Space + Auxiliary Space
```

| Component | Description |
| --- | --- |
| **Input space** | memory to store the input itself (`Θ(n)` for an array) |
| **Auxiliary space** | extra memory the algorithm allocates beyond the input |
| **Instruction space** | the compiled code — constant, usually ignored |
| **Data space** | constants, static variables |
| **Environment/stack space** | activation records for function calls, including recursion |

> **Convention:** When people say “the space complexity of merge sort is `O(n)`”, they almost always mean **auxiliary space**. Always state which you mean.
> 

**In-place algorithm:** uses `O(1)` auxiliary space (or sometimes `O(log n)`, to permit recursion stacks).

## 19.2 Counting Auxiliary Space

| Declaration | Space |
| --- | --- |
| `int x, y, z` | `Θ(1)` |
| `int A[n]` | `Θ(n)` |
| `int A[n][n]` | `Θ(n²)` |
| `int A[n][m]` | `Θ(nm)` |
| A list built to hold `k` results | `Θ(k)` |
| A hash set holding up to `n` keys | `Θ(n)` |
| DP table `dp[n][W]` | `Θ(nW)` |
| Recursion depth `d`, `O(1)` locals per frame | `Θ(d)` |
| Recursion depth `d`, `O(n)` locals per frame | `Θ(dn)` |

## 19.3 Recursion Stack Space

**Rule:** stack space = **maximum depth of the recursion tree** × size of each frame.
It is the *depth*, not the *number of nodes*.

| Algorithm | Recursion depth | Stack space |
| --- | --- | --- |
| Factorial(n) | `n` | `Θ(n)` |
| Binary search (recursive) | `log n` | `Θ(log n)` |
| Merge sort | `log n` | `Θ(log n)` (+ `Θ(n)` for merge buffer) |
| QuickSort worst case | `n` | `Θ(n)` |
| QuickSort with tail-call on larger side | `log n` | `Θ(log n)` |
| Naive Fibonacci | `n` | `Θ(n)` (though `Θ(φⁿ)` **time**) |
| DFS on a graph | `V` | `Θ(V)` |
| Tree traversal | height `h` | `Θ(h)`, `Θ(n)` for a skewed tree |
| Tower of Hanoi | `n` | `Θ(n)` (time `Θ(2ⁿ)`) |
| Merge sort on linked list | `log n` | `Θ(log n)` |

> **Critical distinction:** naive Fibonacci makes `Θ(φⁿ)` calls but only `n` are ever *live at once*, because the tree is explored depth-first. **Time `Θ(φⁿ)`, space `Θ(n)`.**
> 

## 19.4 Space Complexity of Common Algorithms

| Algorithm | Time (avg/worst) | Auxiliary Space |
| --- | --- | --- |
| Linear search | `Θ(n)` | `Θ(1)` |
| Binary search (iterative) | `Θ(log n)` | `Θ(1)` |
| Binary search (recursive) | `Θ(log n)` | `Θ(log n)` |
| Bubble / Selection / Insertion sort | `Θ(n²)` | `Θ(1)` |
| Merge sort (array) | `Θ(n log n)` | `Θ(n)` |
| Merge sort (linked list) | `Θ(n log n)` | `Θ(log n)` |
| In-place merge sort | `Θ(n log² n)` | `Θ(1)` |
| Quick sort | `Θ(n log n)` / `Θ(n²)` | `Θ(log n)` / `Θ(n)` |
| Heap sort | `Θ(n log n)` | `Θ(1)` |
| Counting sort | `Θ(n + k)` | `Θ(n + k)` |
| Radix sort | `Θ(d(n+k))` | `Θ(n + k)` |
| Bucket sort | `Θ(n)` avg | `Θ(n)` |
| Tim sort | `Θ(n log n)` | `Θ(n)` |
| BFS | `Θ(V+E)` | `Θ(V)` |
| DFS | `Θ(V+E)` | `Θ(V)` |
| Dijkstra (binary heap) | `Θ((V+E)log V)` | `Θ(V)` |
| Floyd–Warshall | `Θ(V³)` | `Θ(V²)` |
| 0/1 Knapsack DP (2D) | `Θ(nW)` | `Θ(nW)` |
| 0/1 Knapsack DP (rolling) | `Θ(nW)` | `Θ(W)` |
| LCS DP (2D) | `Θ(nm)` | `Θ(nm)` |
| LCS DP (rolling, length only) | `Θ(nm)` | `Θ(min(n,m))` |
| Fibonacci memoized | `Θ(n)` | `Θ(n)` |
| Fibonacci iterative (2 vars) | `Θ(n)` | `Θ(1)` |
| Matrix chain multiplication | `Θ(n³)` | `Θ(n²)` |
| Trie with `n` strings of length `L` | build `Θ(nL)` | `Θ(nL·σ)` |

## 19.5 Space–Time Tradeoffs

| Technique | Time saved | Space cost |
| --- | --- | --- |
| Memoization | exponential → polynomial | table size |
| Hash table lookup | `O(n)` → `O(1)` | `O(n)` |
| Precomputed prefix sums | `O(n)` per query → `O(1)` | `O(n)` |
| Sparse table (RMQ) | `O(n)` per query → `O(1)` | `O(n log n)` |
| Counting sort | `O(n log n)` → `O(n+k)` | `O(k)` |
| Rolling array DP | — | `O(nW)` → `O(W)` |
| Bitset | 64× constant speedup | `n/64` words |

## 19.6 Worked Space Examples

**Example 1: reverse an array in place**

```
for i = 0 to n/2 − 1:
    swap(A[i], A[n−1−i])
```

Auxiliary: one loop variable + one temp → **`Θ(1)`**. Total with input: `Θ(n)`.

**Example 2: reverse into a new array**

```
B = new array[n]
for i = 0 to n−1: B[i] = A[n−1−i]
```

Auxiliary: **`Θ(n)`**.

**Example 3: recursive sum of an array**

```
SUM(A, n):
    if n == 0: return 0
    return A[n−1] + SUM(A, n−1)
```

Time `Θ(n)`, **stack space `Θ(n)`**. The iterative version uses `Θ(1)`.

**Example 4: generate all subsets**

```
SUBSETS(A, i, current):
    if i == n: output(current); return
    SUBSETS(A, i+1, current)
    SUBSETS(A, i+1, current + [A[i]])
```

Time `Θ(n·2ⁿ)` (2ⁿ subsets, `O(n)` to copy/output each).
Space: recursion depth `n`, plus `current` of size ≤ n → **`Θ(n)`** auxiliary (if outputs are streamed).
If all subsets are stored: `Θ(n·2ⁿ)`.

**Example 5: DP with rolling array**

```
# 0/1 Knapsack, 2D:  dp[i][w]                → Θ(nW) space
# Rolling 1D:
for i in 1..n:
    for w in W down to wt[i]:
        dp[w] = max(dp[w], dp[w−wt[i]] + val[i])
```

→ **`Θ(W)`** space, same `Θ(nW)` time.

**Example 6: two-pointer vs hash set**

```
# Two Sum with hash set:  Time Θ(n), Space Θ(n)
# Two Sum on sorted array with two pointers:  Time Θ(n), Space Θ(1)
# Two Sum by sorting first:  Time Θ(n log n), Space Θ(1) or Θ(n)
```

---

# 20. Worked Analyses — Iterative Algorithms

## 20.1 Selection Sort

```
SELECTION-SORT(A, n):
    for i = 0 to n−2:
        min_idx = i
        for j = i+1 to n−1:
            if A[j] < A[min_idx]: min_idx = j
        swap(A[i], A[min_idx])
```

**Comparisons:** `Σ_{i=0}^{n−2} (n−1−i) = (n−1)+(n−2)+⋯+1 = n(n−1)/2`**Swaps:** exactly `n−1` (always).

|  | Time | Space |
| --- | --- | --- |
| Best | `Θ(n²)` | `Θ(1)` |
| Average | `Θ(n²)` | `Θ(1)` |
| Worst | `Θ(n²)` | `Θ(1)` |

**Property:** the number of comparisons is *input-independent*. Not adaptive. Not stable (in the standard swap-based version). Minimizes writes — useful when writes are expensive (flash memory).

## 20.2 Insertion Sort

```
INSERTION-SORT(A, n):
    for i = 1 to n−1:
        key = A[i]
        j = i − 1
        while j >= 0 and A[j] > key:
            A[j+1] = A[j]
            j = j − 1
        A[j+1] = key
```

Let `t_i` = number of times the while condition is tested for a given `i`.

```
T(n) = c₁n + c₂(n−1) + c₃(n−1) + c₄Σ_{i=1}^{n−1}t_i + c₅Σ(t_i−1) + c₆Σ(t_i−1) + c₇(n−1)
```

- **Best case** (sorted): `t_i = 1` for all `i` → `T(n) = an + b = Θ(n)`
- **Worst case** (reverse sorted): `t_i = i + 1` → `Σ t_i = n(n+1)/2 − 1` → `Θ(n²)`
- **Average case**: on average half the sorted prefix is scanned, `t_i ≈ i/2` → `Θ(n²)`

**Number of inversions:** Insertion sort runs in `Θ(n + I)` where `I` = number of inversions. Since `0 ≤ I ≤ n(n−1)/2`, this recovers both bounds. **Insertion sort is adaptive.**

**Stable:** yes. **In-place:** yes, `Θ(1)`.

## 20.3 Bubble Sort with Early Exit

```
BUBBLE-SORT(A,n):
    for i = 0 to n−2:
        swapped = false
        for j = 0 to n−2−i:
            if A[j] > A[j+1]: swap; swapped = true
        if not swapped: break
```

- Best: `Θ(n)` (one pass)
- Worst / Average: `Θ(n²)`
- Comparisons worst: `n(n−1)/2`; swaps worst: `n(n−1)/2` = number of inversions.

## 20.4 Binary Search

```
BINARY-SEARCH(A, lo, hi, key):
    while lo <= hi:
        mid = lo + (hi − lo)/2
        if A[mid] == key: return mid
        elif A[mid] < key: lo = mid + 1
        else: hi = mid − 1
    return −1
```

**Recurrence:** `T(n) = T(n/2) + Θ(1)` → `T(n) = Θ(log n)`.
**Exact worst-case comparisons:** `⌊log₂ n⌋ + 1`.
**Average successful search:** `≈ log₂ n − 1`.
**Space:** iterative `Θ(1)`, recursive `Θ(log n)`.

**Why `mid = lo + (hi−lo)/2` and not `(lo+hi)/2`:** avoids integer overflow. (Famous bug in the JDK’s `binarySearch` for 20 years.)

## 20.5 Matrix Multiplication (Naive)

```
for i = 0 to n−1:
    for j = 0 to n−1:
        C[i][j] = 0
        for k = 0 to n−1:
            C[i][j] += A[i][k]*B[k][j]
```

**Time `Θ(n³)`**, **space `Θ(n²)`** (for the output) or `Θ(1)` auxiliary if `C` is given.

> ⚠️ Note: input size is `Θ(n²)` numbers. In terms of input size `N = n²`, this is `Θ(N^{1.5})` — sub-quadratic in the *actual* input size.
> 

## 20.6 Kadane’s Algorithm (Maximum Subarray)

```
max_so_far = A[0]; max_ending_here = A[0]
for i = 1 to n−1:
    max_ending_here = max(A[i], max_ending_here + A[i])
    max_so_far = max(max_so_far, max_ending_here)
```

**Time `Θ(n)`, space `Θ(1)`.** Compare: brute force `Θ(n³)` → prefix-sum brute force `Θ(n²)` → divide & conquer `Θ(n log n)` → Kadane `Θ(n)`.

## 20.7 Two Sum Variants

| Approach | Time | Space |
| --- | --- | --- |
| Brute force nested loops | `Θ(n²)` | `Θ(1)` |
| Sort + two pointers | `Θ(n log n)` | `Θ(1)` |
| Hash map (one pass) | `Θ(n)` avg | `Θ(n)` |

## 20.8 Prefix Sums / Range Queries

```
Build: P[0]=0; for i in 1..n: P[i] = P[i−1] + A[i]     Θ(n) time, Θ(n) space
Query sum(l,r) = P[r] − P[l−1]                          Θ(1)
```

Total for `q` queries: `Θ(n + q)` vs naive `Θ(nq)`.

**2D prefix sums:** build `Θ(nm)`, query `Θ(1)` with inclusion–exclusion.

## 20.9 String Matching

| Algorithm | Preprocessing | Matching | Total | Space |
| --- | --- | --- | --- | --- |
| Naive | — | `O((n−m+1)m)` | `O(nm)` | `O(1)` |
| Rabin–Karp | `O(m)` | `O(n+m)` avg, `O(nm)` worst | `O(n+m)` avg | `O(1)` |
| KMP | `O(m)` | `O(n)` | `O(n+m)` | `O(m)` |
| Boyer–Moore | `O(m+σ)` | `O(n/m)` best, `O(nm)` worst | — | `O(m+σ)` |
| Z-algorithm | `O(m)` | `O(n)` | `O(n+m)` | `O(n+m)` |
| Aho–Corasick (k patterns) | `O(Σ\|p_i\|)` | `O(n + occ)` | — | `O(Σ\|p_i\|σ)` |
| Suffix automaton | `O(n)` | `O(m)` | — | `O(n σ)` |

**KMP failure function analysis (amortized):**

```
j = 0
for i = 1 to m−1:
    while j > 0 and P[i] != P[j]: j = π[j−1]
    if P[i] == P[j]: j++
    π[i] = j
```

`j` increases by at most 1 per iteration (at most `m` times total), and each `while` iteration strictly decreases `j`. Since `j ≥ 0`, the total number of `while` iterations across all `i` is at most `m`. → **`Θ(m)` total.** This is the aggregate amortized method.

---
