#include <iostream>
#include <vector>
using namespace std;

class Solution {
 public:
  void moveZeroes(vector<int>& arr) {
    // LOGIC:
    // i keeps track of the position where the next
    // non-zero element should be placed.
    //
    // j scans through the entire array.
    //
    // Whenever arr[j] is non-zero, we put it at arr[i]
    // and increment i.
    //
    // After all non-zero elements have been moved to
    // the beginning, all remaining positions from i
    // onwards are filled with zeroes.

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
};

int main() {
  vector<int> arr = {0, 1, 0, 3, 12};
  Solution obj;
  obj.moveZeroes(arr);
  cout << "Array after moving zeroes: ";
  for (int x : arr) {
    cout << x << " ";
  }
  cout << endl;

  return 0;
}

/*
    TIME COMPLEXITY:
    ----------------
    O(n)

    The first loop traverses the array once: O(n).
    The second loop can also traverse the array in the
    worst case: O(n).

    Therefore:
        O(n) + O(n) = O(n)


    SPACE COMPLEXITY:
    -----------------
    O(1)

    The array is modified in-place.
    We only use two integer variables (i and j).


    EXAMPLE:
    --------
    Input:
        {0, 1, 0, 3, 12}

    After moving non-zero elements:
        {1, 3, 12, ?, ?}

    Fill remaining positions with zeroes:
        {1, 3, 12, 0, 0}

    Output:
        {1, 3, 12, 0, 0}


    KEY IDEA:
    ---------
    This is a two-pointer technique.

    i -> position where the next non-zero element goes.
    j -> scans through the array.
*/