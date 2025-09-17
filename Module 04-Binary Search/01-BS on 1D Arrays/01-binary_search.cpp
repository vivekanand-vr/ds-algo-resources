#include <bits/stdc++.h>

#include <iostream>
using namespace std;

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

/*
  Time Complexity: O(log n)
  Space Complexity: O(1)
*/