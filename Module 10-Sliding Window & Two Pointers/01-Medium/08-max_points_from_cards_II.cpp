#include <bits/stdc++.h>
using namespace std;

// Q: There are several cards in a row, arr[i] points on the i-th card. In
//    one step you take exactly one card from EITHER the beginning or the
//    end of the row. You must take exactly k cards. Return the maximum
//    total score you can obtain.
//
// Example:
// arr = [1,2,3,4,5,6,1], k = 3 -> 12  (take the last three: 5 + 6 + 1)
// arr = [2,2,2],         k = 2 ->  4  (any two cards)
// arr = [9,7,7,9,7,7,9], k = 7 -> 55  (forced to take every card)
//
// NOTE: this is solution II of two. See 08-max_points_from_cards.cpp for
//       the O(1)-space version that slides one running total around the
//       ends instead of building arrays.

/*
    Approach II: Prefix and suffix sums, then read off all k + 1 splits

    - The order of the picks never matters, only WHICH cards end up taken.
      Whatever you do, you finish with some i cards from the front and
      k - i cards from the back, for some i in 0..k. So the answer is

          max over i in 0..k of  (sum of first i)  +  (sum of last k - i)

      which is only k + 1 candidates to compare.
    - Both halves of that expression are cheap if precomputed once:
        pre[i] = sum of the FIRST i cards   (pre[0] = 0)
        suf[j] = sum of the LAST  j cards   (suf[0] = 0)
      Then candidate i is just pre[i] + suf[k-i], an O(1) lookup, and the
      whole answer is one loop over i.
    - Both arrays are sized n + 1 and use a "count, not index" convention:
      the subscript is HOW MANY cards, not which one. That is what makes
      pre[0] = suf[0] = 0 meaningful (take none from that side) and lets
      i run cleanly over the full range 0..k with no special cases.
    - suf is built the natural way first - suf[i] = sum of arr[i..n-1],
      indexed by START POSITION - and then REVERSED to convert it to the
      count convention. After the reverse, entry j holds what was at
      position n - j, i.e. the sum of arr[n-j..n-1] = the last j cards,
      and suf[0] picks up the old suf[n] = 0. The reverse is the whole
      trick; without it the two arrays would not be indexed compatibly.
    - Compared with solution I this trades O(n) time and space for a
      formula that is easier to state and to extend (for instance to
      "best i from the front and j from the back" style variants).

    Algorithm Steps
    ----------------
    1. Allocate pre and suf of size n + 1, both zero-filled.
    2. Forward pass: running sum, pre[i+1] = sum of arr[0..i].
       -> pre[i] = sum of the first i cards.
    3. Backward pass: running sum, suf[i] = sum of arr[i..n-1].
    4. Reverse suf, converting it to "sum of the last j cards" at index j.
    5. For i from 0 to k: res = max(res, pre[i] + suf[k-i]).
    6. Return res.

    Time Complexity: O(n) - two passes to build the arrays, one reverse,
                      and a final loop of k + 1 lookups.
    Space Complexity: O(n) - the two auxiliary arrays of size n + 1.
*/
int maxScore(vector<int>& arr, int k) {
  int res = 0;
  int n = arr.size();
  int sum = 0;
  vector<int> pre(n + 1, 0), suf(n + 1, 0);

  for (int i = 0; i < n; ++i) {
    sum += arr[i];
    pre[i + 1] = sum;              // pre[i] = sum of the FIRST i cards
  }

  sum = 0;
  for (int i = n - 1; i >= 0; --i) {
    sum += arr[i];
    suf[i] = sum;                  // suf[i] = sum of arr[i..n-1], by position
  }

  reverse(begin(suf), end(suf));   // re-index: suf[j] = sum of the LAST j

  for (int i = 0; i <= k; ++i) {
    res = max(res, (pre[i] + suf[k - i]));   // i from front, k-i from back
  }

  return res;
}

