#include <bits/stdc++.h>
using namespace std;

// Q: Given a list of coin denominations (unlimited supply of each) and a
//    target amount amt, return the minimum number of coins needed to make
//    up amt. Return -1 if amt cannot be formed with the given coins.
//
// Example:
// coins = [1,2,5],  amt = 11 -> 3   (5 + 5 + 1)
// coins = [2],      amt = 3  -> -1  (odd amount, only even sums possible)
// coins = [1,2,5],  amt = 0  -> 0   (nothing to make)

/*
    Approach: Plain recursion, trying every coin at every remaining amount

    - This is written as a brute-force recursive search, NOT the greedy
      "always take the largest coin" strategy - that greedy only works for
      special coin systems (like standard currency denominations) and
      fails in general. For example coins = [1,3,4], amt = 6: greedy picks
      4 then is stuck needing 2 more with a 1 and a 3, giving 4+1+1 = 3
      coins, while the true optimum is 3+3 = 2 coins. So this problem is
      solved by trying every coin, not by sorting and taking the biggest
      first.
    - solve(coins, t) answers "minimum coins to make exactly t", by trying
      every coin c as the FIRST coin used and recursing on the remainder
      t - c. The base cases anchor the recursion: solve(0) = 0 (nothing
      left to make) and solve(negative) = INT_MAX (impossible, sentinel
      for "no way").
    - INT_MAX is used as the "impossible" sentinel throughout, which is
      why every recursive result is checked with `if (ans != INT_MAX)`
      before being used in a min - folding INT_MAX + 1 into the min
      unchecked could quietly become a small (wrapped or just wrong)
      number and corrupt the answer.
    - This has no memoization, so the same (coins, t) subproblem is
      recomputed every time it is reached via a different path - the
      complexity below reflects that. Memoizing solve on t (a simple
      1-D array of size amt+1) turns this into the standard O(amt * n)
      coin-change DP without changing the recursive structure at all.

    Algorithm Steps
    ----------------
    solve(coins, t)  -> minimum coins to make exactly t, or INT_MAX
      1. If t == 0, return 0 (target already met).
      2. If t < 0, return INT_MAX (overshot - not a valid path).
      3. res = INT_MAX.
      4. For each coin in coins:
           - ans = solve(coins, t - coin)
           - if ans != INT_MAX, res = min(res, ans + 1)
      5. Return res.

    minCoins(arr, amt)
      1. res = solve(arr, amt).
      2. Return -1 if res is still INT_MAX (unreachable), else res.

    Time Complexity: O(n^amt) in the worst case - every one of the n coins
                      is tried at every level of recursion, down to depth
                      amt, with no memoization to collapse repeated
                      subproblems. Memoizing on t brings this to O(amt*n).
    Space Complexity: O(amt) - the recursion stack depth in the worst case
                      (repeatedly using the smallest coin).
*/
int solve(vector<int>& coins, int t) {
  if (t == 0) return 0;         // nothing left to make - zero coins needed
  if (t < 0) return INT_MAX;    // overshot the target - not a valid path

  int res = INT_MAX;

  for (int coin : coins) {
    int ans = solve(coins, t - coin);   // try this coin first, recurse on the rest

    if (ans != INT_MAX) res = min(res, ans + 1);   // guard: don't mix in "impossible"
  }

  return res;
}

int minCoins(vector<int>& arr, int amt) {
  int res = solve(arr, amt);
  if (res == INT_MAX) return -1;   // target amount is unreachable
  return res;
}

