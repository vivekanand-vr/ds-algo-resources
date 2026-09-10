#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array of size n containing numbers from 1 to n where one number
// is missing and another is repeated twice, find the repeating and missing numbers

/*
    Approach: Sum and Sum of Squares Equations
    - The array should ideally contain each of 1..n exactly once
    - Let x be the repeating number and y be the missing number
    - Comparing the actual sum to the expected sum S gives (x - y)
    - Comparing the actual sum of squares to the expected sum of squares P
      gives (x^2 - y^2) = (x - y)(x + y), which combined with (x - y) yields (x + y)
    - Solve the resulting 2-equation system for x and y

    Algorithm Steps
    ----------------
    1. Compute expected sum S and expected sum of squares P for 1..n
    2. Compute actual sum and actual sum of squares from the array
    3. diff1 = actual sum - S gives (x - y)
    4. diff2 = actual sumSq - P gives (x - y)(x + y); divide by diff1 to get (x + y)
    5. Solve: x = (diff1 + sumXY) / 2, y = x - diff1
    6. Return {x, y}

    Time Complexity: O(n) - one traversal to calculate sums
    Space Complexity: O(1) - constant extra space
*/
vector<int> findTwoElement(vector<int>& arr) {
    int n = arr.size();
    
    // Expected sum of first n natural numbers
    long long S = (1LL * n * (n + 1)) / 2;
    // Expected sum of squares of first n natural numbers
    long long P = (1LL * n * (n + 1) * (2LL * n + 1)) / 6;
    
    long long sum = 0, sqSum = 0;
    
    // Calculate actual sum and sum of squares from the array
    for (int i = 0; i < n; i++) {
        sum += arr[i];
        sqSum += 1LL * arr[i] * arr[i];
    }
    
    // Let x = repeating, y = missing
    // sum - S = (x - y)  → eq(1)
    // sqSum - P = (x^2 - y^2) = (x - y)(x + y)  → eq(2)

    long long diff1 = sum - S;             // (x - y)
    long long diff2 = sqSum - P;           // (x^2 - y^2) ==> (x + y)(x - y) and we already have (x - y)
    long long sumXY = diff2 / diff1;       // (x + y) as we already have (x - y) we get it by division

    // Solving equations
    // (x + y) = sumXy
    // (x - y) = diff1
    // Summing above equations 2x = diff1 + sumXY
    // i.e x = (diff1 + sumXY) / 2
    long long x = (diff1 + sumXY) / 2;    
    long long y = x - diff1;

    return {(int)x, (int)y};
}

