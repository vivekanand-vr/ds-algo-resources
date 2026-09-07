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
