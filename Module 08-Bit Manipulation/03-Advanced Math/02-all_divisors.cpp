#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a positive integer n, find the value of Sum(i=1 to n) F(i),
//    where F(i) is the sum of all divisors of i
//
// Example:
// n = 4
// F(1)=1, F(2)=1+2=3, F(3)=1+3=4, F(4)=1+2+4=7
// Total = 1 + 3 + 4 + 7 = 15

/*
    Approach: Divisor Contribution Counting

    - The brute-force way sums the divisors of every i from 1 to n
      individually (O(n * sqrt(n)) with trial division each time) - far
      too slow for large n.
    - Instead of asking "what are the divisors of each i", flip the
      question around: "for each possible divisor d, how many times
      does d get counted across all F(1)..F(n), and what does it add?"
    - A fixed value d divides i whenever i is a multiple of d, i.e. for
      i = d, 2d, 3d, ..., up to n. There are exactly floor(n / d) such
      multiples, and every one of them adds d (not 1) to the running
      total, since d itself is the divisor being counted.
    - So d's total contribution across the whole sum is d * floor(n/d).
    - Summing that contribution over every d from 1 to n gives the
      answer directly, without ever factoring a single number.

    Algorithm Steps
    ----------------
    1. Initialize total = 0
    2. For d from 1 to n:
         total += d * (n / d)      -> integer division gives floor(n/d)
    3. Return total

    Time Complexity: O(n) - one pass over all possible divisors
    Space Complexity: O(1) - only an accumulator is used
*/
long long sumOfDivisors(long long n) {
  long long total = 0;

  for (long long d = 1; d <= n; d++) {
    total += d * (n / d);   // d contributes to every multiple of itself up to n
  }

  return total;
}

int main() {
  long long n = 4;
  cout << "n = " << n << endl;
  cout << "Sum of F(1) to F(" << n << "): " << sumOfDivisors(n) << endl;

  n = 10;
  cout << "\nn = " << n << endl;
  cout << "Sum of F(1) to F(" << n << "): " << sumOfDivisors(n) << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: n = 4   (answer = 15)
    ======================================================================

    The grid below is the whole idea. Mark an 'x' where d divides i.
    Reading it by COLUMN gives F(i) (the brute-force view); reading it by
    ROW gives d * floor(n/d) (what the code actually computes). Both add
    up to the same 15.

                     i=1   i=2   i=3   i=4    row: count  x  d
      d = 1           x     x     x     x          4  x 1  =  4
      d = 2                 x           x          2  x 2  =  4
      d = 3                       x                1  x 3  =  3
      d = 4                             x          1  x 4  =  4
                    -----------------------                 ----
      F(i) column:    1     3     4     7                     15
      (1)  (1+2)  (1+3)  (1+2+4)      total 1+3+4+7 = 15

    Tracked state:
      d     - the divisor whose contribution is being counted
      n / d - integer division = floor(n/d) = how many multiples of d
              exist in 1..n, i.e. how many of F(1)..F(n) include d
      total - the running sum

    Initial state: d = 1, total = 0

    ----------------------------------------------------------------------
    d = 1
      bound        d = 1 <= n = 4 -> enter
      multiples    n / d = 4 / 1 = 4     (1, 2, 3, 4 - every i)
      contributes  d * (n/d) = 1 * 4 = 4
      total        0 -> 4
      advance      d -> 2

    ----------------------------------------------------------------------
    d = 2
      bound        2 <= 4 -> enter
      multiples    4 / 2 = 2             (2 and 4 only)
      contributes  2 * 2 = 4
      total        4 -> 8
      advance      d -> 3

    ----------------------------------------------------------------------
    d = 3
      bound        3 <= 4 -> enter
      multiples    4 / 3 = 1             floor, NOT 1.33 - only i = 3
      contributes  3 * 1 = 3
      total        8 -> 11
      advance      d -> 4

    ----------------------------------------------------------------------
    d = 4
      bound        4 <= 4 -> enter (the bound is <=, so d = n is included)
      multiples    4 / 4 = 1             only i = 4
      contributes  4 * 1 = 4
      total        11 -> 15
      advance      d -> 5, and 5 <= 4 is false: loop ends

    ----------------------------------------------------------------------
    RETURN total = 15

    ======================================================================
    Summary table
    ======================================================================

    n = 4:

    | d | n / d (floor) | contribution d*(n/d) | total after |
    |---|---------------|----------------------|-------------|
    | 1 |       4       |          4           |      4      |
    | 2 |       2       |          4           |      8      |
    | 3 |       1       |          3           |     11      |
    | 4 |       1       |          4           |     15      |

    The second main() case, n = 10 (answer 87):

    | d  | 10 / d | contribution | total after |
    |----|--------|--------------|-------------|
    |  1 |   10   |      10      |     10      |
    |  2 |    5   |      10      |     20      |
    |  3 |    3   |       9      |     29      |
    |  4 |    2   |       8      |     37      |
    |  5 |    2   |      10      |     47      |
    |  6 |    1   |       6      |     53      |
    |  7 |    1   |       7      |     60      |
    |  8 |    1   |       8      |     68      |
    |  9 |    1   |       9      |     77      |
    | 10 |    1   |      10      |     87      |

      cross-check the other way round, by summing F(i) directly:
        F(1..10) = 1, 3, 4, 7, 6, 12, 8, 15, 13, 18
        1+3=4, +4=8, +7=15, +6=21, +12=33, +8=41, +15=56, +13=69,
        +18 = 87                                    <-- agrees

      note the floor: 10/3 = 3 and 10/4 = 2 discard the fractional part,
      and 10/5 = 2 while 10/6 = 1 - getting these wrong is the classic
      off-by-one in this problem.

    The identity that makes it work:
      Sum(i=1..n) F(i) = Sum(i=1..n) Sum(d | i) d
                       = Sum(d=1..n) d * #{i <= n : d | i}
                       = Sum(d=1..n) d * floor(n/d)
      Both double sums count the same set of (d, i) pairs - the grid
      above - just traversed by column versus by row.

    Step count backing O(n):
      n = 4 took 4 iterations, n = 10 took 10 - one multiply and one
      division each, so 10 iterations for n = 10. The brute force would
      have factored each of the 10 numbers separately (about
      n * sqrt(n) ~ 32 divisions) and grows much faster.
*/
