# Advanced Bit Tricks — The Complete Reference

> Four techniques that go beyond single-bit operations: maximising XOR over a range, enumerating subsets with a bitmask, dividing without `/`, and exponentiating without repeated multiplication.

---

## Table of Contents

1. [Maximising XOR Over a Range](#1-maximising-xor-over-a-range)
2. [Bitmask Enumeration — Generating All Subsets](#2-bitmask-enumeration--generating-all-subsets)
3. [Division via Bit Shifting](#3-division-via-bit-shifting)
4. [Binary (Fast) Exponentiation](#4-binary-fast-exponentiation)
5. [Worked Examples](#5-worked-examples)
6. [Common Pitfalls](#6-common-pitfalls)
7. [Related Problems in This Module](#7-related-problems-in-this-module)

---

## 1. Maximising XOR Over a Range

> Given `l` and `r`, find `max(a ^ b)` over all pairs with `l <= a <= b <= r`.

### The insight

Every bit **above** the highest position where `l` and `r` differ is *forced* to be identical in every number within `[l, r]` — if it weren't, the number would fall outside the range. Only bits from that first differing position downward are free to vary between different numbers in `[l, r]`.

XOR is maximised by making as many high bits differ as possible. So:

1. `diff = l ^ r` marks (with 1s) every bit position where `l` and `r` disagree.
2. The **highest** set bit of `diff` is the highest position where the range allows disagreement.
3. Below and including that position, we can always find *some* `a, b` pair inside `[l, r]` that disagrees on **every** one of those bits simultaneously (a consequence of the range being a contiguous span of integers) — so the maximum achievable XOR sets that entire span of bits to 1.

```cpp
int maximizingXor(int l, int r) {
    int diff = l ^ r;
    if (diff == 0) return 0;         // l == r, only possible xor is 0

    int mask = 1;
    while ((mask << 1) <= diff) mask <<= 1;   // walk mask up to the highest set bit of diff

    return (mask << 1) - 1;          // all 1s from that bit downward
}
```

The answer is always of the form `2^(k+1) - 1` — a run of 1s — never a "sparse" pattern. **O(log(diff)) time, O(1) space.**

→ [06-maximising_xor.cpp](../01-Easy/06-maximising_xor.cpp)

> This is structurally the same "find the highest differing bit" move used in [04-XOR-Properties-and-Applications.md](04-XOR-Properties-and-Applications.md) §3.2 to separate two singleton values — both rely on XOR marking *where two things disagree*.

---

## 2. Bitmask Enumeration — Generating All Subsets

> Given `n` distinct elements, generate every subset (the power set).

### The insight

A set of `n` elements has exactly `2^n` subsets. Every subset corresponds to exactly one `n`-bit pattern: bit `i` is 1 if element `i` is included, 0 if it's excluded. So **counting from `0` to `2^n - 1` and reading off bits is the same as enumerating every subset** — no recursion or explicit "include/exclude" branching needed.

```
n = 3, elements = {1, 2, 3}

mask = 000 -> {}
mask = 001 -> {1}
mask = 010 -> {2}
mask = 011 -> {1,2}
mask = 100 -> {3}
mask = 101 -> {1,3}
mask = 110 -> {2,3}
mask = 111 -> {1,2,3}
```

```cpp
vector<vector<int>> subsets(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> result;

    for (int mask = 0; mask < (1 << n); mask++) {
        vector<int> subset;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) subset.push_back(nums[i]);
        }
        result.push_back(subset);
    }
    return result;
}
```

**Time O(n · 2^n)** — `2^n` masks, O(n) work to read each one. **Space O(n · 2^n)** for the output itself.

→ [04-subsets.cpp](../02-Medium/04-subsets.cpp)

### When to prefer this over recursive backtracking

Both approaches are the same asymptotic complexity. Bitmask enumeration is preferable when:
- `n` is small (typically `<= 20`, since `2^20 ≈ 10^6`)
- you need to iterate subsets in a specific numeric order, or index into them directly
- you want an iterative, stack-free implementation

Recursive backtracking is preferable when the problem needs early pruning (e.g. subsets summing to a target, where a partial sum already exceeding the target can be abandoned early) — bitmask enumeration always generates all `2^n` masks with no way to skip ahead.

---

## 3. Division via Bit Shifting

> Divide two integers without using `*`, `/`, or `%`.

### The insight

Repeated subtraction of the divisor works, but takes `O(dividend / divisor)` steps — e.g. dividing `2^31` by `1` would take two billion steps. Instead, subtract the **largest possible multiple of the divisor** at each step, found by doubling:

Left-shifting the divisor by `k` is the same as multiplying it by `2^k`. So instead of subtracting `divisor` once at a time, find the biggest `k` such that `divisor << k` still fits inside what remains of the dividend, subtract that whole chunk out in one step, and add `2^k` to the running quotient. This is **exponential search**: each outer step removes at least half of what's left, so it terminates in O(log) steps rather than O(n).

```cpp
int divide(int dividend, int divisor) {
    if (dividend == INT_MIN && divisor == -1) return INT_MAX;   // 32-bit overflow guard

    bool negative = (dividend < 0) ^ (divisor < 0);

    long long wideDividend = dividend, wideDivisor = divisor;
    long long a = wideDividend < 0 ? -wideDividend : wideDividend;   // abs, by hand
    long long b = wideDivisor  < 0 ? -wideDivisor  : wideDivisor;
    long long quotient = 0;

    while (a >= b) {
        long long temp = b, multiple = 1;
        while (a >= (temp << 1)) {      // keep doubling while it still fits
            temp <<= 1;
            multiple <<= 1;
        }
        a -= temp;
        quotient += multiple;
    }

    return negative ? (int)(-quotient) : (int)quotient;
}
```

**Time O(log² n)** — the outer loop runs O(log(quotient)) times (each iteration removes at least half the remaining dividend), and the inner doubling loop is also bounded by O(log(quotient)).

Two edge cases specific to fixed-width integers:

- **`INT_MIN / -1`** would produce `2147483648`, one more than `INT_MAX` — outside the representable range for a 32-bit signed result, so it's special-cased.
- **`-INT_MIN` overflows.** Widening to `long long` *before* taking the absolute value avoids ever computing `-INT_MIN` in 32-bit arithmetic.

→ [07-divide_two_integers.cpp](../01-Easy/07-divide_two_integers.cpp)

---

## 4. Binary (Fast) Exponentiation

> Compute `x^n` without `n` sequential multiplications.

### The insight

Write the exponent `n` in binary. Because `x^(a+b) = x^a * x^b`, and every integer is a sum of distinct powers of two (its binary representation), `x^n` can be assembled from the squared powers `x, x^2, x^4, x^8, ...` — multiplying in exactly the ones that correspond to the 1 bits of `n`.

```
n = 13 = 1101(binary) = 8 + 4 + 1
x^13 = x^8 * x^4 * x^1
```

Processing bits from the lowest upward: at each step, check the lowest bit of `n`; if set, fold the *current* squared value of `x` into the answer. Then square `x` (moving to the next power of two) and shift `n` right by one bit — this is precisely long multiplication, performed in binary, on the exponent.

```cpp
double myPow(double x, int n) {
    long long N = n;
    if (N < 0) { x = 1 / x; N = -N; }   // x^-n = (1/x)^n

    double result = 1.0;
    while (N > 0) {
        if (N & 1) result *= x;   // this bit is set - include the current power of x
        x *= x;                  // x, x^2, x^4, x^8, ...
        N >>= 1;
    }
    return result;
}
```

**Time O(log n)** — one loop iteration per bit of `n`, instead of O(n) for naive repeated multiplication.

Same `INT_MIN` concern as division: negating `n` before widening it would overflow, so `n` is widened to `long long` **first**, then negated.

→ [05-pow.cpp](../03-Advanced%20Math/05-pow.cpp)

This same "process the exponent bit by bit, squaring the base each step" idea generalises directly to **modular exponentiation** (`x^n mod m`, essential in cryptography and competitive programming) — just take `% m` after every multiplication.

---

## 5. Worked Examples

### 5.1 Maximising XOR

`l = 8, r = 20`

`diff = 8 ^ 20`. `8 = 01000`, `20 = 10100`. `diff = 11100 (28)`.

`mask` doubles: `1 → 2 → 4 → 8 → 16` (stops since `16 << 1 = 32 > 28`).

`(16 << 1) - 1 = 31`.

Check: `15 (01111) ^ 16 (10000) = 11111 = 31`, and both `15, 16 ∈ [8, 20]`. ✓

### 5.2 Subsets

`nums = {1, 2, 3}`, `n = 3`, masks `0` to `7`:

| mask | binary | subset |
| --- | --- | --- |
| 0 | 000 | {} |
| 1 | 001 | {1} |
| 2 | 010 | {2} |
| 3 | 011 | {1,2} |
| 4 | 100 | {3} |
| 5 | 101 | {1,3} |
| 6 | 110 | {2,3} |
| 7 | 111 | {1,2,3} |

### 5.3 Division: 43 / 5

| a (remaining) | doubling temp/multiple | subtract | quotient |
| --- | --- | --- | --- |
| 43 | 5→10→20→40 (multiple 1→2→4→8), stop (80>43) | 43-40=3 | 8 |
| 3 < 5 | loop ends | — | 8 |

**Answer: 8** (`43 / 5 = 8.6`, truncated).

### 5.4 Fast exponentiation: 2^10

| N (binary) | N&1 | result | x after squaring |
| --- | --- | --- | --- |
| 1010 | 0 | 1 | 4 |
| 0101 | 1 | 4 | 16 |
| 0010 | 0 | 4 | 256 |
| 0001 | 1 | 1024 | 65536 |
| 0000 | loop ends | **1024** | — |

**Answer: 1024** = 2^10. ✓

---

## 6. Common Pitfalls

1. **Maximising XOR: forgetting the `diff == 0` case** when `l == r` — the loop would otherwise never execute correctly (or the result would be wrong for a zero mask).
2. **Bitmask subsets: `n` too large.** `1 << n` overflows a 32-bit `int` once `n >= 31`; this technique is only practical for `n` up to about 20 anyway (2^20 ≈ 10^6 subsets).
3. **Division: forgetting the `INT_MIN / -1` overflow guard**, or negating `INT_MIN` directly instead of widening first.
4. **Division: using `int` instead of `long long` for the running remainder/quotient** — intermediate doubled values can exceed `INT_MAX` even when the final quotient fits.
5. **Fast exponentiation: not handling negative `n`.** `x^n` for negative `n` means `1/x^(-n)` — forgetting to invert `x` first gives completely wrong results.
6. **Fast exponentiation: negating `n` before widening to a larger type**, overflowing on `INT_MIN` exactly like the division case.
7. **Fast exponentiation: squaring `x` even after the last relevant bit**, wasting a multiplication — harmless for correctness, just a minor inefficiency if not guarded by `while (N > 0)`.

---

## 7. Related Problems in This Module

| Problem | Technique | File |
| --- | --- | --- |
| Maximising XOR | Highest differing bit | [01-Easy/06](../01-Easy/06-maximising_xor.cpp) |
| Divide two integers | Bit-shift exponential search | [01-Easy/07](../01-Easy/07-divide_two_integers.cpp) |
| Generate all subsets | Bitmask enumeration | [02-Medium/04](../02-Medium/04-subsets.cpp) |
| pow(x, n) | Binary (fast) exponentiation | [03-Advanced Math/05](../03-Advanced%20Math/05-pow.cpp) |

See also: [01-Bitwise-Operators-Basics.md](01-Bitwise-Operators-Basics.md), [04-XOR-Properties-and-Applications.md](04-XOR-Properties-and-Applications.md)

---

## Cheat Sheet

```cpp
// Max XOR over [l, r]
int diff = l ^ r;
int mask = 1;
while ((mask << 1) <= diff) mask <<= 1;
int answer = (mask << 1) - 1;

// All subsets via bitmask
for (int mask = 0; mask < (1 << n); mask++)
    for (int i = 0; i < n; i++)
        if (mask & (1 << i)) /* include nums[i] */;

// Division via bit-shift doubling
while (a >= b) {
    long long temp = b, multiple = 1;
    while (a >= (temp << 1)) { temp <<= 1; multiple <<= 1; }
    a -= temp; quotient += multiple;
}

// Fast exponentiation
while (N > 0) {
    if (N & 1) result *= x;
    x *= x;
    N >>= 1;
}
```

| Technique | Time | Space |
| --- | --- | --- |
| Max XOR over a range | O(log(diff)) | O(1) |
| Bitmask subset enumeration | O(n · 2^n) | O(n · 2^n) output |
| Division via bit shifting | O(log² n) | O(1) |
| Binary exponentiation | O(log n) | O(1) |
