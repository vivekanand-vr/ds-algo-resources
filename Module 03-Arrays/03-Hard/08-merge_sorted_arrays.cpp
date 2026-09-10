#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Merge two sorted arrays nums1 (with m valid elements plus trailing empty
// space) and nums2 (with n elements) in-place into nums1 as one sorted array

/*
    Approach: Merge from the Back (In-Place, No Extra Array)
    - nums1 has extra trailing space exactly sized to hold nums2's elements,
      so merging from the back avoids overwriting values that haven't been
      compared yet
    - Compare the last unplaced elements of nums1 and nums2, placing the
      larger one at the current back position of nums1
    - Once nums2 is exhausted, nums1's own remaining elements are already in
      their correct sorted position, so nothing more needs to be done
    - Renamed from "merge" to "mergeSortedArrays" to avoid any ambiguity with
      std::merge, since this file uses `using namespace std;`

    Algorithm Steps
    ----------------
    1. Set pointers i = m-1 (nums1), j = n-1 (nums2), k = m+n-1 (write position)
    2. While both i and j are valid, place the larger of nums1[i]/nums2[j] at nums1[k]
    3. Decrement the corresponding pointer(s) after each placement
    4. If nums2 still has leftover elements, copy them into nums1
    5. nums1 is now the merged sorted array

    Time Complexity: O(m + n) - every element is touched at most once
    Space Complexity: O(1) - merging is done in-place
*/
void mergeSortedArrays(vector<int>& nums1, int m, vector<int>& nums2, int n) {
    // Start filling nums1 from the END
    int i = m - 1;        // pointer for nums1's valid part
    int j = n - 1;        // pointer for nums2
    int k = m + n - 1;    // pointer for the final position in nums1

    // Compare elements from nums1 and nums2, fill from the back
    while (i >= 0 && j >= 0) {
        if (nums1[i] > nums2[j]) {
            nums1[k--] = nums1[i--];  // place nums1[i] and move left
        } else {
            nums1[k--] = nums2[j--];  // place nums2[j] and move left
        }
    }

    // If nums2 still has remaining elements, copy them
    while (j >= 0) {
        nums1[k--] = nums2[j--];
    }

    // No need to handle nums1's remaining part, because they are already in place
}

