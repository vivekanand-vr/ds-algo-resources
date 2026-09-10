#include <iostream>
#include <vector>
using namespace std;

// Q: Given a sorted array, remove duplicates in-place such that each unique
// element appears only once, and return the count of unique elements

/*
    Approach: Two Pointer (In-Place Compaction)
    - Since the array is sorted, duplicates are always adjacent
    - i points to the position of the last unique element placed so far
    - j scans through the array looking for the next unique element
    - Whenever nums[j] differs from nums[i], it's a new unique value:
      advance i and place nums[j] at nums[i]

    Algorithm Steps
    ----------------
    1. Initialize i = 0 (points to last unique element)
    2. Scan j from 1 to n-1
    3. If nums[j] == nums[i], it's a duplicate, skip it
    4. If nums[j] != nums[i], increment i and set nums[i] = nums[j]
    5. Return i + 1 (count of unique elements, now in nums[0..i])

    Time Complexity: O(n) - single pass using pointer j
    Space Complexity: O(1) - modifies the array in-place

    IMPORTANT: This solution assumes the input array is SORTED.
*/
int removeDuplicates(vector<int>& nums) {
  int i = 0;
  int n = nums.size();

  for (int j = 1; j < n; ++j) {
    if (nums[j] == nums[i]) {
      continue;
    } else {
      nums[++i] = nums[j];
    }
  }

  // Number of unique elements
  return i + 1;
}

int main() {
  vector<int> nums = {1, 1, 2, 2, 3, 3, 4};

  int k = removeDuplicates(nums);
  cout << "Number of unique elements: " << k << endl;
  cout << "Array after removing duplicates: ";
  for (int i = 0; i < k; ++i) {
    cout << nums[i] << " ";
  }
  cout << endl;

  return 0;
}

/*
    ==========================================================================
    DRY RUN: nums = {1, 1, 2, 2, 3, 3, 4}   (n = 7, answer k = 4)
    ==========================================================================

      index:   0   1   2   3   4   5   6
      value:   1   1   2   2   3   3   4

    Tracked state:
      i - SLOW pointer: index of the last unique value already placed.
          nums[0..i] is the compacted, duplicate-free result.
      j - FAST pointer: the scanner, reads every index once, never writes.

    Invariant: nums[0..i] holds the unique values found in nums[0..j-1],
    in order. The array right of i is scratch and may hold stale values.

    Initial state: i = 0, nums = {1, 1, 2, 2, 3, 3, 4}
    (nums[0] is trivially unique, so the scan starts at j = 1)

    --------------------------------------------------------------------------
    j = 1, nums[j] = 1
      compare      nums[1] = 1 vs nums[i=0] = 1  -> equal
      skip         duplicate, continue; no write, i stays 0
      array        {1, 1, 2, 2, 3, 3, 4}      unique part = [1]

    --------------------------------------------------------------------------
    j = 2, nums[j] = 2
      compare      nums[2] = 2 vs nums[i=0] = 1  -> different -> new unique
      write        nums[++i] = nums[j]
                   step 1: ++i is a PRE-increment, so i becomes 1 first
                   step 2: nums[1] = nums[2] = 2   (overwrites the dup 1)
      array        {1, 2, 2, 2, 3, 3, 4}      unique part = [1, 2]
                    ^  ^
                    unique   i = 1

    --------------------------------------------------------------------------
    j = 3, nums[j] = 2
      compare      nums[3] = 2 vs nums[i=1] = 2  -> equal
      skip         duplicate; i stays 1
      array        {1, 2, 2, 2, 3, 3, 4}      unique part = [1, 2]

                     1   2   2   2   3   3   4
                         ^       ^
                         i       j
                   i trails behind j by the number of duplicates dropped
                   so far (2), which is why the write never clobbers an
                   element j has not read yet.

    --------------------------------------------------------------------------
    j = 4, nums[j] = 3
      compare      nums[4] = 3 vs nums[i=1] = 2  -> different
      write        i -> 2, nums[2] = 3
      array        {1, 2, 3, 2, 3, 3, 4}      unique part = [1, 2, 3]

    --------------------------------------------------------------------------
    j = 5, nums[j] = 3
      compare      nums[5] = 3 vs nums[i=2] = 3  -> equal
      skip         duplicate; i stays 2
      array        {1, 2, 3, 2, 3, 3, 4}      unique part = [1, 2, 3]

    --------------------------------------------------------------------------
    j = 6, nums[j] = 4
      compare      nums[6] = 4 vs nums[i=2] = 3  -> different
      write        i -> 3, nums[3] = 4
      array        {1, 2, 3, 4, 3, 3, 4}      unique part = [1, 2, 3, 4]
                                ^
                                i = 3, j reaches n -> loop ends

    --------------------------------------------------------------------------
    Final array: {1, 2, 3, 4, 3, 3, 4}
                  \__________/ \_____/
                   the answer   leftover junk, ignored by the caller

    RETURN i + 1 = 3 + 1 = 4
    main() then prints only nums[0..k-1]:  1 2 3 4

    ==========================================================================
    Summary table
    ==========================================================================

    | j | nums[j] | nums[i] | action | i | array after         |
    |---|---------|---------|--------|---|---------------------|
    | 1 |    1    |    1    | skip   | 0 | 1 1 2 2 3 3 4       |
    | 2 |    2    |    1    | write  | 1 | 1 2 2 2 3 3 4       |
    | 3 |    2    |    2    | skip   | 1 | 1 2 2 2 3 3 4       |
    | 4 |    3    |    2    | write  | 2 | 1 2 3 2 3 3 4       |
    | 5 |    3    |    3    | skip   | 2 | 1 2 3 2 3 3 4       |
    | 6 |    4    |    3    | write  | 3 | 1 2 3 4 3 3 4       |

    Why the in-place write is always safe:
      i <= j at every step, so nums[++i] can only land on an index j has
      already consumed. The comparison is against nums[i] (the last KEPT
      value), not nums[j-1] - those happen to coincide here, but comparing
      to the kept value is what makes the logic robust.

    Precondition: the array must be SORTED, so equal values are adjacent.
    On {1, 2, 1} this returns 3 and leaves the duplicate in place.
*/
