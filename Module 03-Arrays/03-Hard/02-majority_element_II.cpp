#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find all elements in the array that appear more than n/3 times

/*
    Approach: Boyer-Moore Voting (Extended to 2 Candidates)
    - At most 2 elements can appear more than n/3 times
    - Phase 1: maintain 2 candidates with counters, similar to standard
      Boyer-Moore voting, to narrow down to at most 2 potential candidates
    - Phase 2: recount the actual occurrences of both candidates in the array
    - Phase 3: keep only the candidates whose count truly exceeds n/3

    Algorithm Steps
    ----------------
    1. Traverse nums, updating cand1/cand2 and their counters using voting rules
    2. Reset counters and recount actual occurrences of cand1 and cand2
    3. Add cand1 to result if its count > n/3
    4. Add cand2 to result if its count > n/3
    5. Return the result

    Time Complexity: O(n) - two linear passes over the array
    Space Complexity: O(1) - only a few counters and candidate variables
*/
vector<int> majorityElement(vector<int>& nums) {
    int n = nums.size();

    // Step 1: Find potential candidates (at most 2)
    int cand1 = 0, cand2 = 0, cnt1 = 0, cnt2 = 0;

    for (int num : nums) {
        if (num == cand1) {
            cnt1++;
        } else if (num == cand2) {
            cnt2++;
        } else if (cnt1 == 0) {
            cand1 = num;
            cnt1 = 1;
        } else if (cnt2 == 0) {
            cand2 = num;
            cnt2 = 1;
        } else {
            cnt1--;
            cnt2--;
        }
    }

    // Step 2: Verify counts (since step 1 only gives candidates)
    cnt1 = cnt2 = 0;
    for (int num : nums) {
        if (num == cand1) cnt1++;
        else if (num == cand2) cnt2++;
    }

    // Step 3: Collect valid results
    vector<int> res;
    if (cnt1 > n / 3) res.push_back(cand1);
    if (cnt2 > n / 3) res.push_back(cand2);

    return res;
}

