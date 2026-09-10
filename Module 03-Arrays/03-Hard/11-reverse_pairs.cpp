#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Count the number of reverse pairs in an array, i.e. pairs (i, j) such that
// i < j and arr[i] > 2 * arr[j]

/*
    Approach: Modified Merge Sort (Divide & Conquer)
    - Count reverse pairs while performing a merge sort, avoiding the O(n^2)
      brute-force pair check
    - Recursively count reverse pairs in the left half and the right half
    - Before merging the two sorted halves, count reverse pairs across the
      left and right halves
    - Since both halves are sorted, use a two-pointer approach to count
      cross-half reverse pairs efficiently
    - Finally, merge the two sorted halves

    Important:
    - For every element arr[i] in the left half, find how many elements
      arr[j] in the right half satisfy:

          arr[i] > 2 * arr[j]

    - Because the right half is sorted, the pointer only moves forward.
      This makes counting all cross-half pairs O(n) for each merge step.

    Algorithm Steps
    ----------------
    1. Recursively split the array into halves until subarrays of size 1
    2. Count reverse pairs in the left half recursively
    3. Count reverse pairs in the right half recursively
    4. Count reverse pairs across the two sorted halves using two pointers
    5. Merge the two sorted halves
    6. Sum all three reverse-pair counts

    Time Complexity: O(n log n) - standard merge sort recurrence
    Space Complexity: O(n) - temporary arrays used during merging
*/

// Helper function to count reverse pairs across two sorted halves
long long countPairs(vector<int>& arr, int low, int mid, int high) {
  long long count = 0;
  int right = mid + 1;

  // Left half: arr[low..mid]
  // Right half: arr[mid+1..high]
  for (int i = low; i <= mid; ++i) {
    // Find the first element in the right half that does NOT satisfy
    // arr[i] > 2 * arr[right]
    while (right <= high && (long long)arr[i] > 2LL * arr[right]) {
      right++;
    }

    // All elements before 'right' satisfy the condition
    count += (right - (mid + 1));
  }

  return count;
}

// Helper function to merge two sorted halves
void merge(vector<int>& arr, int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  vector<int> leftArr(n1);
  vector<int> rightArr(n2);

  // Copy left half
  for (int i = 0; i < n1; i++) leftArr[i] = arr[left + i];

  // Copy right half
  for (int j = 0; j < n2; j++) rightArr[j] = arr[mid + 1 + j];

  int i = 0, j = 0, k = left;

  // Merge the two sorted halves
  while (i < n1 && j < n2) {
    if (leftArr[i] <= rightArr[j]) {
      arr[k++] = leftArr[i++];
    } else {
      arr[k++] = rightArr[j++];
    }
  }

  // Copy remaining elements from left half
  while (i < n1) arr[k++] = leftArr[i++];

  // Copy remaining elements from right half
  while (j < n2) arr[k++] = rightArr[j++];
}

// Recursive function (Divide & Conquer)
long long mergeSort(vector<int>& arr, int left, int right) {
  if (left >= right) return 0;

  int mid = left + (right - left) / 2;

  long long count = 0;

  // Count reverse pairs in left half
  count += mergeSort(arr, left, mid);

  // Count reverse pairs in right half
  count += mergeSort(arr, mid + 1, right);

  // Count reverse pairs across the two halves
  // Both halves are already sorted at this point
  count += countPairs(arr, left, mid, right);

  // Merge the two sorted halves
  merge(arr, left, mid, right);

  return count;
}

int reversePairs(vector<int>& nums) {
  return (int)mergeSort(nums, 0, nums.size() - 1);
}

