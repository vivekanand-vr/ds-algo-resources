#include <bits/stdc++.h>
using namespace std;

// Q: A lemonade costs $5. Customers arrive one at a time in the order
//    given by arr and each pays with a $5, $10, or $20 bill. You must give
//    each customer $5 in change (the price of one lemonade), and you start
//    with no change on hand. Return true if you can give correct change to
//    every customer in order, false otherwise.
//
// Example:
// arr = [5,5,5,10,20]    -> true   (change: -, -, -, use a 5, use a 10+5)
// arr = [5,5,10,10,20]   -> false  (by the last customer there is only one
//                                   $5 left, but $20 needs a $5 + a $10)
// arr = [5,5,5,20,20]    -> false  (the first $20 spends all three $5s;
//                                   the second $20 then has no change left)

/*
    Approach: Greedy, always break the largest bill you can afford to

    - This has to be greedy and in ORDER, because customers cannot be
      reordered - the decision for each customer is made immediately and
      cannot be undone, using only the $5 and $10 bills collected so far
      ($20 bills are never given as change back to anyone, so they never
      need to be tracked as spendable).
    - $5 bill: no change is owed, just bank it - c5++.
    - $10 bill: change owed is exactly one $5. If a $5 is available, use
      it (c5--) and bank the $10 (c10++, since a $10 CAN be handed back
      out later as part of $15 change). Otherwise, fail immediately -
      there is no other way to make $5 in change.
    - $20 bill: change owed is $15, which can be made only two ways:
      a $10 + a $5, or three $5s. The greedy choice is to prefer the
      $10 + $5 combination FIRST whenever both are available, saving $5
      bills for later - a $5 bill is more "flexible" change than a $10
      (it is the only way to break a $10 bill, and one of two ways to
      break a $20), so hoarding $5s keeps more future options open. Only
      when a $10 is unavailable does it fall back to spending three $5s.
    - Any prefix that fails the check makes the whole sequence false and
      the function returns immediately - there is no way to "recover"
      once change cannot be made for the customer currently at the
      counter.

    Algorithm Steps
    ----------------
    1. Initialize c5 = 0, c10 = 0 (bills currently on hand).
    2. For each bill in arr, in order:
         a. If bill == 5: c5++.
         b. Else if bill == 10:
              - if c5 > 0: c10++, c5-- (break a $5 for change)
              - else: return false
         c. Else (bill == 20):
              - if c5 > 0 && c10 > 0: c5--, c10-- ($10 + $5 change)
              - else if c5 >= 3: c5 -= 3 (three $5s change)
              - else: return false
    3. All customers served -> return true.

    Time Complexity: O(n) - one pass over arr, O(1) work per customer.
    Space Complexity: O(1) - only two running counters.
*/
bool lemonadeChange(vector<int>& arr) {
  int n = arr.size(), c5 = 0, c10 = 0;

  for (int i : arr) {
    if (i == 5)
      c5++;                    // no change owed, bank the bill
    else if (i == 10) {
      if (c5 > 0) {
        c10++;                 // bank the $10 for possible future change
        c5--;                  // hand back one $5 as change
      } else {
        return false;          // no $5 available to make change
      }
    } else {                   // i == 20
      if (c5 > 0 && c10 > 0) {
        c5--;
        c10--;                 // prefer $10 + $5 - keeps more $5s in hand
      } else if (c5 >= 3) {
        c5 -= 3;                // fall back to three $5s
      } else {
        return false;          // neither combination is available
      }
    }
  }

  return true;
}