int main() {
    vector<int> nums1 = {1, 2, 3, 0, 0, 0};
    int m = 3;
    vector<int> nums2 = {2, 5, 6};
    int n = 3;

    cout << "nums1 (with padding): ";
    for (int x : nums1) cout << x << " ";
    cout << endl;
    cout << "nums2: ";
    for (int x : nums2) cout << x << " ";
    cout << endl;

    mergeSortedArrays(nums1, m, nums2, n);

    cout << "Merged array: ";
    for (int x : nums1) cout << x << " ";
    cout << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: nums1 = {1, 2, 3, 0, 0, 0}, m = 3
             nums2 = {2, 5, 6},          n = 3
             (m + n = 6, answer = {1, 2, 2, 3, 5, 6})
    ==========================================================================

      index:   0   1   2   3   4   5
      nums1:   1   2   3   0   0   0     <-- last 3 slots are padding
                       ^           ^
                       i=m-1       k=m+n-1

      index:   0   1   2
      nums2:   2   5   6
                       ^
                       j=n-1

    Tracked state:
      i - read pointer into nums1's VALID part, starts at m-1 = 2
      j - read pointer into nums2, starts at n-1 = 2
      k - WRITE pointer into nums1, starts at m+n-1 = 5 (the very back)

    Initial state: i = 2, j = 2, k = 5, nums1 = [1, 2, 3, 0, 0, 0]

    --------------------------------------------------------------------------
    write 1 (k = 5)
      compare   nums1[i=2] = 3  >  nums2[j=2] = 6 ?   NO
      take      the else branch -> nums1[5] = nums2[2] = 6
      advance   k -> 4, j -> 1   (i unchanged, still 2)
      nums1     [1, 2, 3, 0, 0, 6]
                             ^  ^
                             i  (k=4 next)   overwrote padding, safe

    --------------------------------------------------------------------------
    write 2 (k = 4)
      compare   nums1[i=2] = 3  >  nums2[j=1] = 5 ?   NO
      take      else branch -> nums1[4] = nums2[1] = 5
      advance   k -> 3, j -> 0
      nums1     [1, 2, 3, 0, 5, 6]        overwrote padding, safe

    --------------------------------------------------------------------------
    write 3 (k = 3)
      compare   nums1[i=2] = 3  >  nums2[j=0] = 2 ?   YES
      take      the if branch -> nums1[3] = nums1[2] = 3
      advance   k -> 2, i -> 1
      nums1     [1, 2, 3, 3, 5, 6]

                This is the first time a value is COPIED WITHIN nums1.
                Index 3 was padding, so nothing was lost. There are now
                two 3s in the array; the one at index 2 is a stale
                leftover that the next write is allowed to clobber.

                  index:   0   1   2   3   4   5
                  nums1:   1   2   3   3   5   6
                               ^   ^
                               i   k        <-- k is now the LAST slot
                                                still to be written, and
                                                the stale copy sits at 2

    --------------------------------------------------------------------------
    write 4 (k = 2)
      compare   nums1[i=1] = 2  >  nums2[j=0] = 2 ?   NO  (2 > 2 is false)
      take      else branch -> nums1[2] = nums2[0] = 2
      advance   k -> 1, j -> -1
      nums1     [1, 2, 2, 3, 5, 6]

                THIS is the write the whole "fill from the back" trick
                exists for. Slot 2 held the value 3 - an original nums1
                element - and it just got destroyed. That is fine only
                because write 3 already copied that 3 to index 3. Had we
                merged front-to-back instead, this write would have
                destroyed a 3 that had not been read yet.

      loop      j = -1, so the condition (i >= 0 && j >= 0) fails and the
                comparison loop ends. i is still 1, k is still 1.

    --------------------------------------------------------------------------
    leftover loop:  while (j >= 0)
      j = -1 already, so this loop body NEVER RUNS for this input.
      nums2 was fully consumed by the main loop.

    Nothing is done about nums1's leftovers either, and that is correct:
    nums1[0..i] = nums1[0..1] = [1, 2] are the smallest remaining values,
    they are already sorted, and they already sit in slots 0..1 - which is
    exactly [0..k] = [0..1], the region still unwritten. They are in place
    by definition, so copying them would just write each onto itself.

    --------------------------------------------------------------------------
    FINAL nums1 = [1, 2, 2, 3, 5, 6]     (in-place, no return value)

    ==========================================================================
    Summary table
    ==========================================================================

    | write | i | j | k | compare      | won  | wrote | nums1 after        |
    |-------|---|---|---|--------------|------|-------|--------------------|
    | start | 2 | 2 | 5 | --           | --   | --    | 1 2 3 0 0 0        |
    |   1   | 2 | 2 | 5 | 3 > 6 false  | num2 | 6 @ 5 | 1 2 3 0 0 6        |
    |   2   | 2 | 1 | 4 | 3 > 5 false  | num2 | 5 @ 4 | 1 2 3 0 5 6        |
    |   3   | 2 | 0 | 3 | 3 > 2 true   | num1 | 3 @ 3 | 1 2 3 3 5 6        |
    |   4   | 1 | 0 | 2 | 2 > 2 false  | num2 | 2 @ 2 | 1 2 2 3 5 6        |
    |  end  | 1 |-1 | 1 | j < 0, stop  | --   | --    | 1 2 2 3 5 6        |

    (i, j, k in each row are the values BEFORE that write)

    The invariant that makes back-to-front safe:  k >= i, always.
      Start: k = m+n-1 and i = m-1, so k - i = n >= 0.
      Every write either takes from nums2 (k--, j--) which INCREASES the
      gap k - i, or takes from nums1 (k--, i--) which leaves it unchanged.
      So the write pointer is never behind the nums1 read pointer, and a
      write can only land on a slot that is padding or on a value already
      copied. Front-to-back merging has no such invariant - that is the
      whole reason for the reversal.

    Step count: 4 comparison-loop writes + 0 leftover writes = 4 = m + n - 2
    here (the loop stopped early because nums2 ran out while nums1 still
    had elements). The worst case is m + n writes, hence O(m + n) time and
    O(1) extra space.

    Note the tie at write 4:  the test is `>`, not `>=`, so when the two
    candidates are equal the nums2 element is the one WRITTEN (it takes the
    higher slot k, i.e. it ends up later in the final order, leaving the
    equal nums1 element ahead of it). Either choice sorts correctly; `>`
    just happens to keep the merge stable with respect to nums1.
*/
