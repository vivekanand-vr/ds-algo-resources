#include <iostream>
#include <vector>
using namespace std;

// Q: Given an integer array nums, move all 0's to the end of it while
// maintaining the relative order of the non-zero elements

/*
    Approach: Two Pointer (In-Place Compaction)
    - i keeps track of the position where the next non-zero element should
      be placed
    - j scans through the entire array
    - Whenever arr[j] is non-zero, place it at arr[i] and advance i
    - After all non-zero elements have been moved to the front, fill the
      remaining positions from i onward with zeroes

    Algorithm Steps
    ----------------
    1. Initialize i = 0
    2. Scan j from 0 to n-1; if arr[j] != 0, set arr[i] = arr[j] and increment i
    3. Fill positions i to n-1 with 0

    Time Complexity: O(n) - one pass to compact non-zero elements, one to fill zeroes
    Space Complexity: O(1) - modifies the array in-place
*/
void moveZeroes(vector<int>& arr) {
  int i = 0;

  for (int j = 0; j < arr.size(); ++j) {
    if (arr[j] != 0) {
      arr[i++] = arr[j];
    }
  }

  // Fill the remaining positions with zeroes.
  while (i < arr.size()) {
    arr[i++] = 0;
  }
}

int main() {
  vector<int> arr = {0, 1, 0, 3, 12};
  moveZeroes(arr);
  cout << "Array after moving zeroes: ";
  for (int x : arr) {
    cout << x << " ";
  }
  cout << endl;

  return 0;
}