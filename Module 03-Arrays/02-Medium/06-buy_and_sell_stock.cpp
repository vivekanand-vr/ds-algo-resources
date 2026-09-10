#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array of stock prices by day, find the maximum profit from a single buy and a single later sell

/*
    Approach: Track Minimum Price So Far
    - Keep track of the minimum price seen so far while traversing
    - At each day, compute the profit if selling today after buying at the minimum seen
    - Track the maximum such profit

    Algorithm Steps
    ----------------
    1. Initialize mn = INT_MAX and mp = 0
    2. For each price, update mn = min(mn, price)
    3. Update mp = max(mp, price - mn)
    4. Return mp

    Time Complexity: O(n) - single pass through prices
    Space Complexity: O(1) - only a few variables used
*/
int maxProfit(vector<int>& arr) {
    int mn = INT_MAX;  // track minimum price seen so far
    int mp = 0;        // track maximum profit

    // Traverse all prices
    for (int price : arr) {
        mn = min(mn, price);        // update min price
        mp = max(mp, price - mn);   // update max profit if selling today is better
    }

    return mp;
}

int main() {
    vector<int> arr = {7, 1, 5, 3, 6, 4};

    cout << "Prices: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Maximum profit: " << maxProfit(arr) << endl;

    return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {7,1,5,3,6,4}   (n = 6, answer = 5)
    ======================================================================

      index:   0   1   2   3   4   5
      price:   7   1   5   3   6   4

    Tracked state:
      mn  - the cheapest price seen so far, i.e. the best day to have
            bought at any point up to and including today
      mp  - the best profit seen so far; starts at 0 so "never trade"
            is always an option and the answer is never negative

    Initial state: mn = INT_MAX, mp = 0

    The loop body is only two lines, and their ORDER is the whole trick:
    mn is updated BEFORE the profit is measured, so price - mn is always
    "sell today, having bought on the cheapest day so far (today
    included)". That is why price - mn can never come out negative.

    ----------------------------------------------------------------------
    price = 7        (day 0)
      min      mn = min(INT_MAX, 7) = 7          <-- first real minimum
      profit   price - mn = 7 - 7 = 0
               mp = max(0, 0) = 0
      state    mn = 7, mp = 0

    ----------------------------------------------------------------------
    price = 1        (day 1)   <-- new minimum, the eventual buy day
      min      mn = min(7, 1) = 1                <-- mn drops
      profit   price - mn = 1 - 1 = 0
               mp = max(0, 0) = 0
      state    mn = 1, mp = 0

               Note that on a day that sets a NEW minimum the profit
               term is forced to exactly 0 (price == mn). A new low can
               never raise mp on the same day - it can only help later
               days. mn = 1 is now locked in for the rest of the run.

    ----------------------------------------------------------------------
    price = 5        (day 2)
      min      mn = min(1, 5) = 1                (unchanged)
      profit   price - mn = 5 - 1 = 4
               mp = max(0, 4) = 4                <-- first real profit
      state    mn = 1, mp = 4      (buy day 1 at 1, sell day 2 at 5)

    ----------------------------------------------------------------------
    price = 3        (day 3)
      min      mn = min(1, 3) = 1                (unchanged)
      profit   price - mn = 3 - 1 = 2
               mp = max(4, 2) = 4                (2 < 4, mp holds)
      state    mn = 1, mp = 4

    ----------------------------------------------------------------------
    price = 6        (day 4)
      min      mn = min(1, 6) = 1                (unchanged)
      profit   price - mn = 6 - 1 = 5
               mp = max(4, 5) = 5                <-- new best

                 7   1   5   3   6   4
                     ^           ^
                    buy         sell        6 - 1 = 5

      state    mn = 1, mp = 5

    ----------------------------------------------------------------------
    price = 4        (day 5)
      min      mn = min(1, 4) = 1                (unchanged)
      profit   price - mn = 4 - 1 = 3
               mp = max(5, 3) = 5                (3 < 5, mp holds)
      state    mn = 1, mp = 5   ->   range-for ends

    ----------------------------------------------------------------------
    RETURN mp = 5        (buy on day 1 at 1, sell on day 4 at 6)

    ======================================================================
    Summary table
    ======================================================================

    | day | price | mn after min | price - mn | mp after max |
    |-----|-------|--------------|------------|--------------|
    |  0  |   7   |      7       |     0      |      0       |
    |  1  |   1   |      1       |     0      |      0       |
    |  2  |   5   |      1       |     4      |      4       |
    |  3  |   3   |      1       |     2      |      4       |
    |  4  |   6   |      1       |     5      |      5       |
    |  5  |   4   |      1       |     3      |      5       |

    Step count backing O(n): 6 iterations, each doing exactly one min and
    one max - 12 comparisons total, no nesting. The brute force would try
    every (buy, sell) pair: 6*5/2 = 15 pairs here, and n^2/2 in general.
    Space is O(1): two ints, regardless of how long the price series is.

    The one subtlety: mp starts at 0, not INT_MIN. On a strictly falling
    series like {7,6,4,1} every price - mn is 0 (each day is its own new
    minimum), so mp stays 0 - the code reports "no profitable trade
    exists" rather than the least-bad loss. That matches the problem
    statement, which allows skipping the trade entirely.
*/
