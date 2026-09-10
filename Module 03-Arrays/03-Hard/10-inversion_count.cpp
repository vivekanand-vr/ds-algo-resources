#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Count the number of inversions in an array, i.e. pairs (i, j) such that
// i < j and arr[i] > arr[j]

/*
    Approach: Modified Merge Sort (Divide & Conquer)
    - Count inversions while performing a merge sort, avoiding the O(n^2)
      brute-force pair check
    - Recursively count inversions in the left half and the right half
    - While merging the two sorted halves, whenever an element from the right
      half is placed before a remaining element from the left half, every
      remaining element in the left half forms an inversion with it
    - Sum inversions from the left half, right half, and the merge step

    Algorithm Steps
    ----------------
    1. Recursively split the array into halves until subarrays of size 1
    2. Count inversions in the left half and the right half recursively
    3. Merge the two sorted halves; each time a right-half element is placed
       before a left-half element, add the count of remaining left elements
    4. Sum all three inversion counts and return the total

    Time Complexity: O(n log n) - standard merge sort recurrence
    Space Complexity: O(n) - temporary arrays used during merging
*/

// Helper function to merge two sorted halves and count inversions
long long mergeAndCount(vector<int>& arr, int l, int m, int r) {
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;
    long long invCount = 0;

    // Merge process
    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
            // All remaining elements in left[] are greater → inversions
            invCount += (left.size() - i);
        }
    }

    // Copy leftovers
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];

    return invCount;
}

// Recursive function (Divide & Conquer)
long long mergeSortCount(vector<int>& arr, int l, int r) {
    if (l >= r) return 0;
    int m = l + (r - l) / 2;

    long long inv = 0;
    inv += mergeSortCount(arr, l, m);      // Count in left half
    inv += mergeSortCount(arr, m + 1, r);  // Count in right half
    inv += mergeAndCount(arr, l, m, r);    // Count across halves

    return inv;
}

int inversionCount(vector<int> &arr) {
    return (int) mergeSortCount(arr, 0, arr.size() - 1);
}

