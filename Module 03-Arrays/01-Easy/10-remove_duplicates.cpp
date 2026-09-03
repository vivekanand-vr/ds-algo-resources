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