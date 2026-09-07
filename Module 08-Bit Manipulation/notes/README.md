# Bit Manipulation — Concept Notes

Theory notes for the recurring techniques behind the problems in [Module 08-Bit Manipulation](../) — Easy, Medium, and Advanced Math. Each note gives the idea, the code template, worked traces, pitfalls, and links to the problem files that use it.

| # | Note | Covers |
| --- | --- | --- |
| 01 | [Bitwise Operators — Basics](01-Bitwise-Operators-Basics.md) | binary representation, two's complement, the six operators, shifts, precedence gotchas |
| 02 | [Bit Masking Techniques](02-Bit-Masking-Techniques.md) | check/set/clear/toggle/update a bit, evenness check, power-of-two check, XOR swap |
| 03 | [Counting Set Bits](03-Counting-Set-Bits.md) | Brian Kernighan's algorithm, popcount builtins, cumulative set-bit count over a range |
| 04 | [XOR — Properties and Applications](04-XOR-Properties-and-Applications.md) | XOR identities, the single-number family, min bit flips, prefix XOR range queries |
| 05 | [Advanced Bit Tricks](05-Advanced-Bit-Tricks.md) | maximising XOR over a range, bitmask subset enumeration, division via shifting, binary exponentiation |
| 06 | [Sieves and Number Theory](06-Sieve-and-Number-Theory.md) | trial division, divisor contribution counting, Sieve of Eratosthenes, SPF sieve |

---

## Which technique for which problem?

```
Working with a SINGLE bit position?
+-- check / set / clear / toggle / update it        -> Bit Masking            [02]
+-- is it even/odd (bit 0)?                          -> Bit Masking            [02]
+-- is the whole number a power of two?              -> n & (n-1)             [02]

Need to COUNT set bits?
+-- in one number                                    -> Brian Kernighan        [03]
+-- summed across a whole range 1..n                 -> Recursive bit formula  [03]

Values that CANCEL IN PAIRS (appear twice, once left over)?
+-- one leftover value                               -> XOR cancellation       [04]
+-- two leftover values                              -> XOR + bit partition    [04]
+-- "distance" between two numbers' bit patterns      -> XOR + Kernighan       [04]
+-- XOR over a contiguous number range                -> Prefix XOR            [04]

Need the BEST / ALL combinations under a bit budget?
+-- max XOR achievable in a numeric range             -> Highest differing bit [05]
+-- every subset of a small set                       -> Bitmask enumeration   [05]

Arithmetic WITHOUT * / /?
+-- division                                          -> Bit-shift doubling    [05]
+-- exponentiation (x^n)                              -> Binary exponentiation [05]

NUMBER THEORY (primes, divisors, factorization)?
+-- factor one number                                 -> Trial division        [06]
+-- factor many numbers                               -> SPF sieve             [06]
+-- count primes in a range                           -> Sieve of Eratosthenes [06]
+-- sum of divisor-sums over a range                  -> Contribution counting [06]
```

---

## Technique index by problem

### 01-Easy

| Problem | Technique |
| --- | --- |
| [01 Check k-th bit](../01-Easy/01-check_kth_bit.cpp) | [Bit Masking](02-Bit-Masking-Techniques.md) |
| [02 Is even](../01-Easy/02-is_even.cpp) | [Bit Masking](02-Bit-Masking-Techniques.md) — LSB check |
| [03 Is power of 2](../01-Easy/03-is_power_of_2.cpp) | [Bit Masking](02-Bit-Masking-Techniques.md) — `n & (n-1)` |
| [04 Count set bits](../01-Easy/04-count_set_bits.cpp) | [Counting Set Bits](03-Counting-Set-Bits.md) — Brian Kernighan |
| [05 Swap two numbers](../01-Easy/05-swap_two_numbers.cpp) | [Bit Masking](02-Bit-Masking-Techniques.md) — XOR swap |
| [06 Maximising XOR](../01-Easy/06-maximising_xor.cpp) | [Advanced Bit Tricks](05-Advanced-Bit-Tricks.md) — highest differing bit |
| [07 Divide two integers](../01-Easy/07-divide_two_integers.cpp) | [Advanced Bit Tricks](05-Advanced-Bit-Tricks.md) — bit-shift division |
| [08 Count set bits 1 to n](../01-Easy/08-count_set_bits_in_1_to_n.cpp) | [Counting Set Bits](03-Counting-Set-Bits.md) — recursive formula |

### 02-Medium

| Problem | Technique |
| --- | --- |
| [01 Min bit flips to convert](../02-Medium/01-min_bits_to_flip_to_convert.cpp) | [XOR Properties](04-XOR-Properties-and-Applications.md) — Hamming distance |
| [02 Single number](../02-Medium/02-single_number.cpp) | [XOR Properties](04-XOR-Properties-and-Applications.md) — cancellation |
| [03 XOR of number range](../02-Medium/03-xor_of_number_range.cpp) | [XOR Properties](04-XOR-Properties-and-Applications.md) — prefix XOR |
| [04 Subsets](../02-Medium/04-subsets.cpp) | [Advanced Bit Tricks](05-Advanced-Bit-Tricks.md) — bitmask enumeration |
| [05 Two numbers appearing odd times](../02-Medium/05-two_numbers_appearing_odd_times.cpp) | [XOR Properties](04-XOR-Properties-and-Applications.md) — bit partitioning |

### 03-Advanced Math

| Problem | Technique |
| --- | --- |
| [01 Prime factors](../03-Advanced%20Math/01-prime_factors.cpp) | [Sieves and Number Theory](06-Sieve-and-Number-Theory.md) — trial division |
| [02 All divisors](../03-Advanced%20Math/02-all_divisors.cpp) | [Sieves and Number Theory](06-Sieve-and-Number-Theory.md) — contribution counting |
| [03 Sieve of Eratosthenes](../03-Advanced%20Math/03-sieve_of_eratosthenes.cpp) | [Sieves and Number Theory](06-Sieve-and-Number-Theory.md) |
| [04 Prime factors using sieve](../03-Advanced%20Math/04-prime_factors_using_sieve.cpp) | [Sieves and Number Theory](06-Sieve-and-Number-Theory.md) — SPF sieve |
| [05 pow(x, n)](../03-Advanced%20Math/05-pow.cpp) | [Advanced Bit Tricks](05-Advanced-Bit-Tricks.md) — binary exponentiation |
