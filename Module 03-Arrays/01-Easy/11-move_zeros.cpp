#include <iostream>
#include <vector>
using namespace std;

// Q: Given an integer array nums, move all 0's to the end of it while
// maintaining the relative order of the non-zero elements

/*
    Approach: Two Pointer (In-Place Compaction)
    - i keeps track of the position where the next non-zero element should
      be placed
    - j scans through the entire array
    - Whenever arr[j] is non-zero, place it at arr[i] and advance i
    - After all non-zero elements have been moved to the front, fill the
      remaining positions from i onward with zeroes

    Algorithm Steps
    ----------------
    1. Initialize i = 0
    2. Scan j from 0 to n-1; if arr[j] != 0, set arr[i] = arr[j] and increment i
    3. Fill positions i to n-1 with 0

    Time Complexity: O(n) - one pass to compact non-zero elements, one to fill zeroes
    Space Complexity: O(1) - modifies the array in-place
*/
void moveZeroes(vector<int>& arr) {
  int i = 0;

  for (int j = 0; j < arr.size(); ++j) {
    if (arr[j] != 0) {
      arr[i++] = arr[j];
    }
  }

  // Fill the remaining positions with zeroes.
  while (i < arr.size()) {
    arr[i++] = 0;
  }
}

int main() {
  vector<int> arr = {0, 1, 0, 3, 12};
  moveZeroes(arr);
  cout << "Array after moving zeroes: ";
  for (int x : arr) {
    cout << x << " ";
  }
  cout << endl;

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {0, 1, 0, 3, 12}   (n = 5, result = {1, 3, 12, 0, 0})
    ==========================================================================

      index:   0   1   2   3   4
      value:   0   1   0   3  12

    Tracked state:
      i - SLOW/write pointer: the next slot a non-zero element belongs in.
          It also equals the count of non-zeroes written so far.
      j - FAST/read pointer: scans every index once and never writes.

    Invariant: arr[0..i-1] holds the non-zero values of arr[0..j-1] in
    their original relative order. Slots i..j-1 are scratch.

    Initial state: i = 0, arr = {0, 1, 0, 3, 12}
    (unlike the remove-duplicates pattern, j starts at 0, not 1)

    --------------------------------------------------------------------------
    PASS 1 - compact the non-zeroes to the front
    --------------------------------------------------------------------------
    j = 0, arr[j] = 0
      compare      arr[0] == 0 -> not a value to keep
      skip         no write; i stays 0
      array        {0, 1, 0, 3, 12}

    --------------------------------------------------------------------------
    j = 1, arr[j] = 1
      compare      arr[1] = 1 != 0 -> keep it
      write        arr[i++] = arr[j]
                   step 1: POST-increment, so the write uses the OLD i = 0
                   step 2: arr[0] = 1   (overwrites the zero already read)
                   step 3: i becomes 1
      array        {1, 1, 0, 3, 12}     kept = [1]
                    ^
                    written here; arr[1] is now a stale copy, harmless
                    because j has already moved past it

    --------------------------------------------------------------------------
    j = 2, arr[j] = 0
      compare      arr[2] == 0 -> skip; i stays 1
      array        {1, 1, 0, 3, 12}     kept = [1]

    --------------------------------------------------------------------------
    j = 3, arr[j] = 3
      compare      arr[3] = 3 != 0 -> keep
      write        arr[1] = 3, then i -> 2
      array        {1, 3, 0, 3, 12}     kept = [1, 3]

                     1   3   0   3  12
                         ^       ^
                         i       j
                   the gap j - i = 2 is exactly the number of zeroes seen
                   so far, so the write index can never overtake the reader

    --------------------------------------------------------------------------
    j = 4, arr[j] = 12
      compare      arr[4] = 12 != 0 -> keep
      write        arr[2] = 12, then i -> 3
      array        {1, 3, 12, 3, 12}    kept = [1, 3, 12]
      end          j reaches size 5 -> pass 1 done, i = 3

    --------------------------------------------------------------------------
    PASS 2 - zero-fill the tail from i to n-1
    --------------------------------------------------------------------------
      state        i = 3, arr = {1, 3, 12, 3, 12}
                   the tail still holds STALE copies (3 and 12), not zeroes,
                   so this second pass is not optional

      fill step 1: i = 3 < 5 -> arr[3] = 0, i -> 4
                   array {1, 3, 12, 0, 12}
      fill step 2: i = 4 < 5 -> arr[4] = 0, i -> 5
                   array {1, 3, 12, 0, 0}
      fill step 3: i = 5, not < 5 -> while loop exits

    --------------------------------------------------------------------------
    FINAL ARRAY: {1, 3, 12, 0, 0}
                  \_______/  \__/
                  non-zeroes  the 2 zeroes, pushed to the end
                  in original
                  order

    (void function - main() prints the mutated vector: 1 3 12 0 0)

    ==========================================================================
    Summary table
    ==========================================================================

    | phase | j / i | value | action | i after | array            |
    |-------|-------|-------|--------|---------|------------------|
    | scan  | j = 0 |   0   | skip   |    0    | 0  1  0  3  12   |
    | scan  | j = 1 |   1   | write  |    1    | 1  1  0  3  12   |
    | scan  | j = 2 |   0   | skip   |    1    | 1  1  0  3  12   |
    | scan  | j = 3 |   3   | write  |    2    | 1  3  0  3  12   |
    | scan  | j = 4 |  12   | write  |    3    | 1  3 12  3  12   |
    | fill  | i = 3 |   -   | zero   |    4    | 1  3 12  0  12   |
    | fill  | i = 4 |   -   | zero   |    5    | 1  3 12  0   0   |

    Step count backing the O(n) claim:
      5 reads in pass 1 plus 2 writes in pass 2 = 7 operations for n = 5,
      i.e. n + (number of zeroes) <= 2n. Order is preserved because pass 1
      copies non-zeroes strictly left-to-right and never swaps.
*/
