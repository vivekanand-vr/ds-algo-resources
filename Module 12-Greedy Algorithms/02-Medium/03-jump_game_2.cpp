#include <bits/stdc++.h>
using namespace std;

// Q: You are given a 0-indexed array of integers arr of length n. You are
//    initially positioned at index 0. Each element arr[i] represents the
//    maximum length of a forward jump from index i. In other words, from
//    index i you can jump to any index in the range [i + 1, i + arr[i]].
//    Return the minimum number of jumps to reach index n - 1. It is
//    guaranteed that you can always reach the last index.
//
// Example:
// arr = [2,3,1,1,4]   -> 2   (jump from index 0 to 1, then from 1 to 4)
// arr = [2,3,0,1,4]   -> 2   (jump from index 0 to 1, then from 1 to 4)

/*
    Approach: Greedy BFS by "jump levels"

    - Think of the array as a graph where index i has an edge to every
      index in [i+1, i+arr[i]]. Finding the minimum number of jumps to
      reach the last index is then a shortest-path / BFS problem, and
      BFS naturally explores the graph level by level, where each level
      corresponds to "all indices reachable using exactly k jumps".
    - Instead of literally doing BFS, the greedy version tracks the
      furthest index reachable from the current level (`far`) while
      scanning through the indices belonging to that level.
    - `boundary` marks the last index of the current jump level. As long
      as i has not reached `boundary`, we are still exploring indices
      within the current level, so no extra jump is needed yet.
    - The moment i reaches `boundary`, every index up to `far` (computed
      while scanning this level) becomes reachable with one more jump,
      so we commit to that jump: increment the jump counter and push
      `boundary` out to `far`, starting the next level.
    - The loop only needs to run up to n - 2 (i.e. i < n - 1): once we
      are forced to jump from the boundary that already covers index
      n - 1, we've counted that jump and don't need to process the last
      index itself.

    Algorithm Steps
    ----------------
    1. Initialize far = 0 (furthest index reachable so far), jump = 0
       (jumps taken so far), boundary = 0 (end of the current jump's
       reach).
    2. For each index i from 0 to n - 2:
       a. Update far = max(far, i + arr[i]).
       b. If i has reached boundary, we must take another jump:
          increment jump and set boundary = far.
    3. Return jump.

    Time Complexity: O(n) - each index is visited exactly once.
    Space Complexity: O(1) - only a handful of scalar variables are used.
*/
int jump(vector<int>& arr) {
  int n = arr.size();
  int far = 0, jump = 0, boundary = 0;
  for (int i = 0; i < n - 1; ++i) {
    far = max(far, i + arr[i]);
    if (i == boundary) {
      jump++;
      boundary = far;
    }
  }

  return jump;
}

int main() {
  struct Test {
    vector<int> arr;
  };
  vector<Test> tests = {
    {{2, 3, 1, 1, 4}},
    {{2, 3, 0, 1, 4}},
    {{1, 1, 1, 1}},
    {{1, 2, 3}},
    {{0}},
    {{2, 1}}
  };

  for (auto& t : tests) {
    cout << "Input:  arr = [";
    for (size_t i = 0; i < t.arr.size(); ++i) {
      cout << t.arr[i] << (i + 1 < t.arr.size() ? "," : "");
    }
    cout << "]" << endl;
    cout << "Output: " << jump(t.arr) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = [2,3,1,1,4]
    ==========================================================================
    n = 5
    Initial: far = 0, jump = 0, boundary = 0
    Loop runs for i = 0 .. n-2 = 3

    i = 0: arr[0] = 2
      far = max(0, 0 + 2) = 2
      i == boundary (0 == 0)? yes -> jump = 1, boundary = far = 2

    i = 1: arr[1] = 3
      far = max(2, 1 + 3) = 4
      i == boundary (1 == 2)? no

    i = 2: arr[2] = 1
      far = max(4, 2 + 1) = 4
      i == boundary (2 == 2)? yes -> jump = 2, boundary = far = 4

    i = 3: arr[3] = 1
      far = max(4, 3 + 1) = 4
      i == boundary (3 == 4)? no

    Loop ends (i would be 4, but condition is i < n - 1 = 4)
    Return jump = 2

    ==========================================================================
    Summary table
    ==========================================================================
    | i | arr[i] | far | i==boundary? | jump | boundary (after) |
    |---|--------|-----|---------------|------|-------------------|
    | 0 |   2    |  2  |  yes (0==0)   |  1   |         2         |
    | 1 |   3    |  4  |  no  (1!=2)   |  1   |         2         |
    | 2 |   1    |  4  |  yes (2==2)   |  2   |         4         |
    | 3 |   1    |  4  |  no  (3!=4)   |  2   |         4         |

    Final answer: 2 jumps (index 0 -> 1 -> 4)

    ==========================================================================
    Notes
    ==========================================================================
    - This is the "BFS level" greedy: within a single jump (level), the
      loop only needs to know the single furthest index reachable from
      anywhere in that level (`far`); it never needs to know exactly
      which index inside the level produced that reach. This is what
      lets the algorithm avoid actually simulating BFS layer by layer
      with a queue.
    - The greedy choice is optimal because a jump is only ever forced
      when we've exhausted every index in the current level (i has hit
      `boundary`); at that point `far` is provably the best possible
      reach achievable with one more jump, since it was computed by
      scanning every reachable index in the current level.
    - Stopping the loop at i < n - 1 is safe: once boundary is pushed
      out to cover n - 1 (or beyond), the jump that will get us there
      has already been counted by the time i would reach it, so
      processing the last index itself is unnecessary.
    - Time Complexity: O(n) - single linear scan over the array.
    - Space Complexity: O(1) - only scalar counters are maintained.
*/
