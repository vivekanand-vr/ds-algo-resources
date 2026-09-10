# Counting Set Bits — The Complete Reference

> How many 1s are in a number's binary form — for a single number (Brian Kernighan's algorithm), and cumulatively across an entire range (the recursive bit-position formula).

---

## Table of Contents

1. [The Problem, Two Ways](#1-the-problem-two-ways)
2. [Naive Approach — Check Every Bit](#2-naive-approach--check-every-bit)
3. [Brian Kernighan's Algorithm](#3-brian-kernighans-algorithm)
4. [Compiler Builtins](#4-compiler-builtins)
5. [Counting Set Bits Across a Range — 1 to n](#5-counting-set-bits-across-a-range--1-to-n)
6. [Worked Examples](#6-worked-examples)
7. [Common Pitfalls](#7-common-pitfalls)
8. [Related Problems in This Module](#8-related-problems-in-this-module)

---

## 1. The Problem, Two Ways

This module contains two related but distinct problems:

| Problem | Question | File |
| --- | --- | --- |
| **Popcount of one number** | How many bits of `n` are set? | [04-count_set_bits.cpp](../01-Easy/04-count_set_bits.cpp) |
| **Cumulative popcount over a range** | What is Σ popcount(i) for i = 1 to n? | [08-count_set_bits_in_1_to_n.cpp](../01-Easy/08-count_set_bits_in_1_to_n.cpp) |

They look similar but need entirely different algorithms — conflating them is the most common mistake with this topic.

---

## 2. Naive Approach — Check Every Bit

Test all 32 (or 64) bit positions, regardless of how many are actually 1:

```cpp
int countSetBits_naive(int n) {
    int count = 0;
    for (int i = 0; i < 32; i++) {
        if ((n >> i) & 1) count++;
    }
    return count;
}
```

**O(32)** always — correct, but wasteful when `n` has few set bits (e.g. `n = 1` still checks all 32 positions).

---

## 3. Brian Kernighan's Algorithm

**Key identity:** `n & (n - 1)` always clears exactly the **lowest set bit** of `n`, leaving every other bit unchanged (derived in [02-Bit-Masking-Techniques.md](02-Bit-Masking-Techniques.md) §4).

```
 n     = 1 0 1 1 0 0     (44)
 n-1   = 1 0 1 0 1 1     (43)
 n&n-1 = 1 0 1 0 0 0     (40 - the lowest set bit, at position 2, is gone)
```

So if we repeat "clear the lowest set bit" until `n` reaches 0, the number of repetitions is exactly the number of set bits `n` started with — **one iteration per set bit**, not per bit position.

```cpp
int countSetBits(int n) {
    int count = 0;
    while (n != 0) {
        n = n & (n - 1);   // clear the lowest set bit
        count++;
    }
    return count;
}
```

**Time O(k)**, where `k` is the number of set bits (worst case O(log n) for a number like `2^31 - 1` with every bit set). This beats the naive O(32) scan whenever `n` is sparse, and is never worse.

→ [04-count_set_bits.cpp](../01-Easy/04-count_set_bits.cpp), also reused inside [01-min_bits_to_flip_to_convert.cpp](../02-Medium/01-min_bits_to_flip_to_convert.cpp) to count differing bits after an XOR.

---

## 4. Compiler Builtins

GCC/Clang expose the CPU's native popcount instruction directly:

```cpp
__builtin_popcount(unsigned int x)          // int width
__builtin_popcountll(unsigned long long x)  // 64-bit width
```

These compile to a single hardware instruction on modern CPUs (`POPCNT`) — faster than Kernighan's loop in practice, but non-standard (GCC/Clang only, not portable to MSVC without an intrinsic swap) and less illustrative of *how* the count is derived. In a learning-focused module like this one, prefer writing Kernighan's algorithm by hand; in competitive programming where every millisecond counts, prefer the builtin.

| Method | Time | Portable? | Educational value |
| --- | --- | --- | --- |
| Naive 32-bit scan | O(32) | Yes | Low |
| Brian Kernighan | O(k), k = set bits | Yes | High |
| `__builtin_popcount` | O(1) (hardware) | GCC/Clang only | Low (it's a black box) |

---

## 5. Counting Set Bits Across a Range — 1 to n

> Find Σ popcount(i) for i = 1 to n.

### 5.1 Why the naive approach is too slow

```cpp
long long total = 0;
for (int i = 1; i <= n; i++) total += __builtin_popcount(i);   // O(n log n)
```

Fine for small `n`, hopeless for `n ~ 10^9`.

### 5.2 The recursive bit-position formula

**Idea:** find `x`, the highest power of two `<= n`. Split the range `[0, n]` into three pieces whose set-bit totals can each be computed without touching every number individually.

```
[0, n]  =  [0, 2^x - 1]   |   [2^x, n]
            \___piece 1___/    \_piece 2 (bit x always 1) + piece 3 (lower bits)___/
```

**Piece 1 — numbers `0` to `2^x - 1`:** among these `2^x` numbers, each of the `x` lower bit positions is 1 in exactly half of them (a basic counting symmetry of binary numbers). So piece 1 contributes:

```
x * (2^x / 2)   =   x * 2^(x-1)
```

**Piece 2 — the x-th bit itself, for numbers `2^x` to `n`:** every one of these `(n - 2^x + 1)` numbers has bit `x` set (that's what makes them `>= 2^x`), so bit `x` alone contributes:

```
n - 2^x + 1
```

**Piece 3 — the remaining lower bits, for numbers `2^x` to `n`:** strip off bit `x` from each of these numbers, and what's left is identical in shape to the numbers `0` to `(n - 2^x)` — so this piece is just the original problem again, on a smaller input. **Recurse.**

```
totalSetBits(n) = x * 2^(x-1)  +  (n - 2^x + 1)  +  totalSetBits(n - 2^x)
```

```cpp
int findLargestPowerOf2(int n) {
    int x = 0;
    while ((n >> (x + 1)) > 0) x++;
    return x;
}

long long countSetBitsIn1ToN(int n) {
    if (n <= 0) return 0;

    int x = findLargestPowerOf2(n);
    long long bitsTill2x   = (x > 0) ? ((long long)x * (1LL << (x - 1))) : 0LL;
    long long msbFrom2xToN = (long long)n - (1LL << x) + 1;
    long long rest         = countSetBitsIn1ToN(n - (1 << x));

    return bitsTill2x + msbFrom2xToN + rest;
}
```

**Time O(log n)** — each recursive call strips off the highest bit of `n`, so there are at most `log2(n)` calls, each doing O(1) work (or O(log n) if `findLargestPowerOf2` uses a linear scan instead of a bit-scan builtin, making the whole thing O(log² n) — still vastly better than O(n log n)).

→ [08-count_set_bits_in_1_to_n.cpp](../01-Easy/08-count_set_bits_in_1_to_n.cpp)

---

## 6. Worked Examples

### 6.1 Brian Kernighan's algorithm on n = 11 (1011)

| step | n before | n - 1 | n & (n-1) | count |
| --- | --- | --- | --- | --- |
| 1 | 1011 (11) | 1010 (10) | 1010 (10) | 1 |
| 2 | 1010 (10) | 1001 (9) | 1000 (8) | 2 |
| 3 | 1000 (8) | 0111 (7) | 0000 (0) | 3 |

Loop ends (`n == 0`). **Answer: 3** set bits — matches `1011` having three 1s.

### 6.2 Range formula on n = 4

`n = 4 (100)`. Highest power of 2 `<= 4` is `x = 2` (`2^2 = 4`).

- `bitsTill2x = 2 * 2^1 = 4` — set bits across `0, 1, 2, 3` (`000,001,010,011`): `0+1+1+2 = 4` ✓
- `msbFrom2xToN = 4 - 4 + 1 = 1` — bit 2 is set once, for the number `4` itself
- `rest = countSetBitsIn1ToN(4 - 4) = countSetBitsIn1ToN(0) = 0`

**Total: 4 + 1 + 0 = 5.** Matches the brute-force sum: `1(1) + 1(2's 1 bit—wait, verify directly below)`.

Direct check: `1=1, 2=10, 3=11, 4=100` → set bits `1 + 1 + 2 + 1 = 5`. ✓

### 6.3 Range formula on n = 17

`x = 4` (`2^4 = 16 <= 17`).

- `bitsTill2x = 4 * 2^3 = 32` (set bits across `0..15`)
- `msbFrom2xToN = 17 - 16 + 1 = 2` (numbers `16, 17` both have bit 4 set)
- `rest = countSetBitsIn1ToN(17 - 16) = countSetBitsIn1ToN(1) = 1`

**Total: 32 + 2 + 1 = 35.** Matches a direct brute-force sum over 1..17 (35).

---

## 7. Common Pitfalls

1. **Conflating "popcount of n" with "sum of popcounts from 1 to n".** They are different problems with different algorithms — see §1.
2. **Using the O(n log n) brute force for the range version** when `n` can be large (10^8+). Use the recursive formula.
3. **Off-by-one in `findLargestPowerOf2`.** It must find `x` such that `2^x <= n < 2^(x+1)`, not `2^x <= n+1` or similar.
4. **Forgetting the base case `n <= 0` returns 0** in the recursive formula — infinite recursion or incorrect results otherwise.
5. **Assuming `__builtin_popcount` is portable.** It is GCC/Clang-specific; MSVC uses `__popcnt` instead.
6. **Applying Kernighan's algorithm to negative numbers naively.** In two's complement, a negative `int` has its sign bit set, and the loop still terminates correctly (since `n` eventually becomes 0 after enough iterations in a fixed-width representation) — but doing this in a language/type without a fixed width (e.g. Python's arbitrary-precision integers) causes an infinite loop, since negative numbers are conceptually infinite in binary there.

---

## 8. Related Problems in This Module

| Problem | Technique | File |
| --- | --- | --- |
| Count set bits in n | Brian Kernighan's algorithm | [01-Easy/04](../01-Easy/04-count_set_bits.cpp) |
| Count total set bits from 1 to n | Recursive bit-position formula | [01-Easy/08](../01-Easy/08-count_set_bits_in_1_to_n.cpp) |
| Minimum bit flips to convert | XOR + Kernighan's algorithm | [02-Medium/01](../02-Medium/01-min_bits_to_flip_to_convert.cpp) |

See also: [01-Bitwise-Operators-Basics.md](01-Bitwise-Operators-Basics.md), [02-Bit-Masking-Techniques.md](02-Bit-Masking-Techniques.md), [04-XOR-Properties-and-Applications.md](04-XOR-Properties-and-Applications.md)

---

## Cheat Sheet

```cpp
// Popcount of a single number - Brian Kernighan's algorithm
int countSetBits(int n) {
    int count = 0;
    while (n != 0) { n &= (n - 1); count++; }
    return count;
}

// Cumulative popcount from 1 to n - recursive bit-position formula
int countSetBitsIn1ToN(int n) {
    if (n <= 0) return 0;
    int x = /* highest power of 2 <= n */;
    return x * (1 << (x - 1)) + (n - (1 << x) + 1) + countSetBitsIn1ToN(n - (1 << x));
}
```

| Task | Approach | Time |
| --- | --- | --- |
| Popcount of one number, dense bits | 32-bit scan | O(32) |
| Popcount of one number, sparse bits | Brian Kernighan | O(k) |
| Popcount of one number, hardware | `__builtin_popcount` | O(1) |
| Cumulative popcount, 1 to n | Naive loop | O(n log n) |
| Cumulative popcount, 1 to n | Recursive formula | O(log n) |
