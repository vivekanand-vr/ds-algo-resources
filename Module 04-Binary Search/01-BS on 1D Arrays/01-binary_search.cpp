#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a sorted array and a target value, return the index of target
// in the array, or -1 if it is not present

/*
    Approach: Binary Search
    - Repeatedly halve the search space by comparing the target with the
      middle element
    - If the middle element equals the target, we're done
    - If the middle element is smaller, the target must be in the right half
    - If the middle element is larger, the target must be in the left half

    Algorithm Steps
    ----------------
    1. Initialize left = 0, right = n - 1
    2. While left <= right, compute mid = left + (right - left) / 2
    3. If arr[mid] == target, return mid
    4. If arr[mid] < target, search the right half (left = mid + 1)
    5. Else search the left half (right = mid - 1)
    6. Return -1 if the loop ends without finding the target

    Time Complexity: O(log n)
    Space Complexity: O(1)
*/
int binarySearch(int target, vector<int> arr) {
  int n = arr.size();
  int left = 0, right = n - 1;

  while (left <= right) {
    int mid = left + (right - left) / 2;  // to prevent overflow
    if (arr[mid] == target) {
      return mid;
    } else if (arr[mid] < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return -1;
}

int main() {
  vector<int> arr = {-1, 0, 3, 5, 9, 12};
  int target = 9;

  cout << "Array: ";
  for (int x : arr) cout << x << " ";
  cout << endl;

  int result = binarySearch(target, arr);
  cout << "Index of " << target << ": " << result << endl;

  return 0;
}