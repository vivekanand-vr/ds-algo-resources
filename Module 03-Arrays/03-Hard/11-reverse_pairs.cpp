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