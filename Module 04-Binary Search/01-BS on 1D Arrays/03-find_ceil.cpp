#include <bits/stdc++.h>

#include <iostream>
using namespace std;

int findCeil(vector<int>& arr, int x) {
  int n = arr.size();
  int l = 0, r = n - 1;
  int ans = -1;  // store ceil index (default -1 if no ceil exists)

  while (l <= r) {
    int mid = l + (r - l) / 2;

    if (arr[mid] == x) {
      return mid;  // exact match is the ceil itself
    } else if (arr[mid] > x) {
      ans = mid;    // candidate for ceil
      r = mid - 1;  // try to find smaller candidate on left
    } else {
      l = mid + 1;  // move right since arr[mid] < x
    }
  }

  return ans;  // returns index of ceil element OR -1 if none found
}

/*
  Time Complexity:  O(log n)  → binary search
  Space Complexity: O(1)      → constant extra space
*/