int main() {
    vector<int> nums = {3, 2, 3};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    vector<int> result = majorityElement(nums);

    cout << "Elements appearing more than n/3 times: ";
    for (int x : result) cout << x << " ";
    cout << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: nums = [3, 2, 3]   (n = 3, n/3 = 1, answer = [3])
    ==========================================================================

      index:   0   1   2
      value:   3   2   3

    Tracked state:
      cand1, cnt1  - first  candidate and its vote counter
      cand2, cnt2  - second candidate and its vote counter
      (both candidates start at the literal value 0, NOT at a sentinel -
       that is why the verification pass in phase 2 is mandatory)

    The else-if chain is tried STRICTLY in this order, and the first match
    wins - no later branch is even evaluated:
      1. num == cand1   -> cnt1++            (reinforce candidate 1)
      2. num == cand2   -> cnt2++            (reinforce candidate 2)
      3. cnt1 == 0      -> cand1 = num, cnt1 = 1   (slot 1 is free, claim it)
      4. cnt2 == 0      -> cand2 = num, cnt2 = 1   (slot 2 is free, claim it)
      5. else           -> cnt1--, cnt2--    (a third distinct value cancels
                                              one vote from EACH candidate)

    ==========================================================================
    PHASE 1 - voting
    ==========================================================================

    Initial state: cand1 = 0, cnt1 = 0, cand2 = 0, cnt2 = 0

    --------------------------------------------------------------------------
    num = 3   (index 0)
      branch 1?    3 == cand1 (0)?  no
      branch 2?    3 == cand2 (0)?  no
      branch 3?    cnt1 == 0?       YES -> claim slot 1
      vote         cand1 = 3, cnt1 = 1
      state        cand1 = 3, cnt1 = 1  |  cand2 = 0, cnt2 = 0

    --------------------------------------------------------------------------
    num = 2   (index 1)
      branch 1?    2 == cand1 (3)?  no
      branch 2?    2 == cand2 (0)?  no
      branch 3?    cnt1 == 0?       no  (cnt1 is 1)
      branch 4?    cnt2 == 0?       YES -> claim slot 2
      vote         cand2 = 2, cnt2 = 1
      state        cand1 = 3, cnt1 = 1  |  cand2 = 2, cnt2 = 1

    --------------------------------------------------------------------------
    num = 3   (index 2)
      branch 1?    3 == cand1 (3)?  YES -> stop here, reinforce
      vote         cnt1++ -> 2
      state        cand1 = 3, cnt1 = 2  |  cand2 = 2, cnt2 = 1
      note         branches 2-5 were never evaluated for this element

    --------------------------------------------------------------------------
    Phase 1 result: cand1 = 3, cand2 = 2
      cnt1 = 2 and cnt2 = 1 are NOT real frequencies - they are surviving
      vote balances. They get thrown away next.

    ==========================================================================
    PHASE 2 - verification (a SECOND, independent pass; cnt1 = cnt2 = 0)
    ==========================================================================

    This pass also uses if / else-if, so an element that equals cand1 is
    never also counted for cand2, and an element equal to neither is simply
    ignored.

      num = 3 (index 0):  == cand1 (3) -> cnt1 = 1           | cnt2 = 0
      num = 2 (index 1):  != cand1, == cand2 (2) -> cnt2 = 1  | cnt1 = 1
      num = 3 (index 2):  == cand1 (3) -> cnt1 = 2           | cnt2 = 1

    True counts: cnt1 = 2 (for value 3), cnt2 = 1 (for value 2)

    ==========================================================================
    PHASE 3 - filtering against n/3
    ==========================================================================

      threshold    n / 3 = 3 / 3 = 1    (integer division)
      cand1        cnt1 = 2 > 1?  YES -> res.push_back(3)   -> res = [3]
      cand2        cnt2 = 1 > 1?  no   -> 2 is REJECTED, it occurs once

    --------------------------------------------------------------------------
    RETURN res = [3]

    ==========================================================================
    Summary table - phase 1 voting on [3, 2, 3]
    ==========================================================================

    | i | num | branch taken        | cand1 | cnt1 | cand2 | cnt2 |
    |---|-----|---------------------|-------|------|-------|------|
    | - |  -  | (initial)           |   0   |  0   |   0   |  0   |
    | 0 |  3  | 3: cnt1 == 0        |   3   |  1   |   0   |  0   |
    | 1 |  2  | 4: cnt2 == 0        |   3   |  1   |   2   |  1   |
    | 2 |  3  | 1: num == cand1     |   3   |  2   |   2   |  1   |

    | i | num | phase 2 match | cnt1 | cnt2 |
    |---|-----|---------------|------|------|
    | 0 |  3  | cand1         |  1   |  0   |
    | 1 |  2  | cand2         |  1   |  1   |
    | 2 |  3  | cand1         |  2   |  1   |

    ==========================================================================
    EXTENDED RUN: nums = [1, 1, 1, 3, 3, 2, 2, 2]
                  (n = 8, n/3 = 2, answer = [1, 2])
    ==========================================================================

    The three-element run above never reaches branch 5, and never has to
    reassign a candidate whose count has collapsed. This longer input does
    both, so it is worth walking as well.

      index:   0   1   2   3   4   5   6   7
      value:   1   1   1   3   3   2   2   2

    Phase 1, starting again from cand1 = cand2 = 0, cnt1 = cnt2 = 0:

    --------------------------------------------------------------------------
    num = 1 (i=0)  branch 3 (cnt1 == 0)  -> cand1 = 1, cnt1 = 1 | 0, 0
    num = 1 (i=1)  branch 1              -> cnt1 = 2            | 0, 0
    num = 1 (i=2)  branch 1              -> cnt1 = 3            | 0, 0
    --------------------------------------------------------------------------
    num = 3 (i=3)  1? 3 == 1 no.  2? 3 == cand2 (0) no.
                   3? cnt1 == 0 no (3).  4? cnt2 == 0 YES
                   -> cand2 = 3, cnt2 = 1        | cand1 = 1, cnt1 = 3
    num = 3 (i=4)  branch 2              -> cnt2 = 2 | cand1 = 1, cnt1 = 3
    --------------------------------------------------------------------------
    num = 2 (i=5)  <-- FIRST element matching neither candidate while BOTH
                       counters are non-zero, so branch 5 finally fires
      branch 1?    2 == 1?  no
      branch 2?    2 == 3?  no
      branch 3?    cnt1 == 0?  no (3)
      branch 4?    cnt2 == 0?  no (2)
      else         cnt1-- -> 2,  cnt2-- -> 1     (one vote cancelled EACH)
      state        cand1 = 1, cnt1 = 2  |  cand2 = 3, cnt2 = 1

    num = 2 (i=6)  branch 5 again
      else         cnt1-- -> 1,  cnt2-- -> 0
      state        cand1 = 1, cnt1 = 1  |  cand2 = 3, cnt2 = 0
                   note cand2 is still the STALE value 3 - the slot is free
                   but the variable has not been cleared

    num = 2 (i=7)  1? no.  2? 2 == 3 no.  3? cnt1 == 0 no (1).
                   4? cnt2 == 0 YES -> the stale slot is overwritten
      vote         cand2 = 2, cnt2 = 1
      state        cand1 = 1, cnt1 = 1  |  cand2 = 2, cnt2 = 1

    Phase 1 result: cand1 = 1, cand2 = 2   (the value 3 has been evicted)

    Phase 2 recount over [1,1,1,3,3,2,2,2]:
      1,1,1  -> cnt1 = 3
      3,3    -> matches neither cand1 (1) nor cand2 (2), ignored entirely
      2,2,2  -> cnt2 = 3
    Phase 3: 3 > 8/3 = 2 for both -> res = [1, 2]

    | i | num | branch | cand1 | cnt1 | cand2 | cnt2 |
    |---|-----|--------|-------|------|-------|------|
    | 0 |  1  |   3    |   1   |  1   |   0   |  0   |
    | 1 |  1  |   1    |   1   |  2   |   0   |  0   |
    | 2 |  1  |   1    |   1   |  3   |   0   |  0   |
    | 3 |  3  |   4    |   1   |  3   |   3   |  1   |
    | 4 |  3  |   2    |   1   |  3   |   3   |  2   |
    | 5 |  2  |   5    |   1   |  2   |   3   |  1   |
    | 6 |  2  |   5    |   1   |  1   |   3   |  0   |
    | 7 |  2  |   4    |   1   |  1   |   2   |  1   |

    Why the chain order matters:
      branches 1 and 2 must precede 3 and 4. If cnt1 == 0 were tested first,
      then at i = 6 above (where cnt2 has just hit 0) the next matching
      element could claim a free slot for a value that is ALREADY the other
      candidate, leaving cand1 == cand2, double-counting one value in phase
      2 while a genuine second majority element goes unrepresented.

    Why phase 2 cannot be skipped:
      phase 1 only guarantees that IF a value occurs more than n/3 times it
      is one of the two survivors - not the converse. On [3,2,3] it handed
      back 2 as cand2 with a leftover balance of 1, and only the recount
      revealed that 2 occurs just once. The counters at the end of phase 1
      are vote balances, never frequencies.

    The one initialization subtlety:
      cand1 and cand2 both start at 0, a real possible array value rather
      than an impossible sentinel. So branch 1 or 2 can match a genuine 0
      in the input while its counter is still 0, and cnt1/cnt2 can be
      incremented for a candidate that was never explicitly claimed. That
      never produces a wrong ANSWER here, because phase 2 recounts from
      scratch and phase 3 applies the real threshold - but it does mean the
      phase 1 counters can look surprising on inputs containing zeros.
*/
