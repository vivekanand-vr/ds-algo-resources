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
