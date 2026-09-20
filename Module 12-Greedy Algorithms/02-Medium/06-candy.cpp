#include <bits/stdc++.h>
using namespace std;

// Q: There are n children standing in a line, each with a rating arr[i].
//    You must distribute candies to them subject to two rules: every child
//    gets at least one candy, and any child with a higher rating than an
//    adjacent (immediate left or right) neighbor must receive more candies
//    than that neighbor. Return the minimum total number of candies needed.
//
// Example:
// arr = [1,0,2]  -> 5   (candies = [2,1,2]; child 0's rating 1 > child 1's
//                        rating 0, so child 0 needs more than child 1;
//                        child 2's rating 2 > child 1's rating 0, so child
//                        2 needs more than child 1)
// arr = [1,2,2]  -> 4   (candies = [1,2,1]; child 1's rating 2 > child 0's
//                        rating 1, so child 1 needs more than child 0;
//                        child 1 and child 2 have equal ratings, so
//                        neither is required to have more than the other)

/*
    Approach: Two-pass greedy (left-to-right, then right-to-left)

    - Every child starts with exactly 1 candy, the minimum allowed.
    - Left-to-right pass: walk from the second child onward. Whenever a
      child's rating is higher than the child to its left, it must get
      more candy than that left neighbor, so bump it to v[i-1] + 1. This
      pass alone correctly satisfies every "higher than left neighbor"
      constraint.
    - Right-to-left pass: walk from the second-to-last child backward.
      Whenever a child's rating is higher than the child to its right, it
      must get more candy than that right neighbor. The guard
      `v[i] <= v[i+1]` is essential here: it only raises v[i] to
      v[i+1] + 1 if the current value isn't already big enough, so a value
      already correctly set by the left-to-right pass is never lowered or
      needlessly disturbed - each child ends up with
      max(left-pass requirement, right-pass requirement) candies.
    - Summing the final per-child candy counts gives the minimum total
      that satisfies both directions of the constraint simultaneously.

    Algorithm Steps
    ----------------
    1. Initialize a candy array v of size n, all entries set to 1.
    2. Left-to-right: for i from 1 to n-1, if arr[i] > arr[i-1] and
       v[i] <= v[i-1], set v[i] = v[i-1] + 1.
    3. Right-to-left: for i from n-2 down to 0, if arr[i] > arr[i+1] and
       v[i] <= v[i+1], set v[i] = v[i+1] + 1.
    4. Return the sum of all entries in v.

    Time Complexity: O(n) - two linear passes over the ratings array.
    Space Complexity: O(n) - the candy array v holds one entry per child.
*/
int candy(vector<int>& arr) {
  int n = arr.size();
  vector<int> v(n, 1);

  for(int i=1; i<n; ++i){
    if(arr[i] > arr[i - 1] and v[i] <= v[i-1]) v[i] = v[i-1] + 1;
  }
  for(int i=n-2; i>=0; --i){
    if(arr[i] > arr[i + 1] and v[i] <= v[i + 1]) v[i] = v[i+1] + 1;
  }

  return accumulate(begin(v), end(v), 0);
}

int main() {
  struct Test {
    vector<int> arr;
  };
  vector<Test> tests = {
    {{1, 0, 2}},
    {{1, 2, 2}},
    {{1, 2, 3, 4, 5}},
    {{5, 4, 3, 2, 1}},
    {{1, 3, 2, 2, 1}},
    {{1}}
  };

  for (auto& t : tests) {
    cout << "Input:  arr = [";
    for (size_t i = 0; i < t.arr.size(); ++i) {
      cout << t.arr[i] << (i + 1 < t.arr.size() ? "," : "");
    }
    cout << "]" << endl;
    cout << "Output: " << candy(t.arr) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = [1, 0, 2]
    ==========================================================================
    n = 3
    Initial: v = [1, 1, 1]

    Left-to-right pass (i = 1 .. n-1):
      i=1: arr[1]=0, arr[0]=1 -> 0 > 1 is false -> no change
           v = [1, 1, 1]
      i=2: arr[2]=2, arr[1]=0 -> 2 > 0 is true
           check v[2] <= v[1] -> 1 <= 1 is true -> v[2] = v[1] + 1 = 2
           v = [1, 1, 2]

    After left-to-right pass: v = [1, 1, 2]

    Right-to-left pass (i = n-2 .. 0):
      i=1: arr[1]=0, arr[2]=2 -> 0 > 2 is false -> no change
           v = [1, 1, 2]
      i=0: arr[0]=1, arr[1]=0 -> 1 > 0 is true
           check v[0] <= v[1] -> 1 <= 1 is true -> v[0] = v[1] + 1 = 2
           v = [2, 1, 2]

    After right-to-left pass: v = [2, 1, 2]

    Sum = 2 + 1 + 2 = 5  -> matches expected output 5

    ==========================================================================
    Summary table
    ==========================================================================
    | Step              | i | Comparison         | Condition | v (after)   |
    |-------------------|---|---------------------|-----------|-------------|
    | Initial           | - | -                   | -         | [1, 1, 1]   |
    | Left-to-right     | 1 | arr[1]=0 > arr[0]=1 | false     | [1, 1, 1]   |
    | Left-to-right     | 2 | arr[2]=2 > arr[1]=0 | true      | [1, 1, 2]   |
    | Right-to-left     | 1 | arr[1]=0 > arr[2]=2 | false     | [1, 1, 2]   |
    | Right-to-left     | 0 | arr[0]=1 > arr[1]=0 | true      | [2, 1, 2]   |
    | Final sum         | - | -                   | -         | 5           |

    ==========================================================================
    Notes
    ==========================================================================
    - The two constraints ("higher rating than left neighbor needs more
      candy" and "higher rating than right neighbor needs more candy") are
      each, individually, easy to satisfy with a single directional sweep.
      The difficulty is satisfying both at once for children that are a
      "local peak" (higher rated than both neighbors).
    - The left-to-right pass guarantees the left-neighbor constraint holds
      everywhere. The right-to-left pass then only *increases* a value
      (via the v[i] <= v[i+1] guard) when the right-neighbor constraint
      isn't already met, so it never violates the left-neighbor constraint
      that was already established. A local peak therefore ends up with
      v[i] = max(left-pass value, right-pass value), satisfying both
      neighbors simultaneously.
    - This greedy strategy is optimal: no child can be given fewer candies
      than 1 + the count of consecutive strictly-increasing ratings running
      into it from whichever side demands more, so the two-pass max is the
      true minimum.
    - Time Complexity: O(n) for the two linear passes.
    - Space Complexity: O(n) for the candy count array v.
*/
