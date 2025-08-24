#include <iostream>
#include <bits/stdc++.h>
using namespace std;

void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
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

/*
Logic:
- Place elements from the back to avoid overwriting nums1’s values.
- Compare nums1[i] and nums2[j], put the larger one at nums1[k].
- Decrease pointers accordingly.
- If nums2 has leftovers, copy them.

Time Complexity:  O(m + n) → every element is touched at most once
Space Complexity: O(1)     → in-place merging
*/