int main() {
    vector<int> arr = {3, 1, 2, 5, 3};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    vector<int> result = findTwoElement(arr);

    cout << "Repeating number: " << result[0] << endl;
    cout << "Missing number: " << result[1] << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {3, 1, 2, 5, 3}   (n = 5, repeating = 3, missing = 4)
    ==========================================================================

    This file implements the SUM / SUM-OF-SQUARES version (not XOR), so the
    trace below solves the two equations with real numbers.

      index:   0   1   2   3   4
      arr:     3   1   2   5   3
                           ^   ^
                           |   the second 3 (the repeat)
                           5 is present, 4 is not

    Tracked state:
      S      - expected sum of 1..n            = n(n+1)/2
      P      - expected sum of squares of 1..n = n(n+1)(2n+1)/6
      sum    - actual sum of arr, accumulated in the loop
      sqSum  - actual sum of squares of arr, accumulated in the loop
      diff1  - sum   - S  , which equals (x - y)
      diff2  - sqSum - P  , which equals (x^2 - y^2) = (x - y)(x + y)
      sumXY  - diff2 / diff1 , which recovers (x + y)
      x, y   - x = the repeating number, y = the missing number

    --------------------------------------------------------------------------
    Setup (before the loop)
      n = arr.size() = 5

      S = (1LL * n * (n + 1)) / 2
        = (5 * 6) / 2 = 30 / 2 = 15
        check: 1 + 2 + 3 + 4 + 5 = 15                        OK

      P = (1LL * n * (n + 1) * (2LL * n + 1)) / 6
        = (5 * 6 * 11) / 6 = 330 / 6 = 55
        check: 1 + 4 + 9 + 16 + 25 = 55                      OK

      sum = 0, sqSum = 0

    --------------------------------------------------------------------------
    Accumulation loop (one pass, five iterations)

      i = 0, arr[0] = 3
        sum   += 3       ->  sum   = 0 + 3 = 3
        sqSum += 3*3     ->  sqSum = 0 + 9 = 9

      i = 1, arr[1] = 1
        sum   += 1       ->  sum   = 3 + 1 = 4
        sqSum += 1*1     ->  sqSum = 9 + 1 = 10

      i = 2, arr[2] = 2
        sum   += 2       ->  sum   = 4 + 2 = 6
        sqSum += 2*2     ->  sqSum = 10 + 4 = 14

      i = 3, arr[3] = 5
        sum   += 5       ->  sum   = 6 + 5 = 11
        sqSum += 5*5     ->  sqSum = 14 + 25 = 39

      i = 4, arr[4] = 3        <-- the repeat, counted a second time
        sum   += 3       ->  sum   = 11 + 3 = 14
        sqSum += 3*3     ->  sqSum = 39 + 9 = 48

      after the loop: sum = 14, sqSum = 48

    --------------------------------------------------------------------------
    Building the two equations

      Think of the array as "the perfect 1..n multiset, but y was replaced
      by a second copy of x". So the actual totals differ from the expected
      ones by exactly one swap of y for x:

        diff1 = sum   - S = 14 - 15 = -1
                and       sum   - S = x - y        -> eq(1):  x - y = -1

        diff2 = sqSum - P = 48 - 55 = -7
                and       sqSum - P = x^2 - y^2    -> eq(2):  x^2 - y^2 = -7

      Factor eq(2):   x^2 - y^2 = (x - y)(x + y)
                              -7 = (-1) * (x + y)

        sumXY = diff2 / diff1 = (-7) / (-1) = 7    -> eq(3):  x + y = 7

      Sanity check on the sign: diff1 came out NEGATIVE here because the
      repeated value 3 is SMALLER than the missing value 4, so the array
      total falls short of 15. The division still works because both diffs
      carry the same sign, and it is exact because diff2 is diff1 times an
      integer by construction.

    --------------------------------------------------------------------------
    Solving eq(1) and eq(3)

              x - y = -1        eq(1)
              x + y =  7        eq(3)
        add:  2x    =  6        (diff1 + sumXY = -1 + 7 = 6)

        x = (diff1 + sumXY) / 2 = 6 / 2 = 3        <-- repeating
        y = x - diff1           = 3 - (-1) = 4     <-- missing

      Verify against the array: 3 does appear twice, and 4 appears zero
      times. Also 1+2+3+3+5 = 14 = 15 - 4 + 3, and
      1+4+9+9+25 = 48 = 55 - 16 + 9.                        OK

    --------------------------------------------------------------------------
    RETURN {(int)x, (int)y} = {3, 4}
           result[0] = 3 = repeating, result[1] = 4 = missing

    ==========================================================================
    Summary table
    ==========================================================================

    | i | arr[i] | sum | sqSum |
    |---|--------|-----|-------|
    | - |   --   |   0 |     0 |
    | 0 |    3   |   3 |     9 |
    | 1 |    1   |   4 |    10 |
    | 2 |    2   |   6 |    14 |
    | 3 |    5   |  11 |    39 |
    | 4 |    3   |  14 |    48 |

    | quantity | formula                | value        |
    |----------|------------------------|--------------|
    | S        | n(n+1)/2               | 15           |
    | P        | n(n+1)(2n+1)/6         | 55           |
    | diff1    | sum - S     = x - y    | -1           |
    | diff2    | sqSum - P   = x^2-y^2  | -7           |
    | sumXY    | diff2/diff1 = x + y    | 7            |
    | x        | (diff1 + sumXY)/2      | 3 (repeats)  |
    | y        | x - diff1              | 4 (missing)  |

    Why the division by diff1 is always safe:
      diff1 = x - y, and the problem guarantees x != y (one number repeats,
      a DIFFERENT one is missing), so diff1 is never 0. And diff2 is
      literally diff1 * sumXY, so the integer division has no remainder -
      it is not a truncation, it is exact.

    The reason for all the 1LL / long long:
      P grows like n^3 / 3, so for n around 1e5, P is about 3.3e14 - far
      past the 2.1e9 that fits in a 32-bit int. Without the 1LL casts, the
      products n*(n+1)*(2n+1) and arr[i]*arr[i] would overflow and every
      number in this trace would come out garbage. That is the single
      easiest way to get this approach wrong.

    Step count: one pass of n = 5 iterations, then a fixed handful of
    arithmetic operations - O(n) time, O(1) space, no hash map or sorting.
*/
