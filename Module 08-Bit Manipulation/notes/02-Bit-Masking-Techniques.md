# Bit Masking Techniques — The Complete Reference

> A mask is just a number used to select, set, or clear specific bits of another number. This note covers the five canonical single-bit operations and the handful of whole-number tricks built on top of them.

---

## Table of Contents

1. [What a Mask Is](#1-what-a-mask-is)
2. [The Five Single-Bit Operations](#2-the-five-single-bit-operations)
3. [Checking Evenness — the LSB Trick](#3-checking-evenness--the-lsb-trick)
4. [Powers of Two — the n & (n-1) Trick](#4-powers-of-two--the-n--n-1-trick)
5. [Swapping Without a Temporary Variable](#5-swapping-without-a-temporary-variable)
6. [Worked Examples](#6-worked-examples)
7. [Common Pitfalls](#7-common-pitfalls)
8. [Related Problems in This Module](#8-related-problems-in-this-module)

---

## 1. What a Mask Is

A **mask** is a number whose bit pattern is chosen deliberately so that combining it with another number, via `&`, `|`, or `^`, affects only the bits you intend.

```cpp
int mask = 1 << k;   // ...0000100...0   (only bit k is 1)
```

Every operation below is this one idea, `1 << k`, combined with a different operator.

---

## 2. The Five Single-Bit Operations

| Operation | Expression | Why it works |
| --- | --- | --- |
| **Check** bit k | `(n >> k) & 1`  or  `n & (1 << k)` | Isolates bit k; non-zero means it was set |
| **Set** bit k to 1 | `n \| (1 << k)` | OR with a 1 forces that position to 1, leaves others untouched (`x \| 0 = x`) |
| **Clear** bit k to 0 | `n & ~(1 << k)` | AND with a 0 forces that position to 0, leaves others untouched (`x & 1 = x`) |
| **Toggle** bit k | `n ^ (1 << k)` | XOR with 1 flips that position, leaves others untouched (`x ^ 0 = x`) |
| **Update** bit k to value v | `(n & ~(1 << k)) \| (v << k)` | Clear it first, then set it if v is 1 |

### Two equivalent ways to check a bit

```cpp
bool checkKthBit_v1(int n, int k) { return (n & (1 << k)) != 0; }
bool checkKthBit_v2(int n, int k) { return (n >> k) & 1; }
```

`v1` builds a mask and tests it directly. `v2` shifts the target bit down to position 0 and reads it. Both are O(1); `v2` generalises more naturally to "read every bit one at a time" loops (see [03-Counting-Set-Bits.md](03-Counting-Set-Bits.md)).

→ [01-check_kth_bit.cpp](../01-Easy/01-check_kth_bit.cpp) uses `v1`.

### Why `~(1 << k)` clears a bit

```
1 << k        = 0 0 0 1 0 0 0 0     (only bit k is 1)
~(1 << k)     = 1 1 1 0 1 1 1 1     (every bit is 1 EXCEPT bit k)

n              = 1 1 1 1 1 1 1 1
n & ~(1 << k)  = 1 1 1 0 1 1 1 1    (bit k forced to 0, everything else preserved)
```

---

## 3. Checking Evenness — the LSB Trick

The least significant bit of any binary number is 1 for odd numbers and 0 for even numbers — that's simply what "the ones place" means in base 2.

```cpp
bool isEven(int n) { return (n & 1) == 0; }
```

`n & 1` is a special case of the "check bit k" operation with `k = 0`, and it is faster than `n % 2 == 0` because `%` typically compiles to a division instruction while `&` is a single cycle.

→ [02-is_even.cpp](../01-Easy/02-is_even.cpp)

> For **negative** numbers, `n & 1` still works correctly in two's complement (the LSB still encodes odd/even), but `n % 2` can return `-1` for odd negatives in C++ (truncating division) — one more reason to prefer the bit trick when portability matters.

---

## 4. Powers of Two — the n & (n-1) Trick

A power of two has **exactly one set bit** (`1 = 0001`, `2 = 0010`, `4 = 0100`, `8 = 1000`, ...). Subtracting 1 from such a number flips that single bit off and turns every bit below it to 1:

```
 8       = 1000
 8 - 1   = 0111
 8 & 7   = 0000     -> zero, so 8 is a power of two
```

For a number that is **not** a power of two, `n - 1` cannot flip every set bit — anything above the lowest set bit survives unchanged, so the AND is non-zero:

```
 6       = 0110
 6 - 1   = 0101
 6 & 5   = 0100     -> non-zero, so 6 is NOT a power of two
```

```cpp
bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}
```

The `n > 0` guard matters: `0 & (0 - 1)` is `0 & -1 = 0`, which would incorrectly say 0 is a power of two if the guard were omitted.

→ [03-is_power_of_2.cpp](../01-Easy/03-is_power_of_2.cpp)

### The general pattern: `n & (n-1)` always clears the lowest set bit

This is not specific to powers of two — for **any** `n`, `n & (n-1)` clears exactly the lowest set bit and leaves everything else the same. That single fact is the entire engine behind Brian Kernighan's algorithm in [03-Counting-Set-Bits.md](03-Counting-Set-Bits.md), and the power-of-two check above is just the special case where clearing the one bit a number has leaves nothing (0).

---

## 5. Swapping Without a Temporary Variable

XOR is used here purely as a **bit-masking / bit-recovery** tool: `a ^ b` encodes exactly the positions where `a` and `b` differ, and re-XORing that encoding with either original value recovers the other.

```cpp
void swapNumbers(int &a, int &b) {
    if (&a == &b) return;   // guard: aliasing zeroes both out otherwise
    a = a ^ b;   // a now holds "where they differ"
    b = a ^ b;   // b becomes the original a
    a = a ^ b;   // a becomes the original b
}
```

Why the self-swap guard is necessary: if `a` and `b` are the same memory location, step 1 computes `a ^ a = 0`, and the rest of the routine zeroes it permanently — there is no second value to recover it from.

→ [05-swap_two_numbers.cpp](../01-Easy/05-swap_two_numbers.cpp). See [04-XOR-Properties-and-Applications.md](04-XOR-Properties-and-Applications.md) for the full theory behind why this works.

---

## 6. Worked Examples

### 6.1 Set, clear, toggle on n = 10 (1010), k = 0

| Operation | Expression | Result | Binary |
| --- | --- | --- | --- |
| Check bit 0 | `10 & 1` | 0 | bit 0 of 1010 is 0 |
| Set bit 0 | `10 \| 1` | 11 | 1011 |
| Clear bit 1 | `10 & ~2` | 8 | 1000 |
| Toggle bit 3 | `10 ^ 8` | 2 | 0010 |

### 6.2 Power-of-two check trace

| n | binary | n-1 | n & (n-1) | Power of 2? |
| --- | --- | --- | --- | --- |
| 16 | 10000 | 01111 | 00000 | Yes |
| 18 | 10010 | 10001 | 10000 | No |
| 1 | 00001 | 00000 | 00000 | Yes |
| 0 | 00000 | (guard) | — | No (n > 0 fails) |

### 6.3 XOR swap trace

`a = 5 (0101)`, `b = 9 (1001)`

| step | operation | a | b |
| --- | --- | --- | --- |
| init | — | 0101 (5) | 1001 (9) |
| 1 | `a = a ^ b` | 1100 (12) | 1001 (9) |
| 2 | `b = a ^ b` | 1100 (12) | 0101 (5) |
| 3 | `a = a ^ b` | 1001 (9) | 0101 (5) |

Final: `a = 9`, `b = 5` — swapped.

---

## 7. Common Pitfalls

1. **Forgetting `n > 0` in the power-of-two check** — `0` and negative numbers slip through incorrectly.
2. **Using `n & (1 << k)` and treating the result as 0/1.** It returns the bit still in position `k` (e.g. `8`, not `1`) — fine for a boolean test (`!= 0`), wrong if you need the literal 0/1 value. Use `(n >> k) & 1` when you need the actual bit value.
3. **Forgetting the `~` when clearing a bit** — `n & (1 << k)` *checks* a bit; `n & ~(1 << k)` *clears* it. Easy to typo one for the other.
4. **XOR swap on aliased references.** Always guard, or just avoid XOR swap outside of teaching/interview contexts — `std::swap` is clearer and equally fast after optimisation.
5. **Off-by-one in bit position.** Bit 0 is the least significant (rightmost) bit, not the leftmost.
6. **Shifting by a negative or too-large amount.** `1 << k` for `k >= 32` (on a 32-bit `int`) is undefined behaviour.

---

## 8. Related Problems in This Module

| Problem | Technique | File |
| --- | --- | --- |
| Check k-th bit | Bit masking | [01-Easy/01](../01-Easy/01-check_kth_bit.cpp) |
| Check even/odd | LSB check | [01-Easy/02](../01-Easy/02-is_even.cpp) |
| Check power of two | `n & (n-1)` | [01-Easy/03](../01-Easy/03-is_power_of_2.cpp) |
| Swap two numbers | XOR masking | [01-Easy/05](../01-Easy/05-swap_two_numbers.cpp) |

See also: [01-Bitwise-Operators-Basics.md](01-Bitwise-Operators-Basics.md), [03-Counting-Set-Bits.md](03-Counting-Set-Bits.md)

---

## Cheat Sheet

```cpp
bool check(int n, int k)  { return (n >> k) & 1; }
int  set(int n, int k)    { return n | (1 << k); }
int  clear(int n, int k)  { return n & ~(1 << k); }
int  toggle(int n, int k) { return n ^ (1 << k); }
int  update(int n, int k, int v) { return (n & ~(1 << k)) | (v << k); }

bool isEven(int n)       { return (n & 1) == 0; }
bool isPowerOfTwo(int n) { return n > 0 && (n & (n - 1)) == 0; }
```

| Idiom | Effect |
| --- | --- |
| `n & (n - 1)` | clears the lowest set bit |
| `n & (-n)` | isolates the lowest set bit |
| `n \| (n + 1)` | sets the lowest unset (0) bit |
