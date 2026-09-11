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
// NOTE: this is solution I of two. See 08-max_points_from_cards_II.cpp for
//       the prefix/suffix-array version, which is easier to read at the
//       cost of O(n) extra space.

/*
    Approach I: Take the first k cards, then slide the window around the ends

    - The order of the picks never matters, only WHICH cards end up taken.
      Whatever you do, you finish with some i cards from the front and
      k - i cards from the back, for some i in 0..k. So there are only
      k + 1 candidate outcomes to compare, not 2^k orderings.
    - Start from the i = k candidate (all k cards from the front) and walk
      down to i = 0 (all k from the back), changing one card at a time.
      Going from "i front cards" to "i-1 front cards" means giving back
      arr[i-1] (the last front card taken) and picking up one more card
      from the back.
    - The back card gained at that step is arr[n - k + i]. Check it with
      i = k-1 on the first iteration: n - k + (k-1) = n-1, the very last
      card - correct, that is the first one taken from the back. On the
      final iteration i = 0 gives arr[n-k], so the back block has grown to
      the last k cards - correct again.
    - This is a sliding window in disguise: the cards NOT taken always form
      one contiguous block of n - k cards in the middle, and the loop slides
      that block leftwards one position at a time. Equivalently you could
      minimise the sum of a fixed-size window of width n - k; this version
      maximises the taken ends directly, which reads closer to the problem.
    - Only one running total `curr` is maintained, updated in O(1) per step,
      so no prefix arrays are needed - hence O(1) extra space.

    Algorithm Steps
    ----------------
    1. Sum the first k cards into res; set curr = res (the i = k candidate).
    2. For i from k-1 down to 0:
         a. curr -= arr[i]            (give back a front card)
         b. curr += arr[n - k + i]    (take one more card from the back)
         c. res = max(res, curr)      (this is the i-front / (k-i)-back split)
    3. Return res.

    Time Complexity: O(k) - one pass to build the initial sum and one pass
                      of k steps to slide; O(1) work per step. Never O(n).
    Space Complexity: O(1) - two running integers, no auxiliary arrays.
*/
int maxScore(vector<int>& arr, int k) {
  int res = 0;
  int n = arr.size();

  for (int i = 0; i < k; ++i) {      // candidate i = k: all from the front
    res += arr[i];
  }

  int curr = res;
  for (int i = k - 1; i >= 0; --i) {
    curr -= arr[i];                  // give back the last front card taken
    curr += arr[n - k + i];          // take one more card from the back
    res = max(res, curr);            // i front cards + (k - i) back cards
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
      value:  1  2  3  4  5  6  1

    There are exactly k + 1 = 4 candidate splits to compare:

        i = 3 front + 0 back    [1,2,3]            = 6
        i = 2 front + 1 back    [1,2]      + [1]   = 4
        i = 1 front + 2 back    [1]        + [6,1] = 8
        i = 0 front + 3 back    [5,6,1]            = 12   <-- best

    The loop walks exactly down this list, moving one card at a time.

    Tracked state:
      curr  - score of the current split (i front cards + (k-i) back cards)
      res   - the best score seen so far

    --------------------------------------------------------------------------
    SETUP: take the first k = 3 cards
      res = arr[0] + arr[1] + arr[2] = 1 + 2 + 3 = 6
      curr = 6

           [ 1  2  3 ] 4  5  6  1
             ^taken^          front = 3, back = 0,  score 6

    --------------------------------------------------------------------------
    i = 2   (move to: 2 front cards + 1 back card)
      give back    curr -= arr[2] = 3   -> curr = 3      (drop the card '3')
      take back    index n - k + i = 7 - 3 + 2 = 6
                   curr += arr[6] = 1   -> curr = 4      (take the last card)
      compare      res = max(6, 4) = 6      (worse - swapped a 3 for a 1)

           [ 1  2 ] 3  4  5  6 [ 1 ]
             front            back        score 1+2+1 = 4

    --------------------------------------------------------------------------
    i = 1   (move to: 1 front card + 2 back cards)
      give back    curr -= arr[1] = 2   -> curr = 2      (drop the card '2')
      take back    index 7 - 3 + 1 = 5
                   curr += arr[5] = 6   -> curr = 8      (take the card '6')
      compare      res = max(6, 8) = 8                   <-- new best

           [ 1 ] 2  3  4  5 [ 6  1 ]
             front             back      score 1+6+1 = 8

    --------------------------------------------------------------------------
    i = 0   (move to: 0 front cards + 3 back cards)
      give back    curr -= arr[0] = 1   -> curr = 7      (drop the card '1')
      take back    index 7 - 3 + 0 = 4
                   curr += arr[4] = 5   -> curr = 12     (take the card '5')
      compare      res = max(8, 12) = 12                 <-- ANSWER

            1  2  3  4 [ 5  6  1 ]
                          back           score 5+6+1 = 12

      loop ends (i would go to -1)

    --------------------------------------------------------------------------
    RETURN res = 12     (take the three cards from the end: 5 + 6 + 1)

    ==========================================================================
    Summary table
    ==========================================================================

    | i | gives back arr[i] | takes arr[n-k+i] | curr | split          | res |
    |---|-------------------|------------------|------|----------------|-----|
    | - |         -         |        -         |   6  | [1,2,3] | -    |  6  |
    | 2 |     arr[2] = 3    |   arr[6] = 1     |   4  | [1,2]   | [1]  |  6  |
    | 1 |     arr[1] = 2    |   arr[5] = 6     |   8  | [1]     | [6,1]|  8  |
    | 0 |     arr[0] = 1    |   arr[4] = 5     |  12  | -   |[5,6,1]   | 12  |

    Watch the untaken middle block slide left as the loop runs:
        i = 3:  1 2 3 |4 5 6 1|     (nothing untaken on the right yet)
        i = 2:  1 2 |3 4 5 6| 1
        i = 1:  1 |2 3 4 5| 6 1
        i = 0:  |1 2 3 4| 5 6 1
      it is always the same width n - k = 4, which is the real sliding
      window hiding inside this problem.

    ==========================================================================
    Notes
    ==========================================================================

    Why the index n - k + i is right:
      at step i the back block must hold k - i cards, i.e. the cards
      arr[n-(k-i) .. n-1] = arr[n-k+i .. n-1]. Its leftmost element is
      arr[n-k+i], and that is precisely the one newly added at this step,
      since the previous step's block started one position further right.

    Edge case k == n:
      every card must be taken, so the answer is the total sum. The setup
      loop already sums everything; the sliding loop then swaps arr[i] out
      and arr[n-n+i] = arr[i] back in each round, leaving curr unchanged.
      The answer is correct, just computed via a no-op pass. For
      arr = [9,7,7,9,7,7,9], k = 7 that gives 55.

    Edge case k == 0:
      the setup loop never runs and the sliding loop never runs, so res
      stays 0 - taking no cards scores nothing. Correct, and importantly
      no out-of-range index is ever formed.

    Comparison with solution II:
      II precomputes prefix and suffix sums and then reads off all k + 1
      candidates directly. It is arguably clearer and generalises better,
      but costs O(n) time and O(n) space. This version is O(k) time and
      O(1) space, and is the one to reach for in an interview once the
      "i from the front, k - i from the back" insight is stated.
*/
