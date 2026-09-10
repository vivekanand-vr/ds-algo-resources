#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given an integer n, return the number of prime numbers that are
//    strictly less than n
//
// Example:
// n = 10
// Primes less than 10: 2, 3, 5, 7 -> count = 4

/*
    Approach: Sieve of Eratosthenes

    - Testing each number below n for primality individually (trial
      division up to sqrt of each) costs O(n * sqrt(n)) overall - slow
      when many primes must be found at once.
    - The sieve instead marks composites in bulk: starting from the
      first prime, 2, every multiple of 2 is composite and gets marked;
      then every multiple of 3 not already marked; and so on.
    - A number i only needs to start marking from i * i, since any
      smaller multiple of i (like 2*i, 3*i, ...) already has a smaller
      prime factor and was marked earlier by that smaller prime.
    - Whatever survives unmarked after processing every i up to sqrt(n)
      is prime, because a composite number less than n must have a
      prime factor <= sqrt(n) and would have been caught by then.

    Algorithm Steps
    ----------------
    1. If n <= 2, there are no primes below n - return 0
    2. Create a boolean array isComposite[0..n-1], all initialized to false
    3. For i from 2 while i * i < n:
         if i is not marked composite:
           mark every multiple of i, starting at i * i, as composite
    4. Count every index from 2 to n - 1 that is still unmarked
    5. Return that count

    Time Complexity: O(n log log n) - the classic sieve bound
    Space Complexity: O(n) - one boolean per number below n
*/
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
  for (int i = 2; i < n; i++) {
    if (!isComposite[i]) count++;
  }

  return count;
}

int main() {
  int n = 10;
  cout << "n = " << n << endl;
  cout << "Primes less than " << n << ": " << countPrimes(n) << endl;

  n = 30;
  cout << "\nn = " << n << endl;
  cout << "Primes less than " << n << ": " << countPrimes(n) << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: n = 30   (answer = 10)
    ======================================================================

    isComposite is a bool array of length n = 30, indices 0..29, all
    false. T = marked composite, F = still unmarked. Indices 0 and 1 are
    never marked and never counted (the counting loop starts at 2).

    Tracked state:
      i            - the candidate prime driving the current marking pass
      isComposite  - the array, shown in two halves below
      count        - primes found, computed only at the very end

    Initial state: every entry F, count not yet started

    ----------------------------------------------------------------------
    i = 2      bound i*i = 4 < 30 -> yes;  isComposite[2] = F -> 2 is prime
      inner loop  j = 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28
                  (j += 2, stops because 30 is not < 30)  -> 13 marks

      idx    :  2  3  4  5  6  7  8  9 10 11 12 13 14 15
      aft i=2:  F  F  T  F  T  F  T  F  T  F  T  F  T  F
      idx    : 16 17 18 19 20 21 22 23 24 25 26 27 28 29
      aft i=2:  T  F  T  F  T  F  T  F  T  F  T  F  T  F

    ----------------------------------------------------------------------
    i = 3      bound i*i = 9 < 30 -> yes;  isComposite[3] = F -> 3 is prime
      inner loop  j = 9, 12, 15, 18, 21, 24, 27  (j += 3)  -> 7 marks
                  12, 18 and 24 were ALREADY T from the i = 2 pass; the
                  code re-writes them anyway (harmless, no if-guard)
      newly T     9, 15, 21, 27

      idx    :  2  3  4  5  6  7  8  9 10 11 12 13 14 15
      aft i=3:  F  F  T  F  T  F  T  T  T  F  T  F  T  T
      idx    : 16 17 18 19 20 21 22 23 24 25 26 27 28 29
      aft i=3:  T  F  T  F  T  T  T  F  T  F  T  T  T  F

    ----------------------------------------------------------------------
    i = 4      bound i*i = 16 < 30 -> yes, BUT isComposite[4] = T
      skipped     4 is composite, so it starts no marking pass. Anything
                  4 would have marked (16, 20, 24, 28) is a multiple of
                  2 and was marked at i = 2 already.
      array       unchanged

    ----------------------------------------------------------------------
    i = 5      bound i*i = 25 < 30 -> yes;  isComposite[5] = F -> 5 is prime
      inner loop  j = 25 only  (next would be 30, not < 30)  -> 1 mark
      newly T     25

      idx    :  2  3  4  5  6  7  8  9 10 11 12 13 14 15
      aft i=5:  F  F  T  F  T  F  T  T  T  F  T  F  T  T   (unchanged)
      idx    : 16 17 18 19 20 21 22 23 24 25 26 27 28 29
      aft i=5:  T  F  T  F  T  T  T  F  T  T  T  T  T  F
                                           ^ 25 flipped

    ----------------------------------------------------------------------
    i = 6      bound i*i = 36 < 30 ?  NO  -> outer loop ends
      note        marking is finished at i = 5 because sqrt(30) ~ 5.48.
                  Any composite below 30 has a prime factor <= 5, so it
                  is already T.

    ======================================================================
    COUNTING pass: indices 2..29, count the F entries
    ======================================================================

      idx    :  2  3  4  5  6  7  8  9 10 11 12 13 14 15
      final  :  F  F  T  F  T  F  T  T  T  F  T  F  T  T
      prime? :  *  *     *     *           *     *
      idx    : 16 17 18 19 20 21 22 23 24 25 26 27 28 29
      final  :  T  F  T  F  T  T  T  F  T  T  T  T  T  F
      prime? :     *     *           *                 *

      running count: 2->1, 3->2, 5->3, 7->4, 11->5, 13->6,
                     17->7, 19->8, 23->9, 29->10

    ----------------------------------------------------------------------
    RETURN count = 10        primes below 30:
                             2, 3, 5, 7, 11, 13, 17, 19, 23, 29

    ======================================================================
    Summary table
    ======================================================================

    | i | i*i | i*i < 30 | comp[i]? | marks written        | count |
    |---|-----|----------|----------|----------------------|-------|
    | 2 |  4  |   yes    | F, prime | 4..28 step 2 (13)    |  13   |
    | 3 |  9  |   yes    | F, prime | 9..27 step 3 (7)     |   7   |
    | 4 | 16  |   yes    | T, skip  | none                 |   0   |
    | 5 | 25  |   yes    | F, prime | 25 (1)               |   1   |
    | 6 | 36  |   no     | loop out | -                    |   -   |

    Why the inner loop starts at i*i and not at 2*i:
      any multiple k*i with k < i has a factor k smaller than i, and k
      itself has a prime factor p <= k < i. That prime p ran its own
      marking pass EARLIER (the outer loop goes upward), and k*i is a
      multiple of p, so k*i is already T. Concretely at i = 5: 10, 15
      and 20 are all already marked (10 and 20 by 2, 15 by 3), so
      starting at 5*5 = 25 skips three redundant writes. Starting at 2*i
      would still be correct, just wasteful.

    Step count backing O(n log log n):
      only 21 marking writes total (13 + 7 + 1) for n = 30, of which 3
      were duplicates (12, 18, 24), plus 4 outer bound checks and 28
      counting reads. Trial-dividing each of 28 numbers up to its own
      square root would have cost roughly 3-4x that.

    One quirk worth knowing:
      the outer bound is i*i < n (strict), matching "primes strictly
      less than n". If n were a perfect square, say n = 25, then i = 5
      fails 25 < 25 and no pass runs for 5 - which is fine, because the
      only multiple of 5 that pass would mark is 25 itself, and 25 is
      not below n = 25.
*/