int main() {
  struct Test {
    vector<int> coins;
    int amt;
  };
  vector<Test> tests = {{{1, 2, 5}, 11}, {{2}, 3}, {{1, 2, 5}, 0}};

  for (auto& t : tests) {
    cout << "Input:  coins = [";
    for (size_t i = 0; i < t.coins.size(); ++i) {
      cout << t.coins[i] << (i + 1 < t.coins.size() ? "," : "");
    }
    cout << "], amt = " << t.amt << endl;
    cout << "Output: " << minCoins(t.coins, t.amt) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: coins = [1,2,5], amt = 5   (answer 1)
    ==========================================================================

    A smaller target than the header example (5 instead of 11) is used
    here to keep the recursion tree small enough to draw in full.

    solve([1,2,5], 5)
      try coin 1: solve(4)
      try coin 2: solve(3)
      try coin 5: solve(0) = 0   <-- immediate hit, 0 + 1 = 1 coin

    Expanding every branch (t < 0 branches are marked and not expanded
    further, since they return INT_MAX immediately):

    solve(5)
    |-- coin 1 -> solve(4)
    |     |-- coin 1 -> solve(3)
    |     |     |-- coin 1 -> solve(2)
    |     |     |     |-- coin 1 -> solve(1)
    |     |     |     |     |-- coin 1 -> solve(0) = 0            -> 1
    |     |     |     |     |-- coin 2 -> solve(-1) = INT_MAX     -> skip
    |     |     |     |     |-- coin 5 -> solve(-4) = INT_MAX     -> skip
    |     |     |     |     solve(1) = min(1) = 1
    |     |     |     |-- coin 2 -> solve(0) = 0                  -> 1
    |     |     |     |-- coin 5 -> solve(-3) = INT_MAX           -> skip
    |     |     |     solve(2) = min(1+1, 0+1) = min(2, 1) = 1
    |     |     |-- coin 2 -> solve(1) = 1 (recomputed - see note below) -> 2
    |     |     |-- coin 5 -> solve(-2) = INT_MAX                 -> skip
    |     |     solve(3) = min(1+1, 1+1) = 2
    |     |-- coin 2 -> solve(2) = 1 (recomputed)                 -> 2
    |     |-- coin 5 -> solve(-1) = INT_MAX                       -> skip
    |     solve(4) = min(2+1, 1+1) = min(3, 2) = 2
    |-- coin 2 -> solve(3) = 2 (recomputed)                       -> 3
    |-- coin 5 -> solve(0) = 0                                    -> 1   <-- BEST

    solve(5) = min(2+1, 2+1, 0+1) = min(3, 3, 1) = 1

    --------------------------------------------------------------------------
    RETURN res = 1     (a single 5-coin makes the amount exactly)

    ==========================================================================
    Why the recursion tree is bigger than it needs to be
    ==========================================================================

    Notice solve(1), solve(2) and solve(3) each got fully recomputed more
    than once above (marked "recomputed" - the tree reaches the same
    remaining amount via different coin choices, e.g. 5 -> coin1 -> coin1
    -> coin2 reaches solve(2), and 5 -> coin1 -> coin2 also reaches
    solve(2)). Without memoization every one of those repeated calls
    re-expands its own full subtree. For amt = 11 in the header example
    this blow-up is much larger; that is exactly the O(n^amt) behaviour
    named in the complexity section, and exactly what caching solve(t) in
    an array of size amt+1 would eliminate.

    ==========================================================================
    Trace of coins = [2], amt = 3 -> -1  (unreachable target)
    ==========================================================================

    solve(3): only coin is 2
      coin 2 -> solve(1)
        coin 2 -> solve(-1) = INT_MAX -> skip
        solve(1) = INT_MAX (no coin produced a valid answer)
      ans = solve(1) = INT_MAX -> skipped, not folded into res
      solve(3) = INT_MAX (res was never updated away from INT_MAX)

    minCoins returns -1, since res == INT_MAX.

    This is precisely why the guard `if (ans != INT_MAX)` exists: without
    it, `res = min(res, ans + 1)` would compute `INT_MAX + 1`, which
    overflows a signed int (undefined behaviour, and in practice often
    wraps to a large negative number) - a broken sentinel could then look
    like a valid, absurdly small coin count instead of "impossible".

    ==========================================================================
    Notes
    ==========================================================================

    Why greedy (largest coin first) is not used:
      it is not correct for arbitrary denominations - see coins=[1,3,4],
      amt=6 in the Approach section, where greedy gives 3 coins but the
      optimum is 2. This file's brute-force recursion is correct for ANY
      coin set, which is the whole reason it tries every coin rather than
      just the largest.

    Suggested optimization (not applied, to keep the recursion as written):
      add a memo array of size amt+1 initialized to -1, check/fill it at
      the top/bottom of solve, and every call becomes O(1) after the first
      time its argument is seen - turning the O(n^amt) blow-up into the
      standard O(amt * n) coin-change DP.
*/
