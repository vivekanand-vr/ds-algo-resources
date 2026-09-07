# XOR — Properties and Applications

> XOR is the single most productive operator in this whole module. One algebraic identity — `x ^ x = 0` — explains the swap trick, the single-number family, and range-XOR queries, all at once.

---

## Table of Contents

1. [The Defining Properties of XOR](#1-the-defining-properties-of-xor)
2. [XOR as "Difference" / Cancellation](#2-xor-as-difference--cancellation)
3. [The Single Number Family](#3-the-single-number-family)
4. [Minimum Bit Flips — XOR as a Distance Metric](#4-minimum-bit-flips--xor-as-a-distance-metric)
5. [Prefix XOR — Range Queries in O(1)](#5-prefix-xor--range-queries-in-o1)
6. [Worked Examples](#6-worked-examples)
7. [Common Pitfalls](#7-common-pitfalls)
8. [Related Problems in This Module](#8-related-problems-in-this-module)

---

## 1. The Defining Properties of XOR

| Property | Statement | Why it matters |
| --- | --- | --- |
| **Identity** | `x ^ 0 = x` | XOR with 0 changes nothing — a safe "start value" for accumulation |
| **Self-inverse** | `x ^ x = 0` | Any value cancels itself out |
| **Commutative** | `a ^ b = b ^ a` | Order of combination doesn't matter |
| **Associative** | `(a ^ b) ^ c = a ^ (b ^ c)` | Grouping doesn't matter — a whole array can be folded in any order |
| **Own inverse** | if `a ^ b = c`, then `a ^ c = b` and `b ^ c = a` | XOR can always be "undone" by XORing again with the same value |

Together, these mean: **XOR-ing a multiset of values cancels every value that appears an even number of times, leaving only what appears an odd number of times.** That single sentence is the mechanism behind nearly every problem in this note.

---

## 2. XOR as "Difference" / Cancellation

Think of `a ^ b` as encoding **exactly the bit positions where `a` and `b` disagree**:

```
a = 1100
b = 1010
a^b= 0110    <- bits 1 and 2 differ; bits 0 and 3 agree
```

This "encodes a difference, recoverable from either side" property is what makes the XOR swap trick work (see [02-Bit-Masking-Techniques.md](02-Bit-Masking-Techniques.md) §5): `a^b` is stored, then XORed against each original value in turn to recover the other.

---

## 3. The Single Number Family

A family of problems built directly on the cancellation property, in increasing difficulty.

### 3.1 One singleton, rest appear twice

> Every element appears exactly twice except one. Find it.

```cpp
int singleNumber(vector<int>& nums) {
    int result = 0;
    for (int x : nums) result ^= x;
    return result;
}
```

Every paired value cancels (`x ^ x = 0`); only the singleton survives, since XOR-ing anything with 0 leaves it unchanged. **O(n) time, O(1) space** — beats a hash-map frequency count on space.

→ [02-single_number.cpp](../02-Medium/02-single_number.cpp)

### 3.2 Two singletons, rest appear twice

> Every element appears twice except **two**. Find both, in ascending order.

XOR-ing everything now gives `xorAll = a ^ b` (the two singletons), not either one directly — the naive approach stalls here.

**The trick: use a bit where `a` and `b` differ to split the array into two independent groups.**

1. `xorAll = a ^ b` is non-zero (since `a != b`), so it has at least one set bit.
2. Isolate any one of them — the lowest is easiest: `diffBit = xorAll & (-xorAll)` (from [01-Bitwise-Operators-Basics.md](01-Bitwise-Operators-Basics.md) §2).
3. `a` and `b` **must** differ at `diffBit` (otherwise that bit would have cancelled out of `xorAll`). So partitioning every array element by "is `diffBit` set?" puts `a` and `b` into *different* groups.
4. Every duplicate pair, by contrast, has **identical** bits — both copies land in the *same* group, so they still cancel there.
5. XOR each group separately: one group's XOR isolates `a`, the other isolates `b`.

```cpp
vector<int> twoOddOccurring(vector<int>& nums) {
    int xorAll = 0;
    for (int x : nums) xorAll ^= x;

    int diffBit = xorAll & (-xorAll);   // any bit where a and b differ

    int a = 0, b = 0;
    for (int x : nums) {
        if (x & diffBit) a ^= x;
        else              b ^= x;
    }

    if (a > b) swap(a, b);   // problem asks for ascending order
    return {a, b};
}
```

**O(n) time, O(1) space**, two linear passes.

→ [05-two_numbers_appearing_odd_times.cpp](../02-Medium/05-two_numbers_appearing_odd_times.cpp)

### 3.3 One singleton, rest appear **three** times (not in this module, but the natural next step)

XOR alone cannot solve this — `x ^ x ^ x = x`, so triple-appearing values don't cancel to 0. This needs a bit-counting approach instead (count how many numbers have each bit set, mod 3) — a reminder that XOR's power comes specifically from *pairs* cancelling, and breaks the moment the multiplicity isn't even/odd in the right way.

---

## 4. Minimum Bit Flips — XOR as a Distance Metric

> Given `start` and `goal`, find the minimum number of single-bit flips to turn `start` into `goal`.

`start ^ goal` produces a 1 in **exactly** the positions that differ between the two numbers, and a 0 everywhere they already agree. Flipping `start` into `goal` therefore means flipping exactly those differing positions — no more, no fewer (flipping an already-agreeing bit would only create a new difference). So the answer is simply the number of set bits in `start ^ goal`, computed with Brian Kernighan's algorithm (see [03-Counting-Set-Bits.md](03-Counting-Set-Bits.md)):

```cpp
int minBitFlips(int start, int goal) {
    int diff = start ^ goal;
    return countSetBits(diff);   // Brian Kernighan's algorithm
}
```

**O(k) time**, where `k` is the number of differing bits.

→ [01-min_bits_to_flip_to_convert.cpp](../02-Medium/01-min_bits_to_flip_to_convert.cpp)

This is the same idea as **Hamming distance** — the number of positions at which two equal-length binary strings differ. XOR computes it in one operation.

---

## 5. Prefix XOR — Range Queries in O(1)

> Given `l` and `r`, find `l ^ (l+1) ^ ... ^ r`.

This mirrors prefix sums exactly (see the Arrays module's prefix-sum note), with `^` replacing `+`. The identity that makes it work is XOR's self-inverse property:

```
xor(l..r) = prefixXor(r) ^ prefixXor(l-1)
```

because XOR-ing the two prefixes cancels everything from `0` to `l-1` (each value there appears in both `prefixXor(r)` and `prefixXor(l-1)`, so it cancels), leaving only `l..r`.

### The O(1) pattern for prefixXor(n)

Rather than looping, `prefixXor(n) = 0^1^2^...^n` follows a period-4 cycle, which can be verified by tracing the running XOR for consecutive n:

| n mod 4 | prefixXor(n) |
| --- | --- |
| 0 | n |
| 1 | 1 |
| 2 | n + 1 |
| 3 | 0 |

```cpp
int prefixXor(int n) {
    if (n < 0) return 0;              // empty range
    switch (n % 4) {
        case 0: return n;
        case 1: return 1;
        case 2: return n + 1;
        default: return 0;            // n % 4 == 3
    }
}

int xorOfRange(int l, int r) {
    return prefixXor(r) ^ prefixXor(l - 1);
}
```

**O(1) time, O(1) space** — no loop at all, unlike the prefix-sum case, which still needs an O(n) precomputation for arbitrary values. This period-4 shortcut works only for the *specific* sequence `0,1,2,...,n` — for an arbitrary array's prefix XOR you would precompute an O(n) array exactly like a prefix-sum array.

→ [03-xor_of_number_range.cpp](../02-Medium/03-xor_of_number_range.cpp)

---

## 6. Worked Examples

### 6.1 Single number

`nums = {4, 1, 2, 1, 2}`

```
0 ^ 4 = 4
4 ^ 1 = 5
5 ^ 2 = 7
7 ^ 1 = 6
6 ^ 2 = 4
```

**Answer: 4.** The pairs `(1,1)` and `(2,2)` each cancelled along the way.

### 6.2 Two singletons

`nums = {1, 2, 1, 3, 5, 2}`

`xorAll = 1^2^1^3^5^2`. The two `1`s and two `2`s cancel, leaving `3 ^ 5 = 6 (110)`.

`diffBit = 6 & (-6)`. Lowest set bit of `110` is `010 = 2`.

Partition by bit 1 (`x & 2`):

| x | x & 2 | group |
| --- | --- | --- |
| 1 (001) | 0 | b |
| 2 (010) | 2 | a |
| 1 (001) | 0 | b |
| 3 (011) | 2 | a |
| 5 (101) | 0 | b |
| 2 (010) | 2 | a |

Group a: `2 ^ 3 ^ 2 = 3`. Group b: `1 ^ 1 ^ 5 = 5`.

`a=3, b=5`, already ascending. **Answer: [3, 5].**

### 6.3 Min bit flips

`start = 10 (1010)`, `goal = 7 (0111)`

`diff = 1010 ^ 0111 = 1101` — three set bits. **Answer: 3.**

### 6.4 XOR of a range

`l = 4, r = 8`

`prefixXor(8)`: `8 % 4 == 0` → `8`.
`prefixXor(3)`: `3 % 4 == 3` → `0`.

`xorOfRange(4,8) = 8 ^ 0 = 8`.

Direct check: `4^5^6^7^8 = 1^6^7^8 = 7^7^8 = 0^8 = 8`. ✓

---

## 7. Common Pitfalls

1. **Applying single-XOR cancellation to the "appears three times" variant.** It does not cancel — needs a different (bit-counting) technique.
2. **Forgetting to sort the two-singleton answer** if the problem demands a specific order (ascending, in this module's version).
3. **Picking the wrong bit to partition on** in the two-singleton problem — it must be a bit where `xorAll` is actually 1; any other bit fails to separate `a` and `b`.
4. **Assuming prefix XOR needs an O(n) array like prefix sum.** For the specific sequence `0..n`, the period-4 formula gives O(1) directly — building an array is unnecessary overhead for this particular case.
5. **Confusing `l-1` with `l`** in the range formula — always subtract one to exclude everything before `l`, exactly as in prefix sums.
6. **Negative `l`.** The `prefixXor(l-1)` call can receive `-1` when `l = 0`; the guard `if (n < 0) return 0;` handles this (an empty range XORs to 0, the identity element).

---

## 8. Related Problems in This Module

| Problem | XOR technique | File |
| --- | --- | --- |
| Swap two numbers | Difference encoding / recovery | [01-Easy/05](../01-Easy/05-swap_two_numbers.cpp) |
| Minimum bit flips to convert | XOR as Hamming distance | [02-Medium/01](../02-Medium/01-min_bits_to_flip_to_convert.cpp) |
| Single number | Pairwise cancellation | [02-Medium/02](../02-Medium/02-single_number.cpp) |
| XOR of a number range | Prefix XOR, period-4 pattern | [02-Medium/03](../02-Medium/03-xor_of_number_range.cpp) |
| Two numbers appearing odd times | Cancellation + bit partitioning | [02-Medium/05](../02-Medium/05-two_numbers_appearing_odd_times.cpp) |

See also: [01-Bitwise-Operators-Basics.md](01-Bitwise-Operators-Basics.md), [05-Advanced-Bit-Tricks.md](05-Advanced-Bit-Tricks.md) (maximising XOR uses the same "highest differing bit" idea as §3.2)

---

## Cheat Sheet

```cpp
// One singleton, rest twice
int result = 0;
for (int x : nums) result ^= x;

// Two singletons, rest twice
int xorAll = 0;
for (int x : nums) xorAll ^= x;
int diffBit = xorAll & (-xorAll);
int a = 0, b = 0;
for (int x : nums) (x & diffBit ? a : b) ^= x;

// Minimum bit flips (Hamming distance)
int flips = countSetBits(start ^ goal);

// XOR of range [l, r]
int xorOfRange(int l, int r) { return prefixXor(r) ^ prefixXor(l - 1); }
```

| Property | Use |
| --- | --- |
| `x ^ x = 0` | cancel pairs / duplicates |
| `x ^ 0 = x` | safe accumulator seed |
| commutative + associative | fold an array in any order |
| own inverse | recover one value from `a^b` and the other |
