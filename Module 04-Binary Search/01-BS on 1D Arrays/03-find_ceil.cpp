#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a sorted array and a value x, find the index of the ceil of x
// (the smallest element in the array that is >= x)

/*
    Approach: Binary Search
    - Binary search for x while tracking the best candidate seen so far
    - Whenever arr[mid] >= x, it's a valid ceil candidate, so record it and
      search the left half for something closer to x
    - Whenever arr[mid] < x, it can't be the ceil, so search the right half

    Algorithm Steps
    ----------------
    1. Initialize l = 0, r = n - 1, ans = -1
    2. While l <= r, compute mid = l + (r - l) / 2
    3. If arr[mid] == x, return mid (exact match is the ceil)
    4. If arr[mid] > x, update ans = mid and search left (r = mid - 1)
    5. Else search right (l = mid + 1)
    6. Return ans (-1 if no ceil exists)

    Time Complexity: O(log n) - binary search
    Space Complexity: O(1) - constant extra space
*/
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

int main() {
  vector<int> arr = {1, 2, 8, 10, 11, 12, 19};
  int x = 5;

  cout << "Array: ";
  for (int n : arr) cout << n << " ";
  cout << endl;

  int idx = findCeil(arr, x);
  cout << "Ceil of " << x << ": ";
  if (idx == -1) cout << "does not exist" << endl;
  else cout << arr[idx] << " (index " << idx << ")" << endl;

  return 0;
}
