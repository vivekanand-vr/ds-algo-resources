#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a number n, find its unique prime factors in increasing order
//
// Example:
// n = 60 (= 2^2 * 3 * 5)
// Unique prime factors: {2, 3, 5}

/*
    Approach: Trial Division up to sqrt(n)

    - A composite number n always has at least one factor <= sqrt(n),
      so it is enough to test divisibility only up to sqrt(n) instead
      of up to n itself.
    - For each candidate divisor i starting at 2, while i still divides
      n, keep dividing n by i - this strips out every copy of i in one
      go, so i is recorded only once even if it appears with a higher
      power (e.g. 2^2 in 60).
    - Once i * i exceeds what remains of n, whatever is left of n (if
      greater than 1) must itself be a prime factor larger than sqrt of
      the original n, and it appears exactly once - record it directly.
    - Testing divisors in increasing order naturally produces the
      factors in increasing order, with no sorting needed afterward.

    Algorithm Steps
    ----------------
    1. Initialize an empty result list
    2. For i from 2 while i * i <= n:
         a. If i divides n, record i once
         b. Keep dividing n by i for as long as it divides evenly
       (this both skips ahead past composite divisors and collapses
        repeated prime powers)
    3. If n > 1 after the loop, the remaining n is one last prime factor
    4. Return the result list

    Time Complexity: O(sqrt(n)) - the loop only runs up to sqrt(n)
    Space Complexity: O(log n) - at most log2(n) distinct prime factors
*/
vector<int> primeFactors(int n) {
  vector<int> factors;

  for (int i = 2; (long long)i * i <= n; i++) {
    if (n % i == 0) {
      factors.push_back(i);
      while (n % i == 0) n /= i;   // strip out every copy of this factor
    }
  }

  if (n > 1) factors.push_back(n);   // whatever remains is prime

  return factors;
}

int main() {
  int n = 60;
  cout << "n = " << n << endl;
  cout << "Unique prime factors: ";
  for (int f : primeFactors(n)) cout << f << " ";
  cout << endl;

  n = 97;
  cout << "\nn = " << n << endl;
  cout << "Unique prime factors: ";
  for (int f : primeFactors(n)) cout << f << " ";
  cout << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: n = 60   (answer = {2, 3, 5})
    ======================================================================

      60 decomposes as   60 = 2 * 2 * 3 * 5 = 2^2 * 3 * 5
      and the UNIQUE prime factors are 2, 3, 5.

      the value shrinking as factors are stripped:
        60  --/2-->  30  --/2-->  15  --/3-->  5  (left over, prime)

    Tracked state:
      i       - the candidate divisor being tested, starting at 2
      n       - the REMAINING part of the number; it shrinks in place as
                factors are divided out, so the loop bound i*i <= n
                tightens as we go
      factors - the collected unique prime factors, in increasing order

    Initial state: i = 2, n = 60, factors = {}

    ----------------------------------------------------------------------
    i = 2
      bound        i*i = 4 <= n = 60 -> enter the loop body
      divides?     60 % 2 = 0 -> yes, 2 is a prime factor
      record       factors = {2}          (recorded ONCE, before the
                                           inner while strips the rest)
      strip        step 1: 60 % 2 = 0 -> n = 60 / 2 = 30
                   step 2: 30 % 2 = 0 -> n = 30 / 2 = 15
                   step 3: 15 % 2 = 1 -> stop stripping
      n            60 -> 15              (both copies of 2 removed)
      advance      i -> 3

    ----------------------------------------------------------------------
    i = 3
      bound        i*i = 9 <= n = 15 -> enter the loop body
      divides?     15 % 3 = 0 -> yes
      record       factors = {2, 3}
      strip        step 1: 15 % 3 = 0 -> n = 15 / 3 = 5
                   step 2:  5 % 3 = 2 -> stop stripping
      n            15 -> 5
      advance      i -> 4

    ----------------------------------------------------------------------
    i = 4
      bound        i*i = 16 <= n = 5 ?  NO  -> loop ends immediately
      note         i = 4 is never even tested for divisibility. It could
                   not have divided n anyway: every factor of 2 was
                   already stripped at i = 2, so no multiple of 2 can
                   still divide the remaining n. That is why testing
                   plain integers (not just primes) is still correct.

    ----------------------------------------------------------------------
    leftover check
      n = 5 > 1 -> the remainder is itself prime, record it
      factors = {2, 3, 5}

      why 5 must be prime: if it had any factor d with 2 <= d <= sqrt(5)
      the loop would have found it, and the loop only exited because
      i*i > n. So the survivor has no factor below its own square root,
      i.e. it is prime, and it appears exactly once.

    ----------------------------------------------------------------------
    RETURN factors = {2, 3, 5}

    ======================================================================
    Summary table
    ======================================================================

    | i | i*i | n at test | i*i <= n | n % i | recorded | divisions | n after |
    |---|-----|-----------|----------|-------|----------|-----------|---------|
    | 2 |  4  |    60     |   yes    |   0   |    2     | 60/2, 30/2|   15    |
    | 3 |  9  |    15     |   yes    |   0   |    3     | 15/3      |    5    |
    | 4 | 16  |     5     |   no     |   -   |    -     |     -     |    5    |
    | - |  -  |     5     | loop out |   -   |    5     |     -     |    1*   |

      (* conceptually 1 - the code does not actually assign it)

    The second main() case, n = 97 (a prime), takes the other shape:
      i = 2..9 all fail the divisibility test (97 % 2..9 = 1,1,1,2,1,6,1,7),
      n never shrinks, and the loop exits at i = 10 because 100 > 97.
      Then n = 97 > 1 is recorded -> {97}.

    Step count backing O(sqrt(n)):
      for n = 60 the loop body ran only TWICE (i = 2, 3) plus one failed
      bound check at i = 4 - not 7 iterations (sqrt(60) ~ 7.75), because
      n itself shrank from 60 to 5 and pulled the bound down with it.
      For n = 97 nothing shrinks, so the full sqrt walk happens: 8 loop
      iterations plus the failing check at i = 10 - which is the genuine
      O(sqrt(n)) worst case, reached exactly when n is prime.
*/
