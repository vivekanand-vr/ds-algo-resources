#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given an integer array queries, return the prime factorization
//    (with multiplicity, in increasing order) of every number in it
//
// Example:
// queries = {12, 17, 60}
// 12 = 2 * 2 * 3        -> {2, 2, 3}
// 17 = 17 (prime)        -> {17}
// 60 = 2 * 2 * 3 * 5     -> {2, 2, 3, 5}

/*
    Approach: Smallest Prime Factor (SPF) Sieve

    - Factoring each query independently with trial division costs
      O(sqrt(value)) per query - fine for one number, wasteful when the
      same small primes get rediscovered from scratch for every query.
    - Instead, precompute once: for every number up to the largest
      possible query, its smallest prime factor (SPF). This is a
      sieve just like Eratosthenes', except instead of only marking
      "composite", we record WHICH prime first marked each number.
    - With the SPF table ready, factoring any single number becomes:
      repeatedly divide it by its own SPF and record that prime, until
      it becomes 1. Each division strips off exactly the smallest
      remaining prime factor, so the factors come out in increasing
      order automatically - no sorting needed.
    - This turns every query into O(log(value)) work after the O(N log
      log N) one-time sieve.

    Algorithm Steps
    ----------------
    1. Build the SPF table for all numbers 0..MAXN-1:
         for i from 2 while i * i < MAXN:
           if SPF[i] is still unset (i is prime):
             for every multiple j of i starting at i * i,
             set SPF[j] = i if it is not already set
         any i left with SPF[i] == 0 after the loop is itself prime,
         so set SPF[i] = i
    2. For each query value x:
         a. While x != 1: append SPF[x] to this query's factor list,
            then set x = x / SPF[x]
    3. Collect the factor lists for all queries and return them

    Time Complexity: O(MAXN log log MAXN) to build the sieve once,
                      then O(log x) per query
    Space Complexity: O(MAXN) for the SPF table
*/
const int MAXN = 100001;
vector<int> spf(MAXN, 0);   // spf[i] = smallest prime factor of i

void buildSpfSieve() {
  for (int i = 2; i < MAXN; i++) {
    if (spf[i] == 0) {          // i has no smaller factor recorded -> i is prime
      for (long long j = (long long)i * i; j < MAXN; j += i) {
        if (spf[j] == 0) spf[j] = i;
      }
      spf[i] = i;                // i itself is its own smallest prime factor
    }
  }
}

vector<int> factorize(int x) {
  vector<int> factors;
  while (x != 1) {
    factors.push_back(spf[x]);
    x /= spf[x];
  }
  return factors;
}

vector<vector<int>> primeFactorsUsingSieve(vector<int>& queries) {
  buildSpfSieve();

  vector<vector<int>> result;
  for (int q : queries) result.push_back(factorize(q));
  return result;
}

int main() {
  vector<int> queries = {12, 17, 60};

  cout << "Queries: ";
  for (int q : queries) cout << q << " ";
  cout << endl;

  vector<vector<int>> result = primeFactorsUsingSieve(queries);

  for (size_t i = 0; i < queries.size(); i++) {
    cout << queries[i] << " = ";
    for (size_t j = 0; j < result[i].size(); j++) {
      cout << result[i][j];
      if (j + 1 < result[i].size()) cout << " * ";
    }
    cout << endl;
  }

  return 0;
}