int main() {
  vector<vector<int>> tests = {
      {5, 5, 5, 10, 20}, {5, 5, 10, 10, 20}, {5, 5, 5, 20, 20}};

  for (auto& arr : tests) {
    cout << "Input:  arr = [";
    for (size_t i = 0; i < arr.size(); ++i) {
      cout << arr[i] << (i + 1 < arr.size() ? "," : "");
    }
    cout << "]" << endl;
    cout << "Output: " << (lemonadeChange(arr) ? "true" : "false") << endl
         << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = [5,5,10,10,20]   (n = 5, answer false)
    ==========================================================================

    Tracked state:
      c5, c10  - how many $5 and $10 bills are currently on hand

    Initial state: c5 = 0, c10 = 0

    --------------------------------------------------------------------------
    customer 0, bill = 5
      action     bank it: c5++
      state      c5 = 1, c10 = 0

    --------------------------------------------------------------------------
    customer 1, bill = 5
      action     bank it: c5++
      state      c5 = 2, c10 = 0

    --------------------------------------------------------------------------
    customer 2, bill = 10
      owed       $5 in change
      available  c5 = 2 > 0 -> use one $5
      action     c10++, c5--
      state      c5 = 1, c10 = 1

    --------------------------------------------------------------------------
    customer 3, bill = 10
      owed       $5 in change
      available  c5 = 1 > 0 -> use it
      action     c10++, c5--
      state      c5 = 0, c10 = 2

                   after 4 customers: c5 = 0, c10 = 2
                   (both $5 bills have been spent as change already)

    --------------------------------------------------------------------------
    customer 4, bill = 20
      owed       $15 in change
      option 1   $10 + $5:  c5 > 0 && c10 > 0?  c5 = 0 -> NO
      option 2   three $5s: c5 >= 3?  c5 = 0 -> NO
      neither combination is available -> RETURN false

    --------------------------------------------------------------------------
    RETURN false

      The problem: by the time the $20 arrives, both original $5 bills
      were already handed out as change for the two $10s, leaving c10 = 2
      but c5 = 0 - and a $10 by itself can never make $15 in change; it
      always needs a $5 alongside it.

    ==========================================================================
    Summary table
    ==========================================================================

    | customer | bill | owed | c5 before | c10 before | action        | c5 after | c10 after |
    |----------|------|------|-----------|------------|---------------|----------|-----------|
    |    0     |  5   |  -   |     0     |     0      | bank          |    1     |     0     |
    |    1     |  5   |  -   |     1     |     0      | bank          |    2     |     0     |
    |    2     |  10  | $5   |     2     |     0      | use one $5    |    1     |     1     |
    |    3     |  10  | $5   |     1     |     1      | use one $5    |    0     |     2     |
    |    4     |  20  | $15  |     0     |     2      | FAIL          |    -     |     -     |

    ==========================================================================
    Contrast: arr = [5,5,5,10,20] -> true, to show the greedy succeeding
    ==========================================================================

      customer 0, bill 5:  bank         -> c5=1, c10=0
      customer 1, bill 5:  bank         -> c5=2, c10=0
      customer 2, bill 5:  bank         -> c5=3, c10=0
      customer 3, bill 10: use a $5     -> c5=2, c10=1
      customer 4, bill 20: c5>0 && c10>0 -> use $10+$5 -> c5=1, c10=0

      all customers served -> return true

      Here there were still $5 bills left when the $20 arrived, because
      three were collected before any change had to be made - exactly the
      margin that was missing in the failing example above.

    ==========================================================================
    Notes
    ==========================================================================

    Why $20 bills are never counted:
      change is only ever handed back in $5 and $10 denominations (no
      customer is due a $20 in change, since the maximum change owed is
      $15 for a $20 payment). So a running count of $20s would never be
      read anywhere and is correctly omitted.

    Why preferring $10+$5 over three $5s is the right greedy choice:
      a $5 bill can settle change for EITHER a $10 payment or a $20
      payment, while a $10 bill can only ever help with a $20 payment (and
      only paired with a $5, never alone). $5s are strictly more useful to
      have in reserve, so spending a $10 first - before dipping into three
      $5s - is what preserves the most future flexibility. Swapping the
      order (checking `c5 >= 3` before the `c10 > 0` combination) can fail
      sequences that the code as written passes: for arr =
      [5,5,5,5,10,20,10], after the $10 payment the state is c5=3, c10=1.
      Preferring $10+$5 for the $20 leaves c5=2, c10=0, and the trailing
      $10 payment succeeds (c5 > 0). Preferring three $5s instead empties
      c5 to 0 while c10 stays at 1, so the trailing $10 payment then
      fails outright - a lone $10 in hand is never valid change for a
      $10 bill.

    Failing fast is required, not just convenient:
      once a customer cannot be given correct change, every later customer
      is irrelevant - the run has already failed at that point in time.
      Returning false immediately (rather than continuing and returning
      false at the end) matches the problem's requirement that EVERY
      customer in sequence receives correct change, with no way to go back
      and redo an earlier transaction.
*/
