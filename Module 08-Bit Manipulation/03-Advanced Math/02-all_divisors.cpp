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
