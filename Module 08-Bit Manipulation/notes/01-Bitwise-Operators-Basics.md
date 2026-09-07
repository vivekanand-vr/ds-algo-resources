# Bitwise Operators — The Complete Reference

> Every trick in this module is built from six operators. Get comfortable with binary representation, two's complement, and operator precedence here, and every later note becomes mechanical.

---

## Table of Contents

1. [Binary Representation](#1-binary-representation)
2. [Two's Complement — How Negative Numbers Work](#2-twos-complement--how-negative-numbers-work)
3. [The Six Bitwise Operators](#3-the-six-bitwise-operators)
4. [Shift Operators in Detail](#4-shift-operators-in-detail)
5. [Operator Precedence — The #1 Source of Bugs](#5-operator-precedence--the-1-source-of-bugs)
6. [Bitwise vs Logical Operators](#6-bitwise-vs-logical-operators)
7. [Worked Examples](#7-worked-examples)
8. [Common Pitfalls](#8-common-pitfalls)
9. [Related Problems in This Module](#9-related-problems-in-this-module)

---

## 1. Binary Representation

Every integer is stored as a fixed-width sequence of bits — 32 for `int`, 64 for `long long`. Bit positions are numbered from **0 at the least significant bit (LSB, rightmost)** upward to the **most significant bit (MSB, leftmost)**.

```
Decimal 13, as a 8-bit int:

  bit position:  7  6  5  4  3  2  1  0
  bit value:     0  0  0  0  1  1  0  1
                             ^  ^     ^
                            8+ 4+    1  = 13
```

Each bit `i` represents `2^i`. A number is the sum of `2^i` for every position `i` where the bit is 1.

| Decimal | Binary (8-bit) |
| --- | --- |
| 0 | 00000000 |
| 1 | 00000001 |
| 5 | 00000101 |
| 8 | 00001000 |
| 13 | 00001101 |
| 255 | 11111111 |

`std::bitset<N>(x)` is the easiest way to print this in C++: `cout << bitset<8>(13);` prints `00001101`.

---

## 2. Two's Complement — How Negative Numbers Work

C++ signed integers use **two's complement**. To negate a number: flip every bit, then add 1.

```
 5  = 00000101
~5  = 11111010     (flip every bit)
-5  = 11111011     (~5 + 1)
```

Equivalently, `-x == ~x + 1`, so `~x == -x - 1`.

### Why this matters for bit tricks

Two's complement is *why* `n & (-n)` isolates the lowest set bit (used throughout this module):

```
 n  = 0 0 1 0 1 1 0 0     (44)
-n  = 1 1 0 1 0 1 0 0     (~n + 1: flips everything left of the lowest set bit,
                            keeps everything from the lowest set bit rightward the same)
n&-n= 0 0 0 0 0 1 0 0     (only the lowest set bit survives)
```

The rule: negation flips all bits **above** the lowest set bit, but leaves the lowest set bit and every 0 below it **unchanged** (a run of trailing zeros stays zero, and the lowest 1 stays 1 because `+1` only carries up to the first 0 it meets, which is exactly the lowest set bit's position). ANDing the two together cancels every differing bit, leaving only that one common 1.

This identity powers [05-Advanced-Bit-Tricks.md](05-Advanced-Bit-Tricks.md) and the two-singleton problem in [04-XOR-Properties-and-Applications.md](04-XOR-Properties-and-Applications.md).

### The `INT_MIN` asymmetry

Two's complement has one more positive-range value than negative — `int` ranges from `-2147483648` to `2147483647`. So `-INT_MIN` **overflows**: there is no positive `int` representation of `2147483648`. This single fact is behind two real bugs in this module:

- [07-divide_two_integers.cpp](../01-Easy/07-divide_two_integers.cpp) special-cases `dividend == INT_MIN && divisor == -1`.
- [05-pow.cpp](../03-Advanced%20Math/05-pow.cpp) widens the exponent to `long long` *before* negating it, since `n` could be `INT_MIN`.

---

## 3. The Six Bitwise Operators

| Operator | Name | Rule | Example (4-bit) |
| --- | --- | --- | --- |
| `&` | AND | 1 only if both bits are 1 | `1100 & 1010 = 1000` |
| `\|` | OR | 1 if either bit is 1 | `1100 \| 1010 = 1110` |
| `^` | XOR | 1 if bits differ | `1100 ^ 1010 = 0110` |
| `~` | NOT | flips every bit | `~1100 = 0011` (within the given width) |
| `<<` | Left shift | shifts bits left, fills with 0 | `0011 << 1 = 0110` |
| `>>` | Right shift | shifts bits right | `1100 >> 1 = 0110` |

### The role each operator plays in this module

| Operator | What it's used for |
| --- | --- |
| `&` | Testing/extracting bits — masking, isolating the lowest set bit, checking evenness |
| `\|` | Setting a bit to 1 without disturbing others |
| `^` | Toggling bits, detecting *difference*, the cancellation trick (`x^x=0`) |
| `~` | Building masks, forming two's complement negation |
| `<<` | Building masks (`1 << k`), multiplying by powers of two, exponential search |
| `>>` | Reading bits one at a time, dividing by powers of two, iterating a mask |

---

## 4. Shift Operators in Detail

### Left shift `<<`

`x << k` moves every bit of `x` left by `k` positions, filling the vacated low bits with 0. This is equivalent to `x * 2^k` (as long as no bit is shifted out of range).

```
5 << 2:
   0101  (5)
 → 10100 (20)      -- same as 5 * 4
```

### Right shift `>>`

For **unsigned** types, `x >> k` fills the vacated high bits with 0 (logical shift) — equivalent to `x / 2^k` (floor division).

For **signed** types, C++ performs an **arithmetic shift**: the vacated high bits are filled with copies of the **sign bit**, so a negative number stays negative and shifting behaves like floor division by `2^k`.

```
 -8 >> 1  =  -4      (arithmetic shift: sign bit 1 propagates in)
  8 >> 1  =   4
```

> This is implementation-defined by the pre-C++20 standard, but is universal in practice (and guaranteed as of C++20). Never rely on right-shifting a negative number in code meant to be perfectly portable across esoteric platforms — but for competitive programming and interviews, arithmetic shift is safe to assume.

### Building a mask with shifts

```cpp
int mask = 1 << k;        // a single 1 at position k, 0 elsewhere
int lowKBits = (1 << k) - 1;   // k consecutive 1s: 000...0111...1 (k ones)
```

The second idiom is used constantly: `(1 << (highBit+1)) - 1` in [06-maximising_xor.cpp](../01-Easy/06-maximising_xor.cpp) builds "all 1s up to a given bit".

---

## 5. Operator Precedence — The #1 Source of Bugs

Bitwise operators bind **more loosely** than almost everyone expects — in particular, looser than comparison operators (`==`, `<`, `>`).

```
Precedence (high to low, relevant subset):
  ~  (unary NOT)
  <<  >>
  <  <=  >  >=
  ==  !=
  &
  ^
  |
```

```cpp
if (n & 1 == 0)     // BUG: parses as n & (1 == 0), i.e. n & 0, always false!
if ((n & 1) == 0)   // correct
```

This repo's [03-is_power_of_2.cpp](../01-Easy/03-is_power_of_2.cpp) writes `n & n-1`, which happens to be **safe** only because `-` (subtraction) binds *tighter* than `&`, so it parses as `n & (n-1)` — the intended meaning. But this is exactly the kind of expression that looks suspicious and should always be parenthesized explicitly:

```cpp
return n > 0 && (n & (n - 1)) == 0;   // parenthesize even when technically unnecessary
```

**Rule of thumb: always wrap a bitwise expression in parentheses when combining it with `==`, `<`, `&&`, or `||`.** It costs nothing and removes an entire category of bugs.

---

## 6. Bitwise vs Logical Operators

| Bitwise | Logical | Difference |
| --- | --- | --- |
| `&` | `&&` | `&` operates on every bit; `&&` short-circuits and returns a single bool |
| `\|` | `\|\|` | same distinction |
| `~` | `!` | `~` flips every bit (`~0 == -1`); `!` maps to bool (`!0 == true`) |

```cpp
5 & 3   // 1   (0101 & 0011 = 0001)
5 && 3  // true (both non-zero)

~0      // -1  (all bits flipped)
!0      // true
```

Using `&`/`|` where `&&`/`||` was intended silently changes both the result *and* removes short-circuiting (both sides always evaluate) — a classic source of subtle bugs, especially when one side has a side effect or would be unsafe to evaluate unconditionally (e.g. `i < n & arr[i] == 0` still evaluates `arr[i]` even when `i >= n`).

---

## 7. Worked Examples

### 7.1 Manual trace of AND, OR, XOR

`a = 12 (1100)`, `b = 10 (1010)`

| bit | a | b | a&b | a\|b | a^b |
| --- | --- | --- | --- | --- | --- |
| 3 | 1 | 1 | 1 | 1 | 0 |
| 2 | 1 | 0 | 0 | 1 | 1 |
| 1 | 0 | 1 | 0 | 1 | 1 |
| 0 | 0 | 0 | 0 | 0 | 0 |

`a & b = 1000 (8)`, `a | b = 1110 (14)`, `a ^ b = 0110 (6)`.

### 7.2 Two's complement negation of 6

```
 6 = 00000110
~6 = 11111001    (flip every bit)
-6 = 11111010    (~6 + 1)

Check: 6 + (-6) should be 0
  00000110
+ 11111010
-----------
 100000000   -> the 9th bit is discarded (fixed width), leaving 00000000 ✓
```

### 7.3 Isolating the lowest set bit

`n = 44 = 00101100`

```
 n  = 00101100
-n  = 11010100
n & -n = 00000100   =  4   (the lowest set bit of 44 is indeed bit 2, value 4)
```

Used directly in [05-two_numbers_appearing_odd_times.cpp](../02-Medium/05-two_numbers_appearing_odd_times.cpp).

---

## 8. Common Pitfalls

1. **Comparing with `&` before parenthesizing.** `n & 1 == 0` never does what you expect. Always write `(n & 1) == 0`.
2. **Using `&`/`|` instead of `&&`/`||`** in conditionals — loses short-circuiting and can crash on out-of-bounds access.
3. **Forgetting arithmetic vs logical shift.** `-1 >> 1` is `-1` (sign-extended), not a large positive number as an unsigned shift would give.
4. **Negating `INT_MIN`.** `-INT_MIN` overflows a 32-bit `int`; widen to `long long` first.
5. **Shifting by >= the type's bit width** (e.g. `1 << 32` on a 32-bit `int`) is undefined behaviour in C++. Use `1LL << 32` or a wider type when the shift amount can reach that far.
6. **Assuming `~x` is the same as `-x`.** `~x == -x - 1`, not `-x`.
7. **Sign confusion in shifts.** Left-shifting a negative signed integer is undefined behaviour in C++ before C++20. Keep left-shift operands non-negative.

---

## 9. Related Problems in This Module

Every problem in this module ultimately rests on these operators. See the topic notes below for problem-specific technique breakdowns:

| Note | Focus |
| --- | --- |
| [02-Bit-Masking-Techniques.md](02-Bit-Masking-Techniques.md) | check/set/clear/toggle a bit, power-of-two test |
| [03-Counting-Set-Bits.md](03-Counting-Set-Bits.md) | Brian Kernighan's algorithm, popcount over a range |
| [04-XOR-Properties-and-Applications.md](04-XOR-Properties-and-Applications.md) | XOR swap, single-number family, prefix XOR |
| [05-Advanced-Bit-Tricks.md](05-Advanced-Bit-Tricks.md) | max XOR, subset enumeration, bit-shift division, fast exponentiation |
| [06-Sieve-and-Number-Theory.md](06-Sieve-and-Number-Theory.md) | sieves and divisor/factorization techniques from Advanced Math |

---

## Cheat Sheet

```cpp
a & b     // AND  - both bits 1
a | b     // OR   - either bit 1
a ^ b     // XOR  - bits differ
~a        // NOT  - flip every bit  (== -a - 1)
a << k    // shift left  == a * 2^k
a >> k    // shift right == a / 2^k (floor, sign-extended for signed types)

1 << k          // mask with only bit k set
(1 << k) - 1    // mask with the lowest k bits set
n & (n - 1)     // clear the lowest set bit of n
n & (-n)        // isolate the lowest set bit of n
```

| Trap | Fix |
| --- | --- |
| `n & 1 == 0` | `(n & 1) == 0` |
| `a & b` for boolean logic | `a && b` |
| `-INT_MIN` | widen to `long long` first |
| `1 << 31` on `int` | use `1LL << 31` or `unsigned` |
