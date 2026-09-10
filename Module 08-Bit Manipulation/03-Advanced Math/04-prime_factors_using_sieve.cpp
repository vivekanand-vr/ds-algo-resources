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

/*
    ======================================================================
    DRY RUN: query x = 60   (answer = {2, 2, 3, 5})
    ======================================================================

    Two phases: build the SPF table once, then factor by repeatedly
    dividing by spf[x]. The real table runs to MAXN = 100001; only the
    window 2..20 is shown, which is enough to follow both phases.

    Tracked state:
      spf[k]  - smallest prime factor of k; 0 means "not yet known"
      i       - the outer sieve driver
      x       - the query value, shrinking toward 1
      factors - the collected primes, with multiplicity

    ======================================================================
    PHASE 1 - build the SPF table   (window 2..20, all entries start 0)
    ======================================================================

    ----------------------------------------------------------------------
    i = 2      spf[2] == 0 -> 2 is prime
      inner    j = 4, 6, 8, ... step 2, set spf[j] = 2 where still 0
               in-window writes: spf[4]=spf[6]=spf[8]=spf[10]=spf[12]
                                 =spf[14]=spf[16]=spf[18]=spf[20]=2
      then     spf[2] = 2        (set AFTER the inner loop)

      idx:  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
      spf:  2  0  2  0  2  0  2  0  2  0  2  0  2  0  2  0  2  0  2

    ----------------------------------------------------------------------
    i = 3      spf[3] == 0 -> 3 is prime
      inner    j = 9, 12, 15, 18, ... step 3
               j =  9: spf[9]  == 0 -> spf[9]  = 3
               j = 12: spf[12] == 2 -> already set, SKIPPED (the guard
                       is what keeps SMALLEST, not latest, prime)
               j = 15: spf[15] == 0 -> spf[15] = 3
               j = 18: spf[18] == 2 -> skipped
      then     spf[3] = 3

      idx:  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
      spf:  2  3  2  0  2  0  2  3  2  0  2  0  2  3  2  0  2  0  2

    ----------------------------------------------------------------------
    i = 4      spf[4] == 2, not 0 -> composite, no pass at all
    ----------------------------------------------------------------------
    i = 5      spf[5] == 0 -> prime; inner starts at 25, outside the
               window (spf[25]=5 etc.); then spf[5] = 5
    ----------------------------------------------------------------------
    i = 6      spf[6] == 2 -> skip
    i = 7      spf[7] == 0 -> prime; inner starts at 49; spf[7] = 7
    i = 8,9,10 already 2, 3, 2 -> skip
    i = 11     prime -> spf[11] = 11   (inner starts at 121)
    i = 12     already 2 -> skip
    i = 13     prime -> spf[13] = 13
    i = 14,15,16 already 2, 3, 2 -> skip
    i = 17     prime -> spf[17] = 17
    i = 18     already 2 -> skip
    i = 19     prime -> spf[19] = 19
    i = 20     already 2 -> skip
    ----------------------------------------------------------------------

    SPF table, window 2..20, fully built:

      idx:  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
      spf:  2  3  2  5  2  7  2  3  2 11  2 13  2  3  2 17  2 19  2

      read it as: spf[k] == k exactly when k is prime; otherwise spf[k]
      is the smallest prime dividing k.

    ======================================================================
    PHASE 2 - factorize(60)
    ======================================================================

    Initial state: x = 60, factors = {}
    (spf[60] = 2, spf[30] = 2, spf[15] = 3, spf[5] = 5 - all outside the
     printed window except spf[15] and spf[5], but built the same way:
     60 and 30 were marked by the i = 2 pass.)

    ----------------------------------------------------------------------
    x = 60     x != 1 -> continue
      lookup   spf[60] = 2
      record   factors = {2}
      divide   x = 60 / 2 = 30

    ----------------------------------------------------------------------
    x = 30     x != 1 -> continue
      lookup   spf[30] = 2
      record   factors = {2, 2}      (multiplicity kept, unlike the
                                      unique-factors variant)
      divide   x = 30 / 2 = 15

    ----------------------------------------------------------------------
    x = 15     x != 1 -> continue
      lookup   spf[15] = 3           2 no longer divides x, so the
                                     smallest prime has moved up
      record   factors = {2, 2, 3}
      divide   x = 15 / 3 = 5

    ----------------------------------------------------------------------
    x = 5      x != 1 -> continue
      lookup   spf[5] = 5            5 is prime, so it is its own SPF
      record   factors = {2, 2, 3, 5}
      divide   x = 5 / 5 = 1

    ----------------------------------------------------------------------
    x = 1      loop condition x != 1 is false -> stop

    ----------------------------------------------------------------------
    RETURN {2, 2, 3, 5}        and 2 * 2 * 3 * 5 = 60   (check)

    ======================================================================
    Summary table
    ======================================================================

    factorize(60):

    | step | x before | spf[x] | factors so far | x after |
    |------|----------|--------|----------------|---------|
    |  1   |    60    |   2    | {2}            |   30    |
    |  2   |    30    |   2    | {2,2}          |   15    |
    |  3   |    15    |   3    | {2,2,3}        |    5    |
    |  4   |     5    |   5    | {2,2,3,5}      |    1    |

    The other two queries in main(), same mechanism:

    | query | divisions                        | factors   |
    |-------|----------------------------------|-----------|
    |  12   | 12 -> 6 (spf 2), 6 -> 3 (spf 2), | {2, 2, 3} |
    |       | 3 -> 1 (spf 3)                   |           |
    |  17   | 17 -> 1 (spf 17)                 | {17}      |

    Why the factors come out sorted with no sorting step:
      each division removes the SMALLEST remaining prime, so the primes
      can only ever stay equal or increase from one step to the next.
      In the 60 trace: 2, 2, 3, 5 - never a step back down.

    Why the inner loop may start at i*i here too:
      a composite k with smallest prime p satisfies k = p*q with q >= p,
      so k >= p*p. Every composite is therefore reached by the pass of
      its own smallest prime, and the "if spf[j] == 0" guard makes sure
      the FIRST (smallest) prime to reach it is the one recorded.

    Step count backing the complexity:
      factoring 60 took 4 table lookups and 4 divisions - not the
      sqrt(60) ~ 8 trial divisions the standalone method needs, and the
      saving compounds over many queries because the O(MAXN log log
      MAXN) sieve is paid once. Each query costs at most log2(x) steps,
      since every division at least halves x (60 -> 30 -> 15 -> 5 -> 1
      is 4 steps, and log2(60) ~ 5.9).

    Edge cases the loop does NOT handle:
      spf[0] and spf[1] are both 0, so factorize(0) would divide by zero
      and factorize(1) returns an empty vector. main() only passes
      values >= 2, so neither is exercised here.
*/