int main() {
  vector<pair<vector<int>, int>> tests = {{{1, 2, 3, 4, 5, 6, 1}, 3},
                                          {{2, 2, 2}, 2},
                                          {{9, 7, 7, 9, 7, 7, 9}, 7}};

  for (auto& t : tests) {
    cout << "Input:  arr = [";
    for (size_t i = 0; i < t.first.size(); ++i) {
      cout << t.first[i] << (i + 1 < t.first.size() ? "," : "");
    }
    cout << "], k = " << t.second << endl;
    cout << "Output: " << maxScore(t.first, t.second) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = [1,2,3,4,5,6,1], k = 3   (n = 7, answer = 12)
    ==========================================================================

      index:  0  1  2  3  4  5  6
      value:  1  2  3  4  5  6  1        total = 28

    --------------------------------------------------------------------------
    STEP 1: forward pass -> pre[i] = sum of the FIRST i cards
    --------------------------------------------------------------------------

      i = 0: sum = 1   -> pre[1] = 1        (first 1 card:  1)
      i = 1: sum = 3   -> pre[2] = 3        (first 2 cards: 1+2)
      i = 2: sum = 6   -> pre[3] = 6        (first 3 cards: 1+2+3)
      i = 3: sum = 10  -> pre[4] = 10
      i = 4: sum = 15  -> pre[5] = 15
      i = 5: sum = 21  -> pre[6] = 21
      i = 6: sum = 22  -> pre[7] = 22       (all 7 cards)

      pre =  [ 0,  1,  3,  6, 10, 15, 21, 22 ]
      index:   0   1   2   3   4   5   6   7
               ^ pre[0] = 0: take nothing from the front

    --------------------------------------------------------------------------
    STEP 2: backward pass -> suf[i] = sum of arr[i..n-1], BY POSITION
    --------------------------------------------------------------------------

      i = 6: sum = 1   -> suf[6] = 1        (arr[6..6])
      i = 5: sum = 7   -> suf[5] = 7        (arr[5..6] = 6+1)
      i = 4: sum = 12  -> suf[4] = 12       (arr[4..6] = 5+6+1)
      i = 3: sum = 16  -> suf[3] = 16
      i = 2: sum = 19  -> suf[2] = 19
      i = 1: sum = 21  -> suf[1] = 21
      i = 0: sum = 22  -> suf[0] = 22

      suf =  [22, 21, 19, 16, 12,  7,  1,  0 ]
      index:   0   1   2   3   4   5   6   7
                                           ^ suf[7] was never written: 0

      Right now this array is indexed by START POSITION, which does NOT
      line up with pre - pre is indexed by COUNT. That is what step 3 fixes.

    --------------------------------------------------------------------------
    STEP 3: reverse(suf) -> suf[j] = sum of the LAST j cards
    --------------------------------------------------------------------------

      before:  [22, 21, 19, 16, 12,  7,  1,  0 ]
      after:   [ 0,  1,  7, 12, 16, 19, 21, 22 ]
      index:     0   1   2   3   4   5   6   7

      check the new meaning:
        suf[0] = 0    take nothing from the back      (was old suf[7])
        suf[1] = 1    last 1 card:  arr[6]     = 1    (was old suf[6])
        suf[2] = 7    last 2 cards: 6 + 1      = 7    (was old suf[5])
        suf[3] = 12   last 3 cards: 5 + 6 + 1  = 12   (was old suf[4])
        ...
      in general new suf[j] = old suf[n - j], and old suf[n-j] is the sum
      of arr[n-j..n-1] - exactly the last j cards. Both arrays now speak
      the same language: "how many cards from this side".

    --------------------------------------------------------------------------
    STEP 4: compare all k + 1 = 4 splits
    --------------------------------------------------------------------------

      i = 0:  pre[0] + suf[3] =  0 + 12 = 12   take [5,6,1] from the back
              res = max(0, 12) = 12                          <-- best
      i = 1:  pre[1] + suf[2] =  1 +  7 =  8   take [1] and [6,1]
              res = max(12, 8) = 12
      i = 2:  pre[2] + suf[1] =  3 +  1 =  4   take [1,2] and [1]
              res = max(12, 4) = 12
      i = 3:  pre[3] + suf[0] =  6 +  0 =  6   take [1,2,3] from the front
              res = max(12, 6) = 12

    --------------------------------------------------------------------------
    RETURN res = 12     (take the three cards from the end: 5 + 6 + 1)

    ==========================================================================
    Summary table
    ==========================================================================

    | i | pre[i] | k-i | suf[k-i] | total | cards taken            |
    |---|--------|-----|----------|-------|------------------------|
    | 0 |    0   |  3  |    12    |  12   | back  [5,6,1]          |
    | 1 |    1   |  2  |     7    |   8   | [1] + [6,1]            |
    | 2 |    3   |  1  |     1    |   4   | [1,2] + [1]            |
    | 3 |    6   |  0  |     0    |   6   | front [1,2,3]          |

    Visual of the four splits (| marks what is left behind):

      i = 0:   1 2 3 4 |5 6 1      12
      i = 1:  [1] 2 3 4 5 |6 1|     8
      i = 2:  [1 2] 3 4 5 6 |1|     4
      i = 3:  [1 2 3] 4 5 6 1       6

    ==========================================================================
    Notes
    ==========================================================================

    Why the arrays are size n + 1:
      the subscript is a COUNT (0 to n inclusive), not an index. Without
      the extra slot there would be nowhere to store "take all n", and the
      i = 0 and i = k ends of the final loop would each need a special
      case.

    Why suf[k-i] is always in range:
      i runs 0..k, so k - i runs k..0, and k <= n by the problem's
      constraints, so every lookup lands inside a length-(n+1) array. The
      same argument covers pre[i].

    An equivalent framing of the whole problem:
      the cards NOT taken always form one contiguous block of width n - k
      in the middle. So maximising the score is the same as MINIMISING the
      sum of a fixed-size sliding window of width n - k, then subtracting
      that from the total. Here 28 - min-window = 28 - 16 = 12, the window
      being arr[0..3] = 1+2+3+4 = 16. Worth knowing, because it turns this
      into a textbook fixed-size-window problem.

    Choosing between the two solutions:
      - II (this file): O(n) time, O(n) space. States the answer as a
        clean formula over precomputed sums, and the two arrays are
        reusable if the problem is extended.
      - I (08-max_points_from_cards.cpp): O(k) time, O(1) space. Keeps a
        single running total and slides it, which is what to write once
        the "i from the front, k - i from the back" insight is clear.
*/