int main() {
  vector<int> arr = {1, 3, 2, 3, 1};

  cout << "Array: ";
  for (int x : arr) cout << x << " ";
  cout << endl;
  cout << "Reverse pair count: " << reversePairs(arr) << endl;

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {1, 3, 2, 3, 1}   (n = 5, answer = 2)
    ==========================================================================

      index:   0   1   2   3   4
      arr:     1   3   2   3   1

    Brute force first, so the trace can be checked against it. All 10 pairs
    (i, j) with i < j, marked * where arr[i] > 2 * arr[j]:

      (0,1) 1 > 6  no    (0,2) 1 > 4 no    (0,3) 1 > 6 no   (0,4) 1 > 2 no
                         (1,2) 3 > 4 no    (1,3) 3 > 6 no   (1,4) 3 > 2 YES *
                                           (2,3) 2 > 6 no   (2,4) 2 > 2 no
                                                            (3,4) 3 > 2 YES *

    Two reverse pairs: (1,4) with 3 > 2*1, and (3,4) with 3 > 2*1. Note
    (2,4) FAILS: 2 > 2 is false, the comparison is strict. The trace must
    total 2.

    Tracked state:
      low/left, mid, high/right - the current subrange and its midpoint,
                  mid = left + (right - left) / 2, so odd-length ranges give
                  the extra element to the LEFT half
      right (in countPairs) - a pointer that scans the RIGHT half; declared
                  ONCE before the i loop and never reset, so it only ever
                  moves forward across the whole call
      count     - reverse pairs found in THIS countPairs call only
      leftArr/rightArr, i/j/k - the merge's copies and pointers

    Structural difference from the inversion-count version: here counting and
    merging are SEPARATE passes. countPairs runs first, over arr[low..high]
    which is still split into two independently-sorted halves; merge runs
    after and destroys that structure. The order is not optional.

    ==========================================================================
    PHASE 1: the split (recursion going down)
    ==========================================================================

    mergeSort(arr, 0, 4)             mid = 0 + (4-0)/2 = 2
    |
    +-- mergeSort(arr, 0, 2)         mid = 0 + (2-0)/2 = 1
    |   |
    |   +-- mergeSort(arr, 0, 1)     mid = 0 + (1-0)/2 = 0
    |   |   |
    |   |   +-- mergeSort(arr, 0, 0)   left >= right -> return 0  (leaf)
    |   |   +-- mergeSort(arr, 1, 1)   left >= right -> return 0  (leaf)
    |   |   +-- countPairs(arr, 0, 0, 1) then merge(arr, 0, 0, 1)   [A]
    |   |
    |   +-- mergeSort(arr, 2, 2)       left >= right -> return 0  (leaf)
    |   +-- countPairs(arr, 0, 1, 2) then merge(arr, 0, 1, 2)       [B]
    |
    +-- mergeSort(arr, 3, 4)         mid = 3 + (4-3)/2 = 3
    |   |
    |   +-- mergeSort(arr, 3, 3)       left >= right -> return 0  (leaf)
    |   +-- mergeSort(arr, 4, 4)       left >= right -> return 0  (leaf)
    |   +-- countPairs(arr, 3, 3, 4) then merge(arr, 3, 3, 4)       [C]
    |
    +-- countPairs(arr, 0, 2, 4) then merge(arr, 0, 2, 4)           [D]

    Ranges as slices:

      [0..4]  1  3  2  3  1
              \_______/  \__/
      [0..2]  1  3  2     [3..4] 3  1
              \___/  |            |  |
      [0..1]  1  3   [2] 2        3  1

    Four count+merge steps, bottom-up, in the order A, B, C, D.

    ==========================================================================
    PHASE 2: count-then-merge, coming back up
    ==========================================================================

    --------------------------------------------------------------------------
        step A: low = 0, mid = 0, high = 1                     depth 3
          arr = [1, 3, 2, 3, 1]
          left half = arr[0..0] = [1],  right half = arr[1..1] = [3]

          countPairs(arr, 0, 0, 1):
            right = mid + 1 = 1, count = 0
            i = 0:  while right(1) <= high(1) and arr[0]=1 > 2*arr[1]=6?
                      1 > 6 is FALSE -> while body never runs, right stays 1
                    count += right - (mid+1) = 1 - 1 = 0
                    count = 0
            return 0

          merge(arr, 0, 0, 1):
            n1 = 0-0+1 = 1, n2 = 1-0 = 1
            leftArr = [1], rightArr = [3], i = 0, j = 0, k = 0
            step 1: leftArr[0]=1 <= rightArr[0]=3?  YES -> arr[0] = 1,
                    i -> 1, k -> 1;  i = n1 -> loop exits
            leftover right: arr[1] = 3, j -> 1, k -> 2
            arr = [1, 3, 2, 3, 1]      (unchanged, already ordered)

          mergeSort(0,1) returns 0
    --------------------------------------------------------------------------

    --------------------------------------------------------------------------
      step B: low = 0, mid = 1, high = 2                        depth 2
        arr = [1, 3, 2, 3, 1]
        left half = arr[0..1] = [1, 3]   <- sorted by step A
        right half = arr[2..2] = [2]

        countPairs(arr, 0, 1, 2):
          right = 2, count = 0

              left:   1   3        right:   2
                      ^                     ^
                      i                   right

          i = 0:  while right(2) <= high(2) and arr[0]=1 > 2*arr[2]=4?
                    1 > 4 FALSE -> stop, right stays 2
                  count += 2 - 2 = 0
          i = 1:  while right(2) <= 2 and arr[1]=3 > 2*arr[2]=4?
                    3 > 4 FALSE -> stop, right stays 2
                  count += 2 - 2 = 0
          return 0

          Nothing qualifies: doubling 2 to 4 puts it above both left values.
          The `2 *` is the whole difficulty of this problem - a merge-sort
          inversion count would have found (3, 2) here, but 3 > 2*2 is false
          so it is NOT a reverse pair.

        merge(arr, 0, 1, 2):
          n1 = 2, n2 = 1, leftArr = [1, 3], rightArr = [2], i=0, j=0, k=0
          step 1: 1 <= 2?  YES -> arr[0] = 1, i -> 1, k -> 1
          step 2: leftArr[1]=3 <= 2?  NO -> arr[1] = 2, j -> 1, k -> 2
                  j = n2 -> loop exits
          leftover left: arr[2] = leftArr[1] = 3, i -> 2, k -> 3
          arr = [1, 2, 3, 3, 1]
                \______/
                arr[0..2] now sorted

        mergeSort(0,2) returns 0 + 0 + 0 = 0
    --------------------------------------------------------------------------

    --------------------------------------------------------------------------
        step C: low = 3, mid = 3, high = 4                      depth 2
          arr = [1, 2, 3, 3, 1]      (arr[3..4] untouched so far = [3, 1])
          left half = arr[3..3] = [3],  right half = arr[4..4] = [1]

          countPairs(arr, 3, 3, 4):
            right = mid + 1 = 4, count = 0
            i = 3:  while right(4) <= high(4) and arr[3]=3 > 2*arr[4]=2?
                      3 > 2 is TRUE -> right -> 5
                    re-test: right(5) <= high(4)?  NO -> exit while
                    count += right - (mid+1) = 5 - 4 = 1
                    count = 1                                  <-- PAIR #1

                    Which pair: left value 3 at arr[3], right value 1 at
                    arr[4]. Original index pair (3, 4), and 3 > 2*1 = 2.
                    Correct.

                    Note `right` ran off the end to 5. That is intended: the
                    count is "how many right-half elements were passed", so
                    right - (mid+1) = 5 - 4 = 1 means ALL ONE element of the
                    right half qualified. The while condition checks
                    right <= high before dereferencing arr[right], so no
                    out-of-bounds read happens.
            return 1

          merge(arr, 3, 3, 4):
            n1 = 1, n2 = 1, leftArr = [3], rightArr = [1], i=0, j=0, k=3
            step 1: leftArr[0]=3 <= rightArr[0]=1?  NO -> arr[3] = 1,
                    j -> 1, k -> 4;  j = n2 -> loop exits
            leftover left: arr[4] = 3, i -> 1, k -> 5
            arr = [1, 2, 3, 1, 3]
                              \____/
                              arr[3..4] now sorted

          mergeSort(3,4) returns 0 + 0 + 1 = 1
    --------------------------------------------------------------------------

    --------------------------------------------------------------------------
    step D: left = 0, mid = 2, high = 4                     depth 1 (root)
      arr = [1, 2, 3, 1, 3]
      left half  = arr[0..2] = [1, 2, 3]   <- sorted by step B
      right half = arr[3..4] = [1, 3]      <- sorted by step C

      countPairs(arr, 0, 2, 4):
        right = mid + 1 = 3, count = 0

            left:   1   2   3        right:   1   3
                    ^                         ^
                    i                       right

        i = 0:  while right(3) <= 4 and arr[0]=1 > 2*arr[3]=2*1=2?
                  1 > 2 FALSE -> stop, right stays 3
                count += 3 - 3 = 0
        i = 1:  while right(3) <= 4 and arr[1]=2 > 2*arr[3]=2?
                  2 > 2 FALSE (strict >, and this is exactly the boundary
                  case that must NOT count) -> stop, right stays 3
                count += 3 - 3 = 0
        i = 2:  while right(3) <= 4 and arr[2]=3 > 2*arr[3]=2?
                  3 > 2 TRUE -> right -> 4
                re-test: right(4) <= 4 and arr[2]=3 > 2*arr[4]=2*3=6?
                  3 > 6 FALSE -> exit while, right stays 4
                count += right - (mid+1) = 4 - 3 = 1
                count = 1                                      <-- PAIR #2

            left:   1   2   3        right:   1   3
                            ^                     ^
                            i                   right

                Which pair: left value 3 (now at arr[2], originally arr[1])
                paired with right value 1 (now at arr[3], originally
                arr[4]). Original index pair (1, 4), and 3 > 2*1 = 2.
                Correct.

                Why the halves being INTERNALLY reshuffled is harmless: the
                left half still holds exactly the original indices 0..2 and
                the right half exactly 3..4, so every cross-half pair still
                has its left member at a smaller original index. Only the
                VALUES matter for the predicate, and the sortedness is what
                makes the single forward-only `right` pointer legal.

                Why `right` is NOT reset per i: left is sorted ascending, so
                a larger arr[i] can only qualify against MORE right-half
                elements, never fewer. The prefix of the right half that
                qualifies grows monotonically with i, so resuming from where
                the last i stopped is correct - and it is what makes this
                O(n) per merge instead of O(n^2). Across the three i values
                here, `right` advanced a total of one step, 3 -> 4.
        return 1

      merge(arr, 0, 2, 4):
        n1 = 2-0+1 = 3, n2 = 4-2 = 2
        leftArr = [1, 2, 3], rightArr = [1, 3], i = 0, j = 0, k = 0
        step 1: leftArr[0]=1 <= rightArr[0]=1?  YES -> arr[0] = 1, i->1, k->1
        step 2: leftArr[1]=2 <= rightArr[0]=1?  NO  -> arr[1] = 1, j->1, k->2
        step 3: leftArr[1]=2 <= rightArr[1]=3?  YES -> arr[2] = 2, i->2, k->3
        step 4: leftArr[2]=3 <= rightArr[1]=3?  YES -> arr[3] = 3, i->3, k->4
                i = n1 -> loop exits
        leftover left:  i = 3, none
        leftover right: arr[4] = rightArr[1] = 3, j -> 2, k -> 5
        arr = [1, 1, 2, 3, 3]        fully sorted

    --------------------------------------------------------------------------
    TOTAL at the root:
      mergeSort(0, 2)        =  0     (steps A + B)
      mergeSort(3, 4)        =  1     (step C)
      countPairs(0, 2, 4)    =  1     (step D)
                             ----
                                2

    RETURN 2        matches the brute-force count of 2. Confirmed.

    ==========================================================================
    Summary table (steps in execution order, bottom-up)
    ==========================================================================

    | step | range  | left half | right half | right ptr | pairs | arr after |
    |------|--------|-----------|------------|-----------|-------|-----------|
    |  A   | [0..1] | [1]       | [3]        | 1 -> 1    |   0   | 1 3 2 3 1 |
    |  B   | [0..2] | [1,3]     | [2]        | 2 -> 2    |   0   | 1 2 3 3 1 |
    |  C   | [3..4] | [3]       | [1]        | 4 -> 5    |   1   | 1 2 3 1 3 |
    |  D   | [0..4] | [1,2,3]   | [1,3]      | 3 -> 4    |   1   | 1 1 2 3 3 |
                                                    total   2

    Which step caught which pair:
      step C -> original (3, 4): arr[3]=3 > 2*arr[4]=2
      step D -> original (1, 4): arr[1]=3 > 2*arr[4]=2
    Every reverse pair is counted by EXACTLY ONE step - the one whose split
    point separates the two original indices for the first time. Indices 3
    and 4 are split at step C (mid = 3); indices 1 and 4 first end up on
    opposite sides at step D (mid = 2). A pair that lands in the same half
    is deferred to a deeper call, so the three subtotals add with no
    double counting and no gaps.

    Step count behind the O(n log n):
      4 count+merge steps over 3 recursion levels. countPairs executed
      1 + 2 + 1 + 3 = 7 loop iterations total, and its `right` pointer
      advanced 0 + 0 + 1 + 1 = 2 times in total across all four calls -
      never re-scanning. The merges performed 2 + 3 + 2 + 5 = 12 element
      writes. Brute force would have evaluated all 10 pairs at n = 5; the
      real gap shows at scale, n^2/2 versus n log n.

    The subtlety that bites people, in one place:
      the predicate is arr[i] > 2 * arr[j], STRICT. Step D's i = 1 case
      (2 > 2) and the brute-force pair (2,4) both fail on exactly this, and
      both would be miscounted by a `>=`. The 2LL cast in the condition
      matters for the same reason 2 * arr[j] can overflow int when arr[j] is
      near INT_MAX or INT_MIN - the code writes
      (long long)arr[i] > 2LL * arr[right] so both sides are evaluated in
      64-bit. And because countPairs reads arr BEFORE merge rewrites it, the
      call order count-then-merge is mandatory: merging first would blend the
      halves and make the two-pointer count meaningless.
*/
