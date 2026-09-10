#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find all leader elements in the array, i.e., elements greater than or equal to
// all elements to their right

/*
    Approach: Right-to-Left Scan with Running Maximum
    - Traverse the array from right to left while tracking the maximum seen so far
    - An element is a leader if it is greater than or equal to this running maximum
    - Collect leaders while scanning, then reverse to restore original order

    Algorithm Steps
    ----------------
    1. Initialize h with the last element of the array
    2. Traverse from the last index down to the first
    3. Update h = max(h, arr[i]); if arr[i] >= h, add arr[i] to the result
    4. Reverse the result to restore left-to-right order and return it

    Time Complexity: O(n) - single pass through array + reverse
    Space Complexity: O(n) - to store leaders
*/
vector<int> leaders(vector<int>& arr) {
    int n = arr.size();
    int h = arr[n - 1];      // keep track of maximum seen so far (from right side)
    vector<int> res;

    // Traverse from right to left
    for (int i = n - 1; i >= 0; --i) {
        // A leader is an element which is greater than or equal to 
        // all elements to its right.
        h = max(h, arr[i]);       // update running maximum
        if (arr[i] >= h) {
            res.push_back(arr[i]);  // if current element is leader, store it
        }
    }

    // We collected leaders from right to left,
    // so reverse to restore original left-to-right order
    reverse(begin(res), end(res));
    return res;
}


int main() {
    vector<int> arr = {10, 22, 12, 3, 0, 6};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    vector<int> res = leaders(arr);

    cout << "Leaders: ";
    for (int x : res) cout << x << " ";
    cout << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {10, 22, 12, 3, 0, 6}   (n = 6, answer = {22, 12, 6})
    ==========================================================================

      index:    0    1    2    3    4    5
      value:   10   22   12    3    0    6

      scan direction:  <-------------------- right to left

    Tracked state:
      h    - running maximum over every element ALREADY visited, i.e. the
             largest value at or to the right of the current index
      res  - leaders collected so far, in right-to-left order
      i    - index being examined

    Initial state: h = arr[n-1] = arr[5] = 6, res = {}

    Careful with the order of the two statements inside the loop: h is
    updated BEFORE the comparison, so h already includes arr[i] itself.
    That makes the test arr[i] >= h true exactly when arr[i] is at least as
    large as everything STRICTLY to its right - which is the definition of a
    leader. Each iteration below is split into those two steps.

    --------------------------------------------------------------------------
    i = 5, arr[i] = 6      (nothing to its right - always a leader)
      step 1: update  h = max(6, 6) = 6
      step 2: compare 6 >= 6  -> true
      collect         res = {6}

    --------------------------------------------------------------------------
    i = 4, arr[i] = 0
      step 1: update  h = max(6, 0) = 6      (h unchanged)
      step 2: compare 0 >= 6  -> false
      collect         nothing; 6 sits to the right of 0

    --------------------------------------------------------------------------
    i = 3, arr[i] = 3
      step 1: update  h = max(6, 3) = 6      (h unchanged)
      step 2: compare 3 >= 6  -> false
      collect         nothing

    --------------------------------------------------------------------------
    i = 2, arr[i] = 12
      step 1: update  h = max(6, 12) = 12    <-- h RAISED by arr[i] itself
      step 2: compare 12 >= 12 -> true
                      the comparison is against the value just written, so it
                      is really "12 >= old h = 6", which is what we want
      collect         res = {6, 12}

    --------------------------------------------------------------------------
    i = 1, arr[i] = 22
      step 1: update  h = max(12, 22) = 22   <-- h raised again
      step 2: compare 22 >= 22 -> true
      collect         res = {6, 12, 22}

    --------------------------------------------------------------------------
    i = 0, arr[i] = 10
      step 1: update  h = max(22, 10) = 22   (h unchanged)
      step 2: compare 10 >= 22 -> false
      collect         nothing; 22 sits to its right
      loop ends       i -> -1

    --------------------------------------------------------------------------
    Reverse pass

      res was built right-to-left:   {6, 12, 22}
      reverse(begin, end)        ->  {22, 12, 6}

           10   22   12    3    0    6
                 ^    ^              ^
                 L    L              L        L = leader

    RETURN res = {22, 12, 6}

    ==========================================================================
    Summary table
    ==========================================================================

    | i | arr[i] | h before | h after | arr[i] >= h | leader? | res         |
    |---|--------|----------|---------|-------------|---------|-------------|
    | 5 |   6    |    6     |    6    |    true     |   yes   | {6}         |
    | 4 |   0    |    6     |    6    |    false    |   no    | {6}         |
    | 3 |   3    |    6     |    6    |    false    |   no    | {6}         |
    | 2 |  12    |    6     |   12    |    true     |   yes   | {6,12}      |
    | 1 |  22    |   12     |   22    |    true     |   yes   | {6,12,22}   |
    | 0 |  10    |   22     |   22    |    false    |   no    | {6,12,22}   |

    Step count behind the O(n) claim:
      6 max() calls and 6 comparisons in the scan, then one reverse of a
      3-element vector - 6 + 6 + 3 elementary steps for n = 6. The naive
      "for each i, look at every j > i" version would cost about n^2 / 2 = 18
      comparisons here and grows quadratically.

    Invariant that makes it correct:
      at the top of the iteration for index i, h equals max(arr[i+1..n-1]).
      Because the loop walks strictly right to left, that suffix maximum is
      always available in O(1), which is exactly the information a leader
      test needs. The first iteration is the degenerate case: the suffix is
      empty, h was seeded with arr[n-1] itself, and 6 >= 6 correctly reports
      the last element as a leader.

    Note on >= versus >:
      the test uses >=, so an element that merely TIES the maximum to its
      right still counts as a leader (this is the usual statement of the
      problem). With arr = {5, 5} the result would be {5, 5}, not just the
      trailing 5.
*/
