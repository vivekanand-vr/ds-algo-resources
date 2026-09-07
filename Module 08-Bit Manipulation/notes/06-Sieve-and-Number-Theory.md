# Sieves and Number Theory — The Complete Reference

> The Advanced Math folder steps just outside bitwise operators into classical number theory — prime factorization, divisor sums, and sieves — because the same "precompute once, answer fast" mindset from bit manipulation applies directly here, and `pow` bridges the two by reusing binary exponentiation.

---

## Table of Contents

1. [Trial Division — Prime Factorization of One Number](#1-trial-division--prime-factorization-of-one-number)
2. [Divisor Contribution Counting](#2-divisor-contribution-counting)
3. [Sieve of Eratosthenes](#3-sieve-of-eratosthenes)
4. [Smallest Prime Factor (SPF) Sieve](#4-smallest-prime-factor-spf-sieve)
5. [Choosing an Approach](#5-choosing-an-approach)
6. [Worked Examples](#6-worked-examples)
7. [Common Pitfalls](#7-common-pitfalls)
8. [Related Problems in This Module](#8-related-problems-in-this-module)

---

## 1. Trial Division — Prime Factorization of One Number

> Find the unique prime factors of a single number `n`.

### The insight

Any composite number `n` has at least one factor `<= sqrt(n)` — if every factor were larger, their product would exceed `n`. So testing divisibility only needs to go up to `sqrt(n)`, not up to `n`.

For each candidate divisor `i` starting at 2: if `i` divides `n`, record it once, then **keep dividing `n` by `i`** until it no longer divides evenly. This strips out every copy of that prime factor in one pass, so a repeated factor (like `2^2` in `60`) is recorded only once, and it also shrinks `n`, making the `i * i <= n` bound tighten as the loop progresses.

```cpp
vector<int> primeFactors(int n) {
    vector<int> factors;
    for (int i = 2; (long long)i * i <= n; i++) {
        if (n % i == 0) {
            factors.push_back(i);
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) factors.push_back(n);   // whatever remains is one last prime factor
    return factors;
}
```

The `if (n > 1)` step at the end matters: after the loop, `n` might still hold a single large prime factor bigger than `sqrt(original n)` — it was never divided out because the loop bound `i*i <= n` became false before `i` reached it.

**Time O(sqrt n)**, since divisors are tested up to `sqrt(n)`.

→ [01-prime_factors.cpp](../03-Advanced%20Math/01-prime_factors.cpp)

---

## 2. Divisor Contribution Counting

> Find `Σ F(i)` for `i = 1` to `n`, where `F(i)` is the sum of all divisors of `i`.

### Why the brute force is too slow

Computing `F(i)` for each `i` individually via trial division costs `O(sqrt(i))`, so the total is `O(n sqrt(n))` — too slow for `n` in the range of `10^7`+.

### The insight — flip the question around

Instead of "what are the divisors of each `i`", ask: **"for a fixed value `d`, how many times, and where, does `d` get counted across the whole sum?"**

A value `d` divides `i` exactly when `i` is a multiple of `d` — that is, `i = d, 2d, 3d, ..., up to n`. There are exactly `floor(n/d)` such multiples. Each time `d` divides one of them, it contributes `d` (not 1) to that number's divisor sum. So `d`'s total contribution across the entire sum `Σ F(i)` is:

```
d * floor(n / d)
```

Summing this over every `d` from 1 to `n` gives the answer directly — no factoring required at all.

```cpp
long long sumOfDivisors(long long n) {
    long long total = 0;
    for (long long d = 1; d <= n; d++) {
        total += d * (n / d);
    }
    return total;
}
```

**Time O(n)** — a dramatic improvement over `O(n sqrt(n))`. (A further `O(sqrt n)` refinement exists using the fact that `floor(n/d)` only takes `O(sqrt n)` distinct values and can be processed in blocks, but `O(n)` is already the standard efficient answer for this problem.)

→ [02-all_divisors.cpp](../03-Advanced%20Math/02-all_divisors.cpp)

This "swap the order of summation — count each contributor's total effect instead of recomputing per-target" idea is the same shape as the *contribution technique* mentioned in the Arrays module's subarray notes — a recurring pattern worth recognising across topics.

---

## 3. Sieve of Eratosthenes

> Count how many primes are strictly less than `n`.

### The insight

Rather than testing each number individually for primality (`O(sqrt(i))` each, `O(n sqrt n)` total), mark composites in bulk: starting from the smallest prime, cross out every one of its multiples. Whatever survives unmarked is prime — a composite number always has a prime factor that will have already crossed it out.

**Optimisation: start marking from `i * i`, not `2i`.** Any smaller multiple of `i` (like `2i`, `3i`, ..., `(i-1)i`) has a prime factor smaller than `i` and was already marked when that smaller prime was processed. Starting at `i*i` skips redundant work.

```cpp
int countPrimes(int n) {
    if (n <= 2) return 0;
    vector<bool> isComposite(n, false);

    for (int i = 2; (long long)i * i < n; i++) {
        if (!isComposite[i]) {
            for (int j = i * i; j < n; j += i) {
                isComposite[j] = true;
            }
        }
    }

    int count = 0;
    for (int i = 2; i < n; i++) if (!isComposite[i]) count++;
    return count;
}
```

**Time O(n log log n)** — the classic sieve bound (each prime `p` contributes roughly `n/p` marking operations, and the sum of `1/p` over all primes up to `n` grows as `log log n`). **Space O(n)**.

→ [03-sieve_of_eratosthenes.cpp](../03-Advanced%20Math/03-sieve_of_eratosthenes.cpp)

---

## 4. Smallest Prime Factor (SPF) Sieve

> Given many query numbers, return the full prime factorization (with multiplicity) of each.

### Why plain trial division per query is wasteful

Factoring each query independently costs `O(sqrt(value))` — fine for one query, but wasteful when many queries share the same small prime factors, which get rediscovered from scratch every time.

### The insight — precompute once, factor in O(log) after

Extend the sieve so that instead of just marking "composite", it records **which prime first marked each number** — its smallest prime factor (SPF).

```cpp
const int MAXN = 100001;
vector<int> spf(MAXN, 0);   // spf[i] = smallest prime factor of i

void buildSpfSieve() {
    for (int i = 2; i < MAXN; i++) {
        if (spf[i] == 0) {                        // i has no smaller factor -> i is prime
            for (long long j = (long long)i * i; j < MAXN; j += i) {
                if (spf[j] == 0) spf[j] = i;       // don't overwrite an earlier, smaller prime
            }
            spf[i] = i;                            // i is its own smallest prime factor
        }
    }
}
```

Once the table is built, factoring any single number is a tight loop: repeatedly divide by its own SPF and record that prime, until it becomes 1. Each division strips off exactly the *smallest* remaining prime factor, so **factors come out in increasing order automatically** — no sorting step needed.

```cpp
vector<int> factorize(int x) {
    vector<int> factors;
    while (x != 1) {
        factors.push_back(spf[x]);
        x /= spf[x];
    }
    return factors;
}
```

**Time O(MAXN log log MAXN)** to build the sieve once, then **O(log x) per query** afterward — a huge win when there are many queries.

→ [04-prime_factors_using_sieve.cpp](../03-Advanced%20Math/04-prime_factors_using_sieve.cpp)

### Why `if (spf[j] == 0) spf[j] = i` (not unconditional)

Primes are processed in increasing order (`i = 2, 3, 5, 7, ...`). The **first** prime to reach a given `j` is necessarily its *smallest* prime factor — so once `spf[j]` is set, it must never be overwritten by a larger prime encountered later.

---

## 5. Choosing an Approach

```
How many numbers need factoring?
|
+-- Just one, and it could be large (up to ~10^9 or more)
|       -> Trial division up to sqrt(n)                     [Section 1]
|
+-- Many numbers, all bounded by some fixed MAXN
        -> Build an SPF sieve once, factor each in O(log)    [Section 4]

Need to know primality / count primes for a whole range?
        -> Sieve of Eratosthenes                              [Section 3]

Need a sum over divisors of every number up to n?
        -> Divisor contribution counting (flip the sum order) [Section 2]
```

| Task | Best approach | Time |
| --- | --- | --- |
| Factor one large number | Trial division | O(sqrt n) |
| Factor many bounded numbers | SPF sieve (build once) | O(MAXN log log MAXN) + O(log x)/query |
| Count primes below n | Sieve of Eratosthenes | O(n log log n) |
| Σ divisor-sum for 1..n | Contribution counting | O(n) |

---

## 6. Worked Examples

### 6.1 Trial division on n = 60

| i | n % i == 0? | record | n after stripping |
| --- | --- | --- | --- |
| 2 | yes | 2 | 60 → 30 → 15 (stop dividing, 15 % 2 ≠ 0) |
| 3 | yes | 3 | 15 → 5 |
| 4 | i*i=16 > 5, loop ends | — | — |

`n = 5 > 1` at the end → record `5`.

**Answer: {2, 3, 5}** — matches `60 = 2² × 3 × 5`.

### 6.2 Divisor contribution counting on n = 4

| d | n / d | d * (n/d) |
| --- | --- | --- |
| 1 | 4 | 4 |
| 2 | 2 | 4 |
| 3 | 1 | 3 |
| 4 | 1 | 4 |

**Total: 15.** Direct check: `F(1)=1, F(2)=3, F(3)=4, F(4)=7`, sum `= 1+3+4+7 = 15`. ✓

### 6.3 Sieve of Eratosthenes on n = 10

Composite marking: `i=2` marks `4, 6, 8`. `i=3`: `i*i=9 < 10`, marks `9`.

Unmarked in `[2, 9]`: `2, 3, 5, 7` → **4 primes.**

### 6.4 SPF factorization of 60

`spf[60] = 2` (60 → 30, record 2). `spf[30] = 2` (30 → 15, record 2). `spf[15] = 3` (15 → 5, record 3). `spf[5] = 5` (5 → 1, record 5).

**Answer: {2, 2, 3, 5}**, already in increasing order.

---

## 7. Common Pitfalls

1. **Trial division: forgetting the final `if (n > 1)` check.** A large leftover prime factor is otherwise silently dropped.
2. **Trial division: using `int` overflow in `i * i <= n`** for large `n` — cast to `long long` for the comparison.
3. **Divisor counting: reverting to per-number trial division** when `n` is large — always use the `O(n)` contribution-counting reformulation instead.
4. **Sieve of Eratosthenes: starting the inner marking loop at `2*i` instead of `i*i`.** Correct either way, but `i*i` avoids redundant marking work already done by smaller primes.
5. **Sieve of Eratosthenes: off-by-one on the "strictly less than n" boundary.** The array should be sized `n`, indices `0..n-1`, and primality should only be counted for `2..n-1`.
6. **SPF sieve: overwriting an existing `spf[j]`** — always guard with `if (spf[j] == 0)`, since primes are processed in increasing order and the first to arrive is correct.
7. **SPF sieve: `MAXN` too small for the query values.** The table must be sized above the largest possible query, or `factorize` will index out of bounds.
8. **SPF sieve: rebuilding the sieve inside a per-query loop.** Build it once, outside any loop over queries — that's the entire point of precomputing it.

---

## 8. Related Problems in This Module

| Problem | Technique | File |
| --- | --- | --- |
| Unique prime factors of n | Trial division to sqrt(n) | [03-Advanced Math/01](../03-Advanced%20Math/01-prime_factors.cpp) |
| Sum of divisor-sums, 1 to n | Divisor contribution counting | [03-Advanced Math/02](../03-Advanced%20Math/02-all_divisors.cpp) |
| Count primes below n | Sieve of Eratosthenes | [03-Advanced Math/03](../03-Advanced%20Math/03-sieve_of_eratosthenes.cpp) |
| Prime factorization of many queries | Smallest Prime Factor sieve | [03-Advanced Math/04](../03-Advanced%20Math/04-prime_factors_using_sieve.cpp) |
| pow(x, n) | Binary exponentiation — see [05-Advanced-Bit-Tricks.md](05-Advanced-Bit-Tricks.md) | [03-Advanced Math/05](../03-Advanced%20Math/05-pow.cpp) |

---

## Cheat Sheet

```cpp
// Trial division - unique prime factors of one n
for (int i = 2; (long long)i*i <= n; i++) {
    if (n % i == 0) {
        record(i);
        while (n % i == 0) n /= i;
    }
}
if (n > 1) record(n);

// Divisor contribution counting - sum of F(1..n)
long long total = 0;
for (long long d = 1; d <= n; d++) total += d * (n / d);

// Sieve of Eratosthenes - primes below n
vector<bool> isComposite(n, false);
for (int i = 2; (long long)i*i < n; i++)
    if (!isComposite[i])
        for (int j = i*i; j < n; j += i) isComposite[j] = true;

// SPF sieve - build once, factor many
for (int i = 2; i < MAXN; i++)
    if (spf[i] == 0) {
        for (long long j = (long long)i*i; j < MAXN; j += i)
            if (spf[j] == 0) spf[j] = i;
        spf[i] = i;
    }
```