int main() {
    vector<int> arr = {2, 4, 1, 3, 5};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Inversion count: " << inversionCount(arr) << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {2, 4, 1, 3, 5}   (n = 5, answer = 3)
    ==========================================================================

      index:   0   1   2   3   4
      arr:     2   4   1   3   5

    Brute-force check first, so the trace has something to be verified
    against. All 10 pairs (i, j) with i < j, marked * where arr[i] > arr[j]:

      (0,1) 2>4 no    (0,2) 2>1 YES *   (0,3) 2>3 no    (0,4) 2>5 no
                      (1,2) 4>1 YES *   (1,3) 4>3 YES * (1,4) 4>5 no
                                        (2,3) 1>3 no    (2,4) 1>5 no
                                                        (3,4) 3>5 no

    Three inversions: (0,2), (1,2), (1,3). The trace below must total 3.

    Tracked state:
      l, m, r   - the current subrange arr[l..r] and its midpoint
                  m = l + (r - l) / 2, so the LEFT half arr[l..m] gets the
                  extra element when the range has odd length
      left[]    - a COPY of arr[l..m]  (already sorted by recursion)
      right[]   - a COPY of arr[m+1..r] (already sorted by recursion)
      i, j      - read pointers into left[] and right[]
      k         - write pointer back into arr, starts at l
      invCount  - inversions found in THIS merge only
      arr       - mutated in place; every merge leaves arr[l..r] sorted

    ==========================================================================
    PHASE 1: the split (recursion going down)
    ==========================================================================

    mergeSortCount(arr, 0, 4)          m = 0 + (4-0)/2 = 2
    |
    +-- mergeSortCount(arr, 0, 2)      m = 0 + (2-0)/2 = 1
    |   |
    |   +-- mergeSortCount(arr, 0, 1)  m = 0 + (1-0)/2 = 0
    |   |   |
    |   |   +-- mergeSortCount(arr, 0, 0)   l >= r -> return 0  (leaf)
    |   |   +-- mergeSortCount(arr, 1, 1)   l >= r -> return 0  (leaf)
    |   |   +-- mergeAndCount(arr, 0, 0, 1)      [merge A]
    |   |
    |   +-- mergeSortCount(arr, 2, 2)   l >= r -> return 0  (leaf)
    |   +-- mergeAndCount(arr, 0, 1, 2)          [merge B]
    |
    +-- mergeSortCount(arr, 3, 4)      m = 3 + (4-3)/2 = 3
    |   |
    |   +-- mergeSortCount(arr, 3, 3)   l >= r -> return 0  (leaf)
    |   +-- mergeSortCount(arr, 4, 4)   l >= r -> return 0  (leaf)
    |   +-- mergeAndCount(arr, 3, 3, 4)          [merge C]
    |
    +-- mergeAndCount(arr, 0, 2, 4)              [merge D]

    Ranges as slices of the array:

      [0..4]  2  4  1  3  5
              \________/  \__/
      [0..2]  2  4  1     [3..4] 3  5
              \___/  |            |  |
      [0..1]  2  4   [2] 1        3  5      (single elements: sorted already)

    Four merges happen, bottom-up, in the order A, B, C, D.

    ==========================================================================
    PHASE 2: the merges (recursion coming back up)
    ==========================================================================

    --------------------------------------------------------------------------
        merge A: mergeAndCount(arr, l=0, m=0, r=1)     depth 3
          left  = arr[0..0] = [2]
          right = arr[1..1] = [4]
          i = 0, j = 0, k = 0, invCount = 0

          step 1: left[0] = 2 <= right[0] = 4?  YES
                  -> arr[0] = 2, i -> 1, k -> 1
                  no inversion: taking from the LEFT half never counts one,
                  because a left element placed before a right element is
                  already in the correct relative order.
                  i = 1 = left.size() -> main loop exits

          leftover left[]:  i = 1, nothing left
          leftover right[]: j = 0 < 1 -> arr[1] = 4, j -> 1, k -> 2

          invCount = 0
          arr = [2, 4, 1, 3, 5]        (unchanged - already in order)
    --------------------------------------------------------------------------
        merge C is at the same depth but runs later; see below.

    --------------------------------------------------------------------------
      merge B: mergeAndCount(arr, l=0, m=1, r=2)       depth 2
        left  = arr[0..1] = [2, 4]      <- sorted by merge A
        right = arr[2..2] = [1]
        i = 0, j = 0, k = 0, invCount = 0

              left:   2   4          right:   1
                      ^                       ^
                      i                       j

        step 1: left[0] = 2 <= right[0] = 1?  NO -> take from RIGHT
                -> arr[0] = 1, j -> 1, k -> 1
                invCount += left.size() - i = 2 - 0 = 2
                invCount = 2

                WHY 2, and why in one shot:
                left[] is sorted, and 1 lost the comparison against left[0].
                So 1 is smaller than left[0] = 2 AND smaller than every
                element after it - left[1] = 4 included. Every one of those
                left elements sits at an ORIGINAL index below 1's original
                index (left half comes first), so each forms an inversion
                with it. The pairs are (2,1) and (4,1), i.e. original index
                pairs (0,2) and (1,2). That is the whole point of the
                algorithm: one comparison retires (left.size() - i)
                inversions instead of one.

                j = 1 = right.size() -> main loop exits

        leftover left[]: i = 0 < 2
                arr[1] = left[0] = 2, i -> 1, k -> 2
                arr[2] = left[1] = 4, i -> 2, k -> 3
                (leftover copies never count inversions - anything still in
                 left[] is larger than everything already written from
                 right[], and those inversions were already tallied above)
        leftover right[]: j = 1, nothing left

        invCount = 2
        arr = [1, 2, 4, 3, 5]
              \______/
              arr[0..2] now sorted

        subtotal for mergeSortCount(0,2) = 0 (left) + 0 (right) + 2 = 2
    --------------------------------------------------------------------------

    --------------------------------------------------------------------------
        merge C: mergeAndCount(arr, l=3, m=3, r=4)     depth 2 (right subtree)
          left  = arr[3..3] = [3]
          right = arr[4..4] = [5]
          i = 0, j = 0, k = 3, invCount = 0

          step 1: left[0] = 3 <= right[0] = 5?  YES
                  -> arr[3] = 3, i -> 1, k -> 4
                  i = 1 = left.size() -> loop exits

          leftover right[]: arr[4] = 5, j -> 1, k -> 5

          invCount = 0
          arr = [1, 2, 4, 3, 5]        (arr[3..4] unchanged, already sorted)

        subtotal for mergeSortCount(3,4) = 0 + 0 + 0 = 0
    --------------------------------------------------------------------------

    --------------------------------------------------------------------------
    merge D: mergeAndCount(arr, l=0, m=2, r=4)         depth 1 (the root)
      left  = arr[0..2] = [1, 2, 4]    <- sorted by merge B
      right = arr[3..4] = [3, 5]       <- sorted by merge C
      i = 0, j = 0, k = 0, invCount = 0

          left:   1   2   4        right:   3   5
                  ^                         ^
                  i                         j

      step 1: left[0] = 1 <= right[0] = 3?  YES
              -> arr[0] = 1, i -> 1, k -> 1        (no inversion)

      step 2: left[1] = 2 <= right[0] = 3?  YES
              -> arr[1] = 2, i -> 2, k -> 2        (no inversion)

      step 3: left[2] = 4 <= right[0] = 3?  NO -> take from RIGHT
              -> arr[2] = 3, j -> 1, k -> 3
              invCount += left.size() - i = 3 - 2 = 1
              invCount = 1

              WHY only 1 here, when merge B credited 2:
              i has already advanced to 2, meaning left[0] = 1 and
              left[1] = 2 were ALREADY written out - they are smaller than 3
              and form no inversion with it. Only left[2] = 4 remains, so
              exactly one pair: (4, 3). In the ORIGINAL array that is
              index pair (1, 3), value pair 4 > 3.  The `- i` in
              (left.size() - i) is what excludes the already-placed
              elements; forgetting it is the classic bug.

          left:   1   2   4        right:   3   5
                          ^                     ^
                          i                     j      after step 3

      step 4: left[2] = 4 <= right[1] = 5?  YES
              -> arr[3] = 4, i -> 3, k -> 4        (no inversion)
              i = 3 = left.size() -> main loop exits

      leftover left[]:  i = 3, nothing left
      leftover right[]: j = 1 < 2 -> arr[4] = 5, j -> 2, k -> 5

      invCount = 1
      arr = [1, 2, 3, 4, 5]        fully sorted

    --------------------------------------------------------------------------
    TOTAL at the root:
      mergeSortCount(0, 2)  =  2      (merges A + B: 0 + 2)
      mergeSortCount(3, 4)  =  0      (merge C)
      mergeAndCount(0,2,4)  =  1      (merge D)
                            ----
                               3

    RETURN 3        matches the brute-force count of 3. Confirmed.

    ==========================================================================
    Summary table (merges in execution order, bottom-up)
    ==========================================================================

    | merge | range   | left[]    | right[] | inv | arr after       |
    |-------|---------|-----------|---------|-----|-----------------|
    |   A   | [0..1]  | [2]       | [4]     |  0  | 2 4 1 3 5       |
    |   B   | [0..2]  | [2,4]     | [1]     |  2  | 1 2 4 3 5       |
    |   C   | [3..4]  | [3]       | [5]     |  0  | 1 2 4 3 5       |
    |   D   | [0..4]  | [1,2,4]   | [3,5]   |  1  | 1 2 3 4 5       |
                                       total  3

    Which merge caught which inversion:
      merge B -> (0,2): 2 > 1   and  (1,2): 4 > 1
      merge D -> (1,3): 4 > 3
    Every inversion is counted by EXACTLY ONE merge: the one whose split
    point falls between the two indices. Indices 1 and 2 are separated for
    the first time at merge B's split (m = 1); indices 1 and 3 straddle merge
    D's split (m = 2). No pair can straddle two different splits at the same
    level, and once a pair lands in the same half it is handled deeper down -
    that is why the three counts simply add with no risk of double counting.

    Step count behind the O(n log n):
      4 merges over 3 levels of recursion; the comparison-loop bodies ran
      1 + 1 + 1 + 4 = 7 times, plus 4 leftover copies, for 11 element moves
      total. Each level moves at most n = 5 elements and there are
      ceil(log2 5) = 3 levels, so ~15 moves is the ceiling. Brute force would
      have tested all 10 pairs here; the gap is n^2/2 versus n log n, which
      at n = 100000 is 5*10^9 versus ~1.7*10^6.

    The invariant that makes the one-shot count valid:
      when mergeAndCount is called, arr[l..m] and arr[m+1..r] are each
      ALREADY SORTED (guaranteed by the two recursive calls above it). Sorted
      halves are what let a single failed comparison certify that ALL
      remaining left elements exceed the current right element. Reordering
      the two halves destroys nothing, because the count is about original
      index order, and every cross-half pair keeps its left-before-right
      relationship no matter how the halves are internally shuffled.
*/
