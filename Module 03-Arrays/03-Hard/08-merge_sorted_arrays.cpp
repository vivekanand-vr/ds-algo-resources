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
