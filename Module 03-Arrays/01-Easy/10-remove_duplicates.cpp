#include <iostream>
#include <vector>
using namespace std;

class Solution {
 public:
  int removeDuplicates(vector<int>& nums) {
    int i = 0;
    int n = nums.size();

    // LOGIC:
    // i points to the position of the last unique element.
    // j scans through the array starting from the second element.
    //
    // If nums[j] == nums[i], it is a duplicate, so we skip it.
    //
    // If nums[j] != nums[i], we found a new unique element.
    // Move i forward and place nums[j] at nums[i].
    //
    // At the end, the first (i + 1) elements contain
    // all unique elements.

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
};

int main() {
  vector<int> nums = {1, 1, 2, 2, 3, 3, 4};
  Solution obj;

  int k = obj.removeDuplicates(nums);
  cout << "Number of unique elements: " << k << endl;
  cout << "Array after removing duplicates: ";
  for (int i = 0; i < k; ++i) {
    cout << nums[i] << " ";
  }
  cout << endl;

  return 0;
}

/*
    TIME COMPLEXITY:
    ----------------
    O(n)

    We traverse the array only once using j.
    Each element is processed exactly once.


    SPACE COMPLEXITY:
    -----------------
    O(1)

    We modify the input array in-place.
    No extra data structure is used apart from a few variables.


    IMPORTANT:
    ----------
    This solution assumes that the input array is SORTED.

    Example:
        Input:  {1, 1, 2, 2, 3, 3, 4}

        Output:
        k = 4

        First 4 elements become:
        {1, 2, 3, 4}
*/