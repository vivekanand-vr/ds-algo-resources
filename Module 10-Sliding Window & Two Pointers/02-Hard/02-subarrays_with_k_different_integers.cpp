#include <bits/stdc++.h>
using namespace std;

// Q: Given an integer array nums and an integer k, return the number of
//    "good" subarrays of nums - a good subarray is one whose number of
//    distinct integers is EXACTLY k.
//
// Example:
// nums = {1, 2, 1, 2, 3}, k = 2 -> 7
//        {1,2}, {2,1}, {1,2}, {1,2,1}, {2,1,2}, {1,2,1,2}, {2,3}
// nums = {1, 2, 1, 3, 4}, k = 3 -> 3
//        {1,2,1,3}, {2,1,3}, {1,3,4}

/*
    Approach: exactly(k) = atMost(k) - atMost(k - 1)

    - Counting subarrays with EXACTLY k distinct values in one sliding
      window does not work: the window cannot decide how far left to
      start, because both ends have to satisfy an equality rather than
      an inequality.
    - Counting subarrays with AT MOST k distinct values, however, is a
      textbook window. Keep [l, r] within budget by shrinking whenever
      the distinct count exceeds k. Then for the current r, EVERY
      subarray ending at r and starting at any index in [l, r] has at
      most k distinct values - and there are exactly (r - l + 1) of
      them. Summing that over all r counts them all, each exactly once,
      because each subarray has a unique right endpoint.
    - Finally, a subarray with at most k distinct values either has
      exactly k, or at most k - 1. So subtracting the two counts leaves
      precisely the subarrays with exactly k:
          exactly(k) = atMost(k) - atMost(k - 1)
    - The helper is called `atMostK` rather than `solve` to make that
      identity readable at the call site.

    Algorithm Steps
    ----------------
    atMostK(arr, k):
      1. Initialize l = 0, r = 0, count = 0 and an empty count map.
      2. While r < n:
           a. Increment the count of arr[r] (grow the window).
           b. While the number of distinct values exceeds k:
                - decrement count[arr[l]], erase the key if it hits 0
                - l++ (shrink from the left)
           c. count += (r - l + 1) - all subarrays ending at r that
              start inside the window are within budget.
           d. r++.
      3. Return count.

    subarraysWithKDistinct(nums, k):
      4. Return atMostK(nums, k) - atMostK(nums, k - 1).

    Time Complexity: O(n log k) - two linear window passes, each map
                      operation O(log k). unordered_map makes it O(n).
    Space Complexity: O(k) - the map never holds more than k + 1 keys
*/
int atMostK(vector<int>& arr, int k) {
  int n = arr.size();
  int l = 0, r = 0, c = 0;
  map<int, int> mp;

  while (r < n) {
    mp[arr[r]]++;                        // grow the window to include arr[r]

    while ((int)mp.size() > k) {         // over budget -> shrink from the left
      mp[arr[l]]--;
      if (mp[arr[l]] == 0) {
        mp.erase(arr[l]);                // drop keys at zero so size() = distinct count
      }
      l++;
    }

    c += (r - l + 1);                    // every start in [l, r] is within budget
    r++;
  }

  return c;
}

int subarraysWithKDistinct(vector<int>& nums, int k) {
  return atMostK(nums, k) - atMostK(nums, k - 1);
}

int main() {
  vector<pair<vector<int>, int>> tests = {{{1, 2, 1, 2, 3}, 2}, {{1, 2, 1, 3, 4}, 3}};

  for (auto& [nums, k] : tests) {
    vector<int> arr = nums;
    cout << "nums = [";
    for (size_t i = 0; i < arr.size(); i++) cout << arr[i] << (i + 1 < arr.size() ? ", " : "");
    cout << "], k = " << k << " -> " << subarraysWithKDistinct(arr, k) << endl;
  }

  return 0;
}
